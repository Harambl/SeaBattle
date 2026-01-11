#include "GameUI.h"

GameUI::GameUI(QWidget* parent_) : parent{parent_}
{
	mainLayout = new QVBoxLayout(parent);
	
	// Заголовок
	titleLabel = new QLabel("Идет бой");
	titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; \
				   background-color: #676767; color: #e0e0e0; padding: 10px;");
	titleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);

	addVSpace(mainLayout, 10, 10);
	mainLayout->addWidget(titleLabel);
	
	// Горизонтальный layout для полей
	boardsLayout = new QHBoxLayout;
	
	// Левое поле (свое поле с кораблями)
	leftLayout = new QVBoxLayout;
	leftLabel = new QLabel("Ваше поле");
	leftLabel->setStyleSheet("font-size: 14px; font-weight: bold;");
	leftLabel->setAlignment(Qt::AlignCenter);

	addVSpace(leftLayout, 20, 30);
	leftLayout->addWidget(leftLabel);
	
	// Поле текущего игрока
	playerBoardWidget = new GameWidget;
	playerBoardWidget->setShowShips(true);
	playerBoardWidget->setStyleSheet("color: #dddddd");
	playerBoardWidget->setAutoFillBackground(true);

	leftLayout->addWidget(playerBoardWidget);
	
	// Количество кораблей текущего игрока
	leftInfoLabel = new QLabel("Корабли: 10");
	leftInfoLabel->setStyleSheet("font-size: 18px; background-color: #777777; color: #eeeeee");
	leftLabel->setStyleSheet("font-size: 18px; font-weight: bold; background-color: #777777; color: #eeeeee");
	leftInfoLabel->setAlignment(Qt::AlignCenter);

	addVSpace(leftLayout, 20, 20);
	leftLayout->addWidget(leftInfoLabel);
	addVSpace(leftLayout, 20, 20);
	
	leftLayout->addStretch(true);
	boardsLayout->addLayout(leftLayout);
	
	// Правое поле (поле противника - только результаты выстрелов)
	rightLayout = new QVBoxLayout;
	rightLabel = new QLabel("Поле противника");
	rightLabel->setStyleSheet("font-size: 18px; font-weight: bold; background-color: #777777; color: #eeeeee");
	rightLabel->setAlignment(Qt::AlignCenter);

	addVSpace(rightLayout, 20, 30);
	rightLayout->addWidget(rightLabel);
	
	// Поле противника
	enemyBoardWidget = new GameWidget;
	enemyBoardWidget->setShowShips(false);

	rightLayout->addWidget(enemyBoardWidget);
	
	// Количество кораблей противника
	rightInfoLabel = new QLabel("Корабли противника: 10");
	rightInfoLabel->setStyleSheet("font-size: 18px; background-color: #777777; color: #eeeeee");
	rightInfoLabel->setAlignment(Qt::AlignCenter);

	addVSpace(rightLayout, 20, 20);
	rightLayout->addWidget(rightInfoLabel);

	rightLayout->addStretch(true);
	boardsLayout->addLayout(rightLayout);
	
	mainLayout->addLayout(boardsLayout);
	
	// Игровая логика
	gameLogic = new GameLogic();

	// Подсоединение слотов к сигналам
	connectSlots();
}

void GameUI::addVSpace(QVBoxLayout* vLayout, int height, int width)
{
	// Добавление пустого пространства в vlayout
	vLayout->addSpacerItem(new QSpacerItem(height, width, QSizePolicy::Minimum, QSizePolicy::Expanding));
}

GameUI::~GameUI()
{
	delete gameLogic;
}

void GameUI::reset()
{
	gameLogic->reset();
	updateBoards();
}

void GameUI::autoPlaceShips()
{
	// Автоматическая расстановка кораблей (пока нигде не используется)
	if (!gameLogic->autoPlaceShips(BoardType::MainPlayerBoard)) {
		QMessageBox::critical(parent, "Ошибка", "Не удалось расставить корабли!");
		throw std::runtime_error("Ships mapping error");
	} updateBoards();
}

void GameUI::updateBoards()
{
	// Визуальное обновление досок и количества кораблей
        playerBoardWidget->setBoard(gameLogic->getPlayerBoard());
        enemyBoardWidget->setBoard(gameLogic->getEnemyBoard());
        leftInfoLabel->setText(QString("Корабли: %1").arg(gameLogic->getPlayerAliveShipsCount()));
        rightInfoLabel->setText(QString("Корабли противника: %1").arg(gameLogic->getEnemyAliveShipsCount()));
}

void GameUI::connectSlots()
{
	// Подключение сигнала клика по полю к соответствующему слоту
    	QObject::connect(enemyBoardWidget, &GameWidget::cellClicked, [&](const QPoint& pos) {
			on_cellClicked_enemyBoardWidget(pos);
	}); // ?? Почему-то со старым синтаксисом не работает
}

void GameUI::on_cellClicked_enemyBoardWidget(const QPoint& pos)
{
	// Слот обработки клика по полю
	if(gameLogic->getEnemyBoard().getCell(pos) != CellType::Empty) return;
	if(gameLogic->getPlayerMoveFlag() == false) return;

	clickedPos = const_cast<QPoint&>(pos);

	gameLogic->changePlayerMoveFlag(false);
	emit signalClickedPos();
}

ShotResult GameUI::enemyShoot(const QPoint& pos)
{
	// Возвращение результата выстрела и обновление досок
	ShotResult result = gameLogic->shoot(pos, BoardType::MainPlayerBoard);
        updateBoards();
	return result;
}

void GameUI::invokeShotMessage(ShotResult result, const QPoint& pos)
{
	// Оповещение пользователя о результате выстрела
	
	if(result == ShotResult::Invalid) return;

        QString resultText;
        switch (result) {
        	case ShotResult::Miss:
        		resultText = "Мимо";
        		break;
        	case ShotResult::Hit:
        		resultText = "Ранил";
        		break;
        	case ShotResult::Destroyed:
        		resultText = "Убил";
        		break;
        	case ShotResult::Invalid:
        		resultText = "Некорректный выстрел";
        		break;
        };
        
        QMessageBox::information(parent, "Результат", 
       		QString("Выстрел в %1%2: %3")
       			.arg(QChar('A' + pos.x()))
       			.arg(pos.y() + 1)
       			.arg(resultText));
}

void GameUI::checkMatchFinish()
{
	// Проверка на окончание матча
        if (gameLogic->areAllEnemyShipsDestroyed()) {
        	QMessageBox::information(parent, "Победа!", "Все корабли противника уничтожены!");
		emit signalWin();
	} else if (gameLogic->areAllPlayerShipsDestroyed()) {
        	QMessageBox::information(parent, "Поражение", "Все корабли уничтожены!");
		emit signalLose();
        }
}

void GameUI::processShotRes(ShotResult result)
{
	// Изменение клетки, по которой стрелял текущий игрок
	gameLogic->setEnemyCell(clickedPos, result);

	// Если выстрел удачный, не передаем право выстрела противнику
	if(result == ShotResult::Hit || result == ShotResult::Destroyed)
		changePlayerMoveFlag(true);

        updateBoards();
}

void GameUI::changePlayerMoveFlag(bool value)
{
	// Изменение флага хода
	// (false - ход противника, true - ход текущего игрока)
	gameLogic->changePlayerMoveFlag(value);
}

bool GameUI::getPlayerMoveFlag()
{
	// Геттер флага хода
	return gameLogic->getPlayerMoveFlag();	
}

void GameUI::setPlayerBoard(const GameBoard& board, vector<DraggedShip>& ships)
{
	// Размещение кораблей на доске gameLogic
	gameLogic->setPlayerShips(ships);

	// Размещение кораблей на доске playerBoardWidget
	playerBoardWidget->setBoard(board);

	updateBoards();
}
