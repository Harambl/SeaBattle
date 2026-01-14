#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "GameBoard.h"
#include "../../common/GameTypes.h"
#include <QPoint>
#include <QVector>

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
    bool placeShip(const QPoint& startPos, int size, ShipDirection direction);
    bool canPlaceShip(const QPoint& startPos, int size, ShipDirection direction) const;
    
    // Автоматическая расстановка всех кораблей
    bool autoPlaceShips();
    
    // Выстрел
    ShotResult shoot(const QPoint& pos);
    
    // Получить доску игрока
    const GameBoard& getPlayerBoard() const { return m_playerBoard; }
    
    // Получить доску противника (для отображения результатов выстрелов)
    const GameBoard& getEnemyBoard() const { return m_enemyBoard; }
    
    // Получить список кораблей
    const QVector<Ship>& getShips() const { return m_ships; }
    
    // Проверка, все ли корабли уничтожены
    bool areAllShipsDestroyed() const;
    
    // Получить количество живых кораблей
    int getAliveShipsCount() const;
    
    // Сброс игры
    void reset();
    
private:
    GameBoard m_playerBoard;
    GameBoard m_enemyBoard;
    QVector<Ship> m_ships;
    
    // Проверка, что клетки вокруг корабля свободны (включая диагонали)
    bool checkNeighbors(const QPoint& startPos, int size, ShipDirection direction) const;
    
    // Получить все клетки вокруг корабля
    QVector<QPoint> getNeighborCells(const QPoint& startPos, int size, ShipDirection direction) const;
    
    // Генерация случайной позиции
    QPoint randomPosition() const;
    ShipDirection randomDirection() const;
};

#endif // GAMELOGIC_H

