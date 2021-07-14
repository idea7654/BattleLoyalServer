#include "pch.h"
#include "MemoryPool.h"

MemoryPool::MemoryPool(size_t PoolSize, size_t MemorySize)
{
	for (size_t i = 0; i < PoolSize; ++i)
	{
		MEMORY_DESC MD;

		MD.isUsing = false;
		MD.pBuffer = new BYTE[MemorySize];
		MD.Index = i;

		pools.push_back(MD);
	}

	freeMemIndex = 0;
	hEvent = CreateEvent(NULL, true, true, NULL);
}

MemoryPool::~MemoryPool()
{
	CloseHandle(hEvent);

	for (size_t i = 0; i < pools.size(); ++i)
	{
		delete[] pools[i].pBuffer;
	}
}

MemoryPool::MEMORY_DESC * MemoryPool::Alloc()
{
	WaitForSingleObject(hEvent, INFINITE);
	//동기화

	if (!pools[freeMemIndex].isUsing)
	{
		pools[freeMemIndex].isUsing = true;
		int32 oldMemIndex = freeMemIndex;
		freeMemIndex++;

		if (freeMemIndex > (pools.size() - 1))
		{
			ResetEvent(hEvent);
			freeMemIndex = 0;
		}

		return &pools[oldMemIndex];
	}

	//언락
	ResetEvent(hEvent);
	return Alloc();
}

void MemoryPool::Release(MEMORY_DESC * pMD)
{
	pMD->isUsing = false;
	freeMemIndex = pMD->Index;
	SetEvent(hEvent);
}