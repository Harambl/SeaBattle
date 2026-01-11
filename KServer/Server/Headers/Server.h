#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QByteArray>
#include <QDataStream>
#include <QString>
#include <QDebug>
#include <QThread>
#include <QPoint>
#include <iostream>
#include <vector>
#include <string.h>
#include "TcpSocket.h"
#include "User.h"
#include "DataBase.h"
#include "GameLogic/GameTypes.h"

// Codes to client
#define WRONG_AUTH_CODE			quint8(31)
#define OK_AUTH_CODE			quint8(17)
#define WRONG_LOGIN_CODE		quint8(15)
#define OK_LOGIN_CODE			quint8(16)
#define USERNAME_CODE			quint8(14)
#define FIGHT_INVINTATION_CODE  	quint8(13)
#define START_FIGHT_CODE		quint8(12)
#define REJECTED_FIGHT_CODE		quint8(11)
#define USER_IN_LOBBY_DISC_CODE		quint8(10)
#define POINT_POSITION_SERVER_CODE	quint8(18)
#define SHOT_RESULT_SERVER_CODE		quint8(19)
#define USER_FIGHTS_CODE		quint8(20)
#define USER_FINISHED_FIGHT_CODE	quint8(21)
#define START_SHIPS_PLANNING		quint8(22)
#define ENEMY_IS_READY_TO_FIGHT_CODE	quint8(23)

// Mixed codes
#define OK_CODE				quint8(111)

// Codes from client
#define LOGIN_CODE			quint16(65535)
#define AUTH_CODE			quint16(65533)
#define FIGHT_REQUEST_CODE		quint16(65534)
#define ACCEPTED_INVINTATION_CODE	quint16(65532)
#define REJECTED_INVINTATION_CODE	quint16(65531)
#define POINT_POSITION_CODE 		quint16(65530)
#define SHOT_RESULT_CODE		quint16(65529)
#define UPDATED_USER_INFO_CODE		quint16(65528)
#define USER_IN_LOBBY_CODE		quint16(65527)
#define READY_TO_FIGHT_CODE		quint16(65526)
#define PRESSED_FIGHT_BUTTON_CODE	quint16(65525)

#define CHECK_DELAY		5

#define USERS_TABLE_NAME	"Users"

using std::vector;
using std::cout;
using std::endl;


class Server : public QTcpServer
{ 
	Q_OBJECT

	DataBase<User> User_DB;
	TcpSocket* currSocket;
	vector<TcpSocket*> Sockets;
	vector<pair<User, TcpSocket*>> ConnectedUsers;
	QByteArray Data;
	quint16 dataSize;

	template <class... Ts>
	void sendDataToClient(TcpSocket* socket, quint8 code_, Ts&&... Args);

	void startShipsPlanning(QString fInvoker, QString fReciever);
	void startFight(QString player1, QString player2);

	void sendCode(quint8 code, TcpSocket* socket);
	void sendUserInfo(User user, TcpSocket* socket);
	void sendUsers(vector<User> Names, TcpSocket* socket);
	void sendAllUsers(TcpSocket* socket);
	void sendConnectedUsers(QString name, TcpSocket* socket);
	void sendFightMessageToOpponent(QString fInvoker, QString fReciever, quint8 CODE);
	void sendUserInLobbyDisconnected(QString name, TcpSocket* socket);
	void sendUsersInLobbyFightState(vector<QString> Users, TcpSocket* socket, bool isFighting);
	void sendAllUsersUserDisconnected(QString discUserName);
	void sendAllLobbyUsersUsersFight(QString fInvoker, QString fReciever);
	void sendAllLobbyUsersUserFinishedFight(QString UserName);
	void sendEnemyUserReadyToFight(QString enemy);
	void sendPointPosToEnemy(QPoint pos, QString enemy);
	void sendShotResultToEnemy(ShotResult result, QString enemy);

	bool login(QString name, QString passHash);
	bool auth(QString name, QString passHash);

	void writeUpdatedInfoToBase(QString, QString, QString, QString);

	User* findUser(TcpSocket* socket);
	User* findUser(QString userName);
	TcpSocket* findUserSocket(QString userName);

public:
	Server();
	~Server();

public slots:
	void incomingConnection(qintptr socketDescriptor);
	void slotSocketDisconnection();
	void slotReadyRead();
};


#endif
