#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>
#include <vector>
#include <string>
#include <iostream>
#include "User.h"

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::pair;


class TcpSocket : public QTcpSocket {
	Q_OBJECT
	vector<TcpSocket*>* pSockets;
	vector<pair<User, TcpSocket*>>* pConnectedUsers;
public:
	TcpSocket();
	TcpSocket(vector<TcpSocket*>* _pSocket,
			vector<pair<User, TcpSocket*>>* _pConnectedUsers);
	static inline TcpSocket* disconnectedSocket;
	~TcpSocket();

signals:
	void socketDisconnected();

public slots:
	void deleteSelf();
};


#endif
