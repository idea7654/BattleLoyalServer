#include "pch.h"
#include "CorePch.h"
#include "TestIocp.h"
// 비정상 종료되는 함수
void Func()
{
	char szBuf[1];

	memset(szBuf, 0, sizeof(szBuf));

	printf("start\n");
	printf("%s\n", szBuf[1000000]);
	printf("end\n");
}


int main()
{
	cout << "Start!!" << endl;
	/*
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
	{
		printf("Error - Can not load 'winsock.dll' file\n");
		return 1;
	}
	TestIocp *testIocp = new TestIocp();
	if (testIocp->Begin())
		cout << "실행성공!" << endl;
	
	getchar();
	//네트워크
	WSACleanup();
	*/
	MiniDump::MiniDump();
	//Func();

	string a = "asdf";
	string b = "erqwer";
	string c = to_string(2341);
	cout << c << endl;
}