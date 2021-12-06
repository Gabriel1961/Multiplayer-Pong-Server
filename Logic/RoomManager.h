#pragma once
#include "RoomData.h"
#include <unordered_map>

struct ConnectedUserData
{
	int roomId;
	bool isHost;
}; 
template <typename T>
static size_t GetRaw(const std::weak_ptr<T> ptr){
	return *((size_t*)&(ptr));
}
struct ConnectionHdl
{
	websocketpp::connection_hdl hdl;
	friend bool operator==(const ConnectionHdl& a,const ConnectionHdl& b)
	{
		return GetRaw(a.hdl) == GetRaw(b.hdl);
	}
};
namespace std
{
	template<>
	struct hash< ConnectionHdl>
	{
		std::size_t operator()(const ConnectionHdl& k) const
		{
			return (size_t)GetRaw(k.hdl);
		}
	};
};
class RoomManager
{
public:
	static RoomManager* instance;
	std::unordered_map<int, RoomData> rooms;
	std::unordered_map< ConnectionHdl, ConnectedUserData> connectedUsers;
	int GenerateRoom(server*s,websocketpp::connection_hdl con1);
	int JoinRoom(websocketpp::connection_hdl con2,int roomId);
	void DeleteRoom(int id);
	void LeaveConnection(server*s,websocketpp::connection_hdl con);
};

