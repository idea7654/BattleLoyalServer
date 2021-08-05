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

class SocketWorker : public SocketUtils
{
public:
	SocketWorker();
	~SocketWorker();

public:
	void					Init();
	void					ReadEvent();
	void					WriteEvent();
	void					ThreadManage();
	bool					CheckPacketNum(Session &session, uint32 PacketNumber);

	void					ReduceSessionTime();
	bool					ResetSessionTime(shared_ptr<Session> &session);
	shared_ptr<Session>		FindSession(string nickname);
	auto					FindContentSession(int32 RoomNum);
	void					GameStart();
	void					SetUserPosition(vector<shared_ptr<ContentSession>> &sessions);

private:
	vector<thread>						mThreadPool;
	vector<shared_ptr<Session>>			mUserSession;
	vector<shared_ptr<ContentSession>>	mContentSession;
	vector<Position>					mInitPos;
	int32								ROOM_MAX_NUM = 1;
	uint16								ROOM_NUM = 1;
};

