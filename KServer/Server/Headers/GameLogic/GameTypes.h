#ifndef GAMETYPES_H
#define GAMETYPES_H
#include <QPoint>


enum class BoardType { MainPlayerBoard, EnemyBoard };
enum class CellType { Empty, Ship, Miss, Hit, Destroyed };
struct ShipConf {
	int size;
	int count;
};
enum class ShipDirection { Horizontal, Vertical };
enum class ShotResult { Hit, Miss, Destroyed, Invalid };
enum class FightEnd_t { Defeat, Wictory };
constexpr int BOARD_SIZE = 10;
constexpr ShipConf STANDART_SHIPS[] = {{4, 1}, {3, 2}, {2, 3}, {1, 4}};

// одобрено


#endif // GAMETYPES_H
