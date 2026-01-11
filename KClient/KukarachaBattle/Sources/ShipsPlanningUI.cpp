#include "ShipsPlanningUI.h"

ShipsPlanningUI::ShipsPlanningUI(QWidget* parent) : QWidget(parent) 
{
	QVBoxLayout* mainLayout {new QVBoxLayout(parent)};
	QWidget* emptyW {new QWidget(parent)};
	QHBoxLayout* hLayout {new QHBoxLayout(emptyW)};

	// Заголовок
	titleLabel = new QLabel("Расстановка кораблей", parent);
	titleLabel->setAlignment(Qt::AlignCenter);
	titleLabel->setStyleSheet("font-size: 18px; background-color: #777777; \
			font-weight: bold; color: #eeeeee");

	// Горизнотальные линии
	QLabel* spaceWidget1 = new QLabel("", parent);
	spaceWidget1->setStyleSheet("background-color: #777777");
	QLabel* spaceWidget2 = new QLabel("", parent);
	spaceWidget2->setStyleSheet("background-color: #777777");

	// Доска для размещения кораблей
	playerBoardWidget = new GameWidget(parent, BoardWidgetType::PlanningBoard);
	playerBoardWidget->setShowShips(true);

	// Кнопка готовности к бою
	readyBtn = new QPushButton("Готово", parent);
	readyBtn->setStyleSheet("font-size: 17px; background-color: #777777; color: #eeeeee");
	
	// Кнопка начала боя
	fightBtn = new QPushButton("Ожидание противника", parent);
	fightBtn->setStyleSheet("font-size: 17px; background-color: #777777; color: #eeeeee");
	fightBtn->setDisabled(true);

	hLayout->addSpacerItem(new QSpacerItem(100, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
	hLayout->addWidget(readyBtn);
	hLayout->addSpacerItem(new QSpacerItem(200, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
	hLayout->addWidget(fightBtn);
	hLayout->addSpacerItem(new QSpacerItem(100, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));

	mainLayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
	mainLayout->addWidget(titleLabel);
	mainLayout->addWidget(spaceWidget1);
	mainLayout->addWidget(playerBoardWidget);
	mainLayout->addWidget(emptyW);
	mainLayout->addStretch(false);

	connect(readyBtn, SIGNAL(clicked()), this, SLOT(onClickedReadyBtn()));
	connect(fightBtn, SIGNAL(clicked()), this, SLOT(onClickedFightBtn()));

	this->show();	
}

void ShipsPlanningUI::onClickedReadyBtn()
{
	// Нажатие на клавишу готовности к бою
	// Если клавиша уже была нажата, выходим из функции
	if(isReadyToFight) return;

	// Если расставлены не все корабли, тоже выходим из функции
	if(!(playerBoardWidget->isAllShipsPlanned())) {
		qDebug() << "not all ships planned";
		return;	
	}

	isReadyToFight = true;

	// Если противник уже готов, делаем кнопку начала боя кликабельной
	if(isEnemyReadyToFight) fightBtn->setDisabled(false);

	emit signalReadyToFight();
}

void ShipsPlanningUI::onClickedFightBtn()
{
	// Нажатие кнопки "Начать бой"
	emit signalStartFight();
}

void ShipsPlanningUI::setEnemyIsReady()
{
	// Изменение кликабельности кнопки начала сражения
	fightBtn->setText("Начать бой");
	isEnemyReadyToFight = true;

	// Если текущий пользователь уже готов, делаем кнопку кликабельной
	if(isReadyToFight) fightBtn->setDisabled(false);
}

void ShipsPlanningUI::reset()
{
	// Сброс интерфейся расстановки кораблей
	playerBoardWidget->reset();
	isReadyToFight = false;
	isEnemyReadyToFight = false;

	fightBtn->setText("Ожидание противника");
	fightBtn->setDisabled(true);
}

const GameBoard& ShipsPlanningUI::getBoard()
{
	return playerBoardWidget->getBoard();	
}

vector<DraggedShip>& ShipsPlanningUI::getDraggedShips()
{
	return playerBoardWidget->getDraggedShips();
}
