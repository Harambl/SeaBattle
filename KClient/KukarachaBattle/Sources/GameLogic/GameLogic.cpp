#include "GameLogic/GameLogic.h"
#include "GameLogic/GameTypes.h"
#include <QRandomGenerator>
#include <algorithm>

GameLogic::GameLogic() : enemyAliveShipsCount {10} {
    reset();
}

bool GameLogic::canPlaceShip(const QPoint& startPos, int size,
	    ShipDirection direction, const GameBoard& board) {
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
        
        if (board.getCell(pos) != CellType::Empty) {
            return false;
        }
    }
    
    // Проверка соседей
    return checkNeighbors(startPos, size, direction, board);
}

bool GameLogic::checkNeighbors(const QPoint& startPos, int size, ShipDirection direction, const GameBoard& board) {
    QVector<QPoint> neighbors = getNeighborCells(startPos, size, direction, board);
    
    for (const QPoint& neighbor : neighbors) {
        if (board.getCell(neighbor) == CellType::Ship) {
            return false;
        }
    }
    
    return true;
}

QVector<QPoint> GameLogic::getNeighborCells(const QPoint& startPos,
		int size, ShipDirection direction, const GameBoard& board) {
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
            
            if (board.isValidPosition(pos)) {
                neighbors.append(pos);
            }
        }
    }
    
    return neighbors;
}

void GameLogic::setEnemyCell(const QPoint& pos, const ShotResult result)
{
	CellType type;
	switch(result) {
		case ShotResult::Miss: type = CellType::Miss; break;
		case ShotResult::Hit: type = CellType::Hit; break;
		case ShotResult::Destroyed:
			type = CellType::Destroyed;
			--enemyAliveShipsCount;
			destrShip(pos);
			break;
		case ShotResult::Invalid: return; // Игнорируем некорректные выстрелы
	}
	m_enemyBoard.setCell(pos, type);
}

void GameLogic::destrShip(const QPoint& pos)
{
	// Bfs
	
	queue<pair<int, int>> q {};
	bool field[10][10] {false};	
	pair<int, int> curr_p {pos.y(), pos.x()};
	pair<int, int> shifts[] {{0,1}, {0,-1}, {1,0}, {-1,0}};
	int curry {pos.y()}, currx {pos.x()}, new_y, new_x;
	
	q.push(curr_p);
	field[curr_p.first][curr_p.second] = true;
	
	while(q.size() != 0) {

		curr_p = q.front();
		q.pop();
		m_enemyBoard.setCell(curr_p.first, curr_p.second, CellType::Destroyed);
		field[curry][currx] = true;

		for(pair<int, int> shift : shifts) {
			new_y = curr_p.first + shift.first;
			new_x = curr_p.second + shift.second;
			if(!m_enemyBoard.isValidPosition(new_y, new_x))
				continue;
			if(field[new_y][new_x] == true)
				continue;
			if(m_enemyBoard.getCell(new_y, new_x) != CellType::Hit)
				continue;
			q.push({new_y, new_x});
		}

	}
}

bool GameLogic::placeShip(const QPoint& startPos, int size, ShipDirection direction, BoardType b_type) {
    // Определяем, на какой доске размещать корабль
    GameBoard& board {getBoard(b_type)};

    if (!canPlaceShip(startPos, size, direction, board)) {
	qDebug() << "cant place ship";
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
        
	board.setCell(pos, CellType::Ship);
        ship.cells.append(pos);
    }
    
    m_ships.append(ship);
    return true;
}

bool GameLogic::autoPlaceShips(BoardType b_type) {
    reset();
    
    QRandomGenerator* rng = QRandomGenerator::global();
    int attempts = 0;
    const int maxAttempts = 1000;
    
    for (const auto& config : STANDART_SHIPS) {
        for (int shipIndex = 0; shipIndex < config.count; ++shipIndex) {
            bool placed = false;
            attempts = 0;
            
            while (!placed && attempts < maxAttempts) {
                QPoint pos = randomPosition();
                ShipDirection dir = randomDirection();
                
                if (placeShip(pos, config.size, dir, b_type)) {
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

ShotResult GameLogic::shoot(const QPoint& pos, BoardType b_type) {
    
    // Определяем, на какой доске отображать выстрел
    GameBoard& board {getBoard(b_type)};

    if (!board.isValidPosition(pos)) {
        return ShotResult::Invalid;
    }
    
    CellType cell = board.getCell(pos);
    
    // Если уже стреляли в эту клетку
    if (cell == CellType::Miss || cell == CellType::Hit || cell == CellType::Destroyed) {
        return ShotResult::Invalid;
    }
    
    // Промах
    if (cell == CellType::Empty) {
        board.setCell(pos, CellType::Miss);
        return ShotResult::Miss;
    }
    
    // Попадание
    if (cell == CellType::Ship) {
        board.setCell(pos, CellType::Hit);
        
        // Находим корабль и увеличиваем счетчик попаданий
        for (Ship& ship : m_ships) {
            if (ship.cells.contains(pos)) {
                ship.hits++;
                
                if (ship.isDestroyed()) {
                    // Помечаем все клетки корабля как уничтоженные
                    for (const QPoint& cellPos : ship.cells) {
                        board.setCell(cellPos, CellType::Destroyed);
                    }
                    return ShotResult::Destroyed;
                }
                
                return ShotResult::Hit;
            }
        }
    }
    
    return ShotResult::Miss;
}

bool GameLogic::areAllPlayerShipsDestroyed() const {
    for (const Ship& ship : m_ships) {
        if (!ship.isDestroyed()) {
            return false;
        }
    }
    return true;
}

bool GameLogic::areAllEnemyShipsDestroyed() const
{
	return ((enemyAliveShipsCount == 0) ? true : false);
}

int GameLogic::getPlayerAliveShipsCount() const {
    int count = 0;
    for (const Ship& ship : m_ships) {
        if (!ship.isDestroyed()) {
            count++;
        }
    }
    return count;
}

int GameLogic::getEnemyAliveShipsCount() const
{
	return enemyAliveShipsCount;	
}

void GameLogic::reset() {
    enemyAliveShipsCount = 10;
    m_playerBoard.clear();
    m_enemyBoard.clear();
    m_ships.clear();
}

void GameLogic::changePlayerMoveFlag(bool value)
{
	isPlayerMove = value;	
}

bool GameLogic::getPlayerMoveFlag()
{
	return isPlayerMove;
}

void GameLogic::setPlayerShips(vector<DraggedShip>& ships)
{
	m_playerBoard.clear();
	m_ships.clear();
	for(DraggedShip ship : ships) {
		placeShip(QPoint(ship.x, ship.y), ship.shipConf.size, ship.dir, BoardType::MainPlayerBoard);
		qDebug() << "placed ship";
	}
}
