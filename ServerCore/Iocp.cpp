#include "pch.h"
#include "Iocp.h"

DWORD WINAPI WorkerThreadCallback(void* parameter)
{
	Iocp *Owner = (Iocp*)parameter;
	Owner->WorkerThreadCallback();

	return 0;
}

void Iocp::WorkerThreadCallback() //데이터 받을 작업스레드
{
	bool Successed					= false;
	DWORD NumberOfByteTransfered	= 0;
	void *CompletionKey				= NULL;
	OVERLAPPED *Overlapped			= NULL;
	_OVERLAPPED_EX *OverlappedEx	= NULL;
	void *Object					= NULL;
	while (true)
	{
		SetEvent(mStartHandle);
		Successed = GetQueuedCompletionStatus(mHandle, &NumberOfByteTransfered, (PULONG_PTR) &CompletionKey, &Overlapped, INFINITE);
		
		if (!CompletionKey)
			return; //by End()

		OverlappedEx = (_OVERLAPPED_EX*)Overlapped;
		Object = OverlappedEx->Object;

		if (!Successed || (Successed && !NumberOfByteTransfered))
		{
			if (OverlappedEx->IoType == IO_TYPE::IO_ACCEPT)
				OnIoConnected(Object);
			else
				OnIoDisconnected(Object);
			continue;
		}
		switch (OverlappedEx->IoType)
		{
		case IO_TYPE::IO_READ:
			OnIoRead(Object, NumberOfByteTransfered);
			break;
		case IO_TYPE::IO_WRITE:
			OnIoWrite(Object, NumberOfByteTransfered);
			break;
		}
	}
}

Iocp::Iocp()
{
	mHandle = NULL;
	mThreadCount = 0;
	mStartHandle = NULL;
}

bool Iocp::BeginIocp()
{
	mHandle = NULL;

	SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);

	mThreadCount = SystemInfo.dwNumberOfProcessors * 2;
	mHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	if (!mHandle)
		return false;
	mStartHandle = CreateEvent(0, false, false, 0);
	if (mStartHandle == NULL)
	{
		End();
		return false;
	}
	for (int32 i = 0; i < mThreadCount; i++)
	{
		HANDLE workerThread = CreateThread(NULL, 0, ::WorkerThreadCallback, this, 0, NULL);
		mThreadVec.push_back(workerThread);
		WaitForSingleObject(mStartHandle, INFINITE);
	}
	return true;
}

bool Iocp::End()
{
	for (auto &i : mThreadVec)
		PostQueuedCompletionStatus(mHandle, 0, 0, NULL);

	for (auto &i : mThreadVec)
	{
		WaitForSingleObject(i, INFINITE);
		CloseHandle(i);
	}

	if (mHandle)
		CloseHandle(mHandle);

	mThreadVec.clear();

	if (mStartHandle)
		CloseHandle(mStartHandle);

	return true;
}

bool Iocp::RegisterSocketToIocp(SOCKET socket, ULONG_PTR completionKey)
{
	if (!socket || !completionKey)
	{
		return false;
	}
	mHandle = CreateIoCompletionPort((HANDLE)socket, mHandle, completionKey, 0);

	if (!mHandle)
		return false;
	return true;
}


