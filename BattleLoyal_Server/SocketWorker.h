#pragma once
class SocketWorker : public SocketUtils
{
public:
	SocketWorker();
	~SocketWorker();

protected:
	enum Gun
	{
		Pistol = 0,
	};
	struct ContentSession : Session
	{
		Position pos;
		Direction dir;
		Gun gun;
		float water;
		float hp;
	};

public:
	void					Init();
	void					ReadEvent();
	void					WriteEvent();
	void					ThreadManage();
	bool					CheckPacketNum(Session &session, uint32 PacketNumber);

	void					ReduceSessionTime();
	bool					ResetSessionTime(shared_ptr<Session> &session);
	shared_ptr<Session>		FindSession(char* remoteAddress, uint16 port);
	auto					FindContentSession(int32 RoomNum);
	void					GameStart();

private:
	vector<thread>						mThreadPool;
	vector<shared_ptr<Session>>			mUserSession;
	vector<shared_ptr<ContentSession>>	mContentSession;
	int32								ROOM_MAX_NUM = 1;
	uint16								ROOM_NUM = 1;
};

