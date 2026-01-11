#include "GameLogic/GameWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QFont>

GameWidget::GameWidget(QWidget* parent, BoardWidgetType type_)
    : QWidget(parent)
    , BoardWType{type_}
    , m_showShips(true)
    , m_cellSize(29)
    , m_offsetX(30)
    , m_offsetY(30)
{
    setMinimumSize(350, 350);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    if(type_ == BoardWidgetType::PlanningBoard) {
	new QShortcut(QKeySequence(Qt::Key_R), this, SLOT(rotateShip()));
	new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z), this, SLOT(undoDragging()));
	new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Y), this, SLOT(redoDragging()));
	reset();
    }
}

void GameWidget::rotateShip()
{
	if(!isDragging || BoardWType != BoardWidgetType::PlanningBoard)	return;
	if(dragShip.second == ShipDirection::Horizontal)
		dragShip.second = ShipDirection::Vertical;
	else
		dragShip.second = ShipDirection::Horizontal;
	update();
}

void GameWidget::undoDragging()
{
	if(BoardWType != BoardWidgetType::PlanningBoard) return;
	if(historyIndex < 0) return;
		
	DraggedShip dShip {draggingHistory[historyIndex--]};
	for(int i{0}; i < dShip.shipConf.size; ++i) {
		if(dShip.dir == ShipDirection::Horizontal)
			m_board.setCell(dShip.y, dShip.x + i, CellType::Empty);
		else 
			m_board.setCell(dShip.y + i, dShip.x, CellType::Empty);
	}

	++(planningShips[dShip.indexInPlanningShips].count);
	update();
}

void GameWidget::redoDragging()
{
	if(BoardWType != BoardWidgetType::PlanningBoard) return;
	if(historyIndex < -1) return;

	++historyIndex;
	if(historyIndex == (int)draggingHistory.size()) {
		--historyIndex;
		return;
	}

	DraggedShip dShip {draggingHistory[historyIndex]};

	for(int i{0}; i < dShip.shipConf.size; ++i) {
		if(dShip.dir == ShipDirection::Horizontal)
			m_board.setCell(dShip.y, dShip.x + i, CellType::Ship);
		else 
			m_board.setCell(dShip.y + i, dShip.x, CellType::Ship);
	}

	--(planningShips[dShip.indexInPlanningShips].count);
	update();
}

void GameWidget::setBoard(const GameBoard& board) {
    m_board = board;
    qDebug() << "m_board = board";
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
    
    switch(BoardWType) {
	case BoardWidgetType::FightBoard:
	    return cellOnBoard(row, col);
	case BoardWidgetType::PlanningBoard:
	    return findShipCell(row, col);
    }
}

QPoint GameWidget::cellOnBoard(int row, int col) const
{
    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE) {
        return QPoint(col, row);
    }
    return QPoint(-1, -1);
}

QPoint GameWidget::findShipCell(int row, int col) const
{
	for(int i {0}; i < (int)planningShips.size(); ++i) {
		if(col >= 17 && col < 17 + planningShips[i].size && row == 2 * i + 1)
			return QPoint(col * m_cellSize, row * m_cellSize);
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
                    //bgColor = QColor(240, 248, 255); MaxColor
                    bgColor = QColor(220, 220, 220);
                    break;
                case CellType::Ship:
                    //bgColor = m_showShips ? QColor(70, 130, 180) : QColor(240, 248, 255); MaxColor
                    bgColor = m_showShips ? QColor(100, 100, 100) : QColor(220, 220, 220);
                    break;
                case CellType::Miss:
                    //bgColor = QColor(255, 255, 255); MaxColor
                    bgColor = QColor(180, 180, 180);
                    break;
                case CellType::Hit:
                    //bgColor = QColor(255, 200, 200); MaxColor
                    //bgColor = QColor(200, 100, 100);
                    bgColor = QColor(100, 100, 100);
                    break;
                case CellType::Destroyed:
                    //bgColor = QColor(200, 0, 0); MaxColor
                    bgColor = QColor(150, 50, 50);
                    break;
            }
            painter.fillRect(rect, bgColor);
            
            // Содержимое клетки
            painter.setPen(QPen(Qt::gray, 1));
            switch (type) {
                case CellType::Ship:
                    if (m_showShips) {
                        //painter.setPen(QPen(Qt::darkBlue, 2)); MaxColor
                        painter.setPen(QPen(Qt::black, 2));
                        painter.drawRect(rect.adjusted(2, 2, -2, -2));
                    }
                    break;
                case CellType::Miss:
                    //painter.setPen(QPen(Qt::gray, 2)); MaxColor
                    painter.setPen(QPen(Qt::black, 2));
                    painter.drawEllipse(rect.adjusted(5, 5, -5, -5));
                    break;
                case CellType::Hit:
                        painter.setPen(QPen(Qt::black, 2));
                        painter.drawRect(rect.adjusted(2, 2, -2, -2));
                case CellType::Destroyed:
                    //painter.setPen(QPen(type == CellType::Destroyed ? Qt::darkRed : Qt::red, 3));
                    painter.setPen(QPen(type == CellType::Destroyed ? Qt::red : QColor(200, 70, 70), 3));
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

    // Если доска используется для боя, завершаем отрисовку
    if(BoardWType != BoardWidgetType::PlanningBoard) return;

    // Дорисовываем модели кораблей сбоку справа

    QRect textRect_, rect_;
    
    for(size_t i {0}; i < planningShips.size(); ++i) {
	textRect_ = cellRect(2 * i + 1, 15);
    	painter.setPen(QPen(Qt::black, 2));
        painter.drawText(textRect_, Qt::AlignCenter, QString::number(planningShips[i].count));

	// Проверка на отсутствие таких кораблей
	if(planningShips[i].count == 0) continue;

	for(int j {0}; j < planningShips[i].size; ++j) {
	    rect_ = cellRect(2 * i + 1, 17 + j);
    	    painter.setPen(QPen(Qt::black, 2));
	    painter.fillRect(rect_, QColor(100, 100, 100));
            painter.drawRect(rect_.adjusted(2, 2, -2, -2));
            painter.setPen(QPen(Qt::gray, 1));
            painter.drawRect(rect_);
	}	

    }

    if(isDragging) {
	for(int i {0}; i < dragShip.first.size; ++i) {
	    if(dragShip.second == ShipDirection::Horizontal)
	    	rect_ = QRect(ship_x + i * m_cellSize, ship_y, m_cellSize, m_cellSize);
	    else
	    	rect_ = QRect(ship_x, ship_y + i * m_cellSize, m_cellSize, m_cellSize);
      	    painter.setPen(QPen(Qt::black, 2));
   	    painter.fillRect(rect_, QColor(100, 100, 100));
      	    painter.drawRect(rect_.adjusted(2, 2, -2, -2));
      	    painter.setPen(QPen(Qt::gray, 1));
      	    painter.drawRect(rect_);
	}
    }
}

void GameWidget::mousePressEvent(QMouseEvent* event) {
    switch(BoardWType) {

	case BoardWidgetType::FightBoard:    	

            if (event->button() == Qt::LeftButton) {
                QPoint cell = mouseToCell(event->pos());
                if (cell.x() >= 0 && cell.y() >= 0) {
                    emit cellClicked(cell);
                }
            }
            QWidget::mousePressEvent(event);
	    break;

	case BoardWidgetType::PlanningBoard:

	    if (event->button() == Qt::LeftButton) {
		QPoint cell {mouseToCell(event->pos())};
		if(cell.x() == -1 && cell.y() == -1) return;

		mouseClickedPos = event->pos();

		if((cell.y() - m_cellSize) / (2 * m_cellSize) >= (int)planningShips.size())
			throw std::runtime_error("index is out of range");
		if((cell.y() - m_cellSize) / (2 * m_cellSize) < 0)
			throw std::runtime_error("index is negative");

		draggingShipIndex = (cell.y() - m_cellSize) / (2 * m_cellSize);

		// Проверка на наличие таких кораблей
		if(planningShips[draggingShipIndex].count <= 0) return;
		
		// Взятие корабля
		dragShip = {planningShips[draggingShipIndex], ShipDirection::Horizontal};

		start_ship_x = startShipsPositions[draggingShipIndex].first;
		start_ship_y = startShipsPositions[draggingShipIndex].second;
		ship_x = start_ship_x;
		ship_y = start_ship_y;

		isDragging = true;
	    }
            QWidget::mousePressEvent(event);
	    break;
    }
}

void GameWidget::mouseMoveEvent(QMouseEvent* event)
{
	if(BoardWType != BoardWidgetType::PlanningBoard) return;
	if(isDragging == false) return;

	QPoint mousePos {event->pos()};
	ship_x = mousePos.x() - mouseClickedPos.x() + start_ship_x;
	ship_y = mousePos.y() - mouseClickedPos.y() + start_ship_y;
	update();
}

void GameWidget::mouseReleaseEvent(QMouseEvent* event)
{
	if(BoardWType != BoardWidgetType::PlanningBoard) return;
	placeDraggingShip();
	isDragging = false;	
	update();
}

void GameWidget::placeDraggingShip()
{
	int _cell_x, _cell_y;
    	int _col = (ship_x + m_cellSize / 2 - m_offsetX) / m_cellSize;
    	int _row = (ship_y + m_cellSize / 2 -  m_offsetY) / m_cellSize;
	QPoint startCell {cellOnBoard(_row, _col)};
	_cell_x = startCell.x();
	_cell_y = startCell.y();

	// Проверка валидности координат
	for(int i {0}; i < dragShip.first.size; ++i) {
		if(dragShip.second == ShipDirection::Horizontal) {
			if(!m_board.isValidPosition(_cell_x + i, _cell_y)) return;
		} else  {	
			if(!m_board.isValidPosition(_cell_x, _cell_y + i)) return;
		}
	}

	// Проверка на пустоту клеток размещения и соседних клеток
	if(!(GameLogic::canPlaceShip(startCell, dragShip.first.size, dragShip.second, m_board)))
		return;

	// Размещение корабля
	for(int i {0}; i < dragShip.first.size; ++i) {
		if(dragShip.second == ShipDirection::Horizontal)
			m_board.setCell(_cell_y, _cell_x + i, CellType::Ship);
		else
			m_board.setCell(_cell_y + i, _cell_x, CellType::Ship);
	}

	// Уменьшение количества
	--planningShips[draggingShipIndex].count;

	// Добавление перемещения в историю
	DraggedShip dShip {_cell_x, _cell_y, draggingShipIndex, dragShip.first, dragShip.second};

	int _histSize {(int)draggingHistory.size()};
	if(historyIndex != -1)
		for(int i {0}; i < _histSize - historyIndex - 1; ++i) {
			draggingHistory.pop_back();
		}
	else
		draggingHistory.clear();

	draggingHistory.push_back({dShip});
	historyIndex = draggingHistory.size() - 1;

	update();
}

bool GameWidget::isAllShipsPlanned()
{
	for(ShipConf shipConf : planningShips) {
		if(shipConf.count != 0) return false;	
	} return true;
}

void GameWidget::reset()
{
	isDragging = false;
		
	m_board.clear();

	planningShips.clear();
	for(ShipConf shipConf : STANDART_SHIPS)
	    planningShips.push_back(shipConf);

	int _start_ship_x, _start_ship_y;
	startShipsPositions.clear();
	for(size_t i {0}; i < planningShips.size(); ++i) {
	    _start_ship_x = 17 * m_cellSize + m_offsetX;
	    _start_ship_y = (2 * i + 1) * m_cellSize + m_offsetY;
	    startShipsPositions.push_back({_start_ship_x, _start_ship_y});
	}

	draggingHistory.clear();
	historyIndex = -1;

	update();
}

vector<DraggedShip>& GameWidget::getDraggedShips()
{
	return draggingHistory;	
}
