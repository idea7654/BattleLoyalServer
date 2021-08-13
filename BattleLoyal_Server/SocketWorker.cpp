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
	//Define Init Position
	mInitPos.push_back(Position{ 6.0f, -1.45f, 2.0f });
	mInitPos.push_back(Position{ -6.0f, -1.45f, -2.0f });

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

	//shared_ptr<Session> target = this->FindSession(remoteAddress, remotePort);
	//if (PacketLength == 8888)
	//{
	//	ResetSessionTime(target);
	//} //USE this or C2S_EXTEND_SESSION Packet

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
		string nickname;
		Position userPos;
		float userDir;
		int32 moveDir;
		READ_PU_C2S_MOVE(RecvData, nickname, userPos, userDir, moveDir);

		int32 size = 0;
		auto data = WRITE_PU_S2C_MOVE(nickname, userPos, userDir, moveDir, size);
		//WriteTo(remoteAddress, remotePort, data, size);
		//여기서 방을 찾고...broadCasting
		auto userRoomNum = FindSession(nickname)->RoomNum;
		ContentSessions roomUsers = FindContentSessionInVec(userRoomNum);
		for (auto &i : roomUsers.Sessions)
		{
			WriteTo(i->remoteAddress, i->port, data, size);
		}
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

		//mLock.EnterWriteLock();
		auto returnData = READ_PU_C2S_REQUEST_LOGIN(RecvData);
		if (returnData == "Incorrect_Email")
		{
			int32 errLength = 0;
			auto packetData = WRITE_PU_S2C_LOGIN_ERROR("Incorrect_Email", errLength);
			WriteTo(remoteAddress, remotePort, packetData, errLength);
		}
		else
		{
			//중복체크
			auto overLogin = FindSession(returnData);
			if (overLogin)
			{
				//LOGIN_ERROR->중복로그인 처리
			}
			int32 dataLength = 0;
			auto packetData = WRITE_PU_S2C_COMPLETE_LOGIN(returnData, dataLength);
			WriteTo(remoteAddress, remotePort, packetData, dataLength);
			//char nick[20];
			//memcpy(nick, returnData, sizeof(returnData));
			const char* nick = returnData;
			auto user = MakeShared<Session>();
			user->isOnline = 10;
			user->nickname = nick;
			user->remoteAddress = remoteAddress;
			user->port = remotePort;
			user->RoomNum = 0;
			mLock.EnterWriteLock();
			mUserSession.emplace_back(user);
			mLock.LeaveWriteLock();
		}
		//mLock.LeaveWriteLock();
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
	case MESSAGE_ID::MESSAGE_ID_C2S_START_MATCHING:
	{
		auto RecvData = static_cast<const C2S_START_MATCHING*>(message->packet());

		mLock.EnterWriteLock();
		auto nickname = READ_PU_C2S_START_MATCHING(RecvData);
		auto originSession = FindSession(nickname);
		auto contentSession = MakeShared<ContentSession>();
		contentSession->PacketNum = originSession->PacketNum;
		contentSession->remoteAddress = originSession->remoteAddress;
		contentSession->port = originSession->port;
		contentSession->RoomNum = ROOM_NUM;
		contentSession->nickname = originSession->nickname;
		contentSession->isOnline = originSession->isOnline;
		mContentSession.push_back(contentSession);
		if (mContentSession.size() == ROOM_MAX_NUM)
		{
			GameStart();
			mContentSessionVec.push_back(ContentSessions{mContentSession, ROOM_NUM - 1});
			mContentSession.clear();
		}
		mLock.LeaveWriteLock();
		break;
	}
	case MESSAGE_ID::MESSAGE_ID_C2S_CANCEL_MATCHING:
	{
		auto RecvData = static_cast<const C2S_CANCEL_MATCHING*>(message->packet());

		mLock.EnterWriteLock();
		auto nickname = READ_PU_C2S_CANCEL_MATCHING(RecvData);
		auto originUser = FindSession(nickname);
		shared_ptr<ContentSession> contentSession = FindContentSession(nickname);
		mContentSession.erase(remove_if(begin(mContentSession), end(mContentSession), [nickname](shared_ptr<ContentSession> const &o) { return o->nickname == nickname; }), end(mContentSession));

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

	mClientInfo.sin_family = AF_INET;
	mClientInfo.sin_addr.s_addr = inet_addr(remoteAddress);
	mClientInfo.sin_port = htons(remotePort);

	int32 returnVal = sendto(mSocket, mWriteBuffer, dataLength, 0, (SOCKADDR*)&mClientInfo, sizeof(mClientInfo));
	if (returnVal < 0)
		return;
	mLock.LeaveWriteLock();
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

shared_ptr<Session> SocketWorker::FindSession(string nickname)
{
	for (auto &i : mUserSession)
	{
		string str(i->nickname);
		if (str == nickname)
			return i;
	}
	return nullptr;
}

auto SocketWorker::FindContentSession(int32 RoomNum)
{
	vector<shared_ptr<ContentSession>> returnVec;
	for (auto &i : mContentSession)
	{
		if (i->RoomNum == RoomNum)
			returnVec.push_back(i);
	}
	return returnVec;
}

shared_ptr<ContentSession> SocketWorker::FindContentSession(string nickname)
{
	for (auto &i : mContentSession)
	{
		string str(i->nickname);
		if (str == nickname)
			return i;
	}
}

SocketWorker::ContentSessions SocketWorker::FindContentSessionInVec(int32 RoomNum)
{
	auto roomInfo = mContentSessionVec[RoomNum];
	return roomInfo;
}

void SocketWorker::GameStart()
{
	int32 OriginRoomNum = ROOM_NUM;
	ROOM_NUM++;
	auto RoomUsers = FindContentSession(OriginRoomNum);
	SetUserPosition(RoomUsers);
	vector<SessionGun> GunInfo = SetGunPosition();
	for (auto &i : RoomUsers)
	{
		int32 packetLength = 0;
		auto packet = WRITE_PU_S2C_GAME_START(packetLength, RoomUsers, i->pos, GunInfo);
		WriteTo(i->remoteAddress, i->port, packet, packetLength);
	}
}

void SocketWorker::SetUserPosition(vector<shared_ptr<ContentSession>> &sessions)
{
	uint16 j = 0;
	for (auto &i : sessions)
	{
		i->pos = mInitPos[j];
		j++;
	}
}

vector<SessionGun> SocketWorker::SetGunPosition()
{
	vector<SessionGun> sessionGun;
	for (int32 i = 0; i < GUN_MAX_NUM; i++)
	{
		auto a = Position{ 5.95f, -1.54f, -5.21f };
		sessionGun.emplace_back(SessionGun{ a, Gun::NORMAL });
	}
	return sessionGun;
}