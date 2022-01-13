#pragma once

const int ALLOC_BLOCK_SIZE = 50;
template <typename T>
class MemoryPool
{
public:
	//static void* operator new(size_t allocLength)
	static void* Alloc(size_t allocLength)
	{
		//assert(sizeof(T) == allocLength);
		//assert(sizeof(T) >= sizeof(char*));

		if (!mFreePointer)
			allocBlock();
		BYTE* ReturnPointer = mFreePointer;
		//printf("Memory : %x \n", ReturnPointer);
		mFreePointer = *reinterpret_cast<BYTE**>(ReturnPointer);
		return reinterpret_cast<void*>(ReturnPointer);
	}

	//static void operator delete(void* deletePointer)
	static void Detach(void* deletePointer)
	{
		*reinterpret_cast<BYTE**>(deletePointer) = mFreePointer;
		//cout << deletePointer << endl;
		mFreePointer = static_cast<BYTE*>(deletePointer);
	}

private:
	static void allocBlock()
	{
		cout << "�����!!" << endl;
		mFreePointer = new BYTE[sizeof(T) * ALLOC_BLOCK_SIZE];
		BYTE** Current = reinterpret_cast<BYTE**>(mFreePointer); //�Ҵ�� ù �� �޸� ������
		BYTE* Next = mFreePointer; //�Ҵ�� ù �޸�
		for (int32 i = 0; i < ALLOC_BLOCK_SIZE - 1; ++i)
		{
			Next += sizeof(T);
			*Current = Next;
			Current = reinterpret_cast<BYTE**>(Next);
		}
		*Current = nullptr;
	}

public:
	static BYTE *mFreePointer;

protected:
	~MemoryPool(){}
};
template <typename T>
BYTE* MemoryPool<T>::mFreePointer;
