#pragma once
class SocketUtils
{
public:
	SocketUtils();
	~SocketUtils();

public:
	bool		Begin();
	bool		End();
	bool		Bind(uint16 port);
	bool		RecvFrom();
	bool		WriteTo(char* remoteAddress, uint16 &remotePort, BYTE *data, DWORD dataLength);
	
public:
	CircleQueue		mReadQueue;
	CircleQueue		mWriteQueue;
	HANDLE			mRecvEvent;
	HANDLE			mWriteEvent;

	RWLock			mLock;

	CHAR			mReadBuffer[MAX_BUFFER_LENGTH];
	CHAR			mWriteBuffer[MAX_BUFFER_LENGTH];

	SOCKET			mSocket;
	SOCKADDR_IN		mClientInfo;
private:
	WSADATA			mWsaData;

	SOCKADDR_IN		mUdpRemoteInfo;
	int32			mPacketNumber = 1;
};

