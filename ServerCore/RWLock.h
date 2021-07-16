#pragma once
class RWLock
{
public:
	RWLock();
	~RWLock();

	RWLock(const RWLock& rhs) = delete;
	RWLock& operator=(const RWLock& rhs) = delete;

	void EnterWriteLock();
	void LeaveWriteLock();
	void EnterReadLock();
	void LeaveReadLock();

	long GetLockFlag() const { return mLockFlag; }

private:
	enum LockFlag
	{
		LP_WRITE_MASK = 0x7FF00000,
		LP_WRITE_FLAG = 0x00100000,
		LP_READ_MASK = 0x000FFFFF
	};

	volatile long mLockFlag;
};