/*
#pragma once

#include "flatbuffers/flatbuffers.h"
#include "../Packet/UdpProtocol_generated.h"
//static flatbuffers::FlatBufferBuilder builder(1024);

inline auto READ_PU_C2S_MOVE(const C2S_MOVE *packet)
{
	string userName = packet->nick_name()->c_str();
	cout << "User Move!" << endl;
}

inline auto READ_PU_C2S_EXTEND_SESSION(const C2S_EXTEND_SESSION* packet, vector<Session> &sessions)
{
	string userName = packet->nick_name()->c_str();
	for (auto &i : sessions)
	{
		if (i.nickname == userName)
		{
			i.isOnline = 10;
			return;
		}
	}
}

inline auto READ_PU_C2S_REQUEST_LOGIN(const C2S_REQUEST_LOGIN* packet)
{
	string email = packet->email()->c_str();
	string password = packet->password()->c_str();
	//여기에 디비조회처리하면됨...
	
}

inline auto Read_PU_C2S_REQUEST_REGISTER(const C2S_REQUEST_REGISTER* packet)
{
	string email = packet->email()->c_str();
	string password = packet->password()->c_str();
	//여기에 디비 만들기 처리하면됨
}

//C2S Protocol Define
*/