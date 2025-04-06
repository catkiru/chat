#include <iostream>
#include <qcoreapplication.h>
#include <QTcpSocket>
#include <thread>

#include "ChatMessage.h"

class ChatClient {
    QTcpSocket socket;

private:
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
        msg.body = QString::fromStdString(login) + ":" + QString::fromStdString(password);
        sendMessage(msg);
    }

    void sendTextMessage(const std::string &message) {
        ChatMessage msg;
        msg.type = TextMessage;
        msg.body = QString::fromStdString(message);
        sendMessage(msg);
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

        sendLogin("katya", "strong_password123");

        QObject::connect(&socket, &QTcpSocket::readyRead, [this]() {
            incomingMessage(socket.readAll().toStdString());
        });

        // Ввод с клавиатуры в другом потоке
        std::thread inputThread([this]() {
            std::string input;
            while (true) {
                std::cout << "type something: " << std::flush;
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
