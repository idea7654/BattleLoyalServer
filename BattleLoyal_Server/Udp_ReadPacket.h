#pragma once
#include "Database.h"
inline auto READ_PU_C2S_MOVE(const C2S_MOVE *packet, string &userName, Position &userPos, Direction &userDir, float &vfront, float &vright, float &vyaw, bool &isJump, bool &isCrouch)
{
	userName = packet->nick_name()->c_str();
	userPos = Position{ packet->pos()->x(), packet->pos()->y(), packet->pos()->z() };
	userDir = Direction{ packet->dir()->x(), packet->dir()->y(), packet->dir()->z() };
	vfront = packet->vfront();
	vright = packet->vright();
	vyaw = packet->vyaw();
	isJump = packet->jump();
	isCrouch = packet->crouch();
}

inline auto READ_PU_C2S_EXTEND_SESSION(const C2S_EXTEND_SESSION* packet, vector<shared_ptr<Session>> &sessions)
{
	string userName = packet->nick_name()->c_str();
	for (auto &i : sessions)
	{
		if (i->nickname == userName)
		{
			i->isOnline = 10;
			return;
		}
	}
}

inline auto READ_PU_C2S_REQUEST_LOGIN(const C2S_REQUEST_LOGIN* packet)
{
	bool isSuccess = false;
	string email = packet->email()->c_str();
	string password = packet->password()->c_str();
	string Query = "SELECT * FROM users WHERE email='" + email + "';";
	auto result = DBManager.SQL_QUERY((char*)Query.c_str(), isSuccess);

	MYSQL_ROW Row;

	if (!isSuccess)
	{
		return (char*)"Incorrect_Email";
	}

	if (result == 0)
	{
		return (char*)"Incorrect_Email";
	}
	
	while ((Row = mysql_fetch_row(result)))
	{
		if (Row[2] == password)
		{
			return Row[1];
		}
		//S2C_COMPLETE_LOGIN
		else
		{
			//cout << "Login Failed!!" << endl; //password incorrect
			return (char*)"Incorrect_Email";
		}
	}

	return (char*)"Incorrect_Email";
}

inline auto READ_PU_C2S_REQUEST_REGISTER(const C2S_REQUEST_REGISTER* packet, char* remoteAddress, uint16 remotePort)
{
	bool isSuccess = false;
	string email = packet->email()->c_str();
	string nickname = packet->nickname()->c_str();
	string password = packet->password()->c_str();

	string Query = "INSERT INTO users VALUES('" + email + "', '" + nickname + "', '" + password + "')";
	auto result = DBManager.SQL_QUERY((char*)Query.c_str(), isSuccess);

	return isSuccess;
}

inline auto READ_PU_C2S_START_MATCHING(const C2S_START_MATCHING* packet)
{
	string nickname = packet->nickname()->c_str();
	return nickname;
}

inline auto READ_PU_C2S_CANCEL_MATCHING(const C2S_CANCEL_MATCHING* packet)
{
	string nickname = packet->nickname()->c_str();
	return nickname;
}

inline auto READ_PU_C2S_PICKUP_GUN(const C2S_PICKUP_GUN *packet, int32 &gunNum)
{
	string nickname = packet->nickname()->c_str();
	gunNum = packet->gunnum();
	return nickname;
}

inline void READ_PU_C2S_SHOOT(const C2S_SHOOT *packet, string &nickname, string &target, float &damage)
{
	nickname = packet->nickname()->c_str();
	target = packet->target()->c_str();
	damage = packet->damage();
}

inline void READ_PU_C2S_MELEE_ATTACK(const C2S_MELEE_ATTACK *packet, string &nickname, string &target, int32 &combo)
{
	nickname = packet->nickname()->c_str();
	target = packet->target()->c_str();
	combo = packet->combo();
}

inline void READ_PU_C2S_EQUIP_GUN(const C2S_EQUIP_GUN *packet, string &nickname, bool &state)
{
	nickname = packet->nickname()->c_str();
	state = packet->state();
}

inline void READ_PU_C2S_CHANGE_GUN(const C2S_CHANGE_GUN *packet, string &nickname, int32 &originID, int32 &nowID)
{
	nickname = packet->nickname()->c_str();
	originID = packet->originid();
	nowID = packet->nowid();
}

inline void READ_PU_C2S_SET_USER_POSITION(const C2S_SET_USER_POSITION *packet, string &nickname, int32 &sector)
{
	nickname = packet->nickname()->c_str();
	sector = packet->sector();
}

inline void READ_PU_C2S_ZONE_DAMAGE(const C2S_ZONE_DAMAGE *packet, string &nickname, int32 &damage)
{
	nickname = packet->nickname()->c_str();
	damage = packet->damage();
}

inline void READ_PU_C2S_RECOVER_HP(const C2S_RECOVER_HP *packet, string &nickname, int32 &objID)
{
	nickname = packet->nickname()->c_str();
	objID = packet->obj();
}
//C2S Protocol Define