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

const int32 MAX_BUFFER_LENGTH = 4096;

using namespace std;

#pragma comment(lib, "ws2_32.lib")
#ifdef _SERVER_SIDE
#define MAX_QUEUE_LENGTH	50
#else
#define MAX_QUEUE_LENGTH	500
#endif

#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>

#include "CircleQueue.h"
#include "MemoryManager.h"

#include "NetworkSession.h"

#include "Log.h"
