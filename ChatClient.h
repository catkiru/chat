//
// Created by kat on 4/17/2025.
//

#ifndef CHATCLIENT_H
#define CHATCLIENT_H
#include <iostream>
#include <QTcpSocket>
#include <thread>

#include "ChatMessage.h"


class ChatClient : public QObject {
    Q_OBJECT

private:
    QTcpSocket socket;
    bool isLogedIn = false;

    void sendMessage(const ChatMessage &msg) {
        QMetaObject::invokeMethod(&socket, [this, msg]() {
            socket.write(msg.toJson());
            socket.flush();
        });
    }

signals:
    void loginResult(bool success);

public:
    void sendLogin(const std::string &login, const std::string &password) {
        auto type = Auth;
        auto loginStr = QString::fromStdString(login);
        auto passwordStr = QString::fromStdString(password);
        sendMessage(ChatMessage(type, loginStr, passwordStr));
    }

    void sendTextMessage(const std::string &message) {
        auto type = TextMessage;
        auto body = QString::fromStdString(message);
        auto msg = ChatMessage(type, body);
        sendMessage(msg);
    }

    void incomingMessage(const std::string &incoming) {
        if (!isLogedIn) {
            if (QString::fromStdString(incoming) == "You logged in") {
                isLogedIn = true;
                emit loginResult(true);
            }
        } else {
            emit loginResult(false);
        }

        std::cout << std::endl << "Incoming: " << incoming << std::endl;
        std::cout << "incoming says: type something: " << std::flush;
    }

    bool connect(QString &host) {
        QTextStream in(stdin);
        QTextStream out(stdout);

        socket.connectToHost(host, 1234);

        if (!socket.waitForConnected(3000)) {
            std::cerr << "Could not connect to server" << std::endl;
            return false;
        }

        std::cout << "Connection to server succefully established" << std::endl;

        QObject::connect(&socket, &QTcpSocket::readyRead, [this]() {
            incomingMessage(socket.readAll().toStdString());
        });

        return true;
    }
};


#endif //CHATCLIENT_H
