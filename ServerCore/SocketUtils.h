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

	virtual void	Init();
	virtual void	ReadEvent();
	virtual void	WriteEvent();
	virtual void	ThreadManage();
	virtual bool	CheckPacketNum(shared_ptr<Session> &session, uint32 PacketNumber);
	
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

