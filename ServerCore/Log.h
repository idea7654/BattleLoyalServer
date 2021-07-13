#pragma once
/*
class Log
{
public:
	static bool WriteLog(const TCHAR* data, ...)
	{
		SYSTEMTIME lst;

		TCHAR CurrentDate[32]				= { 0 };
		TCHAR CurrentFileName[MAX_PATH]		= { 0 };
		FILE* FilePtr						= NULL;
		TCHAR DebugLog[MAX_BUFFER_LENGTH]	= { 0 };

		va_list ap;
		TCHAR Log[MAX_BUFFER_LENGTH]		= { 0 };

		va_start(ap, data);
		_vstprintf(Log, data, ap);
		va_end(ap);

		GetLocalTime(&lst);
		_sntprintf(CurrentDate, 32, _T("%d-%d-%d %d:%d:%d"),
			lst.wYear, lst.wMonth, lst.wDay,
			lst.wHour, lst.wMinute, lst.wSecond);

		_sntprintf(CurrentFileName, MAX_PATH, _T("LOG_%d-%d-%d %d.log"),
			lst.wYear, lst.wMonth, lst.wDay, lst.wHour);

		FilePtr = _tfopen(CurrentFileName, _T("a"));
		if (!FilePtr)
			return false;

		_ftprintf(FilePtr, _T("[%s] %S n"), CurrentDate, Log);
		_sntprintf(DebugLog, MAX_BUFFER_LENGTH, _T("[%s] %S\n"), CurrentDate, Log);

		fflush(FilePtr);
		fclose(FilePtr);

		OutputDebugString(DebugLog);
		_tprintf(_T("%s"), DebugLog);

		return true;
	}
};
*/