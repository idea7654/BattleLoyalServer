#pragma once

static flatbuffers::FlatBufferBuilder builder(1024);

inline auto WRITE_PU_S2C_MOVE(string nickname, Position &pos, Direction &dir, float vfront, float vright, float vyaw, bool isJump, bool isCrouch, int32 &refLength)
{
	auto playerName = builder.CreateString(nickname);
	auto playerPos = Vec3(pos.x, pos.y, pos.z);
	auto playerDir = Vec3(dir.x, dir.y, dir.z);

	auto makePacket = CreateS2C_MOVE(builder, playerName, &playerPos, &playerDir, vfront, vright, vyaw, isJump, isCrouch);
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

inline auto WRITE_PU_S2C_GAME_START(int32 &refLength, vector<shared_ptr<ContentSession>> &Users, Position pos, vector<shared_ptr<SessionGun>> &SessionGun, vector<Position> &RoundPos)
{
	vector<flatbuffers::Offset<InitUserInfo>> user_vector;
	vector<flatbuffers::Offset<InitGunInfo>> gun_vector;
	vector<flatbuffers::Offset<RoundInfo>> round_vector;
	for (auto &i : Users)
	{
		auto UserNicks = builder.CreateString(i->nickname);
		auto playerPos = Vec3(i->pos.x, i->pos.y, i->pos.z);
		auto makeTable = CreateInitUserInfo(builder, UserNicks, &playerPos);
		user_vector.push_back(makeTable);
	}

	for (auto &i : SessionGun)
	{
		int32 type = (int32)i->Gun_Type;
		auto gunPos = Vec3(i->Gun_pos.x, i->Gun_pos.y, i->Gun_pos.z);
		auto makeTable = CreateInitGunInfo(builder, type, &gunPos);
		gun_vector.push_back(makeTable);
	}

	for (int i = 0; i < RoundPos.size(); i++)
	{
		int32 round = i;
		auto roundPos = Vec3(RoundPos[i].x, RoundPos[i].y, RoundPos[i].z);
		auto makeTable = CreateRoundInfo(builder, round, &roundPos);
		round_vector.push_back(makeTable);
	}
	auto datas = builder.CreateVector(user_vector);
	auto guns = builder.CreateVector(gun_vector);
	auto rounds = builder.CreateVector(round_vector);
	auto makePacket = CreateS2C_GAME_START(builder, datas, guns, rounds);
	auto packet = CreateMessage(builder, MESSAGE_ID::MESSAGE_ID_S2C_GAME_START, makePacket.Union());

	builder.Finish(packet);
	refLength = builder.GetSize();

	auto data = builder.GetBufferPointer();
	builder.Clear();
	return data;
}

inline auto WRITE_PU_S2C_PICKUP_GUN(int32 &refLength, string nickname, int32 gunNum)
{
	auto nickName = builder.CreateString(nickname);
	auto makePacket = CreateS2C_PICKUP_GUN(builder, nickName, gunNum);
	auto packet = CreateMessage(builder, MESSAGE_ID::MESSAGE_ID_S2C_PICKUP_GUN, makePacket.Union());

	builder.Finish(packet);
	refLength = builder.GetSize();

	auto data = builder.GetBufferPointer();
	builder.Clear();
	return data;
}

inline auto WRITE_PU_S2C_USER_NOT_FOUND(int32 &refLength)
{
	auto message = builder.CreateString("Error!");
	auto makePacket = CreateS2C_USER_NOT_FOUND(builder, message);
	auto packet = CreateMessage(builder, MESSAGE_ID::MESSAGE_ID_S2C_USER_NOT_FOUND, makePacket.Union());

	builder.Finish(packet);
	refLength = builder.GetSize();

	auto data = builder.GetBufferPointer();
	builder.Clear();
	return data;
}

inline auto WRITE_PU_S2C_USER_DISCONNECT(int32 &refLength, string nickname)
{
	auto nickName = builder.CreateString(nickname);
	auto makePacket = CreateS2C_USER_DISCONNECT(builder, nickName);
	auto packet = CreateMessage(builder, MESSAGE_ID::MESSAGE_ID_S2C_USER_DISCONNECT, makePacket.Union());

	builder.Finish(packet);
	refLength = builder.GetSize();

	auto data = builder.GetBufferPointer();
	builder.Clear();
	return data;
}

inline auto WRITE_PU_S2C_SHOOT(int32 &refLength, string nickname, string target, float damage)
{
	auto nickName = builder.CreateString(nickname);
	auto targetName = builder.CreateString(target);
	auto makePacket = CreateS2C_SHOOT(builder, nickName, targetName, damage);
	auto packet = CreateMessage(builder, MESSAGE_ID::MESSAGE_ID_S2C_SHOOT, makePacket.Union());

	builder.Finish(packet);
	refLength = builder.GetSize();

	auto data = builder.GetBufferPointer();
	builder.Clear();
	return data;
}

inline auto WRITE_PU_S2C_PLAYER_DIE(int32 &refLength, string nickname, string target, string type, int32 combo = 0)
{
	auto nickName = builder.CreateString(nickname);
	auto targetName = builder.CreateString(target);
	auto dieType = builder.CreateString(type);
	auto makePacket = CreateS2C_PLAYER_DIE(builder, nickName, targetName, dieType, combo);
	auto packet = CreateMessage(builder, MESSAGE_ID::MESSAGE_ID_S2C_PLAYER_DIE, makePacket.Union());

	builder.Finish(packet);
	refLength = builder.GetSize();

	auto data = builder.GetBufferPointer();
	builder.Clear();
	return data;
}

inline auto WRITE_PU_S2C_USER_VICTORY(int32 &refLength, string nickname)
{
	auto nickName = builder.CreateString(nickname);
	auto makePacket = CreateS2C_USER_VICTORY(builder, nickName);
	auto packet = CreateMessage(builder, MESSAGE_ID::MESSAGE_ID_S2C_USER_VICTORY, makePacket.Union());

	builder.Finish(packet);
	refLength = builder.GetSize();

	auto data = builder.GetBufferPointer();
	builder.Clear();
	return data;
}

inline auto WRITE_PU_S2C_MELEE_ATTACK(int32 &refLength, string nickname, string target, float damage, int32 combo)
{
	auto nickName = builder.CreateString(nickname);
	auto targetActor = builder.CreateString(target);
	auto makePacket = CreateS2C_MELEE_ATTACK(builder, nickName, targetActor, damage, combo);
	auto packet = CreateMessage(builder, MESSAGE_ID::MESSAGE_ID_S2C_MELEE_ATTACK, makePacket.Union());

	builder.Finish(packet);
	refLength = builder.GetSize();

	auto data = builder.GetBufferPointer();
	builder.Clear();
	return data;
}

inline auto WRITE_PU_S2C_EQUIP_GUN(int32 &refLength, string nickname, bool state)
{
	auto nickName = builder.CreateString(nickname);
	auto makePacket = CreateS2C_EQUIP_GUN(builder, nickName, state);
	auto packet = CreateMessage(builder, MESSAGE_ID::MESSAGE_ID_S2C_EQUIP_GUN, makePacket.Union());

	builder.Finish(packet);
	refLength = builder.GetSize();

	auto data = builder.GetBufferPointer();
	builder.Clear();
	return data;
}

inline auto WRITE_PU_S2C_CHANGE_GUN(int32 &refLength, string nickname, int32 originID, int32 nowID)
{
	auto nickName = builder.CreateString(nickname);
	auto makePacket = CreateS2C_CHANGE_GUN(builder, nickName, originID, nowID);
	auto packet = CreateMessage(builder, MESSAGE_ID::MESSAGE_ID_S2C_CHANGE_GUN, makePacket.Union());

	builder.Finish(packet);
	refLength = builder.GetSize();

	auto data = builder.GetBufferPointer();
	builder.Clear();
	return data;
}

inline auto WRITE_PU_S2C_ROUND_START(int32 &refLength, int32 round)
{
	auto makePacket = CreateS2C_START_ROUND(builder, round);
	auto packet = CreateMessage(builder, MESSAGE_ID::MESSAGE_ID_S2C_START_ROUND, makePacket.Union());

	builder.Finish(packet);
	refLength = builder.GetSize();

	auto data = builder.GetBufferPointer();
	builder.Clear();
	return data;
}

inline auto WRITE_PU_S2C_SET_USER_POSITION(int32 &refLength, string nickname, int32 sector)
{
	auto nickName = builder.CreateString(nickname);
	auto makePacket = CreateS2C_SET_USER_POSITION(builder, nickName, sector);
	auto packet = CreateMessage(builder, MESSAGE_ID::MESSAGE_ID_S2C_SET_USER_POSITION, makePacket.Union());

	builder.Finish(packet);
	refLength = builder.GetSize();

	auto data = builder.GetBufferPointer();
	builder.Clear();
	return data;
}

inline auto WRITE_PU_S2C_START_SIGN(int32 &refLength, vector<shared_ptr<ContentSession>> &Users)
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
	auto makePacket = CreateS2C_START_SIGN(builder, datas);
	auto packet = CreateMessage(builder, MESSAGE_ID::MESSAGE_ID_S2C_START_SIGN, makePacket.Union());

	builder.Finish(packet);
	refLength = builder.GetSize();

	auto data = builder.GetBufferPointer();
	builder.Clear();
	return data;
}