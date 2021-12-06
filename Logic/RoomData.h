#pragma once
#include "../IncludesAndDefines.h"
struct RoomData
{
	int scoreP1=0;
	int scoreP2=0;
	int id;
	websocketpp::connection_hdl con1;
	websocketpp::connection_hdl con2;
};

