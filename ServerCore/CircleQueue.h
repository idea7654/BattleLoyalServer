#pragma once

struct QUEUE_DATA
{
	void	*Object;
	BYTE	Data[MAX_BUFFER_LENGTH];
	DWORD	DataLength;
	
	char	RemoteAddress[32];
	uint16	RemotePort;
	DWORD	Protocol;
};

class CircleQueue
{
public:
	CircleQueue()
	{
		memset(mQueue, 0, sizeof(mQueue));
		mQueueHead = mQueueTail = 0;
	}
	~CircleQueue(){}

private:
	QUEUE_DATA mQueue[MAX_QUEUE_LENGTH];
	DWORD mQueueHead;
	DWORD mQueueTail;

public:
	bool Begin()
	{
		memset(mQueue, 0, sizeof(mQueue));
		return true;
	}

	bool End() { return true; }
	bool Push(QUEUE_DATA data)
	{
		DWORD TempTail = (mQueueTail + 1) % MAX_QUEUE_LENGTH;
		if (TempTail == mQueueHead)
			return false;

		memcpy(&mQueue[TempTail], &data, sizeof(QUEUE_DATA));
		mQueueTail = TempTail;
		return true;
	}
	BYTE* Push(void *object, BYTE *data, DWORD dataLength, char* remoteAddress, uint16 remotePort);

	bool Pop(QUEUE_DATA& data)
	{
		if (mQueueHead == mQueueTail)
			return false;

		DWORD TempHead = (mQueueHead + 1) % MAX_QUEUE_LENGTH;
		memcpy(&data, &mQueue[TempHead], sizeof(QUEUE_DATA));
		mQueueHead = TempHead;
		return true;
	}

	bool Pop(void **object, BYTE *data, DWORD &dataLength, char* remoteAddress, uint16 &remotePort);
	bool IsEmpty()
	{
		if (mQueueHead == mQueueTail)
			return true;
		return false;
	}
};

