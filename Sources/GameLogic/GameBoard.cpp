#include "GameLogic/GameBoard.h"
#include "GameLogic/GameTypes.h"

GameBoard::GameBoard() {
	m_board.resize(BOARD_SIZE);
	for (int i = 0; i < BOARD_SIZE; ++i) {
		m_board[i].resize(BOARD_SIZE);
		for(int j {0}; j < BOARD_SIZE; ++j) {
			m_board[i][j] = CellType::Empty;
		}
    	}
}

void GameBoard::operator=(const GameBoard& other)
{
	for (int i = 0; i < BOARD_SIZE; ++i) {
		m_board[i].resize(BOARD_SIZE);
		for(int j {0}; j < BOARD_SIZE; ++j) {
			m_board[i][j] = other.getCell(i, j);
		}
    	}
	qDebug() << "operator=()";
}

CellType GameBoard::getCell(int row, int col) const {
    if (!isValidPosition(row, col)) {
        return CellType::Empty;
    }
    return m_board[row][col];
}

CellType GameBoard::getCell(const QPoint& pos) const {
    return getCell(pos.y(), pos.x());
}

void GameBoard::setCell(int row, int col, CellType type) {
    if (isValidPosition(row, col)) {
        m_board[row][col] = type;
    }
}

void GameBoard::setCell(const QPoint& pos, CellType type) {
    setCell(pos.y(), pos.x(), type);
}

bool GameBoard::isValidPosition(int row, int col) const {
    return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
}

bool GameBoard::isValidPosition(const QPoint& pos) const {
    return isValidPosition(pos.y(), pos.x());
}

void GameBoard::clear() {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            m_board[i][j] = CellType::Empty;
        }
    }
}

