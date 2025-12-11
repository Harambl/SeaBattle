#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("SeaRaider");
    MainWindow w;
    w.setWindowTitle("SeaBattle");
    w.show();
    return a.exec();
}
