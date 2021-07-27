#pragma once

//#include "flatbuffers/flatbuffers.h"
//#include "../Packet/UdpProtocol_generated.h"
static flatbuffers::FlatBufferBuilder builder(1024);

inline auto WRITE_PU_S2C_MOVE(string nickname, Position &pos, Direction &dir, int32 &refLength)
{
	auto playerName = builder.CreateString(nickname);
	auto playerPos = Vec3(pos.x, pos.y, pos.z);
	auto playerDir = Vec3(dir.x, dir.y, dir.z);

	auto makePacket = CreateS2C_MOVE(builder, playerName, &playerPos, &playerDir);
	auto packet = CreateMessage(builder, MESSAGE_ID::MESSAGE_ID_S2C_MOVE, makePacket.Union());

	builder.Finish(packet);

	auto data = builder.GetBufferPointer();
	refLength = builder.GetSize();

	builder.Clear();
	return data;
}

inline auto WRITE_PU_S2C_LOGIN_ERROR(string errorMessage, int32 &refLength)
{
	auto errMessage = builder.CreateString(errorMessage);
	auto makePacket = CreateS2C_LOGIN_ERROR(builder, errMessage);
	auto packet = CreateMessage(builder, MESSAGE_ID::MESSAGE_ID_S2C_LOGIN_ERROR, makePacket.Union());

	builder.Finish(packet);
	refLength = builder.GetSize();

	auto data = builder.GetBufferPointer();
	builder.Clear();
	return data;
}

//inline auto WRITE_PU_S2C_LOGIN_SUCCESS

//1. Protocol 노필요.
//2. 대신에 Packet내용은 여기서 리턴된 data가 들어감
//3. 따라서 최종 패킷은 이렇게 될 것임
// | ushort(Length) | ushort(PacketNum) | flatBuffers Byte() |