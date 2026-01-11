#include "mainwindow.h"

void MainWindow::startShipsPlanning()
{
	// Начало расстановки кораблей
	
	// Сброс интерфейса расстановки кораблей
	shipsPlanningUI->reset();

	// Переход на страницу расстановки кораблей
	changePage(Page::SHIPS_PLANNING);
	
	// Изменения состояния текущего пользователя
	state = State::FIGHTING;
	
	// Удаление всех пользователей из Players вместе с их виджетами
	// (После окончания боя идет запрос на сервер для обратного получения списка пользователей)
	clearListLobbyUsers();

	// Проверки на работоспособность предыдущей строки
	if(Players.size() != 0) throw std::runtime_error("players v is not empty");
	if(fButtons.size() != 0) throw std::runtime_error("buttons v is not empty");
}

void MainWindow::startFight()
{
	// Начало боя
	// Сброс игрового интерфейса
	resetGameUI();

	// Переход на страницу сражения
	changePage(Page::GAME);

	// Копирование пользовательской расстановки в mainGameUI
	mainGameUI->setPlayerBoard(shipsPlanningUI->getBoard(), shipsPlanningUI->getDraggedShips());
}

void MainWindow::onSignalReadyToFight()
{
	// Отправление кода готовности к бою на сервер
	sendDataToServer(READY_TO_FIGHT_CODE, enemy);
}

void MainWindow::onSignalStartFight()
{
	// Отправление кода нажатия кнопки "Начало боя" на сервер
	sendDataToServer(PRESSED_FIGHT_BUTTON_CODE, QString(CurrentUser.name.c_str()), enemy);
}
