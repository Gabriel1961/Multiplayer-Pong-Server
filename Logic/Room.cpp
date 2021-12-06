#include "Room.h"
#include "RoomManager.h"
using namespace Room;  
Message* Room::Deserialize(const string& str)
{
    MessageType type;
    memcpy(&type, &str[0], sizeof(int));
    switch (type)
    {
    case MessageType::joinRoom:
        return (_Deserialize<JoinRoomMessage>(str));
    case MessageType::deleteRoom:
        return (_Deserialize<DeleteRoomMessage>(str));
    case MessageType::roomId:
        return (_Deserialize<RoomIdMessage>(str));
    case MessageType::dataInt:
        return (_Deserialize<DataIntMessage>(str));
    case MessageType::data3Vec2:
        return (_Deserialize<Data3Vec2Message>(str));
    case MessageType::score:
        return (_Deserialize<ScoreMessage>(str));
    default:
        return _Deserialize<Message>(str);
        break;
    }
}
// Define a callback to handle incoming messages
void Room::OnMessage(server* s, websocketpp::connection_hdl hdl, message_ptr msg) {
#ifdef ROOM_DEBUG
    std::cout << "on_message called with hdl: " << hdl.lock().get()
        << " and message: " << msg->get_payload()
        << std::endl;

    // check for a special command to instruct the server to stop listening so
    // it can be cleanly exited.
    if (msg->get_payload() == "stop-listening") {
        s->stop_listening();
        return;
    }
#endif
    if (msg->get_opcode() == websocketpp::frame::opcode::BINARY)
    {
        auto payload = msg->get_payload();
        auto msg = Deserialize(payload);
        if (msg->type == MessageType::hostRoom)
        {
            int id = RoomManager::instance->GenerateRoom(s,hdl);
            auto reply = make_unique<RoomIdMessage>(id);
            s->send(hdl, Serialize(*reply),websocketpp::frame::opcode::binary);
        }
        else if (msg->type == MessageType::joinRoom)
        {
            auto jmsg =(JoinRoomMessage*) msg;
            int joinStatusCode = RoomManager::instance->JoinRoom(hdl, jmsg->roomId);
            auto reply = make_unique<DataIntMessage>(joinStatusCode);
            s->send(hdl, Serialize(*reply), websocketpp::frame::opcode::binary);
           
            ///Start the game
            if (joinStatusCode == JoinStatus_Succes)
            {
                s->send(hdl, Serialize(Message(MessageType::startGame)), websocketpp::frame::opcode::binary);
                s->send(RoomManager::instance->rooms[jmsg->roomId].con1, Serialize(Message(MessageType::startGame)), websocketpp::frame::opcode::binary);
            }
        }
        else if (msg->type == MessageType::data3Vec2)
        {
            auto dmsg = (Data3Vec2Message*)msg;
            auto room = RoomManager::instance->rooms[RoomManager::instance->connectedUsers[{hdl}].roomId];
            if (GetRaw(hdl) == GetRaw(room.con1))
                s->send(room.con2, Serialize(*dmsg), websocketpp::frame::opcode::binary);
            else 
                s->send(room.con1, Serialize(*dmsg), websocketpp::frame::opcode::binary);
        }
        else if (msg->type == MessageType::score)
        {
            auto dmsg = (ScoreMessage*)msg;
            auto room = RoomManager::instance->rooms[RoomManager::instance->connectedUsers[{hdl}].roomId];
            s->send(room.con2, Serialize(*dmsg), websocketpp::frame::opcode::binary);
        }
        delete msg;
    }
}

void Room::OnOpen(server* s, websocketpp::connection_hdl con)
{

}

void Room::OnClose(server* s, websocketpp::connection_hdl con)
{
    RoomManager::instance->LeaveConnection(s, con);
}
