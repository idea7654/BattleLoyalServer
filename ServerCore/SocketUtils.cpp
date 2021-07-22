#include "pch.h"
#include "SocketUtils.h"
#pragma warning(disable:4996)

SocketUtils::SocketUtils()
{
	memset(&mReadBuffer, 0, sizeof(mReadBuffer));
	memset(&mWriteBuffer, 0, sizeof(mWriteBuffer));
	memset(&mUdpRemoteInfo, 0, sizeof(mUdpRemoteInfo));
	mSocket = NULL;
}


SocketUtils::~SocketUtils()
{
	
}

bool SocketUtils::Begin(vector<Session> &sessionVector)
{
	if (mSocket)
		return false;

	memset(&mReadBuffer, 0, sizeof(mReadBuffer));
	memset(&mWriteBuffer, 0, sizeof(mWriteBuffer));
	memset(&mUdpRemoteInfo, 0, sizeof(mUdpRemoteInfo));

	if (WSAStartup(0x202, &mWsaData) == SOCKET_ERROR) // WSAStartup �������� ���� �߻��ϸ�
	{
		cout << "WinSock �ʱ�ȭ�κп��� ���� �߻� " << endl;
		WSACleanup(); // WS2_32.DLL�� ��� ����
	}

	mSocket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (mSocket == INVALID_SOCKET) // ���� �߻���
	{
		cout << "���� ���� ����!" << endl;
		closesocket(mSocket);
		WSACleanup();
		return false;
	}

	if (!mReadQueue.Begin())
		return false;

	if (!mWriteQueue.Begin())
		return false;

	mUserSession = sessionVector;

	return true;
}

bool SocketUtils::End()
{
	mSocket = NULL;
	memset(&mReadBuffer, 0, sizeof(mReadBuffer));
	memset(&mWriteBuffer, 0, sizeof(mWriteBuffer));
	memset(&mUdpRemoteInfo, 0, sizeof(mUdpRemoteInfo));

	CloseHandle(mRecvEvent);
	CloseHandle(mWriteEvent);
	mReadQueue.End();
	mWriteQueue.End();
	return true;
}

bool SocketUtils::Bind(uint16 port)
{
	if (!mSocket)
		return false;

	mUdpRemoteInfo.sin_family = AF_INET;
	mUdpRemoteInfo.sin_port = htons(port);
	mUdpRemoteInfo.sin_addr.s_addr = htonl(INADDR_ANY);
	if(::bind(mSocket, (SOCKADDR*)&mUdpRemoteInfo, sizeof(mUdpRemoteInfo)) == SOCKET_ERROR)
	{
		cout << "���ε带 �� �� �����ϴ�." << endl;
		closesocket(mSocket);
		WSACleanup();
		return false;
	}
	return true;
}

bool SocketUtils::RecvFrom()
{
	if (!mSocket)
		return false;
	int32 clientSize = sizeof(mClientInfo);
	int32 recvLen = 0;
	/* sockfd �������� ������ �����͸� �޾� recvBuffer�� �����ϰ�
	   Ŭ���̾�Ʈ �ּ� ������ clntAddr�� ���� */
	if ((recvLen = recvfrom(mSocket, mReadBuffer, sizeof(mReadBuffer) - 1, 0, (SOCKADDR*)&mClientInfo, &clientSize)) == -1)
		return false;
	char* remoteAddress = inet_ntoa(mClientInfo.sin_addr);
	int32 remotePort = htons(mClientInfo.sin_port);

	mLock.EnterWriteLock();
	if (mReadQueue.Push(mReadBuffer, recvLen, remoteAddress, remotePort) == false)
		return false;
	SetEvent(mRecvEvent);
	mLock.LeaveWriteLock();
	return true;
}

bool SocketUtils::WriteTo(char * remoteAddress, uint16 & remotePort, BYTE* data, DWORD dataLength)
{
	if (!mSocket)
		return false;

	char SendBuffer[MAX_BUFFER_LENGTH];
	memset(SendBuffer, 0, sizeof(SendBuffer));

	DWORD PacketLength = sizeof(DWORD) * 2 + dataLength;

	//int32 PacketNumber = 1;
	memcpy(SendBuffer, &PacketLength, sizeof(int32));
	memcpy(SendBuffer + sizeof(int32), &mPacketNumber, sizeof(int32));
	memcpy(SendBuffer + sizeof(int32) * 2, data, dataLength);
	//|  int32(PacketLength)  |  int32(PacketNumber)  |  data  |
	mLock.EnterWriteLock();
	if (mWriteQueue.Push(SendBuffer, PacketLength, remoteAddress, remotePort) == false)
		return false;

	SetEvent(mWriteEvent);
	mPacketNumber++;
	mLock.LeaveWriteLock();

	return true;
}

void SocketUtils::ReduceSessionTime() //Reduce SessionTime which in SessionVector, Use with Thread
{
	vector<Session> removeList;
	mLock.EnterReadLock();
	for (auto &i : mUserSession)
	{
		mLock.EnterWriteLock();
		i.isOnline--;
		mLock.LeaveWriteLock();
		if (i.isOnline < 0)
			removeList.push_back(i);
	}
	mLock.LeaveReadLock();
	for (auto &i : removeList)
	{
		mLock.EnterWriteLock();
		//mUserSession.erase(std::remove(mUserSession.begin(), mUserSession.end(), i), mUserSession.end());
		for (std::vector<Session>::iterator it = mUserSession.begin(); it != mUserSession.end(); it++) 
		{
			if (it->remoteAddress == i.remoteAddress && it->port == i.port)//����ü vector�� id���� 4�� ���Ҹ� ����
				mUserSession.erase(it);
		}
		mLock.LeaveWriteLock();
	}
}

bool SocketUtils::ResetSessionTime(Session &session) //Use this or 
//Read_PU_C2S_EXTEND_SESSION in Udp_ReadPacket.h
{
	mLock.EnterWriteLock();
	session.isOnline = 10;
	mLock.LeaveWriteLock();
	return true;
}

Session SocketUtils::FindSession(char* remoteAddress, uint16 port)
{
	for (auto &i : mUserSession)
	{
		if (i.remoteAddress == remoteAddress && i.port == port)
			return i;
	}
}