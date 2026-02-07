#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QShortcut>


class Canvas : public QWidget {
	Q_OBJECT

	QPainterPath path;
	int pensize {3};
	int counter {1};

public:
	Canvas(QWidget* parent = nullptr) : QWidget(parent)
	{
		resize(128, 128);
		new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_K), this, SLOT(saveImage()));
	}

	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void paintEvent(QPaintEvent* event) override;

	~Canvas() = default;

public slots:
	void saveImage();

};


struct WCanvas {
	QWidget* wrapCanvas;
	QWidget* prewrapCanvas;
	Canvas* canvas;	
};


#endif
