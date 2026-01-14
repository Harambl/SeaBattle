#include "GameLogic.h"
#include "../../common/GameTypes.h"
#include <QRandomGenerator>
#include <algorithm>

GameLogic::GameLogic() {
    reset();
}

bool GameLogic::canPlaceShip(const QPoint& startPos, int size, ShipDirection direction) const {
    int row = startPos.y();
    int col = startPos.x();
    
    // Проверка границ
    if (direction == ShipDirection::Horizontal) {
        if (col + size > BOARD_SIZE) {
            return false;
        }
    } else {
        if (row + size > BOARD_SIZE) {
            return false;
        }
    }
    
    // Проверка, что клетки свободны
    for (int i = 0; i < size; ++i) {
        QPoint pos;
        if (direction == ShipDirection::Horizontal) {
            pos = QPoint(col + i, row);
        } else {
            pos = QPoint(col, row + i);
        }
        
        if (m_playerBoard.getCell(pos) != CellType::Empty) {
            return false;
        }
    }
    
    // Проверка соседей
    return checkNeighbors(startPos, size, direction);
}

bool GameLogic::checkNeighbors(const QPoint& startPos, int size, ShipDirection direction) const {
    QVector<QPoint> neighbors = getNeighborCells(startPos, size, direction);
    
    for (const QPoint& neighbor : neighbors) {
        if (m_playerBoard.getCell(neighbor) == CellType::Ship) {
            return false;
        }
    }
    
    return true;
}

QVector<QPoint> GameLogic::getNeighborCells(const QPoint& startPos, int size, ShipDirection direction) const {
    QVector<QPoint> neighbors;
    int row = startPos.y();
    int col = startPos.x();
    
    // Добавляем клетки вокруг корабля
    for (int i = -1; i <= size; ++i) {
        for (int j = -1; j <= 1; ++j) {
            QPoint pos;
            if (direction == ShipDirection::Horizontal) {
                pos = QPoint(col + i, row + j);
            } else {
                pos = QPoint(col + j, row + i);
            }
            
            if (m_playerBoard.isValidPosition(pos)) {
                neighbors.append(pos);
            }
        }
    }
    
    return neighbors;
}

bool GameLogic::placeShip(const QPoint& startPos, int size, ShipDirection direction) {
    if (!canPlaceShip(startPos, size, direction)) {
        return false;
    }
    
    Ship ship;
    ship.startPos = startPos;
    ship.size = size;
    ship.direction = direction;
    ship.hits = 0;
    
    int row = startPos.y();
    int col = startPos.x();
    
    // Размещаем корабль на доске
    for (int i = 0; i < size; ++i) {
        QPoint pos;
        if (direction == ShipDirection::Horizontal) {
            pos = QPoint(col + i, row);
        } else {
            pos = QPoint(col, row + i);
        }
        
        m_playerBoard.setCell(pos, CellType::Ship);
        ship.cells.append(pos);
    }
    
    m_ships.append(ship);
    return true;
}

bool GameLogic::autoPlaceShips() {
    reset();
    
    QRandomGenerator* rng = QRandomGenerator::global();
    int attempts = 0;
    const int maxAttempts = 1000;
    
    for (const auto& config : STANDARD_SHIPS) {
        for (int shipIndex = 0; shipIndex < config.count; ++shipIndex) {
            bool placed = false;
            attempts = 0;
            
            while (!placed && attempts < maxAttempts) {
                QPoint pos = randomPosition();
                ShipDirection dir = randomDirection();
                
                if (placeShip(pos, config.size, dir)) {
                    placed = true;
                }
                ++attempts;
            }
            
            if (!placed) {
                reset();
                return false;
            }
        }
    }
    
    return true;
}

QPoint GameLogic::randomPosition() const {
    QRandomGenerator* rng = QRandomGenerator::global();
    int row = rng->bounded(BOARD_SIZE);
    int col = rng->bounded(BOARD_SIZE);
    return QPoint(col, row);
}

ShipDirection GameLogic::randomDirection() const {
    QRandomGenerator* rng = QRandomGenerator::global();
    return (rng->bounded(2) == 0) ? ShipDirection::Horizontal : ShipDirection::Vertical;
}

ShotResult GameLogic::shoot(const QPoint& pos) {
    if (!m_playerBoard.isValidPosition(pos)) {
        return ShotResult::Invalid;
    }
    
    CellType cell = m_playerBoard.getCell(pos);
    
    // Если уже стреляли в эту клетку
    if (cell == CellType::Miss || cell == CellType::Hit || cell == CellType::Destroyed) {
        return ShotResult::Invalid;
    }
    
    // Промах
    if (cell == CellType::Empty) {
        m_playerBoard.setCell(pos, CellType::Miss);
        return ShotResult::Miss;
    }
    
    // Попадание
    if (cell == CellType::Ship) {
        m_playerBoard.setCell(pos, CellType::Hit);
        
        // Находим корабль и увеличиваем счетчик попаданий
        for (Ship& ship : m_ships) {
            if (ship.cells.contains(pos)) {
                ship.hits++;
                
                if (ship.isDestroyed()) {
                    // Помечаем все клетки корабля как уничтоженные
                    for (const QPoint& cellPos : ship.cells) {
                        m_playerBoard.setCell(cellPos, CellType::Destroyed);
                    }
                    return ShotResult::Destroyed;
                }
                
                return ShotResult::Hit;
            }
        }
    }
    
    return ShotResult::Miss;
}

bool GameLogic::areAllShipsDestroyed() const {
    for (const Ship& ship : m_ships) {
        if (!ship.isDestroyed()) {
            return false;
        }
    }
    return true;
}

int GameLogic::getAliveShipsCount() const {
    int count = 0;
    for (const Ship& ship : m_ships) {
        if (!ship.isDestroyed()) {
            count++;
        }
    }
    return count;
}

void GameLogic::reset() {
    m_playerBoard.clear();
    m_enemyBoard.clear();
    m_ships.clear();
}

