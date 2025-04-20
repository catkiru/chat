//
// Created by kat on 4/17/2025.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ChatWindow.h" resolved

#include "chatwindow.h"
#include "ui_ChatWindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QImageReader>


ChatWindow::ChatWindow(ChatClient *client, QWidget *parent) : QMainWindow(parent), ui(new Ui::ChatWindow) {
    ui->setupUi(this);
    _client = client;
    connect(_client, &ChatClient::textMessage, this, &ChatWindow::onTextMessage);
}

ChatWindow::~ChatWindow() {
    delete ui;
}

void ChatWindow::on_btnSend_clicked() {
    auto msg = ui->txt_message->text();
    ui->txt_message->setText("");
    _client->sendTextMessage(msg.toStdString());
}

void ChatWindow::onTextMessage(QString from, QString msg) {
    QWidget *messageWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(messageWidget);
    layout->setContentsMargins(5, 5, 5, 5);

    QLabel *fromLabel = new QLabel(from);
    QLabel *msgLabel = new QLabel(msg);
    QLabel *imageLabel = new QLabel();

    // QString imagePath = R"(C:\Qt\img.jpg)";
    // auto pix = QPixmap(imagePath).scaledToWidth(200, Qt::SmoothTransformation);
    // imageLabel->setPixmap(pix);

    layout->addWidget(fromLabel);
    layout->addWidget(msgLabel);
    layout->addWidget(imageLabel);

    QListWidgetItem *item = new QListWidgetItem(ui->messages);
    item->setSizeHint(messageWidget->sizeHint());
    ui->messages->addItem(item);
    ui->messages->setItemWidget(item, messageWidget);
    ui->messages->scrollToBottom();
}
