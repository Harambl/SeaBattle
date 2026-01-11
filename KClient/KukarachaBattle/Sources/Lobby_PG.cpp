#include "mainwindow.h"

void MainWindow::sendFightRequest()
{
	// Отправление запроса на сражение
	sendDataToServer(FIGHT_REQUEST_CODE, QString(CurrentUser.name.c_str()), fButtons[0]->fReciever);
}

void MainWindow::createPlayer(QString userName, State state, bool pushInPlayers)
{
	// Создание виджета игрока в лобби
	
	++players;
	if(pushInPlayers == true) Players.push_back({userName, state});

	QString strnumber = QString::number(players);
	
	QWidget* PWidget = new QWidget;

	// Цвет Леши
	//PWidget->setStyleSheet("background-color: BurlyWood; border-radius: 6px; padding: 5px;");
	
	PWidget->setStyleSheet("background-color: #777777; border-radius: 0px; padding: 5px;");
	PWidget->setFixedHeight(60);
	
	QLabel* NumLabel = new QLabel();
	NumLabel->setText(strnumber);
	
	QLabel* NameLabel = new QLabel(userName);
	
	QHBoxLayout* PLayout = new QHBoxLayout(PWidget);
	NumLabel->setStyleSheet("font-weight: bold;");
	
	FightButton* fButton = new FightButton(userName);
	if(userName == QString(CurrentUser.name.c_str())) {
		fButton->setText("Вы");
		fButton->setDisabled(true);
	} else fButton->change(state == State::FIGHTING);

	connect(fButton, SIGNAL(clicked()), fButton, SLOT(slotSetFRecieverName()));
	connect(fButton, SIGNAL(fRecieverSet()), this, SLOT(sendFightRequest()));

	// Цвет Леши
	//fButton->setStyleSheet("background-color: olivedrab");
	
	fButton->setStyleSheet("background-color: #dddddd; border-radius: 2px");

	// Добавление кнопки в общий вектор кнопок предложения боя
	fButtons.push_back(fButton);

	PLayout->addWidget(NumLabel);
	PLayout->addWidget(NameLabel);
	PLayout->addWidget(fButton);
	
	playersLayout->addWidget(PWidget);

	// Странно работающее растяжение
	// Было принято решение убрать этот кусочек кода
#if(0)
	if (!hasStretch) {
		playersLayout->addStretch();
		hasStretch = true;
	} else {
		QLayoutItem *item = playersLayout->itemAt(players - 1);
		playersLayout->takeAt(players - 1);
		playersLayout->addStretch();
		delete item;
	}
#endif
	this->show();
}

void MainWindow::removeDisconnectedUser(QString disconnectedUsername)
{
	--players;				

	// Полная отчистка лобби от всех виджетов пользователей
	
	fButtons.clear();

	while(QLayoutItem* childItem = playersLayout->takeAt(0)) {
		QWidget* widget = childItem->widget();
		QLayout* PLayout = childItem->layout();
		delete widget;
		delete childItem;
	}

	delete playersLayout;
	playersLayout = new QVBoxLayout();
	ui->WidgetContents->setLayout(playersLayout);

	// Добавление обратно всех, кроме отключившегося
	// Отключившегося просто меняем местами с последним в Players и удаляем
	for(size_t i {0}; i < Players.size(); ++i) {
		qDebug() << Players[i].first;
		if(Players[i].first == disconnectedUsername) {
			Players[i] = Players[Players.size() - 1];	
		} else {
			createPlayer(Players[i].first, Players[i].second, false);
		}
	} Players.pop_back();

	this->show();
}

void MainWindow::invokeFightInvintation(QString fInvoker)
{
	// Оповещение пользователя о предложинии сразиться 
	
	QMessageBox* inv {new QMessageBox(this)};
	inv->setWindowTitle("Вызов на бой");
	inv->setText(QString("Игрок %1 вызывает вас на бой").arg(fInvoker));
	inv->setStandardButtons(QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No);
	inv->setIcon(QMessageBox::Icon::Question);
	auto btn {inv->exec()};

	if(btn == QMessageBox::Yes) {
		qDebug() << "accepted";
		sendAcceptedInvintation(fInvoker, CurrentUser.name.c_str(), true);
		mainGameUI->changePlayerMoveFlag(true);
	} else {
		qDebug() << "unaccepted";
		sendAcceptedInvintation(fInvoker, CurrentUser.name.c_str(), false);
	}

	delete inv;
}

void MainWindow::sendAcceptedInvintation(QString fInvoker, QString fReciever, bool accepted)
{
	// Отправление согласия на сражение
	// (Также добавляются имена сражающихся игроков для идентификации сокетов)
	sendDataToServer(
		((accepted == true) ? ACCEPTED_INVINTATION_CODE : REJECTED_INVINTATION_CODE),
		fInvoker, fReciever
	);
}

void MainWindow::updateVisualUserInfo()
{
	// Обновление статистики
	ui->UserLb->setText(CurrentUser.name.c_str());
	ui->WinLb2->setText(CurrentUser.winCount.c_str());
	ui->LoseLb2->setText(CurrentUser.loseCount.c_str());
	ui->MatchLb_2->setText(CurrentUser.matchCount.c_str());
}

void MainWindow::clearListLobbyUsers()
{
	// Удаление всех пользователей из лобби
	while(Players.size() != 0)
		removeDisconnectedUser(Players[0].first);

	players = 0;
}

void MainWindow::changePlayerState(QString name, State state)
{
	// Поиск игрока в Players и изменение его статуса
	for(pair<QString, State>& userPair : Players) {
		if(userPair.first == name) {
			userPair.second = state;	
			break;
		}
	}	
}
