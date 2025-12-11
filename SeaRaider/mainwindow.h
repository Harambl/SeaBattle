#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QTcpSocket>

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

    void connectToServer(const QString &host, quint16 port);
    void disconnectFromServer();

    bool isConnected() const;

    QString getUser() const { return CurrentUser; }
    long long getPlayers() const { return players; }

signals:

    void connected(QString& UserName, int totalPlayers);
    void disconnected();
    void gameReady();
    void opponentDisconnected();
    void errorOccurred(const QString &error);
    void logMessage(const QString &message);

private slots:

    void on_start_button_clicked();
    void on_connect_button_clicked();
    void on_Log_button_clicked();

    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onError(QAbstractSocket::SocketError error);

private:
    Ui::MainWindow *ui;

    QTcpSocket *m_socket;

    QString Ip = "127.0.0.1";
    QString Port = "55555";

    long long players = 0;
    QString CurrentUser = "";
    bool hasStretch = false;

    void log(const QString &message, const QString &level = "INFO");
    QString formatLogMessage(const QString &level, const QString &message) const;
    void processServerMessage(const QString &message);

};
#endif // MAINWINDOW_H
