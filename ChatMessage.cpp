//
// Created by kat on 4/6/2025.
//

#include "ChatMessage.h"

#include <QJsonObject>
#include <QJsonDocument>

ChatMessage::ChatMessage(ChatMessageType type, const QString &body, const QString &login, const QString &password) {
    this->type = type;
    this->body = body;
    this->login = login;
    this->password = password;
}

ChatMessage::ChatMessage(ChatMessageType type, const QString &body)
    : ChatMessage(type, body, "", "") {
}

ChatMessage::ChatMessage(ChatMessageType type, const QString &login, const QString &password)
    : ChatMessage(type, "", login, password) {
}

QByteArray ChatMessage::toJson() const {
    QJsonObject obj;
    obj["type"] = type;
    obj["body"] = body;
    obj["login"] = login;
    obj["password"] = password;
    QByteArray result = QJsonDocument(obj).toJson(QJsonDocument::Indented);
    return result;
}

ChatMessage ChatMessage::fromJson(const QByteArray &data) {
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    auto type = (ChatMessageType) obj["type"].toInt();
    auto body = obj["body"].toString();
    auto login = obj["login"].toString();
    auto password = obj["password"].toString();
    return ChatMessage(type, body, login, password);
}
