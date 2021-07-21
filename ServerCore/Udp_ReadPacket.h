#pragma once

#include "flatbuffers/flatbuffers.h"
#include "../Packet/UdpProtocol_generated.h"
//static flatbuffers::FlatBufferBuilder builder(1024);

inline auto READ_PU_C2S_MOVE(const C2S_MOVE *packet)
{
	string userName = packet->nick_name()->c_str();
	cout << "유저가 이동했습니다!" << endl;
}

//C2S Protocol 처리 정의