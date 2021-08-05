#pragma once

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

inline auto WRITE_PU_S2C_COMPLETE_LOGIN(string nickname, int32 &refLength)
{
	auto mNickname = builder.CreateString(nickname);
	auto makePacket = CreateS2C_COMPLETE_LOGIN(builder, mNickname);
	auto packet = CreateMessage(builder, MESSAGE_ID::MESSAGE_ID_S2C_COMPLETE_LOGIN, makePacket.Union());

	builder.Finish(packet);
	refLength = builder.GetSize();

	auto data = builder.GetBufferPointer();
	builder.Clear();
	return data;
}

inline auto WRITE_PU_S2C_COMPLETE_REGISTER(int32 &refLength)
{
	bool isSuccess = true;
	auto makePacket = CreateS2C_COMPLETE_REGISTER(builder, isSuccess);
	auto packet = CreateMessage(builder, MESSAGE_ID::MESSAGE_ID_S2C_COMPLETE_REGISTER, makePacket.Union());

	builder.Finish(packet);
	refLength = builder.GetSize();

	auto data = builder.GetBufferPointer();
	builder.Clear();
	return data;
}

inline auto WRITE_PU_S2C_REGISTER_ERROR(int32 &refLength, string errMessage)
{
	auto message = builder.CreateString(errMessage);
	auto makePacket = CreateS2C_REGISTER_ERROR(builder, message);
	auto packet = CreateMessage(builder, MESSAGE_ID::MESSAGE_ID_S2C_REGISTER_ERROR, makePacket.Union());

	builder.Finish(packet);
	refLength = builder.GetSize();

	auto data = builder.GetBufferPointer();
	builder.Clear();
	return data;
}

inline auto WRITE_PU_S2C_GAME_START(int32 &refLength, vector<shared_ptr<ContentSession>> &Users, Position pos)
{
	vector<flatbuffers::Offset<InitUserInfo>> user_vector;
	for (auto &i : Users)
	{
		auto UserNicks = builder.CreateString(i->nickname);
		auto playerPos = Vec3(i->pos.x, i->pos.y, i->pos.z);
		auto makeTable = CreateInitUserInfo(builder, UserNicks, &playerPos);
		user_vector.push_back(makeTable);
	}
	auto datas = builder.CreateVector(user_vector);
	auto makePacket = CreateS2C_GAME_START(builder, datas);
	auto packet = CreateMessage(builder, MESSAGE_ID::MESSAGE_ID_S2C_GAME_START, makePacket.Union());

	builder.Finish(packet);
	refLength = builder.GetSize();

	auto data = builder.GetBufferPointer();
	builder.Clear();
	return data;
}