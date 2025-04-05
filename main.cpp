#include <iostream>
#include <QApplication>
#include <QPushButton>
#include <QTcpServer>
#include <QTcpSocket>


class ChatServer : public QTcpServer {
public:
    using QTcpServer::QTcpServer;

protected:
    void incomingConnection(qintptr handle) override {
        auto *socket = new QTcpSocket(this);
        socket->setSocketDescriptor(handle);
        std::cout << "New connection: " << handle << std::endl;

        connect(socket, &QTcpSocket::readyRead, [socket]() {
            std::cout << "Incoming message: " << socket->readAll().toStdString() << std::endl;
        });

        connect(socket, &QTcpSocket::disconnected, [socket]() {
            std::cout << "Client disconnectd" << socket->socketDescriptor() << std::endl;
        });

        socket->write("Hello client");
    }
};

int main(int argc, char *argv[]) {
    std::cout << "Starting server" << std::endl;
    QApplication a(argc, argv);

    int port = 1234;

    if (argc > 1) {
        try {
            port = std::stoi(argv[1]);
        }
        catch (std::exception &e) {
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
