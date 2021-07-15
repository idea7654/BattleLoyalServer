#include "pch.h"
#include "CoreGlobal.h"

LockManager* GLockManager = nullptr;

CoreGlobal::CoreGlobal()
{
	GLockManager = new LockManager();
}

CoreGlobal::~CoreGlobal()
{
	delete GLockManager;
}
