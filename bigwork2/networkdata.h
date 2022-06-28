#ifndef NETWORKDATA_H
#define NETWORKDATA_H

#include <QtCore>

enum class OPCODE : int {
    JOIN_ROOM_OP = 200000,
    JOIN_ROOM_REPLY_OP,
    LEAVE_ROOM_OP,
    CLOSE_ROOM_OP,
    PLAYER_READY_OP,
    START_GAME_OP,
    START_TURN_OP,
    MOVE_OP,
    END_TURN_OP,
    END_GAME_OP,
    ERROR_OP,
};

enum class ERRCODE : int {
    NOT_IN_ROOM = 400000,
    ROOM_IS_RUNNING,
    ROOM_NOT_RUNNING,
    INVALID_JOIN,
    OUTTURN_MOVE,
    INVALID_MOVE,
    INVALID_REQ,
    OTHER_ERROR,
};

class InvalidMessage : public QException {
public:
    InvalidMessage() = delete;
    InvalidMessage(QByteArray message);
    QByteArray messageReceived() const;

private:
    QByteArray message;
};
class NetworkData {
public:
    NetworkData() = delete;
    NetworkData(OPCODE op, QString data1, QString data2);
    NetworkData(QByteArray message);

    OPCODE op;
    QString data1;
    QString data2;

    QByteArray encode();
};

#endif // NETWORKDATA_H
