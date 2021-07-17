#include "pch.h"
#include "MiniDump.h"
#pragma warning(disable:4996)

MiniDump::MiniDump()
{
	SetUnhandledExceptionFilter(GenerateDump);
	cout << "Dump filter setting complete!" << endl;
}

MiniDump::~MiniDump()
{
	
}

wstring s2ws(const string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

LONG WINAPI MiniDump::GenerateDump(PEXCEPTION_POINTERS pExceptionInfo)
{
	time_t currentSec = time(NULL);
	tm *t = localtime(&currentSec);
	string result;
	ostringstream oss;

	switch (t->tm_mon)
	{
	case(0): result = "Jan"; break;
	case(1): result = "Feb"; break;
	case(2): result = "Mar"; break;
	case(3): result = "Apr"; break;
	case(4): result = "May"; break;
	case(5): result = "Jun"; break;
	case(6): result = "Jul"; break;
	case(7): result = "Aug"; break;
	case(8): result = "Sep"; break;
	case(9): result = "Oct"; break;
	case(10): result = "Nov"; break;
	case(11): result = "Dec"; break;
	}

	oss.clear();
	oss << " " << setfill('0') << setw(2) << t->tm_mday << " " << t->tm_year + 1900;
	oss << " " << setfill('0') << setw(2) << t->tm_hour;
	oss << " " << setfill('0') << setw(2) << t->tm_min;
	oss << " " << setfill('0') << setw(2) << t->tm_sec << ".dmp" << '\0';

	result = result + oss.str();

	wstring stemp = s2ws(result);
	HANDLE hFile = CreateFile(stemp.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
		dumpInfo.ThreadId = ::GetCurrentThreadId();
		dumpInfo.ExceptionPointers = pExceptionInfo;
		dumpInfo.ClientPointers = false;
		
		MiniDumpWriteDump(::GetCurrentProcess(), ::GetCurrentProcessId(), hFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
		CloseHandle(hFile);
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

static MiniDump miniDump;