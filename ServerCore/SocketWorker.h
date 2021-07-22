#pragma once
class SocketWorker : public SocketUtils
{
public:
	SocketWorker();
	~SocketWorker();

public:
	void	Init();
	void	ReadEvent();
	void	WriteEvent();
	void	ThreadManage();
	bool	CheckPacketNum(Session &session, uint32 PacketNumber);

	vector<thread>	mThreadPool;
};

