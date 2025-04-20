//
// Created by kat on 4/17/2025.
//

#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>

#include "ChatClient.h"


QT_BEGIN_NAMESPACE

namespace Ui {
    class ChatWindow;
}

QT_END_NAMESPACE

class ChatWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit ChatWindow(ChatClient *client, QWidget *parent = nullptr);

    ChatClient *_client;

    ~ChatWindow() override;

    void on_btn_send_click();

private:
    Ui::ChatWindow *ui;

public slots:
    void onTextMessage(QString from, QString msg, QByteArray image);
    void on_btnSend_clicked();
    void on_btnSelectImage_clicked();
};


#endif //CHATWINDOW_H
