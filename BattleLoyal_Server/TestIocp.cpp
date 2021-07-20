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
		while (packetSession->GetPacket(RemoteAddress, RemotePort, Packet, PacketLength))
		{
			auto message = GetMessage(Packet);
			auto protocol = message->packet_type();
			switch (protocol)
			{
			case MESSAGE_ID::MESSAGE_ID_C2S_MOVE:
				auto RecvData = static_cast<const C2S_MOVE*>(message->packet());
				READ_PU_C2S_MOVE(RecvData);
			//default:
				//break;
			}
		}
	}

	if (!packetSession->InitializeReadFromForIOCP())
	{
		End();
		return;
	}
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

bool TestIocp::Send()
{
	int32 length = 0;
	Position pos{ 1.0f, 2.0f, 3.0f };
	Direction dir{ 1.0f, 2.0f, 3.0f };
	auto packet = WRITE_PU_S2C_MOVE("Edea", pos, dir, length);
	mUdpSession.WriteToPacket("127.0.0.1", 7777, packet, length);

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
