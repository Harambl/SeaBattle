#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void createPlayer(QVBoxLayout* Layout);

private slots:

    void on_start_button_clicked();
    void on_connect_button_clicked();
    void on_Log_button_clicked();

private:
    Ui::MainWindow *ui;

    QString Ip = "127.0.0.1";
    QString Port = "55555";

    long long players = 0;
    QString CurrentUser = "";
    bool hasStretch = false;


};
#endif // MAINWINDOW_H
