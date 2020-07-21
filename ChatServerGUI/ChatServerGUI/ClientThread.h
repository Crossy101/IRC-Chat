#include <iostream>
#include <WS2tcpip.h>
#include <thread>
#include <sstream>
#include <string>

#include "ClientConnection.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Threading;

#pragma once
public ref class ClientThread
{
public:
	ClientConnection^ clientConnection;
	Thread^ currentThread;
};

