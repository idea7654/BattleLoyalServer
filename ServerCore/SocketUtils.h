#pragma once
class SocketUtils
{
public:
	SocketUtils();
	~SocketUtils();

public:
	bool		Begin(vector<Session> &sessionVector);
	bool		End();
	bool		Bind(uint16 port);
	bool		RecvFrom();
	bool		WriteTo(char* remoteAddress, uint16 &remotePort, BYTE *data, DWORD dataLength);
	void		ReduceSessionTime();
	bool		ResetSessionTime(Session &session);
	Session		FindSession(char* remoteAddress, uint16 port);
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
	vector<Session> mUserSession;
private:
	WSADATA			mWsaData;

	SOCKADDR_IN		mUdpRemoteInfo;
	int32			mPacketNumber = 1;
};

