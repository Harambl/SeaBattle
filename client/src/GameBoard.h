#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "../../common/GameTypes.h"
#include <QVector>

class GameBoard {
public:
    GameBoard();
    
    // Получить тип клетки
    CellType getCell(int row, int col) const;
    CellType getCell(const QPoint& pos) const;
    
    // Установить тип клетки
    void setCell(int row, int col, CellType type);
    void setCell(const QPoint& pos, CellType type);
    
    // Проверка валидности координат
    bool isValidPosition(int row, int col) const;
    bool isValidPosition(const QPoint& pos) const;
    
    // Очистить поле
    void clear();
    
    // Получить размер поля
    int size() const { return BOARD_SIZE; }
    
private:
    QVector<QVector<CellType>> m_board;
};

#endif // GAMEBOARD_H

