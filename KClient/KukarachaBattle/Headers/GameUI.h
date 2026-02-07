#ifndef GAME_H
#define GAME_H

#include "GameLogic/GameWidget.h"
#include "GameLogic/GameLogic.h"
#include "GameLogic/GameTypes.h"
#include "CNN.h"
#include "Canvas.h"
#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <QString>
#include <QPoint>
#include <QMessageBox>
#include <QMainWindow>
#include <QDebug>
#include <QObject>
#include <stdexcept>


class GameUI : public QObject {
	Q_OBJECT

	long double bulletStopChance {0.5};
	int bulletDir;

	QWidget* parent;

	QLabel* titleLabel;
	QLabel* leftInfoLabel;
	QLabel* rightInfoLabel;
	QLabel* rightLabel;
	QLabel* leftLabel;

	WCanvas* wcanvas;
	CNNetw* CNN;
	MLPNetw* MLP;

	QHBoxLayout* boardsLayout;

	QVBoxLayout* mainLayout;
	QVBoxLayout* leftLayout;
	QVBoxLayout* rightLayout;

	GameWidget* playerBoardWidget;
	GameWidget* enemyBoardWidget;
	void addVSpace(QVBoxLayout* vLayout, int height, int width);

	GameLogic* gameLogic;

	void updateBoards();
	void connectSlots();
	void sendClickedPos(QPoint pos);

public:
	GameUI(QWidget* parent_, WCanvas* wcanvas_);	
	QPoint clickedPos;
	ShotResult enemyShoot(const QPoint& pos);
	void setPlayerBoard(const GameBoard& other, vector<DraggedShip>& ships);
	void invokeShotMessage(ShotResult result, const QPoint& pos);
	void checkMatchFinish();
	void processShotRes(ShotResult result);
	void autoPlaceShips();
	void changePlayerMoveFlag(bool value);
	bool getPlayerMoveFlag();
	void reset();
	GameUI(const GameUI& other) = delete;
	~GameUI();

signals:
	void signalClickedPos();
	void signalWin();
	void signalLose();

public slots:
	void on_cellClicked_enemyBoardWidget(const QPoint& pos);
};


#endif
