//
// Created by kat on 4/19/2025.
//

#include "ChatServer.h"

#include <iostream>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

ChatServer::ChatServer() {
    connectToDatabase();
}

void ChatServer::sendMessage(QTcpSocket *socket, const ChatMessage &msg) {
    socket->write(msg.toJson());
    socket->flush();
}

void ChatServer::sendAuthResult(QTcpSocket *socket, bool authResult) {
    const auto msg = ChatMessage(AuthResult, authResult ? "ok" : "fail");
    sendMessage(socket, msg);
}

void ChatServer::sendTextMessage(QTcpSocket *socket, QString from, QString message) {
    auto msg = ChatMessage(TextMessage, message);
    msg.from = from;
    sendMessage(socket, msg);
}

bool ChatServer::authUser(QString login, QString password) {
    std::cout << "Драйвер базы:" << db.driverName().toStdString() << std::endl;

    QSqlQuery query(db);
    query.prepare("SELECT id FROM chat.users WHERE login = :login AND password_hash = :password");
    query.bindValue(":login", login);
    query.bindValue(":password", password);


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

void ChatServer::incomingConnection(qintptr handle) {
    auto *socket = new QTcpSocket(this);

    clients[socket] = nullptr;

    socket->setSocketDescriptor(handle);
    std::cout << "New connection: " << handle << std::endl;

    connect(socket, &QTcpSocket::readyRead, [this, socket]() {
        QByteArray data = socket->readAll();

        ChatMessage msg = ChatMessage::fromJson(data);
        switch (msg.type) {
            case TextMessage:
                std::cout << "Incoming message:" << msg.body.toStdString() << std::endl;
                UserInfo *info;
                info = clients[socket];
                for (auto it = clients.begin(); it != clients.end(); ++it) {
                    if (it.key() != socket || true) {
                        sendTextMessage(socket, info->login, msg.body);
                    }
                }
                break;
            case Auth:
                if (authUser(msg.login, msg.password)) {
                    sendAuthResult(socket, true);
                    auto info2 = new UserInfo();
                    info2->login = msg.login;
                    clients[socket] = info2;
                } else {
                    sendAuthResult(socket, false);
                    std::cerr << "invalid password";
                }

                std::cout << "Welcome:" << msg.body.toStdString() << std::endl;
                break;
            case Image:
                std::cerr << "Not implementing yet" << std::endl;
                break;
        }
    });

    connect(socket, &QTcpSocket::disconnected, [this, socket]() {
        std::cout << "Client disconnectd" << socket->socketDescriptor() << std::endl;
        // освободить память
        clients.remove(socket);
    });
}
