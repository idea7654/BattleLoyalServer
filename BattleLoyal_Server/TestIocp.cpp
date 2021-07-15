#include "pch.h"
#include "TestIocp.h"

DWORD WINAPI ResetThreadCallback(void* parameter)
{
	TestIocp *Owner = (TestIocp*)parameter;
	Owner->ResetThreadCallback();

	return 0;
}

TestIocp::TestIocp()
{
}


TestIocp::~TestIocp()
{

}

void TestIocp::OnIoRead(void * Object, DWORD dataLength)
{
	PacketSession *packetSession = (PacketSession*)Object;

	DWORD	Protocol						= 0;
	BYTE	Packet[MAX_BUFFER_LENGTH]		= { 0 };
	DWORD	PacketLength					= 0;
	char	RemoteAddress[32]				= { 0 };
	uint16	RemotePort						= 0;

	BYTE	WriteBuffer[MAX_BUFFER_LENGTH]	= { 0 };

	if (packetSession->ReadFromPacketForIOCP(RemoteAddress, RemotePort, dataLength))
	{
		//while (packetSession->GetPacket(RemoteAddress, RemotePort, Protocol, Packet, PacketLength))
		//{
			//MultiByte
		//	switch (Protocol)
		//	{
			//case PU_C2S_TUNNELING:
				//각 프로토콜에 관한 처리
		//	default:
		//		break;
		//	}
		//}
		cout << "무언가 패킷받음!!" << endl;
	}

	if (!packetSession->InitializeReadFromForIOCP())
		return;
}

void TestIocp::OnIoWrite(void * Object, DWORD dataLength)
{
	PacketSession *packetSession = (PacketSession*)Object;
	packetSession->WriteComplete();
}

void TestIocp::OnIoConnected(void * Object)
{
}

void TestIocp::OnIoDisconnected(void * Object)
{
}

bool TestIocp::Begin()
{
	if (!Iocp::BeginIocp())
		return false;

	if (!mUdpSession.Begin())
		return false;

	if (mUdpSession.UdpBind(PORT) == false)
	{
		cout << "Bind Error" << endl;
		return false;
	}
	
	if (!Iocp::RegisterSocketToIocp(mUdpSession.GetSocket(), (ULONG_PTR)&mUdpSession))
		return false;

	if (!mUdpSession.InitializeReadFromForIOCP())
		return false;

	mResetThreadDestroyEvent	= CreateEvent(NULL, false, false, NULL);
	mResetThreadHandle			= CreateThread(NULL, 0, ::ResetThreadCallback, this, 0, NULL);

	return true;
}

bool TestIocp::End()
{
	SetEvent(mResetThreadDestroyEvent);
	WaitForSingleObject(mResetThreadHandle, INFINITE);

	CloseHandle(mResetThreadDestroyEvent);
	CloseHandle(mResetThreadHandle);

	Iocp::End();
	mUdpSession.End();

	return true;
}

void TestIocp::ResetThreadCallback(void)
{
	while (true)
	{
		DWORD Result = WaitForSingleObject(mResetThreadDestroyEvent, 1000);

		if (Result == WAIT_OBJECT_0)
			return;

		mUdpSession.ResetUdp();
	}
}
