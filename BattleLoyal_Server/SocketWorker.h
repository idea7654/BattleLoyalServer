#pragma once

enum Gun
{
	NORMAL = 0,
};

struct SessionGun
{
	Position Gun_pos;
	Gun Gun_Type;
	uint16 gunNum;
};

struct SessionRecover
{
	Position pos;
	uint16 objNum;
};

struct ContentSession : Session
{
	Position pos;
	Direction dir;
	shared_ptr<SessionGun> gun;
	float water;
	float hp;
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
		vector<shared_ptr<SessionGun>>		Guns;
		vector<shared_ptr<SessionRecover>>  Recovers;
		int32								ROOM_NUM;
	};

public:
	virtual void				Init() override;
	virtual void				ReadEvent() override;
	virtual void				WriteEvent() override;
	virtual void				ThreadManage() override;
	virtual bool				CheckPacketNum(shared_ptr<Session> &session, uint32 PacketNumber) override;

public:
	void								ReduceSessionTime();
	bool								ResetSessionTime(shared_ptr<Session> &session);
	shared_ptr<Session>					FindSession(string nickname);
	shared_ptr<Session>					FindSession(char* remoteAddress, uint16 port);
	vector<shared_ptr<ContentSession>>	FindContentSession(int32 RoomNum);
	shared_ptr<ContentSession>			FindContentSession(string nickname);
	ContentSessions						FindContentSessionInVec(int32 RoomNum);
	shared_ptr<ContentSession>			FindContetnSessionInVecByName(int32 RoomNum, string nickname);
	bool								GunCheck(ContentSessions session, uint16 gunNum);
	void								GameStart(vector<shared_ptr<SessionGun>> &guns, vector<shared_ptr<SessionRecover>> &recovers);
	void								SetUserPosition(vector<shared_ptr<ContentSession>> &sessions);
	void								SetGunPosition();
	void								UserNotFound(char* remoteAddress, uint16 port);
	void								SessionOut(vector<shared_ptr<Session>> &Session);
	void								ReliableProcess(char* remoteAddress, uint16 &remotePort, BYTE *data, DWORD dataLength);
	void								CheckUserDie(ContentSessions &checkVector, string nickname);
	vector<Position>					SetRoundPosition();
	void								SessionOut(ContentSessions &checkVector, string nickname);
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
	//vector<shared_ptr<SessionGun>>		GunInfo;
	vector<Position>						GunPos;
	vector<Position>						mRecoverPos;
};

