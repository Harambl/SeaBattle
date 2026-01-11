#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("SeaRaider");

    MainWindow MainW;
    MainW.setWindowTitle("SeaBattle");
    MainW.show();
    return app.exec();
}
