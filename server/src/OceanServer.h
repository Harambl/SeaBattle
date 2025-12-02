#pragma once
#ifndef OCEANSERVER_H
#define OCEANSERVER_H

#include <QList>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QDateTime>

class OceanServer : public QTcpServer {
  Q_OBJECT

public:
  explicit OceanServer(QObject *parent = nullptr);
  ~OceanServer();
  
  bool startServer(const QString &ip, quint16 port);
  void stop();
  
  int getConnectedClientsCount() const { return m_clients.size(); }
  bool isFull() const { return m_clients.size() >= 2; }

signals:
  void clientConnected(int clientId, const QString &address);
  void clientDisconnected(int clientId);
  void logMessage(const QString &message);

private slots:
  void onNewConnection();
  void onClientDisconnected();

private:
  struct ClientInfo {
    QTcpSocket *socket;
    int id;
    QString address;
    QDateTime connectedAt;
  };
  
  QList<ClientInfo*> m_clients;
  int m_nextClientId;
  
  void log(const QString &message, const QString &level = "INFO");
  QString formatLogMessage(const QString &level, const QString &message) const;
  ClientInfo* findClient(QTcpSocket *socket) const;
  void broadcastMessage(const QString &message, QTcpSocket *exclude = nullptr);
};

#endif // OCEANSERVER_H
