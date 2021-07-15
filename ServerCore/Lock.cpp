#include "pch.h"
#include "Lock.h"

Lock::Lock(char * name)
{
	mLockId = GLockManager->generalId();
	mName = name;

	mLockingFile = { 0 };
	mLockingLine = INVALID_LINE;
}

Lock::~Lock()
{
}

const char * Lock::name()
{
	return nullptr;
}

size_t Lock::lockId()
{
	return size_t();
}

recursive_mutex & Lock::mutex()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
}

void Lock::lock(LPCWSTR fileName, int32 lineNo)
{
}

void Lock::unlock()
{
}

void Lock::setThreadId(size_t id)
{
}

size_t Lock::threadId()
{
	return size_t();
}

LockSafeScope::LockSafeScope(Lock * lock, LPCWSTR fileName, int32 lineNo)
{
}

LockSafeScope::~LockSafeScope()
{
}

LockManager::LockManager()
{
}

Lock * LockManager::searchLockCycle(Lock * newLock)
{
	return nullptr;
}

Lock * LockManager::checkDeadLock(Lock * lock)
{
	return nullptr;
}
