#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include "GameLogic/GameBoard.h"
#include "GameLogic/GameTypes.h"
#include "GameLogic/GameLogic.h"
#include <stdexcept>
#include <QWidget>
#include <QPoint>
#include <QShortcut>
#include <vector>

using std::vector;
using std::pair;


class GameWidget : public QWidget {
    Q_OBJECT

public:
    GameWidget(QWidget* parent = nullptr,
	    BoardWidgetType type = BoardWidgetType::FightBoard);
    
    void setBoard(const GameBoard& board);
    void setShowShips(bool show) { m_showShips = show; update(); }
    bool isAllShipsPlanned();
    const GameBoard& getBoard() { return m_board; }
    vector<DraggedShip>& getDraggedShips();

    // Для BoardWType = BoardWidgetType::PlanningBoard
    void reset();

signals:
    void cellClicked(const QPoint& pos);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    BoardWidgetType BoardWType;
    void updateCellSize();
    QPoint cellOnBoard(int row, int col) const;
    QPoint mouseToCell(const QPoint& mousePos) const;
    QPoint findShipCell(int row, int col) const;
    QRect cellRect(int row, int col) const;
    
    GameBoard m_board;
    bool m_showShips;
    int m_cellSize;
    int m_offsetX;
    int m_offsetY;

    // Для BoardWType = BoardWidgetType::PlanningBoard
    bool isDragging {false};
    int ship_x, ship_y;
    int draggingShipIndex;
    int start_ship_x, start_ship_y;
    void placeDraggingShip();
    QPoint mouseClickedPos;
    pair<ShipConf, ShipDirection> dragShip;
    vector<pair<int, int>> startShipsPositions {};
    vector<DraggedShip> draggingHistory {};
    int historyIndex {-1};
    vector<ShipConf> planningShips {};

public slots:
    void rotateShip();
    void undoDragging();
    void redoDragging();
};

#endif // GAMEWIDGET_H
