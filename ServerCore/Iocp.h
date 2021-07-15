#pragma once
class Iocp
{
public:
	Iocp();
	~Iocp() {};

public:
	bool BeginIocp();
	bool End();

	bool RegisterSocketToIocp(SOCKET socket, ULONG_PTR completionKey);

	void WorkerThreadCallback();

private:
	HANDLE				mHandle;
	int32				mThreadCount;
	HANDLE				mStartHandle;
	vector<HANDLE>		mThreadVec;

protected:
	virtual void		OnIoRead(void *object, DWORD dataLength) = 0;
	virtual void		OnIoWrite(void *object, DWORD dataLength) = 0;
	virtual void		OnIoConnected(void *object) = 0;
	virtual void		OnIoDisconnected(void *object) = 0;
};

