#include "GameWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QFont>

GameWidget::GameWidget(QWidget* parent)
    : QWidget(parent)
    , m_showShips(true)
    , m_cellSize(30)
    , m_offsetX(30)
    , m_offsetY(30)
{
    setMinimumSize(350, 350);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void GameWidget::setBoard(const GameBoard& board) {
    m_board = board;
    update();
}

void GameWidget::resizeEvent(QResizeEvent* event) {
    Q_UNUSED(event);
    updateCellSize();
    update();
}

void GameWidget::updateCellSize() {
    int availableWidth = width() - m_offsetX * 2;
    int availableHeight = height() - m_offsetY * 2;
    
    int cellSizeByWidth = availableWidth / BOARD_SIZE;
    int cellSizeByHeight = availableHeight / BOARD_SIZE;
    
    m_cellSize = qMin(cellSizeByWidth, cellSizeByHeight);
    m_cellSize = qMax(m_cellSize, 20);
}

QPoint GameWidget::mouseToCell(const QPoint& mousePos) const {
    int col = (mousePos.x() - m_offsetX) / m_cellSize;
    int row = (mousePos.y() - m_offsetY) / m_cellSize;
    
    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE) {
        return QPoint(col, row);
    }
    return QPoint(-1, -1);
}

QRect GameWidget::cellRect(int row, int col) const {
    int x = m_offsetX + col * m_cellSize;
    int y = m_offsetY + row * m_cellSize;
    return QRect(x, y, m_cellSize, m_cellSize);
}

void GameWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Фон
    painter.fillRect(rect(), QColor(245, 245, 245));
    
    // Рисуем клетки
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            QRect rect = cellRect(row, col);
            CellType type = m_board.getCell(row, col);
            
            // Фон клетки
            QColor bgColor;
            switch (type) {
                case CellType::Empty:
                    bgColor = QColor(240, 248, 255);
                    break;
                case CellType::Ship:
                    bgColor = m_showShips ? QColor(70, 130, 180) : QColor(240, 248, 255);
                    break;
                case CellType::Miss:
                    bgColor = QColor(255, 255, 255);
                    break;
                case CellType::Hit:
                    bgColor = QColor(255, 200, 200);
                    break;
                case CellType::Destroyed:
                    bgColor = QColor(200, 0, 0);
                    break;
            }
            painter.fillRect(rect, bgColor);
            
            // Содержимое клетки
            painter.setPen(QPen(Qt::gray, 1));
            switch (type) {
                case CellType::Ship:
                    if (m_showShips) {
                        painter.setPen(QPen(Qt::darkBlue, 2));
                        painter.drawRect(rect.adjusted(2, 2, -2, -2));
                    }
                    break;
                case CellType::Miss:
                    painter.setPen(QPen(Qt::gray, 2));
                    painter.drawEllipse(rect.adjusted(5, 5, -5, -5));
                    break;
                case CellType::Hit:
                case CellType::Destroyed:
                    painter.setPen(QPen(type == CellType::Destroyed ? Qt::darkRed : Qt::red, 3));
                    painter.drawLine(rect.topLeft() + QPoint(5, 5), rect.bottomRight() + QPoint(-5, -5));
                    painter.drawLine(rect.topRight() + QPoint(-5, 5), rect.bottomLeft() + QPoint(5, -5));
                    break;
                default:
                    break;
            }
            
            // Рамка
            painter.setPen(QPen(Qt::gray, 1));
            painter.drawRect(rect);
        }
    }
    
    // Подписи координат
    painter.setPen(Qt::black);
    QFont font = painter.font();
    font.setPointSize(10);
    font.setBold(true);
    painter.setFont(font);
    
    // Буквы сверху
    for (int col = 0; col < BOARD_SIZE; ++col) {
        QRect rect = cellRect(-1, col);
        painter.drawText(rect, Qt::AlignCenter, QString(QChar('A' + col)));
    }
    
    // Цифры слева
    for (int row = 0; row < BOARD_SIZE; ++row) {
        QRect rect = cellRect(row, -1);
        painter.drawText(rect, Qt::AlignCenter, QString::number(row + 1));
    }
}

void GameWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        QPoint cell = mouseToCell(event->pos());
        if (cell.x() >= 0 && cell.y() >= 0) {
            emit cellClicked(cell);
        }
    }
    QWidget::mousePressEvent(event);
}
