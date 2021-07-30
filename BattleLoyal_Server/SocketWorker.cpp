#include "pch.h"
#include "SocketWorker.h"
#include "../Packet/UdpProtocol_generated.h"
#include "Udp_ReadPacket.h"
#include "Udp_WritePacket.h"
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
	DBManager.SQL_INIT();
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

	shared_ptr<Session> target = this->FindSession(remoteAddress, remotePort);
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
	//if (!CheckPacketNum(target, PacketNumber) && (remainLength <= PacketLength))
	//	return;

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
	case MESSAGE_ID::MESSAGE_ID_C2S_REQUEST_LOGIN:
	{
		auto RecvData = static_cast<const C2S_REQUEST_LOGIN*>(message->packet());

		mLock.EnterWriteLock();
		auto returnData = READ_PU_C2S_REQUEST_LOGIN(RecvData);
		
		if (returnData == "Incorrect_Email")
		{
			int32 errLength = 0;
			auto packetData = WRITE_PU_S2C_LOGIN_ERROR("Incorrect_Email", errLength);
			WriteTo(remoteAddress, remotePort, packetData, errLength);
		}
		else
		{
			int32 dataLength = 0;
			auto packetData = WRITE_PU_S2C_COMPLETE_LOGIN(returnData, dataLength);
			WriteTo(remoteAddress, remotePort, packetData, dataLength);
			auto user = MakeShared<Session>();
			user->isOnline = 10;
			user->nickname = returnData;
			user->remoteAddress = remoteAddress;
			user->port = remotePort;
			user->RoomNum = 0;
			mUserSession.push_back(user);
		}
		
		mLock.LeaveWriteLock();
		break;
	}
	case MESSAGE_ID::MESSAGE_ID_C2S_REQUEST_REGISTER:
	{
		auto RecvData = static_cast<const C2S_REQUEST_REGISTER*>(message->packet());

		mLock.EnterWriteLock();
		auto returnData = READ_PU_C2S_REQUEST_REGISTER(RecvData, remoteAddress, remotePort);

		if (returnData)
		{
			int32 Success = 0;
			auto packetData = WRITE_PU_S2C_COMPLETE_REGISTER(Success);
			WriteTo(remoteAddress, remotePort, packetData, Success);
		}
		else 
		{
			int32 errLength = 0;
			auto packetData = WRITE_PU_S2C_REGISTER_ERROR(errLength, "Register_ERROR");
			WriteTo(remoteAddress, remotePort, packetData, errLength);
		}
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

void SocketWorker::ReduceSessionTime() //Reduce SessionTime which in SessionVector, Use with Thread
{
	vector<shared_ptr<Session>> removeList;
	mLock.EnterReadLock();
	for (auto &i : mUserSession)
	{
		mLock.EnterWriteLock();
		i->isOnline--;
		mLock.LeaveWriteLock();
		if (i->isOnline < 0)
			removeList.push_back(i);
	}
	mLock.LeaveReadLock();
	for (auto &i : removeList)
	{
		mLock.EnterWriteLock();
		//mUserSession.erase(std::remove(mUserSession.begin(), mUserSession.end(), i), mUserSession.end());
		for (std::vector<shared_ptr<Session>>::iterator it = mUserSession.begin(); it != mUserSession.end(); it++)
		{
			if ((*it)->remoteAddress == i->remoteAddress && (*it)->port == i->port)//구조체 vector의 id값이 4인 원소를 삭제
				mUserSession.erase(it);
		}
		mLock.LeaveWriteLock();
	}
}

bool SocketWorker::ResetSessionTime(shared_ptr<Session> &session) //Use this or 
//Read_PU_C2S_EXTEND_SESSION in Udp_ReadPacket.h
{
	mLock.EnterWriteLock();
	session->isOnline = 10;
	mLock.LeaveWriteLock();
	return true;
}

shared_ptr<Session> SocketWorker::FindSession(char* remoteAddress, uint16 port)
{
	for (auto &i : mUserSession)
	{
		if (i->remoteAddress == remoteAddress && i->port == port)
			return i;
	}
}