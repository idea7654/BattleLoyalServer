#include "pch.h"
#include "CorePch.h"
#include "TestIocp.h"
// 비정상 종료되는 함수

int main()
{
	cout << "Start!!" << endl;
	MiniDump::MiniDump();
	
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
	{
		printf("Error - Can not load 'winsock.dll' file\n");
		return 1;
	}
	TestIocp *testIocp = new TestIocp();
	if (testIocp->Begin())
		cout << "실행성공!" << endl;

	testIocp->Send();
	getchar();
	//네트워크
	WSACleanup();
}