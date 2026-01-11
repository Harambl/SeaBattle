#include "mainwindow.h"

void MainWindow::sendClickedCellPos(QPoint pos)
{
	// Отправляем позицию клика и имя противника для идентефикации сокета
	sendDataToServer(POINT_POSITION_CODE, pos, enemy);
}

void MainWindow::sendShotResult(ShotResult result)
{
	// Отправляем результат выстрела и имя противника для идентефикации сокета
	sendDataToServer(SHOT_RESULT_CODE, result, enemy);
}

void MainWindow::sendToServerMatchesInfo()
{
	// Отправляем информацию о пользователе
	sendDataToServer(
			UPDATED_USER_INFO_CODE, 
			QString(CurrentUser.name.c_str()),
			QString(CurrentUser.winCount.c_str()),
			QString(CurrentUser.loseCount.c_str()),
			QString(CurrentUser.matchCount.c_str())
	);
}

void MainWindow::onSignalClickedPos()
{
	sendClickedCellPos(mainGameUI->clickedPos);
}

void MainWindow::onSignalWin()
{
	// Победа
	endFight(FightEnd_t::Wictory);
}

void MainWindow::onSignalLose()
{
	// Поражение
	endFight(FightEnd_t::Defeat);
}

void MainWindow::endFight(FightEnd_t fightEnd)
{
	// Общая для победы и поражения функция завершения боя
	
	if(fightEnd == FightEnd_t::Wictory) {
		// +1 к победам и +1 количеству матчей
		changeUserMatchesInfo(1, 0, 1);
		log("Player won the game", "INFO");
	}
	else {
		// +1 к поражениям и +1 количеству матчей
		changeUserMatchesInfo(0, 1, 1);
		log("Player lose the game", "INFO");
	}

	sendToServerMatchesInfo();

	// Изменение страницы и состояния пользователя
	changePage(Page::LOBBY);
	state = State::IN_LOBBY;

	QThread::sleep(0.3);
	// Оповещение всех пользователей о переходе в лобби
	sendNameAndCodeToServer(USER_IN_LOBBY_CODE);

	// Cброс игрового интерфейса
	mainGameUI->reset();
	mainGameUI->changePlayerMoveFlag(false);
}


