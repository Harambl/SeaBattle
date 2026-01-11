#include "mainwindow.h"

void MainWindow::onReadyRead()
{
	QDataStream in(m_socket);
	in.setVersion(QDataStream::Qt_5_2);
	User user;
	quint8 code;

	if(m_socket->bytesAvailable() == 1) {
		log("Получен код от сервера", "INFO");
		in >> code; processServerCode(code);
		return;
	}

	dataSize = 0;

	if(in.status() == QDataStream::Ok) {
		// Цикл обработки информации от сервера
		
		for(;;) {
			if(dataSize == 0) {
				if(m_socket->bytesAvailable() < 2) continue;
				in >> dataSize;
			}
			if(m_socket->bytesAvailable() < dataSize) {
				log(QString("data loosed: recieved %1 / %2")
						.arg(m_socket->bytesAvailable()).arg(dataSize), "INFO");
				continue;
			}
			in >> code;

			// По коду идет идентефикация типа информации сервера
			
			if(code == OK_AUTH_CODE) {

				// Удачная авторизация, сервер предоставил все данные пользователя
				log("authorized successfully", "INFO");
				qDebug() << "OK_AUTH_CODE";
				QString name, winCount, loseCount, matchCount;
				in >> name >> winCount >> loseCount >> matchCount;
				qDebug() << name << winCount << loseCount << matchCount;
				CurrentUser = User({
							name.toUtf8().constData(),
							string(""),
							winCount.toUtf8().constData(),
							loseCount.toUtf8().constData(),
							matchCount.toUtf8().constData(),
						});
				updateVisualUserInfo();
				ui->stackedWidget->setCurrentIndex(Page::LOBBY);
				break;
			}

			else if(code == USERNAME_CODE) {

				// Получены имена и статусы игроков для отображения в лобби
				log("recieved player name to playersLayout", "INFO");
				vector<QString> Names; quint16 size;
				QString otherUserName; bool isFighting; State newUserState;
				// Считываем отправленное количество игроков
				in >> size;
				for(quint16 i {0}; i < size; ++i) {
					// Считываем каждого игрока
					in >> otherUserName;
					in >> isFighting;
					newUserState = (isFighting) ? State::FIGHTING : State::IN_LOBBY;
					createPlayer(otherUserName, newUserState, true);
				}
				break;
			}

			else if(code == FIGHT_INVINTATION_CODE) {

				// Получено приглашение к бою от одного из игроков в лобби
				// Считываетсяа имя отправителя и отображается само приглашение
				QString fInvoker; in >> fInvoker;
				invokeFightInvintation(fInvoker);
				break;

			} else if(code == START_SHIPS_PLANNING) {

				// Переход на страницу размещения кораблей
				in >> enemy;
				log(QString("Начался бой с %1").arg(enemy), "INFO");
				startShipsPlanning();
				break;
			
			} else if(code == ENEMY_IS_READY_TO_FIGHT_CODE) {
					
				// Уведомление о готовности противника начать бой
				if(shipsPlanningUI == nullptr) throw std::runtime_error("is not planning");
				shipsPlanningUI->setEnemyIsReady();
				break;

			} else if(code == START_FIGHT_CODE) {
					
				// Начало сражения
				startFight();
				break;

			} else if(code == REJECTED_FIGHT_CODE) {
					
				// Противник отклонил предложения боя
				in >> enemy;
				log(QString("Отклонен бой с %1").arg(enemy), "INFO");

				// Оповещение об этом
				if(enemy != QString(CurrentUser.name.c_str())) {
					QMessageBox* errorMessage = new QMessageBox;
					errorMessage->information(this, 
							"Opponent anwer", "Противние отклонил вызов");
					delete errorMessage;
				}
				break;

			} else if(code == USER_IN_LOBBY_DISC_CODE) {
					
				// Один из пользователей отключился от сервера
				// Считывается его имя
				QString disconnectedUsername;
				in >> disconnectedUsername;

				// Убираем из Players и игроков в лобби
				removeDisconnectedUser(disconnectedUsername);
				log(QString("Удален пользователь %1 из списка пользователей")
						.arg(disconnectedUsername), "INFO");
				break;

			} else if(code == POINT_POSITION_SERVER_CODE) {
					
				// Получены координаты выстрела противника
				// Считываем из
				QPoint pos;
				ShotResult result;
				in >> pos;

				// Визуально отображаем выстрел противника по этим координатам
				result = mainGameUI->enemyShoot(pos);

				// Уведомляем текущего пользователя о результате выстрела
				mainGameUI->invokeShotMessage(result, pos);
				if(result == ShotResult::Miss)  
					mainGameUI->changePlayerMoveFlag(true);

				// После закрытия уведмления о выстреле уведомляем противника о результате
				sendShotResult(result);

				// Проверка на окончание боя
				mainGameUI->checkMatchFinish();
				break;
				
			} else if(code == SHOT_RESULT_SERVER_CODE) {
					
				// Получен результат выстрела текущего игрока
				ShotResult result;
				in >> result;

				// Обработка его
				processShotResult(result);

				// Проверка на окончание боя
				mainGameUI->checkMatchFinish();
				break;

			} else if(code == USER_FIGHTS_CODE) {
				
				// Игроки в лобби начали бой
				// Считывание их имен
				QString fightsUserName1, fightsUserName2;
				in >> fightsUserName1 >> fightsUserName2;

				// Изменяем соответствующие им кнопки
				changeFButton(fightsUserName1, true);
				changeFButton(fightsUserName2, true);

				// Изменяем состояния этих игроков в Players
				changePlayerState(fightsUserName1, State::FIGHTING);
				changePlayerState(fightsUserName2, State::FIGHTING);
				break;

			} else if(code == USER_FINISHED_FIGHT_CODE) {
				
				// Один из игроков вернулся в лобби после боя
				// Считываем его имя
				QString connectedUserName;
				in >> connectedUserName;

				// Изменяем соответствующую ему кнопку
				changeFButton(connectedUserName, false);

				// Измеиняем его состояние в векторе Players
				changePlayerState(connectedUserName, State::IN_LOBBY);
				break;

			} else 
				// Сервер отправил сообщение, начинающееся с невалидного кода
				qDebug() << "strange message from server";
		}	

		log("Data reading finished", "INFO");
	} else
		log("Error reading data", "INFO");
}

