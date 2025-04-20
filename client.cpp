#include <iostream>
#include <QApplication>
#include <qcoreapplication.h>
#include <QTcpSocket>
#include <thread>
#include "chatwindow.h"
#include "authform.h"
#include "ChatClient.h"
#include "ChatMessage.h"



int main(int argc, char *argv[]) {
    system("chcp 65001");
    QApplication a(argc, argv);

    ChatClient client;

    auto authForm = new AuthForm(&client);
    authForm->exec();

    auto chatWindow = new ChatWindow(&client);
    chatWindow->show();

    return a.exec();
}
