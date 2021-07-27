#include "pch.h"
#include "CircleQueue.h"
#include <stdio.h>
#pragma warning(disable:4996)

bool CircleQueue::Push(char * data, DWORD dataLength, char * remoteAddress, uint16 remotePort)
{
	mLock.EnterWriteLock();
	DWORD TempTail = (mQueueTail + 1) % MAX_QUEUE_LENGTH;
	if (TempTail == mQueueHead)
		return false;
	
	mQueue[TempTail].DataLength = dataLength;
	mQueue[TempTail].RemotePort = remotePort;

	memset(mQueue[TempTail].RemoteAddress, 0, sizeof(mQueue[TempTail].RemoteAddress));
	strncpy_s(mQueue[TempTail].RemoteAddress, remoteAddress, sizeof(mQueue[TempTail].RemoteAddress));

	memset(mQueue[TempTail].Data, 0, sizeof(mQueue[TempTail].Data));
	memcpy(mQueue[TempTail].Data, data, dataLength);

	mQueueTail = TempTail;
	mLock.LeaveWriteLock();
	return true;
}


bool CircleQueue::Pop(char *data, DWORD &dataLength, char *remoteAddress, uint16 &remotePort)
{
	mLock.EnterWriteLock();
	if (!data)
		return false;
	if (mQueueHead == mQueueTail)
		return false;

	DWORD TempHead = (mQueueHead + 1) % MAX_QUEUE_LENGTH;

	dataLength = mQueue[TempHead].DataLength;
	remotePort = mQueue[TempHead].RemotePort;

	strncpy(remoteAddress, mQueue[TempHead].RemoteAddress, sizeof(mQueue[TempHead].RemoteAddress));
	memcpy(data, mQueue[TempHead].Data, mQueue[TempHead].DataLength);

	mQueueHead = TempHead;
	mLock.LeaveWriteLock();
	return true;
}
/*
bool CircleQueue::Pop(void ** object, BYTE * data, DWORD & dataLength, char* remoteAddress, uint16 & remotePort)
{
	//동기화

	if (!object || !data)
		return false;

	if (mQueueHead == mQueueTail)
		return false;

	DWORD TempHead = (mQueueHead + 1) % MAX_QUEUE_LENGTH;

	dataLength = mQueue[TempHead].DataLength;
	//*object = mQueue[TempHead].Object;
	remotePort = mQueue[TempHead].RemotePort;

	strncpy(remoteAddress, mQueue[TempHead].RemoteAddress, sizeof(mQueue[TempHead].RemoteAddress));
	memcpy(data, mQueue[TempHead].Data, mQueue[TempHead].DataLength);

	mQueueHead = TempHead;

	return true;
}

bool CircleQueue::Pop()
{
	//동기화

	if (mQueueHead == mQueueTail)
		return false;

	DWORD TempHead = (mQueueHead + 1) % MAX_QUEUE_LENGTH;
	mQueueHead = TempHead;
	return true;
}
*/