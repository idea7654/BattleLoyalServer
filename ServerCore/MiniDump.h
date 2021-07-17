#pragma once
#include <DbgHelp.h>
#include <stdio.h>
#pragma comment(lib, "Dbghelp.lib")

class MiniDump
{
public:
	MiniDump();
	~MiniDump();
public:
	static LONG WINAPI	GenerateDump(PEXCEPTION_POINTERS pExceptionInfo);
public:
	//static LPTOP_LEVEL_EXCEPTION_FILTER mTopLevelExceptionFilter;
};

