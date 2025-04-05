#include <iostream>
#include <qcoreapplication.h>
#include <QTcpSocket>

int main(int argc, char *argv[]) {
    const int port = 1234;
    QCoreApplication a(argc, argv);

    QTcpSocket socket;

    socket.connectToHost("127.0.0.1", 1234);

    if (!socket.waitForConnected(3000)) {
        std::cerr << "Could not connect to server" << std::endl;
        return 1;
    }

    std::cout << "Connection to server succefully established" << std::endl;

    std::string message = "hello world";
    while (true) {
        std::cout << "Enter a messege" << std::endl;
        std::cin >> message;
        socket.write(message.c_str());
        // socket.flush();

        if (!socket.waitForBytesWritten(3000)) {
            std::cerr << "Fail to send message" << std::endl;
        } else {
            std::cout << "Message sended" << std::endl;
        }

    }

    return 0;
}
