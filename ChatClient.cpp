//
// Created by kat on 4/17/2025.
//

#include "ChatClient.h"

void ChatClient::sendMessage(const ChatMessage &msg) {
    QMetaObject::invokeMethod(&socket, [this, msg]() {
        socket.write(msg.toJson());
        socket.flush();
    });
}

void ChatClient::sendLogin(const std::string &login, const std::string &password) {
    auto type = Auth;
    auto loginStr = QString::fromStdString(login);
    auto passwordStr = QString::fromStdString(password);
    sendMessage(ChatMessage(type, loginStr, passwordStr));
}

void ChatClient::sendRegister(QString login, QString pass) {
    auto msg = ChatMessage(Register, login, pass);
    sendMessage(msg);
}

void ChatClient::sendTextMessage(const std::string &message) {
    auto type = TextMessage;
    auto body = QString::fromStdString(message);
    auto msg = ChatMessage(type, body);
    sendMessage(msg);
}

void ChatClient::incomingMessage(const ChatMessage msg) {
    switch (msg.type) {
        case AuthResult:
            if (msg.type == AuthResult && msg.body == "ok") {
                isLogedIn = true;
                emit loginResult(true);
            } else {
                emit loginResult(false);
            }
            break;
        case TextMessage:
            emit textMessage(msg.from, msg.body);
        default:
            break;
    }
}

bool ChatClient::connect(QString &host) {
    socket.disconnect();
    QTextStream in(stdin);
    QTextStream out(stdout);

    socket.connectToHost(host, 1234);

    if (!socket.waitForConnected(3000)) {
        std::cerr << "Could not connect to server" << std::endl;
        return false;
    }

    std::cout << "Connection to server succefully established" << std::endl;

    QObject::connect(&socket, &QTcpSocket::readyRead, [this]() {
        auto messageStr = socket.readAll();
        auto msg = ChatMessage::fromJson(messageStr);
        incomingMessage(msg);
    });

    return true;
}
