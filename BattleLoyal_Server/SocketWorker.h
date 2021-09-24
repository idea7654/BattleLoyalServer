#pragma once

enum Gun
{
	NORMAL = 0,
};
struct ContentSession : Session
{
	Position pos;
	Direction dir;
	Gun gun;
	float water;
	float hp;
};

struct SessionGun
{
	Position Gun_pos;
	Gun Gun_Type;
};

class SocketWorker : public SocketUtils
{
public:
	SocketWorker();
	~SocketWorker();

public:
	struct ContentSessions
	{
		vector<shared_ptr<ContentSession>>	Sessions;
		int32								ROOM_NUM;
	};

public:
	void						Init();
	void						ReadEvent();
	void						WriteEvent();
	void						ThreadManage();
	bool						CheckPacketNum(shared_ptr<Session> &session, uint32 PacketNumber);

	void						ReduceSessionTime();
	bool						ResetSessionTime(shared_ptr<Session> &session);
	shared_ptr<Session>			FindSession(string nickname);
	shared_ptr<Session>			FindSession(char* remoteAddress, uint16 port);
	auto						FindContentSession(int32 RoomNum);
	shared_ptr<ContentSession>	FindContentSession(string nickname);
	ContentSessions				FindContentSessionInVec(int32 RoomNum);
	void						GameStart();
	void						SetUserPosition(vector<shared_ptr<ContentSession>> &sessions);
	vector<SessionGun>			SetGunPosition();
	void						UserNotFound(char* remoteAddress, uint16 port);
	void						SessionOut(vector<shared_ptr<Session>> &Session);
	void						ReliableProcess(char* remoteAddress, uint16 &remotePort, BYTE *data, DWORD dataLength);
private:
	vector<thread>							mThreadPool;
	vector<shared_ptr<Session>>				mUserSession;
	vector<shared_ptr<ContentSession>>		mContentSession;
	vector<ContentSessions>					mContentSessionVec;
	vector<Position>						mInitPos;
	uint16									ROOM_MAX_NUM = 1;
	uint16									GUN_MAX_NUM = 1;
	uint16									ROOM_NUM = 0;
	uint16									SESSION_REDUCE_TIME = 1000;
	HANDLE									mReliableHandle;
};

