//
// Created by kat on 4/6/2025.
//

#include "ChatMessage.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

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

ChatMessage::ChatMessage(ChatMessageType type): ChatMessage(type, "") {
}

QJsonObject ChatMessage::toJsonObject(const ChatMessage *msg) {
    QJsonObject obj;
    obj["type"] = msg->type;
    obj["body"] = msg->body;
    obj["login"] = msg->login;
    obj["password"] = msg->password;
    obj["from"] = msg->from;
    obj["image"] = QString::fromUtf8(msg->image.toBase64());
    return obj;
}

QByteArray ChatMessage::toJson() const {
    QJsonObject obj = ChatMessage::toJsonObject(this);

    QJsonArray historyArray;
    for (const auto &msg: history) {
        // QJsonDocument subDoc = QJsonDocument::fromJson(msg.toJson());
        historyArray.append(ChatMessage::toJsonObject(&msg));
    }
    obj["history"] = historyArray;
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
    auto from = obj["from"].toString();
    auto image = QByteArray::fromBase64(obj["image"].toString().toUtf8());
    auto result = ChatMessage(type, body, login, password);
    result.from = from;
    result.image = image;
    // Десериализация истории
    if (obj.contains("history") && obj["history"].isArray()) {
        QJsonArray historyArray = obj["history"].toArray();
        for (const auto &value: historyArray) {
            if (value.isObject()) {
                QJsonObject subObj = value.toObject();
                QByteArray subData = QJsonDocument(subObj).toJson();
                result.history.append(ChatMessage::fromJson(subData));
            }
        }
    }
    return result;
}
