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
			YieldProcessor(); //�ش� �����尡 CPU���� ���ɴ�� ���¶�°��� �˷��־�
			//CPU�� ������ �����忡 �����ϴ� �ٸ� ���� ���μ����� �ڽ��� �۾��� ������ �� �ְ� ��
			//->�ٸ����� WRITELOCK�� Ǯ���ٶ����� ���
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