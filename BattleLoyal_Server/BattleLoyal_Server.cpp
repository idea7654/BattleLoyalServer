#include "pch.h"
#include <iostream>
#include "CorePch.h"
class CDataMP// : public MemoryPool<CDataMP>
{
public:
	~CDataMP() { /*cout << "제대로 삭제됨!" << endl;*/ };
public:
	BYTE a[1024];
	BYTE b[1024];
	int32 c = 3;
};

int main()
{
	cout << "Start!!" << endl;
	chrono::system_clock::time_point start = chrono::system_clock::now();
	for (int32 i = 0; i < 1000000; i++)
	{
		//CDataMP *pData = new CDataMP();
		//delete pData;
		auto a = MakeShared<CDataMP>();
		//make_shared<CDataMP>();
	}
	chrono::system_clock::time_point end = chrono::system_clock::now();
	cout << "끝: " << end.time_since_epoch().count() - start.time_since_epoch().count() << endl;
	getchar();
}