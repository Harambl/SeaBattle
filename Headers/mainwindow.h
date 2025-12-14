#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QTcpSocket>
#include <QInputDialog>
#include <QApplication>
#include <QDataStream>
#include <QString>
#include <QDateTime>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <string.h>
#include <functional>
#include "User.h"
#include "./ui_mainwindow.h"

#define WRONG_AUTH_CODE	 		quint8(31)
#define OK_AUTH_CODE	 		quint8(17)
#define WRONG_LOGIN_CODE 		quint8(15)
#define OK_LOGIN_CODE	 		quint8(16)
#define USERNAME_CODE	 		quint8(14)
#define FIGHT_INVINTATION_CODE 		quint8(13)
#define START_FIGHT_CODE		quint8(12)
#define REJECTED_FIGHT_CODE		quint8(11)

#define OK_CODE				quint8(111)

#define LOGIN_CODE			quint16(65535)
#define AUTH_CODE			quint16(65533)
#define FIGHT_REQUEST_CODE		quint16(65534)
#define ACCEPTED_INVINTATION_CODE	quint16(65532)
#define REJECTED_INVINTATION_CODE	quint16(65532)

QT_BEGIN_NAMESPACE
namespace Ui {
	class MainWindow;
}
QT_END_NAMESPACE

enum State { UNCONNECTED, CONNECTED, AUTHORIZED, LOGNIED };
enum Page { MENU, CONNECT, LOGIN_AUTH, LOBBY, SHIPS_PLANNING };
enum MessageType { CODE, AUTH, LOGIN };

class MainWindow : public QMainWindow {
	Q_OBJECT

	Ui::MainWindow *ui;
	
	State state {State::UNCONNECTED};
	QTcpSocket *m_socket;
	QByteArray data;
	quint16 dataSize {0};
	
	QString Ip {"127.0.0.1"};
	QString Port {"4242"};
	QVBoxLayout* playersLayout;
	
	long long players {0};
	User CurrentUser;
	QString enemy;
	bool hasStretch {false};
	
	void changePage(int ind);
	void log(const QString &message, const QString &level = "INFO");
	QString formatLogMessage(const QString &level, const QString &message) const;
	void sendUserInfo(MessageType type, QString name, QString passHash);
	void sendFightRequest(QString userName);
	void sendAcceptedInvintation(QString fInvoker, QString fReciever, bool accepted);
	void updateVisualUserInfo();

	void createPlayer(QString userName);
	void invokeFightInvintation(QString fInvoker);

	static QString _hashed(QString password);

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	void processServerCode(quint8 code);


	void connectToServer(const QString &host, quint16 port);
	void disconnectFromServer();

	bool isConnected() const;

	User getUser() const { return CurrentUser; }
	long long getPlayers() const { return players; }

	bool is_valid_port(quint16 port);
	bool is_valid_ip(QString ip);

signals:
	void connected(QString& UserName, int totalPlayers);
	void disconnected();
	void gameReady();
	void opponentDisconnected();
	void errorOccurred(const QString &error);
	void logMessage(const QString &message);

private slots:
	void on_start_button_clicked();			// Page::Menu
	void on_Exit_button_clicked();			// Page::Menu
	void on_connect_button_clicked();		// Page::CONNECT
	void on_Log_button_clicked();			// Page::LOGIN_AUTH
	void on_Reg_button_clicked();			// Page::LOGIN_AUTH

	void onConnected();
	void onDisconnected();
	void onReadyRead();
	void onError(QAbstractSocket::SocketError error);
};


#endif // MAINWINDOW_H
