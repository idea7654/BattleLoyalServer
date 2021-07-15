#pragma once

const int32 MAX_SESSION = 10;
class TestIocp : public Iocp
{
public:
	TestIocp();
	~TestIocp();

private:
	const uint16 PORT = 9999;

	PacketSession mUdpSession;

	HANDLE mResetThreadHandle;
	HANDLE mResetThreadDestroyEvent;

protected:
	void OnIoRead(void *Object, DWORD dataLength);
	void OnIoWrite(void *Object, DWORD dataLength);
	void OnIoConnected(void *Object);
	void OnIoDisconnected(void *Object);

public:
	bool Begin();
	bool End();

	void ResetThreadCallback(void);
};

