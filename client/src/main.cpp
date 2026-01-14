#include "SeaBattleClient.h"
#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  app.setApplicationName("SeaBattle Client");

  QWidget window;
  window.setWindowTitle("Морской бой - Клиент");
  window.resize(700, 600);

  QVBoxLayout *mainLayout = new QVBoxLayout(&window);

  QLabel *titleLabel = new QLabel("Морской бой - Клиент");
  titleLabel->setStyleSheet(
      "font-size: 18px; font-weight: bold; padding: 10px;");
  mainLayout->addWidget(titleLabel);

  QWidget *connectPanel = new QWidget;
  QHBoxLayout *connectLayout = new QHBoxLayout(connectPanel);

  QLabel *hostLabel = new QLabel("Хост:");
  QLineEdit *hostEdit = new QLineEdit("127.0.0.1");
  QLabel *portLabel = new QLabel("Порт:");
  QLineEdit *portEdit = new QLineEdit("12345");
  QPushButton *connectBtn = new QPushButton("Подключиться");
  QPushButton *disconnectBtn = new QPushButton("Отключиться");
  disconnectBtn->setEnabled(false);

  connectLayout->addWidget(hostLabel);
  connectLayout->addWidget(hostEdit);
  connectLayout->addWidget(portLabel);
  connectLayout->addWidget(portEdit);
  connectLayout->addWidget(connectBtn);
  connectLayout->addWidget(disconnectBtn);
  connectLayout->addStretch();

  mainLayout->addWidget(connectPanel);

  QLabel *statusLabel = new QLabel("Статус: Не подключен");
  statusLabel->setStyleSheet("padding: 5px; background-color: #f0f0f0;");
  mainLayout->addWidget(statusLabel);

  QLabel *logLabel = new QLabel("Логи:");
  mainLayout->addWidget(logLabel);

  QTextEdit *logArea = new QTextEdit;
  logArea->setReadOnly(true);
  logArea->setFont(QFont("Courier", 9));
  mainLayout->addWidget(logArea);

  SeaBattleClient client;

  QObject::connect(connectBtn, &QPushButton::clicked, [&]() {
    bool ok;
    quint16 port = portEdit->text().toUShort(&ok);
    if (!ok) {
      QMessageBox::warning(&window, "Ошибка", "Неверный порт!");
      return;
    }

    hostEdit->setEnabled(false);
    portEdit->setEnabled(false);
    connectBtn->setEnabled(false);
    statusLabel->setText("Статус: Подключение...");
    statusLabel->setStyleSheet("padding: 5px; background-color: #fffacd;");

    client.connectToServer(hostEdit->text(), port);
  });

  QObject::connect(disconnectBtn, &QPushButton::clicked,
                   [&]() { client.disconnectFromServer(); });

  QObject::connect(
      &client, &SeaBattleClient::connected,
      [&](int playerId, int totalPlayers) {
        statusLabel->setText(QString("Статус: Подключен (Игрок #%1, Всего: %2)")
                                 .arg(playerId)
                                 .arg(totalPlayers));
        statusLabel->setStyleSheet("padding: 5px; background-color: #90ee90;");

        hostEdit->setEnabled(false);
        portEdit->setEnabled(false);
        connectBtn->setEnabled(false);
        disconnectBtn->setEnabled(true);
      });

  QObject::connect(&client, &SeaBattleClient::disconnected, [&]() {
    statusLabel->setText("Статус: Не подключен");
    statusLabel->setStyleSheet("padding: 5px; background-color: #f0f0f0;");

    hostEdit->setEnabled(true);
    portEdit->setEnabled(true);
    connectBtn->setEnabled(true);
    disconnectBtn->setEnabled(false);
  });

  QObject::connect(&client, &SeaBattleClient::errorOccurred,
                   [&](const QString &error) {
                     QMessageBox::critical(&window, "Ошибка", error);
                     hostEdit->setEnabled(true);
                     portEdit->setEnabled(true);
                     connectBtn->setEnabled(true);
                     disconnectBtn->setEnabled(false);
                   });

  QObject::connect(&client, &SeaBattleClient::logMessage,
                   [logArea](const QString &msg) {
                     logArea->append(msg);
                     // Автопрокрутка вниз
                     QTextCursor cursor = logArea->textCursor();
                     cursor.movePosition(QTextCursor::End);
                     logArea->setTextCursor(cursor);
                   });

  QObject::connect(&client, &SeaBattleClient::gameReady, [logArea]() {
    logArea->append("[СИСТЕМА] Оба игрока подключены! Игра может начаться.");
  });

  QObject::connect(
      &client, &SeaBattleClient::opponentDisconnected,
      [logArea]() { logArea->append("[СИСТЕМА] Противник отключился"); });

  window.show();
  return app.exec();
}
