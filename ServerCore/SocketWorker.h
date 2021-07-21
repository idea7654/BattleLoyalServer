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

	vector<thread>	mThreadPool;
};

