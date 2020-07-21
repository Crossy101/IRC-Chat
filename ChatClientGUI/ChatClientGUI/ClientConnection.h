#include <iostream>
#include <WS2tcpip.h>

using namespace System::Threading;

#pragma once
ref class ClientConnection
{
public:
	//Current socket being used
	SOCKET clientSocket;
	//Current Client Thread
	Thread^ clientThread;

	//Booleans for certain events
	bool isDisconnecting = false;
	bool firstConnection = false;

};

