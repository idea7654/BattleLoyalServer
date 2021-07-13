#include "pch.h"
#include "NetworkSession.h"

DWORD WINAPI ReliableUdpThreadCallback(void* parameter)
{
	NetworkSession *Owner = (NetworkSession*)parameter;
	Owner->ReliableUdpThreadCallback();

	return 0;
}

NetworkSession::NetworkSession()
{
	memset(&mAcceptOverlapped, 0, sizeof(mAcceptOverlapped));
	memset(&mReadOverlapped, 0, sizeof(mReadOverlapped));
	memset(&mWriteOverlapped, 0, sizeof(mWriteOverlapped));
	memset(&mReadBuffer, 0, sizeof(mReadBuffer));
	memset(&mUdpRemoteInfo, 0, sizeof(mUdpRemoteInfo));

	mSocket = NULL;
	mReliableUdpThreadHandle = NULL;
	mReliableUdpThreadStartupEvent = NULL;
	mReliableUdpThreadDestroyEvent = NULL;
	mReliableUdpThreadWakeUpEvent = NULL;
	mReliableUdpWriteCompleteEvent = NULL;
	mIsReliableUdpSending = false;

	mAcceptOverlapped.IoType = IO_TYPE::IO_ACCEPT;
	mReadOverlapped.IoType = IO_TYPE::IO_READ;
	mWriteOverlapped.IoType = IO_TYPE::IO_WRITE;

	mAcceptOverlapped.Object = this;
	mReadOverlapped.Object = this;
	mWriteOverlapped.Object = this;
}

bool NetworkSession::Begin()
{
	//Lock넣기
	if (mSocket)
		return false;

	memset(&mReadBuffer, 0, sizeof(mReadBuffer));
	memset(&mUdpRemoteInfo, 0, sizeof(mUdpRemoteInfo));

	mSocket = NULL;
	mReliableUdpThreadHandle = NULL;
	mReliableUdpThreadStartupEvent = NULL;
	mReliableUdpThreadDestroyEvent = NULL;
	mReliableUdpThreadWakeUpEvent = NULL;
	mReliableUdpWriteCompleteEvent = NULL;
	mIsReliableUdpSending = false;

	return true;
}

bool NetworkSession::End()
{
	//동기화

	if (!mSocket)
		return false;

	shutdown(mSocket, SD_BOTH);
	closesocket(mSocket);

	if (mReliableUdpThreadHandle)
	{
		SetEvent(mReliableUdpThreadDestroyEvent);
		WaitForSingleObject(mReliableUdpThreadHandle, INFINITE);
		CloseHandle(mReliableUdpThreadHandle);
	}

	if (mReliableUdpThreadDestroyEvent)
		CloseHandle(mReliableUdpThreadDestroyEvent);

	if (mReliableUdpThreadStartupEvent)
		CloseHandle(mReliableUdpThreadStartupEvent);

	if (mReliableUdpThreadWakeUpEvent)
		CloseHandle(mReliableUdpThreadWakeUpEvent);

	if (mReliableUdpWriteCompleteEvent)
		CloseHandle(mReliableUdpWriteCompleteEvent);

	//mReliableWriteQueue.End();
	return true;
}

bool NetworkSession::UdpBind(uint16 port)
{
	//동기화

	if (mSocket)
		return false;

	SOCKADDR_IN RemoteAddressInfo;

	RemoteAddressInfo.sin_family = AF_INET;
	RemoteAddressInfo.sin_port = htons(port);
	RemoteAddressInfo.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	mSocket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (mSocket == INVALID_SOCKET)
		return false;

	if (::bind(mSocket, (SOCKADDR*)&RemoteAddressInfo, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		End();
		return false;
	}

	mReliableUdpThreadDestroyEvent = CreateEvent(0, false, false, 0);
	if (mReliableUdpThreadDestroyEvent == NULL)
	{
		End();
		return false;
	}

	mReliableUdpThreadStartupEvent = CreateEvent(0, false, false, 0);
	if (mReliableUdpThreadStartupEvent == NULL)
	{
		End();
		return false;
	}

	mReliableUdpThreadWakeUpEvent = CreateEvent(0, false, false, 0);
	if (mReliableUdpThreadWakeUpEvent == NULL)
	{
		End();
		return false;
	}

	mReliableUdpWriteCompleteEvent = CreateEvent(0, false, false, 0);
	if (mReliableUdpWriteCompleteEvent == NULL)
	{
		End();
		return false;
	}
	
	if (!mReliableWriteQueue.Begin())
	{
		End();
		return false;
	}
	
	DWORD ReliableUdpThreadId = 0;
	mReliableUdpThreadHandle = CreateThread(NULL, 0, ::ReliableUdpThreadCallback, this, 0, &ReliableUdpThreadId);
	WaitForSingleObject(mReliableUdpThreadStartupEvent, INFINITE);

	return true;
}

void NetworkSession::ReliableUdpThreadCallback()
{
	DWORD EventID = 0;

	HANDLE ThreadEvents[2] = { mReliableUdpThreadDestroyEvent, mReliableUdpThreadWakeUpEvent };

	char RemoteAddress[32] = { 0 };
	uint16 RemotePort = 0;
	BYTE Data[MAX_BUFFER_LENGTH] = { 0 };
	DWORD DataLength = 0;
	void *Object = NULL;

	while (true)
	{
		SetEvent(mReliableUdpThreadStartupEvent);
		EventID = WaitForMultipleObjects(2, ThreadEvents, false, INFINITE);
		switch (EventID)
		{
		case WAIT_OBJECT_0:
			return;
		case WAIT_OBJECT_0 + 1:
		NEXT_DATA:
			if (mReliableWriteQueue.Pop(&Object, Data, DataLength, RemoteAddress, RemotePort)) //Write일경우 1개의 보낼 데이터 Pop
			{
		RETRY:
				if (!WriteTo2(RemoteAddress, RemotePort, Data, DataLength))
					return;

				DWORD Result = WaitForSingleObject(mReliableUdpWriteCompleteEvent, 10);
				if (Result == WAIT_OBJECT_0)
					goto NEXT_DATA;
				else
					goto RETRY;
			}
			else
			{
				mIsReliableUdpSending = false; //큐에 더이상 보낼데이터x
			}
			break;
		}
	}
}

bool NetworkSession::WriteTo(char * remoteAddress, uint16 remotePort, BYTE * data, DWORD dataLength)
{
	//동기화
	if (!mSocket)
		return false;

	if (!remoteAddress || remotePort <= 0 || !data || dataLength <= 0)
		return false;

	if (!mReliableWriteQueue.Push(this, data, dataLength, remoteAddress, remotePort))
		return false;

	if (!mIsReliableUdpSending)
	{
		mIsReliableUdpSending = true;
		SetEvent(mReliableUdpThreadWakeUpEvent);
	}

	return true;
}

bool NetworkSession::WriteTo2(char * remoteAddress, uint16 remotePort, BYTE * data, DWORD dataLength)
{
	//동기화

	if (!mSocket)
		return false;
	if (!remoteAddress || remotePort <= 0 || !data || dataLength <= 0)
		return false;

	WSABUF wsaBuf;
	DWORD WriteBytes = 0;
	DWORD WriteFlag = 0;

	SOCKADDR_IN RemoteAddressInfo;
	int32 RemoteAddressInfoSize = sizeof(RemoteAddressInfo);

	wsaBuf.buf = (char*)data;
	wsaBuf.len = dataLength;

	RemoteAddressInfo.sin_family = AF_INET;
	RemoteAddressInfo.sin_addr.S_un.S_addr = inet_addr(remoteAddress);
	RemoteAddressInfo.sin_port = htons(remotePort);

	int32 ReturnValue = WSASendTo(mSocket, &wsaBuf, 1, &WriteBytes, WriteFlag, (SOCKADDR*)&RemoteAddressInfo, RemoteAddressInfoSize
		, &mWriteOverlapped.Overlapped, NULL);

	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		End();
		return false;
	}
	return true;
}


