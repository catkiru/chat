//
// Created by kat on 4/17/2025.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ChatWindow.h" resolved

#include "chatwindow.h"
#include "ui_ChatWindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QImageReader>
#include <QFileDialog>
#include <QBuffer>


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

    QPixmap pixmap = ui->imagePreviewLabel->pixmap();
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");
    ui->imagePreviewLabel->clear();

    _client->sendTextMessage(msg, byteArray);
}

void ChatWindow::on_btnSelectImage_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this,
    "Выберите изображение", "", "Изображения (*.png *.jpg *.jpeg *.bmp)");

    if (!fileName.isEmpty()) {
        QPixmap pixmap(fileName);
        if (!pixmap.isNull()) {
            ui->imagePreviewLabel->setPixmap(pixmap.scaled(ui->imagePreviewLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            ui->imagePreviewLabel->setText("Не удалось загрузить изображение.");
        }
    }
}

void ChatWindow::onTextMessage(QString from, QString msg, QByteArray image) {
    QWidget *messageWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(messageWidget);
    layout->setContentsMargins(5, 5, 5, 5);

    QLabel *fromLabel = new QLabel(from);
    QLabel *msgLabel = new QLabel(msg);
    QLabel *imageLabel = new QLabel();
    QPixmap pixmap;
    pixmap.loadFromData(image);
    imageLabel->setPixmap(pixmap);
    layout->addWidget(fromLabel);
    layout->addWidget(msgLabel);
    layout->addWidget(imageLabel);

    QListWidgetItem *item = new QListWidgetItem(ui->messages);
    item->setSizeHint(messageWidget->sizeHint());
    ui->messages->addItem(item);
    ui->messages->setItemWidget(item, messageWidget);
    ui->messages->scrollToBottom();
}
