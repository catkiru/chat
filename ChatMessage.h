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
    AuthResult
};

class ChatMessage {

public:
    ChatMessage(ChatMessageType type, const QString & body, const QString & chars, const QString & string);
    ChatMessage(ChatMessageType type, const QString & body);
    ChatMessage(ChatMessageType type, const QString & body, const QString & password);

    inline static const QString message_type = "message_type";

    ChatMessageType type;
    QString body;
    QString login;
    QString password;
    QString from;

    QByteArray toJson() const;

    static ChatMessage fromJson(const QByteArray &data);
};


#endif //CHATMESSAGE_H
