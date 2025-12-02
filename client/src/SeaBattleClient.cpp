#include "SeaBattleClient.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextStream>

SeaBattleClient::SeaBattleClient(QObject *parent)
    : QObject(parent), m_socket(new QTcpSocket(this)), m_playerId(0),
      m_totalPlayers(0) {
  connect(m_socket, &QTcpSocket::connected, this,
          &SeaBattleClient::onConnected);
  connect(m_socket, &QTcpSocket::disconnected, this,
          &SeaBattleClient::onDisconnected);
  connect(m_socket, &QTcpSocket::readyRead, this,
          &SeaBattleClient::onReadyRead);
  connect(m_socket, &QAbstractSocket::errorOccurred, this,
          &SeaBattleClient::onError);

  log("Клиент инициализирован");
}

SeaBattleClient::~SeaBattleClient() { disconnectFromServer(); }

void SeaBattleClient::connectToServer(const QString &host, quint16 port) {
  log(QString("Подключение к серверу %1:%2...").arg(host).arg(port), "INFO");
  m_socket->connectToHost(host, port);
}

void SeaBattleClient::disconnectFromServer() {
  if (m_socket->state() == QAbstractSocket::ConnectedState) {
    log("Отключение от сервера...", "INFO");
    m_socket->disconnectFromHost();
  }
}

bool SeaBattleClient::isConnected() const {
  return m_socket->state() == QAbstractSocket::ConnectedState;
}

void SeaBattleClient::onConnected() { log("Подключено к серверу!", "INFO"); }

void SeaBattleClient::onDisconnected() {
  log("Отключено от сервера", "INFO");
  m_playerId = 0;
  m_totalPlayers = 0;
  emit disconnected();
}

void SeaBattleClient::onReadyRead() {
  QByteArray data = m_socket->readAll();
  QString message = QString::fromUtf8(data).trimmed();

  if (message.isEmpty()) {
    return;
  }

  log(QString("Получено от сервера: %1").arg(message), "RECV");
  processServerMessage(message);
}

void SeaBattleClient::onError(QAbstractSocket::SocketError error) {
  QString errorString = m_socket->errorString();
  log(QString("ОШИБКА подключения: %1").arg(errorString), "ERROR");
  emit errorOccurred(errorString);
}

void SeaBattleClient::processServerMessage(const QString &message) {
  if (message.startsWith("CONNECTED|")) {
    // Формат: CONNECTED|playerId|totalPlayers
    QStringList parts = message.split("|");
    if (parts.size() >= 3) {
      m_playerId = parts[1].toInt();
      m_totalPlayers = parts[2].toInt();
      log(QString("Вы игрок #%1 из %2").arg(m_playerId).arg(m_totalPlayers),
          "INFO");
      emit connected(m_playerId, m_totalPlayers);
    }
  } else if (message == "SERVER_FULL") {
    log("Сервер полон! Максимум 2 игрока.", "ERROR");
    emit errorOccurred("Сервер полон");
  } else if (message == "GAME_READY|2") {
    log("Оба игрока подключены! Игра может начаться.", "INFO");
    emit gameReady();
  } else if (message == "OPPONENT_DISCONNECTED") {
    log("Противник отключился", "WARNING");
    emit opponentDisconnected();
  }
}

void SeaBattleClient::log(const QString &message, const QString &level) {
  QString formattedMsg = formatLogMessage(level, message);

  qDebug() << formattedMsg;

  QDir logDir("logs");
  if (!logDir.exists()) {
    logDir.mkpath(".");
  }

  QString logFileName =
      QString("logs/client_%1.log")
          .arg(m_playerId > 0 ? QString::number(m_playerId) : "unknown");
  QFile logFile(logFileName);
  if (logFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
    QTextStream stream(&logFile);
    stream << formattedMsg << "\n";
    logFile.close();
  }

  emit logMessage(formattedMsg);
}

QString SeaBattleClient::formatLogMessage(const QString &level,
                                          const QString &message) const {
  QString timestamp =
      QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
  return QString("[%1] [%2] %3").arg(timestamp).arg(level).arg(message);
}
