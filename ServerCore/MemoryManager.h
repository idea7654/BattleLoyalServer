#pragma once
#include "MemoryPool.h"
/*
template <typename T, typename... Args>
T* Attach()
{
	T* myPtr = static_cast<T*>(MemoryPool<T>::Alloc(sizeof(T)));
	//cout << myPtr << endl;
	new(myPtr) T(forward<Args>(args)...);
	return myPtr; 
}

template <typename T>
void Detatch(T *obj)
{
	obj->~T();
	MemoryPool<T>::Detach(obj);
}

template <typename T, typename... Args>
shared_ptr<T> MakeShared()
{
	return shared_ptr<T>{ Attach<T>(forward<Args>(args)...), Detatch<T> };
}
*/

/*
template <typename T, typename... Args>
T* Attach()
{
	MemoryPool::MEMORY_DESC *desc = GMemoryPool.Alloc();
	T* myPtr = (T*)desc->pBuffer;
	new(myPtr) T(forward<Args>(args)...);
	return myPtr;
}

template <typename T>
void Detatch(T *obj)
{
	obj->~T();
	//MemoryPool<T>::Detach(obj);
}

template <typename T, typename... Args>
shared_ptr<T> MakeShared()
{
	return shared_ptr<T>{ Attach<T>(forward<Args>(args)...), Detatch<T> };
}
*/