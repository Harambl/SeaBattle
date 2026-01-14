#include "OceanServer.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextStream>

OceanServer::OceanServer(QObject *parent)
    : QTcpServer(parent), m_nextClientId(1) {
  connect(this, &QTcpServer::newConnection, this,
          &OceanServer::onNewConnection);
  log("Сервер инициализирован");
}

OceanServer::~OceanServer() { stop(); }

bool OceanServer::startServer(const QString &ip, quint16 port) {
  QHostAddress address(ip);

  if (!listen(address, port)) {
    log(QString("ОШИБКА: Не удалось запустить сервер на %1:%2 - %3")
            .arg(ip)
            .arg(port)
            .arg(errorString()),
        "ERROR");
    return false;
  }

  log(QString("Сервер запущен на %1:%2").arg(ip).arg(port), "INFO");
  log(QString("Ожидание подключения игроков (максимум 2)..."), "INFO");
  return true;
}

void OceanServer::stop() {
  log("Остановка сервера...", "INFO");

  for (auto *client : m_clients) {
    if (client->socket) {
      client->socket->disconnectFromHost();
      client->socket->deleteLater();
    }
    delete client;
  }
  m_clients.clear();

  close();
  log("Сервер остановлен", "INFO");
}

void OceanServer::onNewConnection() {
  QTcpSocket *clientSocket = nextPendingConnection();
  if (!clientSocket) {
    log("ОШИБКА: Получен невалидный сокет", "ERROR");
    return;
  }

  if (isFull()) {
    log(QString("Попытка подключения отклонена: сервер полон (2/2 игроков)")
            .arg(clientSocket->peerAddress().toString()),
        "WARNING");
    clientSocket->write("SERVER_FULL\n");
    clientSocket->disconnectFromHost();
    clientSocket->deleteLater();
    return;
  }

  ClientInfo *clientInfo = new ClientInfo;
  clientInfo->socket = clientSocket;
  clientInfo->id = m_nextClientId++;
  clientInfo->address = QString("%1:%2")
                            .arg(clientSocket->peerAddress().toString())
                            .arg(clientSocket->peerPort());
  clientInfo->connectedAt = QDateTime::currentDateTime();

  m_clients.append(clientInfo);

  log(QString("Игрок #%1 подключен: %2")
          .arg(clientInfo->id)
          .arg(clientInfo->address),
      "INFO");

  connect(clientSocket, &QTcpSocket::disconnected, this,
          &OceanServer::onClientDisconnected);

  QString welcomeMsg =
      QString("CONNECTED|%1|%2\n").arg(clientInfo->id).arg(m_clients.size());
  clientSocket->write(welcomeMsg.toUtf8());

  emit clientConnected(clientInfo->id, clientInfo->address);

  if (m_clients.size() == 2) {
    log("Оба игрока подключены! Игра может начаться.", "INFO");
    broadcastMessage("GAME_READY|2\n");
  }
}

void OceanServer::onClientDisconnected() {
  QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
  if (!clientSocket) {
    return;
  }

  ClientInfo *clientInfo = findClient(clientSocket);
  if (clientInfo) {
    int clientId = clientInfo->id;
    QString address = clientInfo->address;

    log(QString("Игрок #%1 отключен: %2").arg(clientId).arg(address), "INFO");

    m_clients.removeAll(clientInfo);
    delete clientInfo;

    emit clientDisconnected(clientId);

    // Уведомляем другого игрока
    if (m_clients.size() == 1) {
      broadcastMessage("OPPONENT_DISCONNECTED\n");
    }
  }

  clientSocket->deleteLater();
}

void OceanServer::log(const QString &message, const QString &level) {
  QString formattedMsg = formatLogMessage(level, message);

  qDebug() << formattedMsg;

  QDir logDir("logs");
  if (!logDir.exists()) {
    logDir.mkpath(".");
  }

  QFile logFile("logs/server.log");
  if (logFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
    QTextStream stream(&logFile);
    stream << formattedMsg << "\n";
    logFile.close();
  }

  emit logMessage(formattedMsg);
}

QString OceanServer::formatLogMessage(const QString &level,
                                      const QString &message) const {
  QString timestamp =
      QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
  return QString("[%1] [%2] %3").arg(timestamp).arg(level).arg(message);
}

OceanServer::ClientInfo *OceanServer::findClient(QTcpSocket *socket) const {
  for (auto *client : m_clients) {
    if (client->socket == socket) {
      return client;
    }
  }
  return nullptr;
}

void OceanServer::broadcastMessage(const QString &message,
                                   QTcpSocket *exclude) {
  for (auto *client : m_clients) {
    if (client->socket && client->socket != exclude) {
      client->socket->write(message.toUtf8());
    }
  }
}
