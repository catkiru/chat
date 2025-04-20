//
// Created by kat on 4/6/2025.
//

#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H
#include <qjsonobject.h>
#include <qstring.h>

enum ChatMessageType {
    TextMessage,
    Auth,
    Image,
    AuthResult,
    Register,
    History
};

class ChatMessage {
public:
    ChatMessage(ChatMessageType type, const QString &body, const QString &login, const QString &password);

    ChatMessage(ChatMessageType type, const QString &body);

    ChatMessage(ChatMessageType type, const QString &login, const QString &password);

    ChatMessage(ChatMessageType type);

    static QJsonObject toJsonObject(const ChatMessage* msg);

    inline static const QString message_type = "message_type";

    ChatMessageType type;
    QString body;
    QString login;
    QString password;
    QString from;
    QByteArray image;
    QList<ChatMessage> history;

    QByteArray toJson() const;

    static ChatMessage fromJson(const QByteArray &data);
};


#endif //CHATMESSAGE_H
