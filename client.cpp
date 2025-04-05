#include <iostream>
#include <qcoreapplication.h>
#include <QTcpSocket>
#include <thread>

class ChatClient {
    QTcpSocket socket;

public:
    void sendMessage(const std::string &message) {
        QString msg = QString::fromStdString(message);
        QMetaObject::invokeMethod(&socket, [this, msg]() {
            socket.write(msg.toUtf8());
            socket.flush();
        });
    }

    void incomingMessage(const std::string &incoming) {
        std::cout << std::endl << "Incoming: " << incoming << std::endl;
        std::cout << "type something: " << std::flush;
    }

    bool connect() {
        QTextStream in(stdin);
        QTextStream out(stdout);

        socket.connectToHost("127.0.0.1", 1234);

        if (!socket.waitForConnected(3000)) {
            std::cerr << "Could not connect to server" << std::endl;
            return false;
        }

        std::cout << "Connection to server succefully established" << std::endl;

        QObject::connect(&socket, &QTcpSocket::readyRead, [this]() {
            incomingMessage(socket.readAll().toStdString());
        });

        // Ввод с клавиатуры в другом потоке
        std::thread inputThread([this]() {
            std::string input;
            while (true) {
                std::cout << "type something: " << std::flush;
                std::getline(std::cin, input);
                sendMessage(input);
            }
        });
        inputThread.detach();

        return true;
    }
};

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    ChatClient client;
    if (!client.connect()) {
        std::cerr << "Can't connect to server" << std::endl;
        return 1;
    }

    return a.exec();
}
