#include "pch.h"
#include "PacketSession.h"
/*
#pragma warning(disable:4996)
PacketSession::PacketSession()
{
	memset(mPacketBuffer, 0, sizeof(mPacketBuffer));
	mRemainLength = 0;
	mCurrentPacketNumber = 0;
	mLastReadPacketNumber = 0;
}

PacketSession::~PacketSession()
{

}

bool PacketSession::Begin()
{
	//동기화
	mLock.EnterWriteLock();
	memset(mPacketBuffer, 0, sizeof(mPacketBuffer));
	mRemainLength = 0;
	mCurrentPacketNumber = 0;
	mLastReadPacketNumber = 0;

	if (!writeQueue.Begin())
		return false;

	mLock.LeaveWriteLock();
	return NetworkSession::Begin();
}

bool PacketSession::End()
{
	//동기화
	mLock.EnterWriteLock();
	mLastReadPacketInfoVectorForUdp.clear(); //UDP에서 사용할 받은 패킷정보 저장벡터

	if (!writeQueue.End())
		return false;
	mLock.LeaveWriteLock();
	return NetworkSession::End();
}

bool PacketSession::GetPacket(char * remoteAddress, uint16 remotePort, BYTE * packet, DWORD & packetLength)
{
	//동기화
	mLock.EnterWriteLock();
	if (!packet)
		return false;
	//여기는 Protocol에 맞지않으면 인식 x
	if (mRemainLength < sizeof(DWORD))
		return false;
	int32 PacketLength = 0;
	
	memcpy(&PacketLength, mPacketBuffer, sizeof(uint16));

	if (PacketLength > MAX_BUFFER_LENGTH || PacketLength <= 0)
	{
		mRemainLength = 0;
		return false;
	}
	
	if (PacketLength <= mRemainLength)
	{
		DWORD PacketNumber = 0;
		//DWORD Protocol = 0;
		memcpy(&PacketNumber, mPacketBuffer + sizeof(DWORD), sizeof(DWORD));//패킷번호
		//memcpy(&Protocol, mPacketBuffer + sizeof(DWORD) + sizeof(DWORD), sizeof(DWORD)); //프로토콜 번호

		//protocol = Protocol;
		packetLength = PacketLength - (sizeof(DWORD) * 2);

		memcpy(packet, mPacketBuffer + (sizeof(DWORD) * 2), packetLength);

		if (mRemainLength - PacketLength > 0)
			MoveMemory(mPacketBuffer, mPacketBuffer + PacketLength, mRemainLength - PacketLength);

		mRemainLength -= PacketLength;

		if (mRemainLength <= 0)
		{
			mRemainLength = 0;
			memset(mPacketBuffer, 0, sizeof(mPacketBuffer));
		}

		//mLastReadPacketInfoVectorForUdp에서 데이터를 받은적이 있는지 검사
		//있다면 그 정보에 패킷 번호만 업뎃
		for (auto &i : mLastReadPacketInfoVectorForUdp)
		{
			if (!strcmp(i.RemoteAddress, remoteAddress) && i.RemotePort == remotePort)
			{
				if (i.PacketNumber < PacketNumber)
				{
					i.PacketNumber = PacketNumber;
					return true;
				}
				else
					return false;
			}
		}

		READ_PACKET_INFO ReadPacketInfo;
		ReadPacketInfo.PacketNumber = PacketNumber;
		ReadPacketInfo.RemotePort = remotePort;
		strcpy(ReadPacketInfo.RemoteAddress, remoteAddress);
		
		mLastReadPacketInfoVectorForUdp.push_back(ReadPacketInfo);
		return true;
	}
	mLock.LeaveWriteLock();
	return false;
}

bool PacketSession::ReadFromPacketForIOCP(char * remoteAddress, uint16 & remotePort, DWORD readLength)
{
	//동기화
	mLock.EnterWriteLock();
	if (!NetworkSession::ReadFromForIOCP(remoteAddress, remotePort, mPacketBuffer + mRemainLength, readLength))
		return false;

	mRemainLength += readLength;
	mLock.LeaveWriteLock();
	return true;
}

bool PacketSession::WriteToPacket(char * remoteAddress, uint16 remotePort, const BYTE * packet, DWORD packetLength)
{
	//동기화
	mLock.EnterWriteLock();
	// LENGTH(4) | PACKET_NUMBER(4) | DATA(4084)
	if (!packet)
		return false;

	DWORD PacketLength = sizeof(DWORD) * 2 + packetLength;
	if (PacketLength >= MAX_BUFFER_LENGTH)
		return false;

	mCurrentPacketNumber++;

	BYTE TempBuffer[MAX_BUFFER_LENGTH] = { 0 };

	memcpy(TempBuffer, &PacketLength, sizeof(DWORD)); // LENGTH
	memcpy(TempBuffer + sizeof(DWORD), &mCurrentPacketNumber, sizeof(DWORD)); //packetNum
	//memcpy(TempBuffer + (sizeof(DWORD) * 2), &protocol, sizeof(DWORD));
	memcpy(TempBuffer + (sizeof(DWORD) * 2), packet, packetLength);

	BYTE* WriteData = writeQueue.Push(this, TempBuffer, PacketLength);

	mLock.LeaveWriteLock();
	return NetworkSession::WriteTo(remoteAddress, remotePort, WriteData, PacketLength);
}

bool PacketSession::WriteComplete()
{
	//동기화
	return writeQueue.Pop();
}

bool PacketSession::ResetUdp()
{
	//동기화
	mLock.EnterWriteLock();
	mLastReadPacketInfoVectorForUdp.clear();
	mLock.LeaveWriteLock();
	return true;
}
*/