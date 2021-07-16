#pragma once

enum IO_TYPE : uint8
{
	IO_READ,
	IO_WRITE,
	IO_ACCEPT
};

struct _OVERLAPPED_EX
{
	OVERLAPPED Overlapped;
	IO_TYPE IoType;
	void* Object;
};

class NetworkSession
{
public:
	NetworkSession();
	~NetworkSession() {};

	bool			Begin();
	bool			End();
	bool			UdpBind(uint16 port);

	void			ReliableUdpThreadCallback();

	bool			InitializeReadFromForIOCP();
	bool			ReadFromForIOCP(char* remoteAddress, uint16 &remotePort, BYTE *data, DWORD &dataLength);
	bool			ReadFromForEventSelect(char* remoteAddress, uint16 &remotePort, BYTE* data, DWORD &dataLength);

	bool			Write(BYTE *data, DWORD dataLength);
	bool			WriteTo(char* remoteAddress, uint16 remotePort, BYTE *data, DWORD dataLength);
	bool			WriteTo2(char* remoteAddress, uint16 remotePort, BYTE *data, DWORD dataLength);

	SOCKET			GetSocket();
private:
	_OVERLAPPED_EX	mAcceptOverlapped;
	_OVERLAPPED_EX	mReadOverlapped;
	_OVERLAPPED_EX	mWriteOverlapped;

	BYTE			mReadBuffer[MAX_BUFFER_LENGTH];
	SOCKADDR_IN		mUdpRemoteInfo;

	SOCKET			mSocket;

	HANDLE			mReliableUdpThreadHandle;
	HANDLE			mReliableUdpThreadStartupEvent;
	HANDLE			mReliableUdpThreadDestroyEvent;
	HANDLE			mReliableUdpThreadWakeUpEvent;
	HANDLE			mReliableUdpWriteCompleteEvent;

	CircleQueue		mReliableWriteQueue;
	RWLock			mLock;

	bool			mIsReliableUdpSending;
};

