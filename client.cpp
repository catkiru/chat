#include <iostream>
#include <qcoreapplication.h>
#include <QTcpSocket>
#include <thread>

#include "ChatMessage.h"

class ChatClient {
private:
    QTcpSocket socket;
    bool isLogedIn = false;

    void sendMessage(const ChatMessage &msg) {
        QMetaObject::invokeMethod(&socket, [this, msg]() {
            socket.write(msg.toJson());
            socket.flush();
        });
    }

public:
    void sendLogin(const std::string &login, const std::string &password) {
        ChatMessage msg;
        msg.type = Auth;
        msg.login = QString::fromStdString(login);
        msg.password = QString::fromStdString(password);
        sendMessage(msg);
    }

    void sendTextMessage(const std::string &message) {
        if (!isLogedIn) {
            std::cerr << "Login first!" << std::endl;
            return;
        }

        ChatMessage msg;
        msg.type = TextMessage;
        msg.body = QString::fromStdString(message);
        sendMessage(msg);
    }

    void incomingMessage(const std::string &incoming) {
        if (QString::fromStdString(incoming) == "You logged in") {
            isLogedIn = true;
        }

        std::cout << std::endl << "Incoming: " << incoming << std::endl;
        std::cout << "incoming says: type something: " << std::flush;
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

        sendLogin("katya", "123");

        QObject::connect(&socket, &QTcpSocket::readyRead, [this]() {
            incomingMessage(socket.readAll().toStdString());
        });

        // Ввод с клавиатуры в другом потоке
        std::thread inputThread([this]() {
            std::string input;
            while (true) {
                std::getline(std::cin, input);
                sendTextMessage(input);
            }
        });
        inputThread.detach();

        return true;
    }
};

int main(int argc, char *argv[]) {
    system("chcp 65001");
    QCoreApplication a(argc, argv);

    ChatClient client;
    if (!client.connect()) {
        std::cerr << "Can't connect to server" << std::endl;
        return 1;
    }

    return a.exec();
}
