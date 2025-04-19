//
// Created by kat on 4/17/2025.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ChatWindow.h" resolved

#include "chatwindow.h"
#include "ui_ChatWindow.h"


ChatWindow::ChatWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::ChatWindow) {
    ui->setupUi(this);
}

ChatWindow::~ChatWindow() {
    delete ui;
}

void ChatWindow::on_btnSend_clicked() {
    auto msg = ui->txt_message->text();
    ui->txt_message->setText("");
    _client->sendTextMessage(msg.toStdString());
}
