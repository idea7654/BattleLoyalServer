#pragma once
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
	shared_ptr<Session>		FindSession(char* remoteAddress, uint16 port);

	vector<thread>	mThreadPool;
	vector<shared_ptr<Session>> mUserSession;
};
