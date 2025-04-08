//
// Created by kat on 4/6/2025.
//

#include "ChatMessage.h"

#include <QJsonObject>
#include <QJsonDocument>


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
    ChatMessage result;
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    result.type = (ChatMessageType)obj["type"].toInt();
    result.body = obj["body"].toString();
    result.login = obj["login"].toString();
    result.password = obj["password"].toString();
    return result;
}
