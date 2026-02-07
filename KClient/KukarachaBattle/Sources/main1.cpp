#include "Canvas.h"
#include <QApplication>
#include <QMainWindow>

int main1(int argc, char** argv)
{
	QApplication app(argc, argv);

	QMainWindow MW;
	MW.resize(128, 128);

	Canvas CV(&MW);

	MW.show();
	app.exec();

	return 0;
}
