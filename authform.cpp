//
// Created by kat on 4/17/2025.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AuthForm.h" resolved

#include "authform.h"
#include "ui_AuthForm.h"
#include <QMessageBox>


AuthForm::AuthForm(QWidget *parent) : QDialog(parent), ui(new Ui::AuthForm) {
    ui->setupUi(this);
}

AuthForm::AuthForm(ChatClient *client, QWidget *parent) : AuthForm(parent) {
    _client = client;
    connect(_client, &ChatClient::loginResult, this, &AuthForm::onLoginResult);
}

AuthForm::~AuthForm() {
    delete ui;
}

void AuthForm::on_buttonBox_accepted() {
    QString host = ui->txt_serverHost->text();
    if (_client->connect(host)) {
        auto login = ui->txt_login->text();
        auto pass = ui->txt_password->text();
        _client->sendLogin(login.toStdString(), pass.toStdString());
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось подключиться к серверу");
    }
}

void AuthForm::on_registerButton_clicked() {
    QString host = ui->txt_serverHost->text();
    if (_client->connect(host)) {
        auto login = ui->txt_login->text();
        auto pass = ui->txt_password->text();
        _client->sendRegister(login, pass);
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось подключиться к серверу");
    }
}

void AuthForm::onLoginResult(bool success) {
    if (success) {
        accept();
    } else {
        QMessageBox::warning(this, "Ошибка", "Неверный логин или пароль");
    }
}
