#include "Server.h"

void Server::slotReadyRead()
{
	currSocket = static_cast<TcpSocket*>(sender());
	QString name, passHash;
	QDataStream in(currSocket);
	in.setVersion(QDataStream::Qt_5_2);
	dataSize = 0;

	quint16 code {0};

	if(in.status() == QDataStream::Ok) {
		cout << "reading data..." << endl;		

		for(;;) {

			if(dataSize == 0) {
				if(currSocket->bytesAvailable() < 4) {
					cout << "dataSize reading error..." << endl;
					continue;		
				}
				in >> dataSize;	
			}

			if(currSocket->bytesAvailable() < dataSize) {
				qDebug() << "data loosed: " << currSocket->bytesAvailable() << "/" << dataSize;
				continue;
			}

			in >> code;
			//processing code

			if(code == LOGIN_CODE) {

				in >> name >> passHash;
				if(login(name, passHash) == true)
					sendCode(OK_LOGIN_CODE, currSocket);
				else
					sendCode(WRONG_LOGIN_CODE, currSocket);
				break;

			} else if(code == AUTH_CODE) {

				in >> name >> passHash;
				if(auth(name, passHash) == true) {
					string _name = name.toUtf8().constData();
					User user = User_DB.getObj(_name, 0, USERS_TABLE_NAME);
					user.state = User::state_t::IN_LOBBY;
					sendUserInfo(user, currSocket);
					ConnectedUsers.push_back({user, currSocket});
					sendConnectedUsers(name, currSocket);
					QThread::sleep(0.5);
				} else
					sendCode(WRONG_AUTH_CODE, currSocket);
				break;

			} else if(code == FIGHT_REQUEST_CODE) {

				QString fReciever, fInvoker;
				in >> fInvoker >> fReciever;
				qDebug() << fInvoker << fReciever;
				sendFightMessageToOpponent(fInvoker, fReciever, FIGHT_INVINTATION_CODE);
				break;

			} else if(code == ACCEPTED_INVINTATION_CODE) {

				QString fInvoker, fReciever;
				in >> fInvoker >> fReciever;
				startShipsPlanning(fInvoker, fReciever);
				break;	

			} else if(code == REJECTED_INVINTATION_CODE) {
							
				QString fInvoker, fReciever;
				in >> fInvoker >> fReciever;
				sendFightMessageToOpponent(fReciever, fInvoker, REJECTED_FIGHT_CODE);
				break;

			} else if(code == POINT_POSITION_CODE) {
					
				QPoint pos; QString enemy;
				in >> pos >> enemy;
				sendPointPosToEnemy(pos, enemy);
				break;

			} else if(code == SHOT_RESULT_CODE) {
					
				ShotResult result; QString enemy;
				in >> result >> enemy;
				sendShotResultToEnemy(result, enemy);
				break;

			} else if(code == UPDATED_USER_INFO_CODE) {
				
				QString name, winCount, loseCount, matchCount;
				in >> name >> winCount >> loseCount >> matchCount;
				writeUpdatedInfoToBase(name, winCount, loseCount, matchCount);
				break;

			} else if(code == USER_IN_LOBBY_CODE) {
				
				QString connectedUserName;
				in >> connectedUserName;
				User* pU {findUser(connectedUserName)};
				pU->state = User::state_t::IN_LOBBY;
				sendAllUsers(findUserSocket(connectedUserName));
				sendAllLobbyUsersUserFinishedFight(connectedUserName);
				break;

			} else if(code == READY_TO_FIGHT_CODE) {
				
				QString enemy;
				in >> enemy;
				sendEnemyUserReadyToFight(enemy);
				break;

			} else if(code == PRESSED_FIGHT_BUTTON_CODE) {
					
				QString player1, player2;
				in >> player1 >> player2;
				startFight(player1, player2);
				break;
				
			}
		}

		cout << "reading finished" << endl;
	}
	else cout << "data reading error..." << endl;
}

