#include "pch.h"
#include "CorePch.h"
#include "TestIocp.h"
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

	Logger test(LOG_LEVEL_WARN);

	test.fatal("fatal", 1);
}