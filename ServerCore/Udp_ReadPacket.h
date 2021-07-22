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

//C2S Protocol Define