#pragma once
#include "Database.h"
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
	//char *Query = "SELECT * FROM users WHERE email="; //Query완성하고..
	string Query = "SELECT * FROM users WHERE email=" + email + ";";
	auto result = DBManager.SQL_QUERY((char*)Query.c_str());
	MYSQL_ROW Row;
	while ((Row = mysql_fetch_row(result)) != NULL)
	{
		cout << Row[0] << endl << Row[1] << endl << Row[2] << endl;
	}
	//여기서 값 있는지 체크해서 있으면 해당 정보 읽어서 WritePacket리턴
	return true;
}

inline auto Read_PU_C2S_REQUEST_REGISTER(const C2S_REQUEST_REGISTER* packet)
{
	string email = packet->email()->c_str();
	string password = packet->password()->c_str();
	//여기에 디비 만들기 처리하면됨
}

//C2S Protocol Define