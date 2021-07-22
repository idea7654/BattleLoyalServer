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

	for (int32 i = 0; i < 8; i++) //8 WorkerThreads
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
	int32 remainLength = dataLength;
	int32 NextPacket = 0;
RETRY:
	int32 PacketLength = 0;
	::memcpy(&PacketLength, mReadBuffer + NextPacket, sizeof(int32));

	Session target = this->FindSession(remoteAddress, remotePort);
	if (PacketLength == 8888)
	{
		ResetSessionTime(target);
	} //USE this or C2S_EXTEND_SESSION Packet

	//Ack 9999 -> Reliable

	int32 PacketNumber = 0;
	::memcpy(&PacketNumber, mReadBuffer + sizeof(int32) + NextPacket, sizeof(int32));

	//Check PacketNumber
	//WARN! 나중에 보낸 패킷이 먼저오면 전에꺼 보낸 패킷 무시해버림.
	//따라서 해당 기능에 영향을 많이 받는다면 TCP를 사용하거나 로직을 바꿔야함
	//This for only Check SamePacket...
	if (!CheckPacketNum(target, PacketNumber) && (remainLength <= PacketLength))
		return;

	char packet[MAX_BUFFER_LENGTH];
	::memcpy(&packet, mReadBuffer + sizeof(int32) * 2 + NextPacket, PacketLength);

	auto message = GetMessage(packet);
	auto protocol = message->packet_type();

	switch (protocol)
	{
	case MESSAGE_ID::MESSAGE_ID_C2S_MOVE:
	{
		auto RecvData = static_cast<const C2S_MOVE*>(message->packet());
		READ_PU_C2S_MOVE(RecvData);
		/*Testing*/

		Position pos{ 1.0f, 2.0f, 3.0f };
		Direction dir{ 1.0f, 2.0f, 3.0f };
		int32 size = 0;
		auto data = WRITE_PU_S2C_MOVE("Edea", pos, dir, size);
		WriteTo(remoteAddress, remotePort, data, size);
		break;
	}
		/*Echo Sample*/
	case MESSAGE_ID::MESSAGE_ID_C2S_EXTEND_SESSION:
	{
		auto RecvSession = static_cast<const C2S_EXTEND_SESSION*>(message->packet());

		mLock.EnterWriteLock();
		READ_PU_C2S_EXTEND_SESSION(RecvSession, mUserSession);
		mLock.LeaveWriteLock();
		break;
	}
		//Process of according to Protocol
	}

	//Check if Packet comes several
	if (remainLength > PacketLength)
	{
		remainLength = remainLength - PacketLength;
		NextPacket += PacketLength;
		goto RETRY;
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

	mClientInfo.sin_family = AF_INET;
	mClientInfo.sin_addr.s_addr = inet_addr(remoteAddress);
	mClientInfo.sin_port = htons(remotePort);

	int32 returnVal = sendto(mSocket, mWriteBuffer, dataLength, 0, (SOCKADDR*)&mClientInfo, sizeof(mClientInfo));

	if (returnVal < 0)
		return;
}

bool SocketWorker::CheckPacketNum(Session &session, uint32 PacketNumber)
{
	return session.PacketNum >= PacketNumber ? false : true;
}
