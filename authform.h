//
// Created by kat on 4/17/2025.
//

#ifndef AUTHFORM_H
#define AUTHFORM_H

#include <QDialog>
#include "ChatClient.h"


class ChatClient;
QT_BEGIN_NAMESPACE

namespace Ui {
    class AuthForm;
}

QT_END_NAMESPACE

class AuthForm : public QDialog {
    Q_OBJECT
    ChatClient *_client;

public:
    explicit AuthForm(QWidget *parent = nullptr);

    explicit AuthForm(ChatClient *client, QWidget *parent = nullptr);

    ~AuthForm() override;

public slots:
    void on_buttonBox_accepted();

    void onLoginResult(bool success);

private:
    Ui::AuthForm *ui;
};


#endif //AUTHFORM_H
