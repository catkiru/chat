#include <iostream>
#include <QApplication>

#include "ChatMessage.h"
#include "ChatServer.h"

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
