#pragma once
class Lock
{
public:
	Lock();
	~Lock();
private:
	recursive_mutex		mMutex;
	char*				mName;
	size_t				mLockId;
	size_t				mThreadId;

	char				mLockingFile;
	int32				mLockingLine;

public:
	Lock(char* name);
	virtual ~Lock();

	const char* name();
	size_t lockId();

	recursive_mutex& mutex();
	void lock(LPCWSTR fileName, int32 lineNo);
	void unlock();

	void setThreadId(size_t id);
	size_t threadId();
};

class LockSafeScope
{
public:
	LockSafeScope(Lock *lock, LPCWSTR fileName, int32 lineNo);
	~LockSafeScope();

private:
	Lock *mLock;
};

#define SAFE_LOCK(lock)	LockSafeScope __lockSafe(&lock, _W_(__FILE__), __LINE__);

class LockManager //Global
{
public:
	LockManager();

	Lock* searchLockCycle(Lock *newLock);
	Lock* checkDeadLock(Lock *lock);
private:
	size_t isSeed;
};

