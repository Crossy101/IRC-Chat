#include <iostream>
#include <WS2tcpip.h>
#include <thread>
#include <sstream>
#include <string>
#include <vector>

#include "ClientConnection.h"

#pragma comment (lib, "ws2_32.lib")

#pragma once

namespace ChatServerGUI {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;
	using namespace System::Collections::Generic;



	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			this->allClients = gcnew List<ClientConnection^>();
		}

		//All clients connected in a list variable
		List<ClientConnection^>^ allClients;

		//If the first client has connected
		bool firstConnection = false;

		delegate void TestDelegate(String^ text);

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^ StartServerButton;
	protected:

	protected:
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::RichTextBox^ ChatTextBox;
	private: System::Windows::Forms::Label^ ServerStatusLabel;





	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->StartServerButton = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->ChatTextBox = (gcnew System::Windows::Forms::RichTextBox());
			this->ServerStatusLabel = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// StartServerButton
			// 
			this->StartServerButton->Location = System::Drawing::Point(96, 12);
			this->StartServerButton->Name = L"StartServerButton";
			this->StartServerButton->Size = System::Drawing::Size(75, 23);
			this->StartServerButton->TabIndex = 0;
			this->StartServerButton->Text = L"Start Server";
			this->StartServerButton->UseVisualStyleBackColor = true;
			this->StartServerButton->Click += gcnew System::EventHandler(this, &MyForm::StartServerButton_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->BackColor = System::Drawing::SystemColors::Control;
			this->label1->ForeColor = System::Drawing::Color::Black;
			this->label1->Location = System::Drawing::Point(78, 38);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(77, 13);
			this->label1->TabIndex = 1;
			this->label1->Text = L"Server Status: ";
			// 
			// ChatTextBox
			// 
			this->ChatTextBox->Location = System::Drawing::Point(12, 54);
			this->ChatTextBox->Name = L"ChatTextBox";
			this->ChatTextBox->Size = System::Drawing::Size(260, 169);
			this->ChatTextBox->TabIndex = 2;
			this->ChatTextBox->Text = L"";
			// 
			// ServerStatusLabel
			// 
			this->ServerStatusLabel->AutoSize = true;
			this->ServerStatusLabel->BackColor = System::Drawing::SystemColors::Control;
			this->ServerStatusLabel->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->ServerStatusLabel->ForeColor = System::Drawing::Color::Red;
			this->ServerStatusLabel->Location = System::Drawing::Point(151, 38);
			this->ServerStatusLabel->Name = L"ServerStatusLabel";
			this->ServerStatusLabel->Size = System::Drawing::Size(37, 13);
			this->ServerStatusLabel->TabIndex = 5;
			this->ServerStatusLabel->Text = L"Offline";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 231);
			this->Controls->Add(this->ServerStatusLabel);
			this->Controls->Add(this->ChatTextBox);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->StartServerButton);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"MyForm";
			this->Text = L"Chat Server";
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &MyForm::MyForm_Closing);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		//This function is executed when the server adminstrator has closed the server form
		private: void MyForm_Closing(Object^ sender, FormClosedEventArgs^ e)
		{
			for (size_t i = 0; i < this->allClients->Count; i++)
			{
				if (this->allClients[i]->clientSocket != NULL)
				{
					closesocket(this->allClients[i]->clientSocket);
					this->allClients[i]->isDisconnecting = true;
					this->allClients[i]->runningThread->Join();
				}
				WSACleanup();
			}

			Application::Exit();
		}

		//This function takes care of calling the server connection thread and updates the GUI with nescceary changes
		private: System::Void StartServerButton_Click(System::Object^ sender, System::EventArgs^ e) {
			

			//Create a thread for the server to rely on.
			ThreadStart^ threadDelegate = gcnew ThreadStart(this, &MyForm::StartServer);
			Thread^ newThread = gcnew Thread(threadDelegate);

			//Start the server thread
			newThread->Start();

			//Change the GUI look
			this->ChatTextBox->Text += "Server Started! \n";
			this->ChatTextBox->Text += "Server Connection: 127.0.0.1:54000 \n";
			this->ServerStatusLabel->Text = "Online";
			this->ServerStatusLabel->ForeColor = System::Drawing::Color::Green;
			this->StartServerButton->Enabled = false;
		}
		
		//This function adds text to the server chat box
		public: void ServerAddText(String^ text)
		{
			this->ChatTextBox->Text += text;
		}

		//This function is created for each client and run on their own thread
		public: void ClientThread(Object^ obj)
		{
			ClientConnection^ params = (ClientConnection^)obj;

			char buf[4096];

			char host[NI_MAXHOST];		//clients remote name
			char service[NI_MAXSERV];	// service (i.e port) the client is connected on

			//Empty both host and service buffers
			ZeroMemory(host, NI_MAXHOST);
			ZeroMemory(service, NI_MAXSERV);

			//Get client information from their socket address
			if (getnameinfo((sockaddr*)params->socketAddress, sizeof(params->socketAddress), host, NI_MAXHOST, service, NI_MAXSERV, 0))
			{
				std::cout << host << " connected on port " << service << std::endl;
			}
			else
			{
				inet_ntop(AF_INET, &params->socketAddress->sin_addr, host, NI_MAXHOST); 
				std::cout << host << " connected on port " << service << std::endl;
			}

			//Keep executing whilst the client is still connected
			while (params->isDisconnecting == false)
			{
				ZeroMemory(buf, 4096);

				//Wait for client to send data
				int bytesRecieved = recv(params->clientSocket, buf, 4096, 0);
				if (bytesRecieved == SOCKET_ERROR)
				{
					char fullMessage[4096];
					strcpy(fullMessage, params->Username);
					strcat(fullMessage, " disconnected.");

					String^ clientText = gcnew String(fullMessage);
					LogText(clientText);

					allClients->Remove(params);
					params->runningThread->Join();
					break;
				}

				//If we stop recieving bytes from the client disconnect them
				if (bytesRecieved == 0)
				{
					char fullMessage[4096];
					strcpy(fullMessage, params->Username);
					strcat(fullMessage, " disconnected.");

					params->isDisconnecting = true;

					String^ clientText = gcnew String(fullMessage);
					LogText(clientText);
					allClients->Remove(params);
					break;
				}

				String^ clientText = gcnew String(buf);
				
				//Check if the text the client has sent is a command
				int clientCommand = GetClientCommand(params, buf);

				//If the client did not send a command just send add plain text
				if (clientCommand == 0)
				{
					//Create a message with the Username and Message the user created!
					char fullMessage[4096];
					strcpy(fullMessage, params->Username);
					strcat(fullMessage, ": ");
					strcat(fullMessage, buf);

					String^ text = gcnew String(fullMessage);
					LogText(text);
					SendAllClientsMessage(params, fullMessage);
				}
				//If there was a command of changing the username execute it 
				else if (clientCommand == 1)
				{
					std::string tempString = std::string(buf);
					tempString.erase(0, 12);

					char bufferUsernameChange[1024];
					strcpy(bufferUsernameChange, tempString.c_str());

					params->Username = bufferUsernameChange;
				}

				//If the client has their first connection tell other clients someone has connected
				if (!params->firstConnection)
				{
					char fullMessage[4096];
					ZeroMemory(fullMessage, 4096);

					strcpy(fullMessage, params->Username);
					strcat(fullMessage, " Connected");

					String^ text = gcnew String(fullMessage);
					LogText(text);

					SendAllClientsMessage(params, fullMessage);

					params->firstConnection = true;
				}
			}

			//Close the socket
			closesocket(params->clientSocket);
		}

		public: void StartServer()
		{

			//Intialise Winsock
			WSADATA wsData;
			WORD ver = MAKEWORD(2, 2);

			int wsok = WSAStartup(ver, &wsData);
			if (wsok != 0)
			{

				LogText("Can't intialize winsock! Quitting");
				std::cout << "Can't intialize winsock! Quitting" << std::endl;
			}
			
			//Create a socket
			SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
			if (listening == INVALID_SOCKET)
			{
				LogText("Cannot create a socket! Quitting");
				std::cout << "Cannot create a socket! Quitting" << std::endl;
				return;
			}


			//Bind the IP address and Port to a socket
			sockaddr_in hint;
			hint.sin_family = AF_INET;
			hint.sin_port = htons(54000);
			hint.sin_addr.S_un.S_addr = INADDR_ANY;

			//bind the listening port
			bind(listening, (sockaddr*)&hint, sizeof(hint));

			//Tell Winsock this socket is for listening
			listen(listening, SOMAXCONN);

			//wait for a connection
			sockaddr_in client;
			int clientSize = sizeof(client);

			int clientNumber = 0;
			while (true)
			{
				SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

				ParameterizedThreadStart^ params = gcnew ParameterizedThreadStart(this, &MyForm::ClientThread);
				Thread^ clientThread = gcnew Thread(params);

				ClientConnection^ newClient = gcnew ClientConnection("", &client, clientSocket);
				newClient->runningThread = clientThread;

 				allClients->Add(newClient);
				clientThread->Start(newClient);
			}

			//close listening socket
			closesocket(listening);

			//Cleanup winsocket
			WSACleanup();
		}

		//This function executes setting the clients username when requested
		private: void SetClientUsername(ClientConnection^ client, char username[])
		{
			for each (ClientConnection^ currentClient in allClients)
			{
				if (client == currentClient)
				{
					char buffer[4096];
					ZeroMemory(buffer, 4096);
					strcat(buffer, currentClient->Username);
					strcat(buffer, " changed their username to: ");
					strcat(buffer, username);

					currentClient->Username = username;

					String^ usernameChangeText = gcnew String(buffer);
					LogText(usernameChangeText);
				}
			}
		}

		//This function checks the user text sent and compares it to see if any commands match
		private: int GetClientCommand(ClientConnection^ client, char command[])
		{
			std::string strCommand = command;
			if (command[0] == '/')
			{
				if (strCommand.find("/setusername") != std::string::npos)
				{

					std::string tempString = std::string(command);
					tempString.erase(0, 12);

					char bufferUsernameChange[1024];
					strcpy(bufferUsernameChange, tempString.c_str());

					SetClientUsername(client, bufferUsernameChange);
					
					return 1;
				}
				else
					return 0;
			}
		}

		//This function sends all clients a message
		private: void SendAllClientsMessage(ClientConnection^ currentUser, char msg[])
		{
			char buf[4096];
			ZeroMemory(buf, 4096);

			strcpy(buf, msg);
			for (int i = 0; i < allClients->Count; i++)
			{
				ClientConnection^ currentClient = allClients[i];
				if (currentUser != currentClient)
				{
					int bytesSent = send(currentClient->clientSocket, buf, sizeof(buf), 0);
					if (bytesSent <= 0)
					{
						std::cout << "Error Sending Client Message!" << std::endl;
					}
				}
			}
		}

		//This function creates a delegate for adding text to the chatbox
		private: void LogText(String^ text)
		{
			ChatTextBox->BeginInvoke(gcnew TestDelegate(this, &MyForm::ServerAddText), text + "\n");
		}
	};
}
