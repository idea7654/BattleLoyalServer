#pragma once
#include "Database.h"
inline auto READ_PU_C2S_MOVE(const C2S_MOVE *packet)
{
	string userName = packet->nick_name()->c_str();
	cout << "User Move!" << endl;
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

//C2S Protocol Define