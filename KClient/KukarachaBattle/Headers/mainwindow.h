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
#include <QThread>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <assert.h>
#include <string.h>
#include <functional>
#include "User.h"
#include "GameUI.h"
#include "GameLogic/GameTypes.h"
#include "FightButton.h"
#include "ShipsPlanningUI.h"
#include "./ui_mainwindow.h"

// Codes from server
#define WRONG_AUTH_CODE	 		quint8(31)
#define OK_AUTH_CODE	 		quint8(17)
#define WRONG_LOGIN_CODE 		quint8(15)
#define OK_LOGIN_CODE	 		quint8(16)
#define USERNAME_CODE	 		quint8(14)
#define FIGHT_INVINTATION_CODE 		quint8(13)
#define START_FIGHT_CODE		quint8(12)
#define REJECTED_FIGHT_CODE		quint8(11)
#define USER_IN_LOBBY_DISC_CODE		quint8(10)
#define POINT_POSITION_SERVER_CODE 	quint8(18)
#define SHOT_RESULT_SERVER_CODE		quint8(19)
#define USER_FIGHTS_CODE		quint8(20)
#define USER_FINISHED_FIGHT_CODE	quint8(21)
#define START_SHIPS_PLANNING		quint8(22)
#define ENEMY_IS_READY_TO_FIGHT_CODE	quint8(23)

// Mixed codes
#define OK_CODE				quint8(111)

// Codes to server
#define LOGIN_CODE			quint16(65535)
#define AUTH_CODE			quint16(65533)
#define FIGHT_REQUEST_CODE		quint16(65534)
#define ACCEPTED_INVINTATION_CODE	quint16(65532)
#define REJECTED_INVINTATION_CODE	quint16(65531)
#define POINT_POSITION_CODE 		quint16(65530)
#define SHOT_RESULT_CODE		quint16(65529)
#define UPDATED_USER_INFO_CODE		quint16(65528)
#define USER_IN_LOBBY_CODE		quint16(65527)
#define READY_TO_FIGHT_CODE		quint16(65526)
#define PRESSED_FIGHT_BUTTON_CODE	quint16(65525)

QT_BEGIN_NAMESPACE
namespace Ui {
	class MainWindow;
}
QT_END_NAMESPACE

enum State { UNCONNECTED, CONNECTED, AUTHORIZED, LOGINED, FIGHTING, IN_LOBBY };
enum Page { MENU, CONNECT, LOGIN_AUTH, LOBBY, SHIPS_PLANNING, GAME, GAME_RESULTS };
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
	vector<pair<QString, State>> Players;
	vector<FightButton*> fButtons;
	User CurrentUser;
	QString enemy;
	bool hasStretch {false};

	GameUI* mainGameUI {nullptr};
	ShipsPlanningUI* shipsPlanningUI {nullptr};

	template <class... Ts>
	void sendDataToServer(quint16 code, Ts&&... Args);
	
	void changePage(int ind);
	void changeUserMatchesInfo(int winDelta, int loseDelta, int mCountDelta);
	void log(const QString &message, const QString &level = "INFO");
	QString formatLogMessage(const QString &level, const QString &message) const;

	void sendUserInfo(MessageType type, QString name, QString passHash);
	void sendAcceptedInvintation(QString fInvoker, QString fReciever, bool accepted);
	void sendClickedCellPos(QPoint pos);
	void sendShotResult(ShotResult result);
	void sendNameAndCodeToServer(quint16 code);
	void sendToServerMatchesInfo();

	void removeDisconnectedUser(QString disconnectedUsername);
	void updateVisualUserInfo();

	void changeFButton(QString figthsUserName, bool disable);
	void clearListLobbyUsers();
	void changePlayerState(QString playerName, State state);
	void createPlayer(QString userName, State state, bool pushInPlayers);
	void invokeFightInvintation(QString fInvoker);
	void resetGameUI();
	void endFight(FightEnd_t fightEnd);
	void startShipsPlanning();
	void startFight();

	static QString _hashed(QString password);

	void processServerCode(quint8 code);
	void processShotResult(ShotResult result);

public:
	MainWindow(QWidget *parent = nullptr);

	void connectToServer(const QString &host, quint16 port);
	void disconnectFromServer();

	bool isConnected() const;

	User getUser() const { return CurrentUser; }
	long long getPlayers() const { return players; }

	bool is_valid_port(quint16 port);
	bool is_valid_ip(QString ip);

	~MainWindow();

signals:
	void connected(QString& UserName, int totalPlayers);
	void disconnected();
	void gameReady();
	void opponentDisconnected();
	void errorOccurred(const QString &error);
	void logMessage(const QString &message);

private slots:
	void on_start_button_clicked();				// Page::Menu
	void on_Exit_button_clicked();				// Page::Menu
	void on_connect_button_clicked();			// Page::CONNECT
	void on_Log_button_clicked();				// Page::LOGIN_AUTH
	void on_Reg_button_clicked();				// Page::LOGIN_AUTH

	void onConnected();
	void onDisconnected();
	void onReadyRead();
	void onError(QAbstractSocket::SocketError error);
	void onSignalClickedPos();				// Page::GAME
	void sendFightRequest();				// Page::GAME
								
	void onSignalWin();					// Page::GAME
	void onSignalLose();					// Page::GAME
	void onSignalReadyToFight();				// Page::SHIPS_PLANNING
	void onSignalStartFight();				// Page::SHIPS_PLANNING
};


#endif // MAINWINDOW_H
