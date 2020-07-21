#include <iostream>
#include <thread>
#include <WS2tcpip.h>
#include <string>

#include <msclr\marshal_cppstd.h>

#include "ClientConnection.h"

#pragma comment(lib, "ws2_32.lib")

#pragma once

namespace ChatClientGUI {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		ClientConnection^ clientConnection;

		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			this->clientConnection = gcnew ClientConnection();
		}


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
	private: System::Windows::Forms::RichTextBox^ ChatBox;
	protected:

	protected:

	private: System::Windows::Forms::TextBox^ ChatInput;
	private: System::Windows::Forms::Button^ ChatSendMessage;
	protected:


	private: System::Windows::Forms::TextBox^ textBox2;
	private: System::Windows::Forms::TextBox^ UsernameInput;
	private: System::Windows::Forms::Button^ ServerConnectButton;


	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->ChatBox = (gcnew System::Windows::Forms::RichTextBox());
			this->ChatInput = (gcnew System::Windows::Forms::TextBox());
			this->ChatSendMessage = (gcnew System::Windows::Forms::Button());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->UsernameInput = (gcnew System::Windows::Forms::TextBox());
			this->ServerConnectButton = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// ChatBox
			// 
			this->ChatBox->Location = System::Drawing::Point(12, 58);
			this->ChatBox->Name = L"ChatBox";
			this->ChatBox->ReadOnly = true;
			this->ChatBox->Size = System::Drawing::Size(290, 171);
			this->ChatBox->TabIndex = 0;
			this->ChatBox->Text = L"";
			// 
			// ChatInput
			// 
			this->ChatInput->Location = System::Drawing::Point(12, 235);
			this->ChatInput->Name = L"ChatInput";
			this->ChatInput->Size = System::Drawing::Size(290, 20);
			this->ChatInput->TabIndex = 1;
			// 
			// ChatSendMessage
			// 
			this->ChatSendMessage->Location = System::Drawing::Point(121, 261);
			this->ChatSendMessage->Name = L"ChatSendMessage";
			this->ChatSendMessage->Size = System::Drawing::Size(86, 26);
			this->ChatSendMessage->TabIndex = 2;
			this->ChatSendMessage->Text = L"Send Message";
			this->ChatSendMessage->UseVisualStyleBackColor = true;
			this->ChatSendMessage->Click += gcnew System::EventHandler(this, &MyForm::ChatSendMessage_Click);
			// 
			// textBox2
			// 
			this->textBox2->Enabled = false;
			this->textBox2->Location = System::Drawing::Point(12, 32);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(100, 20);
			this->textBox2->TabIndex = 3;
			this->textBox2->Text = L"127.0.0.1:54000";
			// 
			// UsernameInput
			// 
			this->UsernameInput->Location = System::Drawing::Point(121, 32);
			this->UsernameInput->Name = L"UsernameInput";
			this->UsernameInput->Size = System::Drawing::Size(100, 20);
			this->UsernameInput->TabIndex = 4;
			// 
			// ServerConnectButton
			// 
			this->ServerConnectButton->Location = System::Drawing::Point(227, 29);
			this->ServerConnectButton->Name = L"ServerConnectButton";
			this->ServerConnectButton->Size = System::Drawing::Size(75, 23);
			this->ServerConnectButton->TabIndex = 5;
			this->ServerConnectButton->Text = L"Connect";
			this->ServerConnectButton->UseVisualStyleBackColor = true;
			this->ServerConnectButton->Click += gcnew System::EventHandler(this, &MyForm::ServerButtonConnect_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(13, 13);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(58, 13);
			this->label1->TabIndex = 6;
			this->label1->Text = L"IP Address";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(121, 13);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(55, 13);
			this->label2->TabIndex = 7;
			this->label2->Text = L"Username";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(318, 299);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->ServerConnectButton);
			this->Controls->Add(this->UsernameInput);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->ChatSendMessage);
			this->Controls->Add(this->ChatInput);
			this->Controls->Add(this->ChatBox);
			this->Name = L"MyForm";
			this->Text = L"ChatClient";
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &MyForm::MyForm_Closing);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	//This function is executed when the client closes the form and exits
	private: void MyForm_Closing(Object^ sender, FormClosedEventArgs^ e)
	{
		if (this->clientConnection->clientSocket != NULL)
		{
			closesocket(this->clientConnection->clientSocket);
			WSACleanup();
		}
		this->clientConnection->isDisconnecting = true;
		this->clientConnection->clientThread->Join();

		Application::Exit();
	}

	//This function is executed when the user clickes the connect button
	private: System::Void ServerButtonConnect_Click(Object^ sender, EventArgs^ e) {
		if (this->UsernameInput->TextLength <= 0)
		{
			MessageBox::Show("You need to have a username before connecting to the server!");
			return;
		}

		if (ConnectToChatServer() == 1)
		{
			MessageBox::Show("Successfully connected to server!");
			this->ServerConnectButton->Enabled = false;
		}
		else
		{
			MessageBox::Show("Cannot connect to server!");
		}
	}

	//This function is for when the client has recieved a message from the server
	private: void RecieveServerMessage(Object^ obj)
	{
		ClientConnection^ clientConnection = (ClientConnection^)obj;

		char buf[4096];
		do {
			ZeroMemory(buf, 4096);
			int bytesRecieved = recv(this->clientConnection->clientSocket, buf, 4096, 0);
			if (bytesRecieved > 0)
			{
				String^ currentText = gcnew String(buf);
				LogText(currentText);
			}
		} while (this->clientConnection->isDisconnecting == false);
	}

	//This function is executed when the client is connecting to the server.
	private: int ConnectToChatServer()
	{
		//Ip address and port of server
		std::string ipAddress = "127.0.0.1";
		int port = 54000;

		//Intialise Winsock
		WSAData data;
		WORD ver = MAKEWORD(2, 2);

		int wsResult = WSAStartup(ver, &data);
		if (wsResult != 0)
		{
			std::cout << "Cannot start winsock, Error number: " << wsResult << "\n" << std::endl;
			return 0;
		}

		//Create socket
		this->clientConnection->clientSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (this->clientConnection->clientSocket == INVALID_SOCKET)
		{
			std::cout << "Failed to create socket, Error: " << WSAGetLastError() << "\n" << std::endl;
			WSACleanup();
			return 0;
		}

		//Fill in a hint structure
		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(port);
		inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

		//Connect to server
		int connResult = connect(this->clientConnection->clientSocket, (sockaddr*)&hint, sizeof(hint));
		if (connResult == SOCKET_ERROR)
		{
			std::cout << "Cannot connect to the server!, Error: " << WSAGetLastError() << "\n" << std::endl;
			closesocket(this->clientConnection->clientSocket);
			WSACleanup();
			return 0;
		}

		//When the user first connects to the server, set a username for them
		char bufferUsernameChange[1024];
		ZeroMemory(bufferUsernameChange, 1024);

		msclr::interop::marshal_context context;
		std::string usernameInput = context.marshal_as<std::string>(this->UsernameInput->Text);

		//Execute a command on the server to set their username
		strcat(bufferUsernameChange, "/setusername ");
		strcat(bufferUsernameChange, usernameInput.c_str());

		//Send the command
		send(this->clientConnection->clientSocket, bufferUsernameChange, sizeof(bufferUsernameChange), 0);

		//Create a new thread for the client messages to be recieved from.
		ParameterizedThreadStart^ params = gcnew ParameterizedThreadStart(this, &MyForm::RecieveServerMessage);
		Thread^ clientThread = gcnew Thread(params);
		clientConnection->clientThread = clientThread;
		clientThread->Start(this->clientConnection);

		return 1;
	}

	//This function is executed when a client has test ready to send to the server
	private: System::Void ChatSendMessage_Click(System::Object^ sender, System::EventArgs^ e) {
		if (this->ChatInput->TextLength <= 0)
		{
			MessageBox::Show("Make sure that you have actually typed something into the text box!");
		}
		else
		{
			msclr::interop::marshal_context context;
			std::string chatInput = context.marshal_as<std::string>(this->ChatInput->Text);
			std::string username = context.marshal_as<std::string>(this->UsernameInput->Text);

			char buffer[4096];
			ZeroMemory(buffer, 4096);
			strcpy(buffer, username.c_str());
			strcat(buffer, ": ");
			strcat(buffer, chatInput.c_str());

			String^ chatboxTextDuplicate = gcnew String(buffer);
			LogText(chatboxTextDuplicate);

			int sendResult = send(this->clientConnection->clientSocket,  chatInput.c_str(), chatInput.size() + 1, 0);

			this->ChatInput->Text = "";
		}
	}

	//Delegation for adding text to the chat box
	private: void LogText(String^ text)
	{
		this->ChatBox->BeginInvoke(gcnew TestDelegate(this, &MyForm::ClientAddText), text + "\n");
	}

	//Add chatbox text
	private: void ClientAddText(String^ text)
	{
		this->ChatBox->Text += text;
	}
};
}
