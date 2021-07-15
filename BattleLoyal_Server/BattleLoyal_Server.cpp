#include "pch.h"
//#include "CorePch.h"
#include "TestIocp.h"

int main()
{
	cout << "Start!!" << endl;
	//chrono::system_clock::time_point start = chrono::system_clock::now();
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
	{
		printf("Error - Can not load 'winsock.dll' file\n");
		return 1;
	}
	TestIocp *testIocp = new TestIocp();
	if (testIocp->Begin())
		cout << "실행성공!" << endl;
	//chrono::system_clock::time_point end = chrono::system_clock::now();
	//cout << "끝: " << end.time_since_epoch().count() - start.time_since_epoch().count() << endl;
	//testIocp->End();
	//delete testIocp;
	getchar();

	testIocp->End();
	WSACleanup();
}