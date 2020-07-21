#include <iostream>
#include <WS2tcpip.h>
#include <thread>
#include <sstream>
#include <string>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Threading;

#pragma once
public ref class ClientConnection
{
public:
	char* Username;

	sockaddr_in* socketAddress;

	SOCKET clientSocket;

	bool isDisconnecting = false;
	bool firstConnection = false;

	Thread^ runningThread;

	ClientConnection();
	ClientConnection(ClientConnection^ clientConnection);
	ClientConnection(char* Username, sockaddr_in* socketAddress, SOCKET clientSocket);
};

