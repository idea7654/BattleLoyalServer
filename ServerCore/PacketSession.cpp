#include "pch.h"
#include "PacketSession.h"
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
	memset(mPacketBuffer, 0, sizeof(mPacketBuffer));
	mRemainLength = 0;
	mCurrentPacketNumber = 0;
	mLastReadPacketNumber = 0;

	if (!writeQueue.Begin())
		return false;
	return NetworkSession::Begin();
}

bool PacketSession::End()
{
	//동기화
	mLastReadPacketInfoVectorForUdp.clear(); //UDP에서 사용할 받은 패킷정보 저장벡터

	if (!writeQueue.End())
		return false;

	return NetworkSession::End();
}

bool PacketSession::GetPacket(char * remoteAddress, uint16 remotePort, DWORD & protocol, BYTE * packet, DWORD & packetLength)
{
	//동기화
	if (!packet)
		return false;

	if (mRemainLength < sizeof(DWORD))
		return false;

	int32 PacketLength = 0;
	memcpy(&PacketLength, mPacketBuffer, sizeof(int32));

	if (PacketLength > MAX_BUFFER_LENGTH || PacketLength <= 0)
	{
		mRemainLength = 0;
		return false;
	}

	if (PacketLength <= mRemainLength)
	{
		DWORD PacketNumber = 0;
		DWORD Protocol = 0;

		memcpy(&PacketNumber, mPacketBuffer + sizeof(DWORD), sizeof(DWORD));//패킷번호
		memcpy(&Protocol, mPacketBuffer + sizeof(DWORD) + sizeof(DWORD), sizeof(DWORD)); //프로토콜 번호

		protocol = Protocol;
		packetLength = PacketLength - (sizeof(DWORD) * 3);

		memcpy(packet, mPacketBuffer + (sizeof(DWORD) * 3), packetLength);

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

	return false;
}

bool PacketSession::ReadFromPacketForIOCP(char * remoteAddress, uint16 & remotePort, DWORD readLength)
{
	//동기화

	if (!NetworkSession::ReadFromForIOCP(remoteAddress, remotePort, mPacketBuffer + mRemainLength, readLength))
		return false;

	mRemainLength += readLength;

	return true;
}

bool PacketSession::ReadFromPacketForEventSelect(char * remoteAddress, uint16 & remotePort)
{
	//동기화

	DWORD ReadLength = 0;
	if (!NetworkSession::ReadFromForEventSelect(remoteAddress, remotePort, mPacketBuffer + mRemainLength, ReadLength))
		return false;

	mRemainLength += ReadLength;
	return true;
}

bool PacketSession::WriteToPacket(char * remoteAddress, uint16 remotePort, DWORD protocol, const BYTE * packet, DWORD packetLength)
{
	//동기화
	// LENGTH(4) | PACKET_NUMBER(4) | PROTOCOL(4) | DATA(4084)
	if (!packet)
		return false;

	DWORD PacketLength = sizeof(DWORD) * 3 + packetLength;
	if (PacketLength >= MAX_BUFFER_LENGTH)
		return false;

	mCurrentPacketNumber++;

	BYTE TempBuffer[MAX_BUFFER_LENGTH] = { 0 };

	memcpy(TempBuffer, &PacketLength, sizeof(DWORD)); // LENGTH
	memcpy(TempBuffer + sizeof(DWORD), &mCurrentPacketNumber, sizeof(DWORD));
	memcpy(TempBuffer + (sizeof(DWORD) * 2), &protocol, sizeof(DWORD));
	memcpy(TempBuffer + (sizeof(DWORD) * 3), &packet, packetLength);

	BYTE* WriteData = writeQueue.Push(this, TempBuffer, PacketLength);
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
	mLastReadPacketInfoVectorForUdp.clear();
	return true;
}
