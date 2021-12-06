#include "RoomManager.h"
#include "Room.h"
RoomManager* RoomManager::instance = 0;
std::mutex genRoomMtx;
std::mutex joinRoomMtx;
std::mutex deleteRoomMtx;
std::mutex leaveConnection;


int RoomManager::GenerateRoom(server*s,websocketpp::connection_hdl con1)
{
	std::unique_lock<std::mutex> lk(genRoomMtx);
	int id = rand();
	RoomData data;
	data.id = id;
	data.con1 = con1;
	
	if (connectedUsers.find({ con1 }) != connectedUsers.end())
	{
		auto user = connectedUsers.find({ con1 });
		user->second.isHost = true;
		LeaveConnection(s, rooms[user->second.roomId].con2); // Remove user connected to the old room
		rooms.erase(user->second.roomId); // Remove old room
		user->second.roomId = id;
	}
	else
	{
		connectedUsers.insert({ {con1},{id,true} });
	}
	
	rooms.insert({ id, data });
	
	return id;
}

/// Returns 0 if the joining procces was succesful 
/// else it returns -1 if the room is invalid or if the user 
/// is already connected in another room
int RoomManager::JoinRoom(websocketpp::connection_hdl con2,int roomId)
{
	std::unique_lock<std::mutex> lk(joinRoomMtx);
	if (connectedUsers.find({con2}) != connectedUsers.end())
		return Room::JoinStatus_AlreadyInRoom;
	if (rooms.find(roomId) == rooms.end())
		return Room::JoinStatus_InvalidRoomCode;
	connectedUsers.insert({ {con2},{roomId,false} });
	rooms.find(roomId)->second.con2 = con2;
	return Room::JoinStatus_Succes;
}

void RoomManager::DeleteRoom(int id)
{
	std::unique_lock<std::mutex> lk(deleteRoomMtx);
	if(rooms.find(id) != rooms.end())
		rooms.erase(id);
}

void RoomManager::LeaveConnection(server* s, websocketpp::connection_hdl con)
{
	std::unique_lock<std::mutex> lk(leaveConnection);
	auto user = connectedUsers.find({con});
	if (user == connectedUsers.end())
		return;
	else
	{
		auto& userData = user->second;
		if (userData.isHost)
		{
			auto room = rooms.find(userData.roomId);
			if (room != rooms.end())
			{
				auto otherUser = connectedUsers.find({ room->second.con2 });
				if (otherUser != connectedUsers.end())
				{///Disconnect the other connected user
					s->pause_reading(room->second.con2);
					s->close(room->second.con2, websocketpp::close::status::going_away, "");
					connectedUsers.erase(otherUser);
				}
				DeleteRoom(userData.roomId);
			}
		}
		s->pause_reading(con);
		s->close(con, websocketpp::close::status::going_away, "");
		connectedUsers.erase(user);
	}
}
               