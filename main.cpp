#include <iostream>
#include <QApplication>
#include <QPushButton>
#include <QTcpServer>
#include <QTcpSocket>

#include "ChatMessage.h"
#include "UserInfo.h"


class ChatServer : public QTcpServer {
public:
    using QTcpServer::QTcpServer;

protected:
    void incomingConnection(qintptr handle) override {
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
                        if (it.key() != socket) {
                            QByteArray reply = (info->login + " says: >> " + msg.body).toUtf8();
                            it.key()->write(reply);
                            it.key()->flush();
                        }
                    }
                    break;
                case Auth:
                    if (msg.password == "123") {
                        socket->write("You logged in");
                        socket->flush();
                        auto info2 = new UserInfo();
                        info2->login = msg.login;
                        clients[socket] = info2;
                    } else {
                        socket->write("Password incorect");
                        socket->flush();
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

private:
    QHash<QTcpSocket *, UserInfo *> clients;
};

int main(int argc, char *argv[]) {
    system("chcp 65001");

    std::cout << "Starting server" << std::endl;
    QApplication a(argc, argv);

    int port = 1234;

    if (argc > 1) {
        try {
            port = std::stoi(argv[1]);
        } catch (std::exception &e) {
            std::cerr << "Exception: " << e.what() << std::endl;
        }
        catch (...) {
            std::cerr << "something wents wrong" << std::endl;
        }
    }

    ChatServer server;
    if (!server.listen(QHostAddress::Any, port)) {
        std::cerr << "Cant start server" << std::endl;
        return 1;
    }

    std::cout << "start listening server on port: " << port << std::endl;

    return QApplication::exec();
}
