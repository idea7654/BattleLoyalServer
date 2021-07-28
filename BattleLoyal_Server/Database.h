#pragma once
#include <mysql.h>
#pragma comment(lib, "libmySQL.lib")

class Database
{
public:
	Database() {};
	~Database() {};

public:
	bool SQL_INIT()
	{
		mysql_init(&Conn);
		ConnPtr = mysql_real_connect(&Conn, "127.0.0.1", "Admin", "osm980811", /*접속할 데이터베이스*/"userInfo", 3306, (char*)NULL, 0);
		if (ConnPtr == NULL)
		{
			cout << "MySql connect error" << endl;
			return false;
		}
		return true;
	}

	auto SQL_QUERY(char* query, bool &isSuccess)
	{
		//char *Query = "SELECT * FROM users";
		Stat = mysql_query(ConnPtr, query);
		if (Stat != 0)
		{
			isSuccess = false;
			return Result;
		}
		//락 걸기
		Result = mysql_store_result(ConnPtr);
		mysql_free_result(Result);
		isSuccess = true;
		return Result;
	}

private:
	MYSQL Conn;
	MYSQL* ConnPtr = NULL;
	MYSQL_RES* Result;
	MYSQL_ROW Row;
	int Stat;
};

static Database DBManager;