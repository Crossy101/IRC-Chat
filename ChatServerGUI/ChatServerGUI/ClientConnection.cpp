#include "ClientConnection.h"

ClientConnection::ClientConnection()
{

}

ClientConnection::ClientConnection(ClientConnection^ clientConnection)
{
	this->Username = clientConnection->Username;
	this->socketAddress = clientConnection->socketAddress;
	this->clientSocket = clientConnection->clientSocket;
}

ClientConnection::ClientConnection(char* Username, sockaddr_in* socketAddress, SOCKET clientSocket)
{
	this->Username = Username;
	this->socketAddress = socketAddress;
	this->clientSocket = clientSocket;
}