#pragma once
#ifndef SEABATTLECLIENT_H
#define SEABATTLECLIENT_H

#include <QDateTime>
#include <QObject>
#include <QString>
#include <QTcpSocket>

class SeaBattleClient : public QObject {
  Q_OBJECT

public:
  explicit SeaBattleClient(QObject *parent = nullptr);
  ~SeaBattleClient();

  void connectToServer(const QString &host, quint16 port);
  void disconnectFromServer();

  bool isConnected() const;
  int getPlayerId() const { return m_playerId; }
  int getTotalPlayers() const { return m_totalPlayers; }

signals:
  void connected(int playerId, int totalPlayers);
  void disconnected();
  void gameReady();
  void opponentDisconnected();
  void errorOccurred(const QString &error);
  void logMessage(const QString &message);

private slots:
  void onConnected();
  void onDisconnected();
  void onReadyRead();
  void onError(QAbstractSocket::SocketError error);

private:
  QTcpSocket *m_socket;
  int m_playerId;
  int m_totalPlayers;

  void log(const QString &message, const QString &level = "INFO");
  QString formatLogMessage(const QString &level, const QString &message) const;
  void processServerMessage(const QString &message);
};

#endif // SEABATTLECLIENT_H
