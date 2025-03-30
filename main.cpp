#include <iostream>
#include <QApplication>
#include <QPushButton>
#include <QTcpServer>


class ChatServer : public QTcpServer {
public:
    using QTcpServer::QTcpServer;
protected:

};

int main(int argc, char *argv[]) {
    std::cout << "Starting server";
    QApplication a(argc, argv);

    int port = 1234;

    ChatServer server;
    if (!server.listen(QHostAddress::Any, port)) {
        std::cerr << "Cant start server";
        return 1;
    }

    std::cout << "start litening server on port: " << port << std::endl;

    return QApplication::exec();
}
