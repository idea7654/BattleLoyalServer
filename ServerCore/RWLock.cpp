#include "pch.h"
#include "RWLock.h"


RWLock::RWLock()
{
}


RWLock::~RWLock()
{
}

void RWLock::EnterWriteLock()
{
	while (true)
	{
		while (mLockFlag & LP_WRITE_MASK)
			YieldProcessor(); //해당 스레드가 CPU에게 스핀대기 상태라는것을 알려주어
			//CPU는 하이퍼 스레드에 존재하는 다른 논리적 프로세서가 자신의 작업을 수행할 수 있게 함
			//->다른놈이 WRITELOCK을 풀어줄때까지 대기
		if ((InterlockedAdd(&mLockFlag, LP_WRITE_FLAG) & LP_WRITE_MASK) == LP_WRITE_FLAG)
		{
			while (mLockFlag & LP_READ_MASK)
				YieldProcessor();

			return;
		}
		InterlockedAdd(&mLockFlag, -LP_WRITE_FLAG);
	}
}

void RWLock::LeaveWriteLock()
{
	InterlockedAdd(&mLockFlag, -LP_WRITE_FLAG);
}

void RWLock::EnterReadLock()
{
	while (true)
	{
		while (mLockFlag & LP_WRITE_FLAG)
			YieldProcessor();

		if ((InterlockedIncrement(&mLockFlag) & LP_WRITE_MASK) == 0)
			return;
		else
			InterlockedDecrement(&mLockFlag);
	}
}

void RWLock::LeaveReadLock()
{
	InterlockedDecrement(&mLockFlag);
}