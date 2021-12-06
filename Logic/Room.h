#pragma once
#include "../IncludesAndDefines.h"
#include "../ExceptionLogger.h"
//#define ROOM_DEBUG
namespace Room
{
    using namespace std;
    enum class MessageType
    {
        none,
        hostRoom,
        roomId,
        deleteRoom,
        leaveRoom,
        joinRoom,
        dataInt,
        data3Vec2,
        startGame,
        stopGame,
        score
    };
    enum JoinStatus
    {
        JoinStatus_NONE = 0,
        JoinStatus_Succes = 1,
        JoinStatus_InvalidRoomCode = -1,
        JoinStatus_AlreadyInRoom = -2,
    };
    struct Message
    {
        Message() {}
        Message(MessageType type) : type(type) {}
        MessageType type;
    };
    struct JoinRoomMessage : public Message
    {
        JoinRoomMessage() : Message(MessageType::joinRoom) {  }
        JoinRoomMessage(int id) : roomId(id), Message(MessageType::joinRoom) {  }
        int roomId;
    };
    struct DeleteRoomMessage : public Message
    {
        DeleteRoomMessage() : Message(MessageType::deleteRoom) { }
        DeleteRoomMessage(int id) : roomId(id), Message(MessageType::deleteRoom) { }
        int roomId;
    };
    struct RoomIdMessage : public Message
    {
        RoomIdMessage() : Message(MessageType::roomId) { }
        RoomIdMessage(int id) : roomId(id), Message(MessageType::roomId) { }
        int roomId;
    };
    struct DataIntMessage : public Message
    {
        DataIntMessage() : Message(MessageType::dataInt) { }
        DataIntMessage(int val) : val(val), Message(MessageType::dataInt) { }
        int val;
    };
    struct ScoreMessage : public Message
    {
        ScoreMessage() : Message(MessageType::score) { }
        ScoreMessage(int p1, int p2) : p1(p1), p2(p2), Message(MessageType::score) { }
        int p1, p2;

    };
    struct MsgVec2
    {
        float x, y;
    };

    struct Data3Vec2Message : public Message
    {
        Data3Vec2Message() : Message(MessageType::data3Vec2) { }
        MsgVec2 v1 = { 0,0 }, v2 = { 0,0 }, v3 = { 0,0 };
    };

    template<class T>
    std::string Serialize(const T& message)
    {
        int size = sizeof(message);
        std::string s(size, ' ');
        memcpy(&s[0], &message, size);
        return s;
    }

    template<class T>
    T* _Deserialize(const std::string& str)
    {
        T* mes = new T();
        memcpy(mes, &str[0], sizeof(T));
        return mes;
    }
    Message* Deserialize(const string& str);
    // Define a callback to handle incoming messages
    void OnMessage(server* s, websocketpp::connection_hdl hdl, message_ptr msg);

    void OnOpen(server* s, websocketpp::connection_hdl con);
    
    void OnClose(server* s, websocketpp::connection_hdl con);
}
