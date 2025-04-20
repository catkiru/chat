//
// Created by kat on 4/19/2025.
//

#ifndef CHATSERVER_H
#define CHATSERVER_H
#include <QSqlDatabase>
#include <QTcpServer>
#include <QTcpSocket>

#include "ChatMessage.h"
#include "UserInfo.h"


class ChatServer : public QTcpServer {
private:
    void sendMessage(QTcpSocket *socket, const ChatMessage &msg);
    void sendAuthResult(QTcpSocket *socket, bool authResult);
    void sendTextMessage(QTcpSocket *socket, QString from, QString message, QByteArray image);
    bool authUser(QString login, QString password);
    bool registerUser(QString login, QString password);
    void saveMessage(QString login, QString body, QByteArray image);
    bool connectToDatabase();

    QHash<QTcpSocket *, UserInfo *> clients;
    QSqlDatabase db;

protected:
    void incomingConnection(qintptr handle) override;

public:
    using QTcpServer::QTcpServer;
    ChatServer();
};


#endif //CHATSERVER_H
