#include "pch.h"
#include "SocketWorker.h"
#pragma warning(disable:4996)

SocketWorker::SocketWorker()
{
}


SocketWorker::~SocketWorker()
{
}

void SocketWorker::Init()
{
	mRecvEvent = CreateEvent(0, false, false, NULL);
	mWriteEvent = CreateEvent(0, false, false, NULL);

	for (int32 i = 0; i < 8; i++) //WorkerThread 8개
	{
		mThreadPool.emplace_back(thread(&SocketWorker::ThreadManage, this));
	}

	while (true)
	{
		RecvFrom();
	}
	
	for (auto& thread : mThreadPool)
		thread.join();
}

void SocketWorker::ThreadManage()
{
	HANDLE ThreadEvents[2] = { mRecvEvent, mWriteEvent };
	DWORD EventID = 0;
	
	while (true)
	{
		EventID = WaitForMultipleObjects(2, ThreadEvents, false, INFINITE);
		//EventID = WaitForSingleObject(mRecvEvent, INFINITE);
		switch (EventID)
		{
		case WAIT_OBJECT_0:
			this->ReadEvent();
			break;
		case WAIT_OBJECT_0 + 1:
			this->WriteEvent();
			break;
		default:
			break;
		}
	}
}

void SocketWorker::ReadEvent()
{
	//mReadQueue
	mLock.EnterWriteLock();
	DWORD dataLength = 0;
	char remoteAddress[32] = { 0 };
	uint16 remotePort = 0;

	if (!mReadQueue.Pop(mReadBuffer, dataLength, remoteAddress, remotePort))
		return;
	mLock.LeaveWriteLock();
	int32 PacketLength = 0;
	memcpy(&PacketLength, mReadBuffer, sizeof(int32));

	int32 PacketNumber = 0;
	memcpy(&PacketNumber, mReadBuffer + sizeof(int32), sizeof(int32));

	char packet[MAX_BUFFER_LENGTH];
	memcpy(&packet, mReadBuffer + sizeof(int32) * 2, PacketLength);

	auto message = GetMessage(packet);
	auto protocol = message->packet_type();

	switch (protocol)
	{
	case MESSAGE_ID::MESSAGE_ID_C2S_MOVE:
		auto RecvData = static_cast<const C2S_MOVE*>(message->packet());
		READ_PU_C2S_MOVE(RecvData);
		/*테스팅*/
		cout << remoteAddress << " : " << remotePort;
		/*
		Position pos{ 1.0f, 2.0f, 3.0f };
		Direction dir{ 1.0f, 2.0f, 3.0f };
		int32 size = 0;
		auto data = WRITE_PU_S2C_MOVE("Edea", pos, dir, size);
		WriteTo(remoteAddress, remotePort, data, size);
		*/
		//프로토콜에 따른 처리
	}
}

void SocketWorker::WriteEvent()
{
	//cout << "Write Event!!" << endl;
	mLock.EnterWriteLock();
	DWORD dataLength = 0;
	char remoteAddress[32] = { 0 };
	uint16 remotePort = 0;

	if (!mWriteQueue.Pop(mWriteBuffer, dataLength, remoteAddress, remotePort))
		return;
	mLock.LeaveWriteLock();

	mClientInfo.sin_port = htons(remotePort);
	mClientInfo.sin_family = AF_INET;
	mClientInfo.sin_addr.S_un.S_addr = inet_addr(remoteAddress);

	int32 returnVal = sendto(mSocket, mWriteBuffer, dataLength, 0, (SOCKADDR*)&mClientInfo, sizeof(mClientInfo));
	if (returnVal < 0)
		return;
}
