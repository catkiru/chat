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

    void sendMessage(const ChatMessage &msg);

signals:
    void textMessage(QString from, QString msg);
    void loginResult(bool success);


public:
    void sendLogin(const std::string &login, const std::string &password);

    void sendTextMessage(const std::string &message);

    void incomingMessage(const ChatMessage msg);

    bool connect(QString &host);
};


#endif //CHATCLIENT_H
