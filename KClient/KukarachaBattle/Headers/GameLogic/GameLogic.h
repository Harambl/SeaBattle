#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "GameLogic/GameBoard.h"
#include "GameLogic/GameTypes.h"
#include <QPoint>
#include <QVector>
#include <QDebug>

#include <vector>
#include <queue>

using std::queue;
using std::pair;
using std::vector;

struct Ship {
    QPoint startPos;
    int size;
    ShipDirection direction;
    QVector<QPoint> cells;  // Все клетки корабля
    int hits;               // Количество попаданий
    
    bool isDestroyed() const { return hits >= size; }
};

class GameLogic {
public:
    GameLogic();
    
    // Расстановка кораблей
    bool placeShip(const QPoint& startPos, int size, ShipDirection direction, BoardType b_type);
    static bool canPlaceShip(const QPoint& startPos, int size, ShipDirection direction, const GameBoard& board);
    
    // Автоматическая расстановка всех кораблей
    bool autoPlaceShips(BoardType b_type);
    
    // Выстрел
    ShotResult shoot(const QPoint& pos, BoardType b_type);

    // Изменение клетки доски противника
    void setEnemyCell(const QPoint& pos, const ShotResult result);
    
    // Получение ссылки на доску по ее типу
    GameBoard& getBoard(BoardType b_type) {
	    return ((b_type == BoardType::EnemyBoard) ? m_enemyBoard : m_playerBoard);
    }
    
    // Получить доску игрока
    const GameBoard& getPlayerBoard() const { return m_playerBoard; }
    
    // Получить доску противника (для отображения результатов выстрелов)
    const GameBoard& getEnemyBoard() const { return m_enemyBoard; }

    // Получить список кораблей
    const QVector<Ship>& getShips() const { return m_ships; }
    
    // Проверка, все ли корабли основного игрока уничтожены
    bool areAllPlayerShipsDestroyed() const;

    // Проверка, все ли корабли соперника уничтожены
    bool areAllEnemyShipsDestroyed() const;
    
    // Получить количество живых кораблей игрока
    int getPlayerAliveShipsCount() const;
    
    // Получить количество живых кораблей соперника
    int getEnemyAliveShipsCount() const;

    // Изменить флаг хода игрока
    void changePlayerMoveFlag(bool value);

    // Получить флаг хода игрока
    bool getPlayerMoveFlag();

    // Разместить корабли из вектора
    void setPlayerShips(vector<DraggedShip>& ships);

    // Сброс игры
    void reset();
    
private:
    GameBoard m_playerBoard;
    GameBoard m_enemyBoard;
    QVector<Ship> m_ships;
    int enemyAliveShipsCount;
    bool isPlayerMove {false};
    
    // Проверка, что клетки вокруг корабля свободны (включая диагонали)
    static bool checkNeighbors(const QPoint& startPos, int size, ShipDirection direction, const GameBoard& board);
    
    // Получить все клетки вокруг корабля
    static QVector<QPoint> getNeighborCells(const QPoint& startPos,
		    int size, ShipDirection direction, const GameBoard& board);
    
    // Уничтожение корабля
    void destrShip(const QPoint& pos);

    // Генерация случайной позиции
    QPoint randomPosition() const;
    ShipDirection randomDirection() const;
};

#endif // GAMELOGIC_H

