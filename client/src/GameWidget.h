#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include "GameBoard.h"
#include "../../common/GameTypes.h"
#include <QWidget>
#include <QPoint>

class GameWidget : public QWidget {
    Q_OBJECT

public:
    explicit GameWidget(QWidget* parent = nullptr);
    
    void setBoard(const GameBoard& board);
    void setShowShips(bool show) { m_showShips = show; update(); }

signals:
    void cellClicked(const QPoint& pos);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    void updateCellSize();
    QPoint mouseToCell(const QPoint& mousePos) const;
    QRect cellRect(int row, int col) const;
    
    GameBoard m_board;
    bool m_showShips;
    int m_cellSize;
    int m_offsetX;
    int m_offsetY;
};

#endif // GAMEWIDGET_H
