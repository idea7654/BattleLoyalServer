#pragma once

#include "Types.h"
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <cstring>
#include <vld.h>
#include <WinSock2.h>
#include <string.h>

const int32 MAX_BUFFER_LENGTH = 1024 * 10;

using namespace std;

#pragma comment(lib, "ws2_32.lib")

#ifdef _SERVER_SIDE
#define MAX_QUEUE_LENGTH	50
#else
#define MAX_QUEUE_LENGTH	500
#endif

#include <MSWSock.h>
#include <WS2tcpip.h>

#include "RWLock.h"
#include "CoreMacro.h"
#include "CircleQueue.h"
#include "MemoryManager.h"

#include "SocketUtils.h"
#include "Log.h"
#include "CoreGlobal.h"
#include "MiniDump.h"