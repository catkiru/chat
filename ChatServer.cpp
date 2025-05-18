//
// Created by kat on 4/19/2025.
//

#include "ChatServer.h"

#include <iostream>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QCryptographicHash>

ChatServer::ChatServer() {
    connectToDatabase();
}

void ChatServer::sendMessage(QTcpSocket *socket, const ChatMessage &msg) {
    auto json = msg.toJson();
    socket->write(json);
    socket->flush();
}

void ChatServer::sendAuthResult(QTcpSocket *socket, bool authResult) {
    const auto msg = ChatMessage(AuthResult, authResult ? "ok" : "fail");
    sendMessage(socket, msg);
}

void ChatServer::sendTextMessage(QTcpSocket *socket, QString from, QString message, QByteArray image) {
    auto msg = ChatMessage(TextMessage, message);
    msg.from = from;
    msg.image = image;
    sendMessage(socket, msg);
}

bool ChatServer::authUser(QString login, QString password) {
    QString hash = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
    QSqlQuery query(db);
    query.prepare("SELECT login FROM chat.users WHERE login = :login AND password_hash = :password");
    query.bindValue(":login", login);
    query.bindValue(":password", hash);


    std::cout << "SQL:" << query.lastQuery().toStdString() << std::endl;

    if (!query.exec()) {
        std::cerr << "PostgreSQL error: " << query.lastError().nativeErrorCode().toStdString() << std::endl;
        std::cerr << "Error text: " << query.lastError().text().toStdString() << std::endl;
        return false;
    }

    // Если есть хотя бы одна строка — пользователь найден
    if (query.next()) {
        return true;
    } else {
        return false;
    }
}

bool ChatServer::registerUser(QString login, QString password) {
    // Хэшируем пароль перед сохранением
    QString hash = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());

    QSqlQuery query(db);
    query.prepare("INSERT INTO chat.users (login, user_name, password_hash) VALUES (:login, :login, :password)");
    query.bindValue(":login", login);
    query.bindValue(":password", hash);

    if (!query.exec()) {
        std::cerr << "PostgreSQL error: " << query.lastError().nativeErrorCode().toStdString() << std::endl;
        std::cerr << "Error text: " << query.lastError().text().toStdString() << std::endl;
        return false;
    }

    return true;
}

void ChatServer::saveMessage(QString login, QString body, QByteArray image) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO chat.msg (from_login, body, image) VALUES (:login, :body, :image)");
    query.bindValue(":login", login);
    query.bindValue(":body", body);
    query.bindValue(":image", image);

    if (!query.exec()) {
        std::cerr << "PostgreSQL error: " << query.lastError().nativeErrorCode().toStdString() << std::endl;
        std::cerr << "Error text: " << query.lastError().text().toStdString() << std::endl;
    }
}

bool ChatServer::connectToDatabase() {
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("chat_db");
    db.setUserName("postgres");
    db.setPassword("123");
    db.setPort(5432);

    if (!db.open()) {
        std::cerr << "Ошибка подключения к базе данных:" << db.lastError().text().toStdString() << std::endl;
        return false;
    } else {
        std::cout << "Успешное подключение к базе PostgreSQL!" << std::endl;
        return true;
    }
}

void ChatServer::sendHistory(QTcpSocket *socket) {
    QSqlQuery query(db);
    query.prepare(" select from_login, send_at, body, image from chat.msg order by send_at desc limit 10; ");

    std::cout << "SQL:" << query.lastQuery().toStdString() << std::endl;

    if (!query.exec()) {
        std::cerr << "PostgreSQL error: " << query.lastError().nativeErrorCode().toStdString() << std::endl;
        std::cerr << "Error text: " << query.lastError().text().toStdString() << std::endl;
    }

    QList<ChatMessage> history;

    while (query.next()) {
        auto login = query.value("from_login").toString();
        auto body = query.value("body").toString();
        auto image = query.value("image").toByteArray();
        ChatMessage cm(TextMessage, body);
        cm.image = image;
        cm.from = login;
        history.prepend(cm);
    }

    ChatMessage chatHistory(History);
    chatHistory.history = history;
    sendMessage(socket, chatHistory);
}

void ChatServer::incomingConnection(qintptr handle) {
    auto *socket = new QTcpSocket(this);

    clients[socket] = nullptr;

    socket->setSocketDescriptor(handle);
    std::cout << "New connection: " << handle << std::endl;

    connect(socket, &QTcpSocket::readyRead, [this, socket]() {
        QByteArray data = socket->readAll();

        ChatMessage msg = ChatMessage::fromJson(data);
        switch (msg.type) {
            case Register:
                if (registerUser(msg.login, msg.password)) {
                    sendAuthResult(socket, true);
                    auto info2 = new UserInfo();
                    info2->login = msg.login;
                    clients[socket] = info2;
                    sendHistory(socket);
                } else {
                    sendAuthResult(socket, false);
                }
                break;
            case TextMessage:
                std::cout << "Incoming message:" << msg.body.toStdString() << std::endl;
                UserInfo *info;
                info = clients[socket];
                saveMessage(info->login, msg.body, msg.image);
                for (auto it = clients.begin(); it != clients.end(); ++it) {
                    if (it.key() != socket || true) {
                        sendTextMessage(socket, info->login, msg.body, msg.image);
                        sendAuthResult(socket, true);
                    }
                }
                break;
            case Auth:
                if (authUser(msg.login, msg.password)) {
                    sendAuthResult(socket, true);
                    auto info2 = new UserInfo();
                    info2->login = msg.login;
                    clients[socket] = info2;
                    sendHistory(socket);
                } else {
                    sendAuthResult(socket, false);
                    std::cerr << "invalid password";
                }

                std::cout << "Welcome:" << msg.body.toStdString() << std::endl;
                break;

        }
    });

    connect(socket, &QTcpSocket::disconnected, [this, socket]() {
        std::cout << "Client disconnectd" << socket->socketDescriptor() << std::endl;
        // освободить память
        clients.remove(socket);
    });
}
