#ifndef GAMETYPES_H
#define GAMETYPES_H

#include <QPoint>


enum class BoardWidgetType { FightBoard, PlanningBoard};
enum class BoardType { MainPlayerBoard, EnemyBoard };
enum class CellType { Empty, Ship, Miss, Hit, Destroyed };
enum class ShipDirection { Horizontal, Vertical };
struct ShipConf {
	int size;
	int count;
};
struct DraggedShip {
	int x; int y;
	int indexInPlanningShips;
	ShipConf shipConf;
	ShipDirection dir;
};
enum class ShotResult { Hit, Miss, Destroyed, Invalid };
enum class FightEnd_t { Defeat, Wictory };
constexpr int BOARD_SIZE = 10;
constexpr ShipConf STANDART_SHIPS[] = {{4, 1}, {3, 2}, {2, 3}, {1, 4}};

// одобрено

#endif // GAMETYPES_H
