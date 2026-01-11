#include "Server.h"
#include "slotReadyRead.cpp"

Server::Server() : 
	User_DB {DataBase<User>("/home/eust/SeaBattleDataBase.db", USERS_TABLE_NAME)},
	Sockets{}, ConnectedUsers{}, dataSize{0}
{
	if(this->listen(QHostAddress::Any, 55555)) {
		cout << "Server started" << endl;	
	} else {
		cout << "Error with starting server" << endl;
	}		
}

Server::~Server()
{ }

template <class... Ts>
void Server::sendDataToClient(TcpSocket* socket, quint8 code_, Ts&&... Args)
{
	// Основная шаблонная функция отправки информации клиенту
	
	Data.clear();
	QDataStream out(&Data, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_2);

	out << quint16(0);
	out << code_;
	(..., (out << std::forward<Ts>(Args)));

	out.device()->seek(0);
	out << quint16(Data.size() - sizeof(quint16));

	socket->write(Data);
	QThread::sleep(0.2);
	socket->flush();
	qDebug() << "info to client sent";
}

void Server::sendCode(quint8 code, TcpSocket* socket)
{
	// Отправка очень коротких сообщений
	
	Data.clear();
	QDataStream out(&Data, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_2);

	out << code;

	socket->write(Data);
	QThread::sleep(0.2);
	socket->flush();
	qDebug() << "code sent" ;
}

void Server::sendUserInfo(User user, TcpSocket* socket)
{
	// Отправка информации о пользователе
	sendDataToClient(
		socket,
		OK_AUTH_CODE,
		QString(user.name.c_str()),
		QString(user.winCount.c_str()),
		QString(user.loseCount.c_str()),
		QString(user.matchCount.c_str())
	);
}

void Server::sendUsers(vector<User> Users, TcpSocket* socket)
{
	// Отправка списка пользователей с их статусами
	
	Data.clear();
	QDataStream out(&Data, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_2);

	out << quint16(0);
	out << USERNAME_CODE;
	out << quint16(Users.size());
	for(User user : Users) {
		out << QString(user.name.c_str());	
		out << ((user.state == User::state_t::FIGHTING) ? true : false);
	}

	out.device()->seek(0);
	out << quint16(Data.size() - sizeof(quint16));

	socket->write(Data);
	QThread::sleep(0.4);
	socket->flush();
	qDebug() << "user sent";
}

void Server::sendAllUsers(TcpSocket* socket)
{
	// Отправка всех пользователей в ConnectedUsers через функцию sendUsers
	
	vector<User> _users {};
	for(pair<User, TcpSocket*> userPair : ConnectedUsers)
		_users.push_back(userPair.first);

	sendUsers(_users, socket);
}

void Server::sendConnectedUsers(QString name, TcpSocket* socket)
{
	vector<User> _users {};
	User* user {findUser(name)};

	// Важная задержка
	QThread::sleep(1);

	// Оповещение всез пользователей в лобби о подключении новго пользователя
	for(pair<User, TcpSocket*> userPair : ConnectedUsers)
		if(QString(userPair.first.name.c_str()) != name &&
			   userPair.first.state == User::state_t::IN_LOBBY)
			sendUsers({*user}, userPair.second);

	// Отправление новоподключенному пользователю списка всех пользователей с их статусами
	sendAllUsers(socket);
}

void Server::sendEnemyUserReadyToFight(QString enemy)
{
	// Отправление противнику кода готовности оппонента к сражению
	sendDataToClient(findUserSocket(enemy), ENEMY_IS_READY_TO_FIGHT_CODE);
}

void Server::sendFightMessageToOpponent(QString fInvoker, QString fReciever, quint8 CODE)
{
	sendDataToClient(findUserSocket(fReciever), CODE, fInvoker);
}

void Server::sendUserInLobbyDisconnected(QString name, TcpSocket* socket)
{
	// Отправление кода и имени вышедшего из лобби пользователя игроку
	sendDataToClient(socket, USER_IN_LOBBY_DISC_CODE, name);
}

void Server::sendPointPosToEnemy(QPoint pos, QString enemy)
{
	// Отправление координат выстрела противнику
	sendDataToClient(findUserSocket(enemy), POINT_POSITION_SERVER_CODE, pos);
}

void Server::sendShotResultToEnemy(ShotResult result, QString enemy)
{
	// Отправление результата выстрела стреляющему
	sendDataToClient(findUserSocket(enemy), SHOT_RESULT_SERVER_CODE, result);
}

void Server::sendUsersInLobbyFightState(vector<QString> Users, TcpSocket* socket, bool isFighting)
{
	// Отправление списка пользователей в лобби и их статусов
	
	Data.clear();
	QDataStream out(&Data, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_2);

	out << quint16(0) << ((isFighting) ? USER_FIGHTS_CODE : USER_FINISHED_FIGHT_CODE);
	for(QString userName : Users) out << userName;
	out.device()->seek(0);
	out << quint16(Data.size() - sizeof(quint16));

	socket->write(Data);
	QThread::sleep(0.2);
	socket->flush();

	cout << "Users list sent" << endl;
}

void Server::incomingConnection(qintptr socketDescriptor)
{
	// Новый пользователь подключился
	
	currSocket = new TcpSocket(&Sockets, &ConnectedUsers);
	currSocket->setSocketDescriptor(socketDescriptor);
	connect(currSocket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
	connect(currSocket, SIGNAL(disconnected()), currSocket, SLOT(deleteSelf()));
	connect(currSocket, SIGNAL(socketDisconnected()), this, SLOT(slotSocketDisconnection()));

	// Добавление сокета в общий вектор сокетов
	Sockets.push_back(currSocket);

	// Оповещение сокета об удачном подключении
	sendCode(OK_CODE, currSocket);

	cout << "Client connected" << endl;
}

void Server::startShipsPlanning(QString fInvoker, QString fReciever)
{
	//  Изменение статуса игрока на FIGHTING
	User* pfInvoker {findUser(fInvoker)};
	pfInvoker->state = User::state_t::FIGHTING;

	//  Изменение статуса игрока на FIGHTING
	User* pfReciever {findUser(fReciever)};
	pfReciever->state = User::state_t::FIGHTING;

	// Отправление кода начала расстановки обоим учасникам сражения
	sendFightMessageToOpponent(fInvoker, fReciever, START_SHIPS_PLANNING);
	sendFightMessageToOpponent(fReciever, fInvoker, START_SHIPS_PLANNING);

	// Оповещение остальных пользователей в лобби о начале потенциального сражения
	sendAllLobbyUsersUsersFight(fInvoker, fReciever);
}

void Server::startFight(QString player1, QString player2)
{
	// Отправление обоим игрокам кода начала боя
	sendDataToClient(findUserSocket(player1), START_FIGHT_CODE);
	sendDataToClient(findUserSocket(player2), START_FIGHT_CODE);
}

User* Server::findUser(TcpSocket* socket)
{
	// Поиск пользователя по сокету
	for(pair<User, TcpSocket*>& userPair : ConnectedUsers)
		if(userPair.second == socket) return &(userPair.first);
	throw std::runtime_error("there are no such user in ConnectUsers");
}
User* Server::findUser(QString userName)
{
	// Поиск пользователя по имени
	string strUserName {userName.toUtf8().constData()};
	for(pair<User, TcpSocket*>& userPair : ConnectedUsers)
		if(userPair.first.name.c_str() == strUserName) return &(userPair.first);
	throw std::runtime_error("there are no such user in ConnectUsers");
}

TcpSocket* Server::findUserSocket(QString userName)
{
	// Поиск сокета по имени пользователя
	qDebug() << "try to find user by name: " << userName;
	for(pair<User, TcpSocket*> userPair : ConnectedUsers) {
		qDebug() << userPair.first.name.c_str();
		if(QString(userPair.first.name.c_str()) == userName) return userPair.second;
	}
	throw std::runtime_error("there are no such user in ConnectUsers");
}

void Server::slotSocketDisconnection()
{
	// Пользователь отключился
	
	qDebug() << "socket disconnected";
	if(Sockets.size() < 1 || ConnectedUsers.size() < 1) throw std::runtime_error("Users vector is empty");
	TcpSocket* disconnectedSocket {Sockets[0]->disconnectedSocket};
	User disconnectedUser;

	// Удаление сокета из вектора сокетов
	for(size_t i {0}; i < Sockets.size(); ++i) {
		if(Sockets[i] == disconnectedSocket) {
			Sockets[i] = Sockets[Sockets.size() - 1];
			Sockets.pop_back();
			break;
		}	
	}	

	// Удавление пользователя из вектора подключенных пользователей
	for(size_t i {0}; i < ConnectedUsers.size(); ++i) {
		if(ConnectedUsers[i].second == disconnectedSocket) {
			disconnectedUser = ConnectedUsers[i].first;
			ConnectedUsers[i] = ConnectedUsers[ConnectedUsers.size() - 1];
			ConnectedUsers.pop_back();
			break;
		}	
	}	

	// Оповещение всех пользователей об отключении игрока
	sendAllUsersUserDisconnected(disconnectedUser.name.c_str());
}

void Server::sendAllUsersUserDisconnected(QString discUserName)
{
	// Оповещение всех пользователей об отключении игрока
	for(pair<User, TcpSocket*> userPair : ConnectedUsers) {
		if(userPair.first.state == User::state_t::IN_LOBBY)
			sendUserInLobbyDisconnected(discUserName, userPair.second);
	}
}

void Server::sendAllLobbyUsersUsersFight(QString fInvoker, QString fReciever)
{
	// Оповещение всех пользователей о начале потенциального сражения
	for(pair<User, TcpSocket*> userPair : ConnectedUsers) {
		if(QString(userPair.first.name.c_str()) != fInvoker &&
		    QString(userPair.first.name.c_str()) != fReciever &&
		    userPair.first.state == User::state_t::IN_LOBBY) {
			sendUsersInLobbyFightState({fInvoker, fReciever}, userPair.second, true);
		}
	}
}

void Server::sendAllLobbyUsersUserFinishedFight(QString connectedUserName)
{
	// Оповещение всех пользователей об окончании сражения
	for(pair<User, TcpSocket*> userPair : ConnectedUsers) {
		if(QString(userPair.first.name.c_str()) != connectedUserName &&
			   userPair.first.state == User::state_t::IN_LOBBY)
			sendUsersInLobbyFightState({connectedUserName}, userPair.second, false);
	}
}

bool Server::login(QString name, QString passHash)
{
	// Регистрация нового пользователя
	
	User findingUser;
	findingUser.name = name.toUtf8().constData();
	findingUser.passHash = passHash.toUtf8().constData();
	findingUser.winCount = "0";
	findingUser.loseCount = "0";
	findingUser.matchCount = "0";

	if(User_DB.findObj(&findingUser, {0, 1}, USERS_TABLE_NAME)) return false;

	// Запись нового пользователя в базу данных
	User_DB.writeObj(findingUser, USERS_TABLE_NAME);
	qDebug() << name + QString(" logined sucessfully");	

	return true;
}

bool Server::auth(QString name, QString passHash)
{
	// Попытка входа
	
	qDebug() << name << passHash;
	User findingUser;

	findingUser.name = name.toUtf8().constData();
	findingUser.passHash = passHash.toUtf8().constData();

	return User_DB.findObj(&findingUser, {0, 1}, USERS_TABLE_NAME);
}

void Server::writeUpdatedInfoToBase(QString name, QString winCount, QString loseCount, QString matchCount)
{
	// Запись статистики игрока в базу данных
	
	qDebug() << winCount << " " << loseCount << " " << matchCount;

	vector<pair<size_t, string>> updateParams {
		{2, string(winCount.toUtf8().constData())},		
		{3, string(loseCount.toUtf8().constData())},		
		{4, string(matchCount.toUtf8().constData())}		
	};

	User_DB.updateObj(updateParams, name.toUtf8().constData(), 0, USERS_TABLE_NAME);
}

