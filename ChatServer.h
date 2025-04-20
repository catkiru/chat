//
// Created by kat on 4/19/2025.
//

#ifndef CHATSERVER_H
#define CHATSERVER_H
#include <QTcpServer>
#include <QTcpSocket>

#include "ChatMessage.h"
#include "UserInfo.h"


class ChatServer : public QTcpServer {
private:
    void sendMessage(QTcpSocket *socket, const ChatMessage &msg);
    void sendAuthResult(QTcpSocket *socket, bool authResult);
    void sendTextMessage(QTcpSocket *socket, QString from, QString message);

    QHash<QTcpSocket *, UserInfo *> clients;

protected:
    void incomingConnection(qintptr handle) override;

public:
    using QTcpServer::QTcpServer;
};


#endif //CHATSERVER_H
