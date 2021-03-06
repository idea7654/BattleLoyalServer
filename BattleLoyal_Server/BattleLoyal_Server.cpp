#include "pch.h"
#include "CorePch.h"
#include "Database.h"
#include "flatbuffers/flatbuffers.h"
#include "SocketWorker.h"

int main()
{
	cout << "Start!!" << endl;
	MiniDump::MiniDump();
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

	testIocp->Send();
	getchar();
	//네트워크
	WSACleanup();
	*/
	SocketWorker *socketWorker = new SocketWorker();

	if (!socketWorker->Begin())
		return false;
	if (!socketWorker->Bind(9999))
		return false;
	
	thread t1(&SocketWorker::Init, socketWorker);
	//컨텐츠만 넣으면 작동
	thread t2(&SocketWorker::ReduceSessionTime, socketWorker);

	t1.join();
	t2.join();
	if (!socketWorker->End())
		return false;
	delete socketWorker;
	//getchar();
}