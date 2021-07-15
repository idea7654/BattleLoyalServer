#pragma once

class PacketSession : public NetworkSession
{
public:
	struct READ_PACKET_INFO
	{
		char	RemoteAddress[14];
		uint16	RemotePort;
		DWORD	PacketNumber;
	};
public:
	PacketSession();
	~PacketSession();

public:
	bool	Begin();
	bool	End();

	bool	GetPacket(char* remoteAddress, uint16 remotePort, DWORD &protocol, BYTE* packet, DWORD &packetLength);

	bool	ReadFromPacketForIOCP(char* remoteAddress, uint16 &remotePort,DWORD readLength);
	bool	ReadFromPacketForEventSelect(char* remoteAddress, uint16 &remotePort);
	bool	WriteToPacket(char* remoteAddress, uint16 remotePort, DWORD protocol, const BYTE* packet, DWORD packetLength);

	bool	WriteComplete();

	bool	ResetUdp();
private:
	BYTE								mPacketBuffer[MAX_BUFFER_LENGTH * 3];
	int32								mRemainLength;
	DWORD								mCurrentPacketNumber;

	DWORD								mLastReadPacketNumber;

	CircleQueue							writeQueue;

	vector<READ_PACKET_INFO>			mLastReadPacketInfoVectorForUdp;
};

