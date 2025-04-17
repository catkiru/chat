//
// Created by kat on 4/6/2025.
//

#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H
#include <qstring.h>

enum ChatMessageType {
    TextMessage,
    Auth,
    Image
};

class ChatMessage {
    ChatMessage(ChatMessageType type, const QString & body, const QString & chars, const QString & string);

public:
    inline static const QString message_type = "message_type";

    ChatMessageType type;
    QString body;
    QString login;
    QString password;

    QByteArray toJson() const;

    static ChatMessage fromJson(const QByteArray &data);
};


#endif //CHATMESSAGE_H
