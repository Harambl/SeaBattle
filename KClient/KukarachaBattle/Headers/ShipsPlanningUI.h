#ifndef SHIPSPLANNING_H
#define SHIPSPLANNING_H

#include "GameLogic/GameWidget.h"
#include "GameLogic/GameBoard.h"
#include "GameLogic/GameTypes.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSpacerItem>
#include <QLabel>

class GameLogic;

class ShipsPlanningUI : public QWidget {
	Q_OBJECT
	QLabel* titleLabel;
	GameWidget* playerBoardWidget;	
	QPushButton* readyBtn;
	QPushButton* fightBtn;
	bool isReadyToFight {false};
	bool isEnemyReadyToFight {false};

public:
	ShipsPlanningUI(QWidget* parent = nullptr);
	void setEnemyIsReady();
	void reset();
	vector<DraggedShip>& getDraggedShips();
	const GameBoard& getBoard();
	~ShipsPlanningUI() = default;	

signals:
	void signalReadyToFight();
	void signalStartFight();

public slots:
	void onClickedReadyBtn();
	void onClickedFightBtn();
};


#endif  // SHIPSPLANNING_H
