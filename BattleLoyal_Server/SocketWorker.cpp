#include "pch.h"
#include "SocketWorker.h"
#include "../Packet/UdpProtocol_generated.h"
#include "Udp_ReadPacket.h"
#include <random>
#include "Udp_WritePacket.h"
#include <future>

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
	mInitPos.push_back(Position{ 17017.322266f, -17165.876953f, 176.05896f }); //Sector1
	mInitPos.push_back(Position{ 4458.508789f, 20358.816406f, 168.702896f }); //Sector2
	mInitPos.push_back(Position{ -21984.056641f, -11971.955078f, 294.17807f }); //Sector3
	
	GunPos.push_back(Position{ 270.0f, -28390.0f, 180.0f });
	GunPos.push_back(Position{ 53.0f, -26309.0f, 185.0f });
	GunPos.push_back(Position{ -20293.199219f, -12244.154297f , 271.0f });
	GunPos.push_back(Position{ -20293.199219f, -13267.0f , 302.0f });
	GunPos.push_back(Position{ -22627.914062f, -11673.779297f , 200.0f });
	GunPos.push_back(Position{ -22967.089844f, -13350.819336f , 250.0f });
	GunPos.push_back(Position{ -19108.0f, -13488.0f , 300.0f }); //Gun pos
	
	mRecoverPos.push_back(Position{ -11680.0f, -11480.0f, 80.0f });

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

	int32 PacketNumber = 0;
	::memcpy(&PacketNumber, mReadBuffer + sizeof(int32) + NextPacket, sizeof(int32));

	//Check PacketNumber
	//WARN! 나중에 보낸 패킷이 먼저오면 전에꺼 보낸 패킷 무시해버림.
	//따라서 해당 기능에 영향을 많이 받는다면 TCP를 사용하거나 로직을 바꿔야함
	//This for only Check SamePacket...
	//shared_ptr<Session> target = this->FindSession(remoteAddress, remotePort);

	//if (remainLength <= PacketLength)
	//	return;

	/*if (target)
	{
		if (!CheckPacketNum(target, PacketNumber))
			return;
	}*/
	if (PacketLength == 8888)
	{
		//Reliable UDP
		//int32 packetData = 8888;
		//BYTE bytes[sizeof packetData];
		//std::copy(static_cast<const char*>(static_cast<const void*>(&packetData)),
		//	static_cast<const char*>(static_cast<const void*>(&packetData)) + sizeof packetData,
		//	bytes);
		//WriteTo(remoteAddress, remotePort, bytes, packetData);
		SetEvent(mReliableHandle);
		return;
	}

	char packet[MAX_BUFFER_LENGTH];
	if (PacketLength < 0) return;
	::memcpy(&packet, mReadBuffer + sizeof(int32) * 2 + NextPacket, PacketLength);
	//여기 에러 고치자
	auto message = GetMessage(packet);
	auto protocol = message->packet_type();

	switch (protocol)
	{
	case MESSAGE_ID::MESSAGE_ID_C2S_MOVE:
	{
		auto RecvData = static_cast<const C2S_MOVE*>(message->packet());
		string nickname;
		Position userPos;
		Direction userDir;
		float vfront = 0.0f;
		float vright = 0.0f;
		float vyaw = 0.0f;
		bool isJump = false;
		bool isCrouch = false;
		mLock.EnterWriteLock();
		READ_PU_C2S_MOVE(RecvData, nickname, userPos, userDir, vfront, vright, vyaw, isJump, isCrouch);
		mLock.LeaveWriteLock();

		int32 size = 0;
		mLock.EnterWriteLock();
		auto data = WRITE_PU_S2C_MOVE(nickname, userPos, userDir, vfront, vright, vyaw, isJump, isCrouch, size);
		mLock.LeaveWriteLock();
		//WriteTo(remoteAddress, remotePort, data, size);
		//여기서 방을 찾고...broadCasting
		if (FindSession(nickname) == nullptr)
		{
			UserNotFound(remoteAddress, remotePort);
			break;
		}
		else {
			auto userRoomNum = FindSession(nickname)->RoomNum;
			ContentSessions roomUsers = FindContentSessionInVec(userRoomNum);
			for (auto &i : roomUsers.Sessions)
			{
				bool a = WriteTo(i->remoteAddress, i->port, data, size);
			}
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
			//while (mReliableHandle)
			//{

			//}
			mLock.EnterWriteLock();
			mReliableHandle = CreateEvent(0, false, false, NULL);
			mLock.LeaveWriteLock();

			//
			//ReliableProcess(remoteAddress, remotePort, packetData, dataLength);
			//cout << "패킷 보냄!" << endl;
			WriteTo(remoteAddress, remotePort, packetData, dataLength);
				
			string nick = returnData;
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
		break;
	}
	case MESSAGE_ID::MESSAGE_ID_C2S_REQUEST_REGISTER:
	{
		auto RecvData = static_cast<const C2S_REQUEST_REGISTER*>(message->packet());

		mLock.EnterWriteLock();
		auto returnData = READ_PU_C2S_REQUEST_REGISTER(RecvData, remoteAddress, remotePort);

		cout << returnData << endl;
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

		//mLock.EnterWriteLock();
		auto nickname = READ_PU_C2S_START_MATCHING(RecvData);

		mLock.EnterReadLock();
		auto originSession = FindSession(nickname);
		mLock.LeaveReadLock();

		if (originSession == nullptr)
		{
			UserNotFound(remoteAddress, remotePort);
			break;
		}
		originSession->RoomNum = ROOM_NUM;
		auto contentSession = MakeShared<ContentSession>();
		contentSession->PacketNum = originSession->PacketNum;
		contentSession->remoteAddress = originSession->remoteAddress;
		contentSession->port = originSession->port;
		contentSession->RoomNum = originSession->RoomNum;
		contentSession->nickname = originSession->nickname;
		contentSession->isOnline = originSession->isOnline;
		contentSession->hp = 100.0f;
		
		mLock.EnterWriteLock();
		mContentSession.push_back(contentSession);
		mLock.LeaveWriteLock();

		if (mContentSession.size() == ROOM_MAX_NUM)
		{
			mLock.EnterWriteLock();
			vector<shared_ptr<SessionGun>> GunInfo;
			for (int32 i = 0; i < GunPos.size(); i++)
			{
				auto newSession = MakeShared<SessionGun>();
				newSession->Gun_pos = GunPos[i];
				newSession->Gun_Type = Gun::NORMAL;
				newSession->gunNum = i;
				GunInfo.push_back(newSession);
			}
			vector<shared_ptr<SessionRecover>> RecoverInfo;
			for (int32 i = 0; i < mRecoverPos.size(); i++)
			{
				auto newSession = MakeShared<SessionRecover>();
				newSession->pos = mRecoverPos[i];
				newSession->objNum = i;
				RecoverInfo.push_back(newSession);
			}
			mLock.LeaveWriteLock();
			GameStart(GunInfo, RecoverInfo);
			mLock.EnterWriteLock();
			mContentSessionVec.push_back(ContentSessions{mContentSession, GunInfo, RecoverInfo, ROOM_NUM - 1 });
			mContentSession.clear();
			int32 MyRoomNum = ROOM_NUM - 1;
			mLock.LeaveWriteLock();
			
			future<void> future = async(std::launch::async, [this, &MyRoomNum]() {
				Sleep(35000);
				int32 packetLength = 0;
				ContentSessions RoomUsersLamb = FindContentSessionInVec(MyRoomNum);
				uint8_t* packet = WRITE_PU_S2C_START_SIGN(packetLength, RoomUsersLamb.Sessions);
				for (auto &i : RoomUsersLamb.Sessions)
				{
					WriteTo(i->remoteAddress, i->port, packet, packetLength);
				}

				Sleep(20000);
				packet = WRITE_PU_S2C_ROUND_START(packetLength, 1);
				for (auto &i : RoomUsersLamb.Sessions)
				{
					WriteTo(i->remoteAddress, i->port, packet, packetLength);
				}
				Sleep(180000);
				packet = WRITE_PU_S2C_ROUND_START(packetLength, 2);
				for (auto &i : RoomUsersLamb.Sessions)
				{
					WriteTo(i->remoteAddress, i->port, packet, packetLength);
				}
			});
		}
		break;
	}
	case MESSAGE_ID::MESSAGE_ID_C2S_CANCEL_MATCHING:
	{
		auto RecvData = static_cast<const C2S_CANCEL_MATCHING*>(message->packet());

		mLock.EnterWriteLock();
		auto nickname = READ_PU_C2S_CANCEL_MATCHING(RecvData);
		auto originUser = FindSession(nickname);
		if (originUser == nullptr)
		{
			UserNotFound(remoteAddress, remotePort);
			break;
		}
		shared_ptr<ContentSession> contentSession = FindContentSession(nickname);
		mContentSession.erase(remove_if(begin(mContentSession), end(mContentSession), [nickname](shared_ptr<ContentSession> const &o) { return o->nickname == nickname; }), end(mContentSession));
		mLock.LeaveWriteLock();

		break;
	}
	case MESSAGE_ID::MESSAGE_ID_C2S_PICKUP_GUN:
	{
		auto RecvData = static_cast<const C2S_PICKUP_GUN*>(message->packet());
		int32 packetLen = 0;
		mLock.EnterWriteLock();
		int32 gunNum = 0;
		auto nickname = READ_PU_C2S_PICKUP_GUN(RecvData, gunNum);
		auto userSession = FindSession(nickname);
		if (userSession == nullptr)
		{
			UserNotFound(remoteAddress, remotePort);
			break;
		}
		
		auto userRoom = FindContentSessionInVec(userSession->RoomNum);
		auto user = FindContetnSessionInVecByName(userSession->RoomNum, nickname);

		bool GunValid = GunCheck(userRoom, gunNum);

		if (!GunValid)
			break;
		
		if (user != nullptr)
			user->gun = userRoom.Guns[gunNum];
		else
			break;

		cout << nickname << ", " << gunNum;
		auto packet = WRITE_PU_S2C_PICKUP_GUN(packetLen, nickname, gunNum);
		mLock.LeaveWriteLock();

		for (auto &i : userRoom.Sessions)
		{
			WriteTo(i->remoteAddress, i->port, packet, packetLen);
		}
		
		break;
	}
	case MESSAGE_ID::MESSAGE_ID_C2S_SHOOT:
	{
		auto RecvData = static_cast<const C2S_SHOOT*>(message->packet());
		int32 packetLen = 0;
		int32 packetLength = 0;
		string nickname;
		string target;
		float damage = 0.0f;

		mLock.EnterWriteLock();
		READ_PU_C2S_SHOOT(RecvData, nickname, target, damage);

		auto userSession = FindSession(nickname);

		if (userSession == nullptr)
		{
			UserNotFound(remoteAddress, remotePort);
			break;
		}

		auto userRoom = FindContentSessionInVec(userSession->RoomNum);
		auto packet = WRITE_PU_S2C_SHOOT(packetLen, nickname, target, damage);

		if (damage != 0.0f)
		{
			for (auto &i : userRoom.Sessions)
			{
				if (i->nickname == target)
				{
					i->hp -= damage;
					if (i->hp <= 0)
					{
						auto diePacket = WRITE_PU_S2C_PLAYER_DIE(packetLength, nickname, target, "SHOOT");
						for (auto &i : userRoom.Sessions)
						{
							WriteTo(i->remoteAddress, i->port, diePacket, packetLength);
						}
						SessionOut(userRoom, i->nickname);
					}
					else {
						for (auto &i : userRoom.Sessions)
						{
							WriteTo(i->remoteAddress, i->port, packet, packetLen);
						}
					}
					//Sleep(300);
					//CheckUserDie(userRoom, nickname);
					break;
				}
			}
		}
		else {
			for (auto &i : userRoom.Sessions)
			{
				WriteTo(i->remoteAddress, i->port, packet, packetLen);
			}
		}
		mLock.LeaveWriteLock();
		break;
	}
	case MESSAGE_ID::MESSAGE_ID_C2S_MELEE_ATTACK:
	{
		auto RecvData = static_cast<const C2S_MELEE_ATTACK*>(message->packet());
		string nickname;
		string target;
		int32 combo = 0;
		int32 packetLen = 0;
		int32 packetLength = 0;
		mLock.EnterWriteLock();
		READ_PU_C2S_MELEE_ATTACK(RecvData, nickname, target, combo);
		float damage;
		if (target == "")
			damage = 0.0f;
		else
			damage = (float)combo * 10.0f;

		auto userSession = FindSession(nickname);
		if (userSession == nullptr)
		{
			UserNotFound(remoteAddress, remotePort);
			break;
		}

		auto userRoom = FindContentSessionInVec(userSession->RoomNum);
		auto packet = WRITE_PU_S2C_MELEE_ATTACK(packetLen, nickname, target, damage, combo);

		if (target == "")
		{
			for (auto &i : userRoom.Sessions)
			{
				WriteTo(i->remoteAddress, i->port, packet, packetLen);
			}
		}
		else {
			for (auto &i : userRoom.Sessions)
			{
				if (i->nickname == target)
				{
					i->hp -= damage;
					if (i->hp <= 0)
					{
						auto diePacket = WRITE_PU_S2C_PLAYER_DIE(packetLength, nickname, target, "MELEE", combo);
						for (auto &i : userRoom.Sessions)
						{
							WriteTo(i->remoteAddress, i->port, diePacket, packetLength);
						}
						SessionOut(userRoom, target);
					}
					else {
						for (auto &i : userRoom.Sessions)
						{
							WriteTo(i->remoteAddress, i->port, packet, packetLen);
						}
					}
					break;
				}
			}
		}
		mLock.LeaveWriteLock();
		Sleep(600);
		CheckUserDie(userRoom, nickname);
		break;
	}

	case MESSAGE_ID::MESSAGE_ID_C2S_EQUIP_GUN:
	{
		auto RecvData = static_cast<const C2S_EQUIP_GUN*>(message->packet());
		string nickname;
		bool state = false;
		int32 packetLen = 0;
		mLock.EnterWriteLock();
		READ_PU_C2S_EQUIP_GUN(RecvData, nickname, state);

		auto userSession = FindSession(nickname);
		if (userSession == nullptr)
		{
			UserNotFound(remoteAddress, remotePort);
			break;
		}

		auto userRoom = FindContentSessionInVec(userSession->RoomNum);
		auto packet = WRITE_PU_S2C_EQUIP_GUN(packetLen, nickname, state);

		for (auto &i : userRoom.Sessions)
		{
			WriteTo(i->remoteAddress, i->port, packet, packetLen);
		}
		mLock.LeaveWriteLock();

		break; 
	}

	case MESSAGE_ID::MESSAGE_ID_C2S_CHANGE_GUN:
	{
		auto RecvData = static_cast<const C2S_CHANGE_GUN*>(message->packet());
		string nickname;
		int32 originID = 0;
		int32 nowID = 0;
		int32 packetLen = 0;
		mLock.EnterWriteLock();
		READ_PU_C2S_CHANGE_GUN(RecvData, nickname, originID, nowID);

		auto userSession = FindSession(nickname);
		if (userSession == nullptr)
		{
			UserNotFound(remoteAddress, remotePort);
			break;
		}
		auto userRoom = FindContentSessionInVec(userSession->RoomNum);
		auto user = FindContetnSessionInVecByName(userSession->RoomNum, nickname);
		bool GunValid = GunCheck(userRoom, nowID);
		if (!GunValid)
			break;
		user->gun = userRoom.Guns[nowID];
		userRoom.Guns[originID]->Gun_pos = userRoom.Guns[nowID]->Gun_pos;

		auto packet = WRITE_PU_S2C_CHANGE_GUN(packetLen, nickname, originID, nowID);

		for (auto &i : userRoom.Sessions)
		{
			WriteTo(i->remoteAddress, i->port, packet, packetLen);
		}
		mLock.LeaveWriteLock();
		break;
	}
	case MESSAGE_ID::MESSAGE_ID_C2S_SET_USER_POSITION:
	{
		auto RecvData = static_cast<const C2S_SET_USER_POSITION*>(message->packet());
		string nickname;
		int32 sector = 0;
		mLock.EnterWriteLock();
		READ_PU_C2S_SET_USER_POSITION(RecvData, nickname, sector);
		auto userSession = FindSession(nickname);
		if (userSession == nullptr)
		{
			UserNotFound(remoteAddress, remotePort);
			break;
		}
		
		auto userRoom = FindContentSessionInVec(userSession->RoomNum);
		int32 packetSize = 0;
		auto packet = WRITE_PU_S2C_SET_USER_POSITION(packetSize, nickname, sector);
		for (auto &i : userRoom.Sessions)
		{
			if (i->nickname == nickname)
				i->pos = mInitPos[sector - 1];
			WriteTo(i->remoteAddress, i->port, packet, packetSize);
		}

		mLock.LeaveWriteLock();
		break;
	}
	case MESSAGE_ID::MESSAGE_ID_C2S_ZONE_DAMAGE:
	{
		auto RecvData = static_cast<const C2S_ZONE_DAMAGE*>(message->packet());
		string nickname;
		int32 damage = 0;
		int32 packetLen = 0;
		mLock.EnterWriteLock();
		READ_PU_C2S_ZONE_DAMAGE(RecvData, nickname, damage);
		auto userSession = FindSession(nickname);
		if (userSession == nullptr)
		{
			UserNotFound(remoteAddress, remotePort);
			break;
		}

		auto userRoom = FindContentSessionInVec(userSession->RoomNum);
		auto packet = WRITE_PU_S2C_ZONE_DAMAGE(packetLen, nickname, damage);
		
		shared_ptr<ContentSession> user;
		for (auto &i : userRoom.Sessions)
		{
			if (i->nickname == nickname)
			{
				i->hp -= damage;
				if (i->hp < 0)
				{
					user = i;
				}
				break;
			}
		}

		for (auto &i : userRoom.Sessions)
		{
			WriteTo(i->remoteAddress, i->port, packet, packetLen);
		}

		if (user != nullptr)
		{
			int32 packetSize = 0;
			auto diePacket = WRITE_PU_S2C_PLAYER_DIE(packetSize, "Zone", user->nickname, "Zone");
			for (auto &i : userRoom.Sessions)
			{
				WriteTo(i->remoteAddress, i->port, diePacket, packetSize);
			}
			SessionOut(userRoom, user->nickname);
			//유저 죽은거 처리->Session에서 빼버리기
		}

		mLock.LeaveWriteLock();
		CheckUserDie(userRoom, nickname);
		break;
	}
	case MESSAGE_ID::MESSAGE_ID_C2S_RECOVER_HP:
	{
		auto RecvData = static_cast<const C2S_RECOVER_HP*>(message->packet());
		string nickname;
		int32 objID = 0;
		int32 packetSize = 0;
		mLock.EnterWriteLock();
		READ_PU_C2S_RECOVER_HP(RecvData, nickname, objID);
		auto userSession = FindSession(nickname);
		if (userSession == nullptr)
		{
			UserNotFound(remoteAddress, remotePort);
			break;
		}

		auto userRoom = FindContentSessionInVec(userSession->RoomNum);
		auto packet = WRITE_PU_S2C_RECOVER_HP(packetSize, objID, nickname);

		shared_ptr<SessionRecover> removeObj;
		bool isValid = false;
		for (auto &i : userRoom.Recovers)
		{
			if (i->objNum == objID)
			{
				removeObj = i;
				isValid = true;
				break;
			}
		}

		if (isValid)
		{
			if (removeObj)
			{
				userRoom.Recovers.erase(remove_if(userRoom.Recovers.begin(), userRoom.Recovers.end(), [removeObj](shared_ptr<SessionRecover> userData) {
					return userData->objNum == removeObj->objNum;
					}), userRoom.Recovers.end());
			}
			for (auto &i : userRoom.Sessions)
			{
				if (i->nickname == nickname)
				{
					i->hp += 20;
					if (i->hp > 100)
						i->hp = 100;
					break;
				}
			}
		}
		
		for (auto &i : userRoom.Sessions)
		{
			WriteTo(i->remoteAddress, i->port, packet, packetSize);
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

	mClientInfo.sin_family = AF_INET;
	mClientInfo.sin_addr.s_addr = inet_addr(remoteAddress);
	mClientInfo.sin_port = htons(remotePort);

	int32 returnVal = sendto(mSocket, mWriteBuffer, dataLength, 0, (SOCKADDR*)&mClientInfo, sizeof(mClientInfo));

	if (returnVal < 0)
		return;
	mLock.LeaveWriteLock();
}

bool SocketWorker::CheckPacketNum(shared_ptr<Session> &session, uint32 PacketNumber)
{
	return session->PacketNum >= PacketNumber ? false : true;
}

void SocketWorker::ReduceSessionTime() //Reduce SessionTime which in SessionVector, Use with Thread
{
	while (true)
	{
		vector<shared_ptr<Session>> removeList;
		mLock.EnterReadLock();
		for (auto &i : mUserSession)
		{
			i->isOnline--;
			if (i->isOnline < 1)
				removeList.push_back(i);
		}
		mLock.LeaveReadLock();
		SessionOut(removeList);
		Sleep(SESSION_REDUCE_TIME);
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

shared_ptr<Session> SocketWorker::FindSession(char * remoteAddress, uint16 port)
{
	for (auto &i : mUserSession)
	{
		if (i->remoteAddress == remoteAddress && i->port == port)
			return i;
	}
	//return shared_ptr<Session>();
}

vector<shared_ptr<ContentSession>> SocketWorker::FindContentSession(int32 RoomNum)
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

shared_ptr<ContentSession> SocketWorker::FindContetnSessionInVecByName(int32 RoomNum, string nickname)
{
	auto roomInfo = mContentSessionVec[RoomNum];
	for (auto &i : roomInfo.Sessions)
	{
		if (i->nickname == nickname)
		{
			return i;
		}
	}
	return nullptr;
}

bool SocketWorker::GunCheck(ContentSessions session, uint16 gunNum)
{
	for (auto i : session.Sessions)
	{
		if (i->gun != nullptr)
			if(i->gun->gunNum == gunNum)
				return false;
	}
	return true;
}

void SocketWorker::GameStart(vector<shared_ptr<SessionGun>> &guns, vector<shared_ptr<SessionRecover>> &recovers)
{
	//Lock불필요->동시에 게임이 잡히는 경우 극히 드물것..
	//필요할 경우 추후 write-read섞어서 구현하면됨

	int32 OriginRoomNum = ROOM_NUM;
	ROOM_NUM++;
	auto RoomUsers = FindContentSession(OriginRoomNum);
	SetUserPosition(RoomUsers);
	auto roundInfo = SetRoundPosition();

	int32 packetLength = 0;
	auto packet = WRITE_PU_S2C_GAME_START(packetLength, RoomUsers, mInitPos[0], guns, roundInfo, recovers);
	for (auto &i : RoomUsers)
	{
		//WriteTo(i->remoteAddress, i->port, packet, packetLength);
		mReliableHandle = CreateEvent(0, false, false, NULL);
		ReliableProcess(i->remoteAddress, i->port, packet, packetLength);
	}
}

void SocketWorker::SetUserPosition(vector<shared_ptr<ContentSession>> &sessions)
{
	for (auto &i : sessions)
	{
		i->pos = mInitPos[0];
	}
}

void SocketWorker::SetGunPosition()
{
	vector<SessionGun> sessionGun;
	/*for (int32 i = 0; i < GUN_MAX_NUM; i++)
	{
		auto a = Position{ 270.0f, -28390.0f, 180.0f };
		sessionGun.emplace_back(SessionGun{ a, Gun::NORMAL });
	}*/
	auto a = Position{ 270.0f, -28390.0f, 180.0f };
	auto b = Position{ 53.0f, -26309.0f, 185.0f };
	auto c = Position{ -20293.199219f, -12244.154297f , 360.351746f };
	sessionGun.emplace_back(SessionGun{ a, Gun::NORMAL, 0 });
	sessionGun.emplace_back(SessionGun{ b, Gun::NORMAL, 1 });
	sessionGun.emplace_back(SessionGun{ c, Gun::NORMAL, 2 });
}

void SocketWorker::UserNotFound(char * remoteAddress, uint16 port)
{
	int32 size = 0;
	auto packet = WRITE_PU_S2C_USER_NOT_FOUND(size);
	WriteTo(remoteAddress, port, packet, size);
}

void SocketWorker::SessionOut(vector<shared_ptr<Session>>& Session)
{
	mLock.EnterWriteLock();
	for (auto &i : Session)
	{
		mUserSession.erase(std::remove(mUserSession.begin(), mUserSession.end(), i), mUserSession.end());

		for (auto &j : mContentSessionVec)
		{
			if (i->RoomNum == j.ROOM_NUM)
			{
				j.Sessions.erase(remove_if(j.Sessions.begin(), j.Sessions.end(), [i](shared_ptr<ContentSession> userData) {
					return !userData->nickname.compare(i->nickname);
				}), j.Sessions.end());
				for (auto &k : j.Sessions)
				{
					int32 size = 0;
					auto makePacket = WRITE_PU_S2C_USER_DISCONNECT(size, i->nickname);
					WriteTo(k->remoteAddress, k->port, makePacket, size);
				}
				break;
			}
		}
		i = nullptr;
	}
	mLock.LeaveWriteLock();
}

void SocketWorker::ReliableProcess(char* remoteAddress, uint16 &remotePort, BYTE *data, DWORD dataLength)
{
	uint16 DisconnectCount = 0;
RELIABLE:
	bool IsSucces = WriteTo(remoteAddress, remotePort, data, dataLength);

	DWORD EventID = WaitForSingleObject(mReliableHandle, 500);

	DisconnectCount++;
	if (EventID != WAIT_OBJECT_0)
	{
		if (DisconnectCount < 10)
			goto RELIABLE;
		else
			cout << "INVALID USER" << endl;
		//처리..
	}
	CloseHandle(mReliableHandle);
	mReliableHandle = NULL;
}

void SocketWorker::CheckUserDie(ContentSessions & checkVector, string nickname)
{
	int32 count = 0;
	string winner;
	for (auto &i : checkVector.Sessions)
	{
		if (i->hp > 0)
		{
			count++;
			winner = i->nickname;
		}
	}
	if (count == 1)
	{
		int32 packetSize = 0;
		auto victoryPacket = WRITE_PU_S2C_USER_VICTORY(packetSize, winner);
		for (auto &i : checkVector.Sessions)
		{
			WriteTo(i->remoteAddress, i->port, victoryPacket, packetSize);
		}
	}
}

vector<Position> SocketWorker::SetRoundPosition()
{
	random_device rd;
	mt19937 gen(rd());

	Position firstRound;
	uniform_int_distribution<int> firstXRange(-10125, 10125);
	int firstX = firstXRange(gen);
	uniform_int_distribution<int> firstYRange(-10125, 10125);
	int firstY = firstYRange(gen);
	firstRound = Position{ (float)firstX, (float)firstY, 0 };

	int secondXPos1 = firstX - 3375;
	int secondXPos2 = firstX + 3375;
	int secondYPos1 = firstY - 3375;
	int secondYPos2 = firstY + 3375;

	Position secondRound;
	uniform_int_distribution<int> secondXRange(secondXPos1, secondXPos2);
	int secondX = secondXRange(gen);
	uniform_int_distribution<int> secondYRange(secondYPos1, secondYPos2);
	int secondY = secondYRange(gen);
	secondRound = Position{ (float)secondX, (float)secondY, 0 };

	vector<Position> returnVec;
	returnVec.push_back(firstRound);
	returnVec.push_back(secondRound);
	return returnVec;
}

void SocketWorker::SessionOut(ContentSessions & checkVector, string nickname)
{
	shared_ptr<ContentSession> logoutUser;
	for (auto &i : checkVector.Sessions)
	{
		if (i->nickname == nickname)
		{
			logoutUser = i;
		}
	}

	if (logoutUser != nullptr)
	{
		checkVector.Sessions.erase(remove_if(checkVector.Sessions.begin(), checkVector.Sessions.end(), [logoutUser](shared_ptr<ContentSession> userData) {
			return logoutUser->nickname == userData->nickname;
		}), checkVector.Sessions.end());
		if (checkVector.Sessions.size() <= 0)
		{
			mContentSessionVec.erase(remove_if(mContentSessionVec.begin(), mContentSessionVec.end(), [checkVector](ContentSessions userData) {
				return userData.ROOM_NUM == checkVector.ROOM_NUM;
			}), mContentSessionVec.end());
		}
	}
}
