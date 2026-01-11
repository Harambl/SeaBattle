#include "TcpSocket.h"

TcpSocket::TcpSocket()
{ }

TcpSocket::TcpSocket(vector<TcpSocket*>* _pSockets,
		vector<pair<User, TcpSocket*>>* _pConnectedUsers)
{
	pSockets = _pSockets;
	pConnectedUsers = _pConnectedUsers;
}

TcpSocket::~TcpSocket()
{ }

void TcpSocket::deleteSelf()
{
	disconnectedSocket = this;
	emit socketDisconnected();
	cout << "User disconnected && Socket deleted" << endl;
}

