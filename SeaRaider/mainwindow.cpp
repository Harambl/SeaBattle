#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QFile>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), m_socket(new QTcpSocket(this)), CurrentUser(""),
    players(0)
{
    ui->setupUi(this);

    connect(m_socket, &QTcpSocket::connected, this,
            &MainWindow::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, this,
            &MainWindow::onDisconnected);
    connect(m_socket, &QTcpSocket::readyRead, this,
            &MainWindow::onReadyRead);
    connect(m_socket, &QAbstractSocket::errorOccurred, this,
            &MainWindow::onError);

    log("Клиент инициализирован");

    ui->stackedWidget->setCurrentIndex(0);

    QVBoxLayout* playersLayout = new QVBoxLayout();
    playersLayout->setContentsMargins(0, 0, 0, 0);
    playersLayout->setSpacing(2);
    playersLayout->stretch(0);
    ui->WidgetContents->setLayout(playersLayout);

    connect(ui->Add_button, &QPushButton::clicked, this, [this, playersLayout](){
        createPlayer(playersLayout);


    });

}

MainWindow::~MainWindow()
{
    disconnectFromServer();
    delete ui;
}

void MainWindow::createPlayer(QVBoxLayout* Layout){
    players++;
    QString strnumber = QString::number(players);

    QWidget* PWidget = new QWidget;
    PWidget->setStyleSheet("background-color: BurlyWood; border-radius: 6px; padding: 5px;");
    PWidget->setFixedHeight(60);

    QLabel* NumLabel = new QLabel();
    NumLabel->setText(strnumber);

    QLabel* NameLabel = new QLabel(CurrentUser);

    QHBoxLayout* PLayout = new QHBoxLayout(PWidget);
    NumLabel->setStyleSheet("font-weight: bold;");

    QPushButton* fButton = new QPushButton("Предложить бой");
    connect(fButton, &QPushButton::clicked, this, [this](){

    });
    fButton->setStyleSheet("background-color: olivedrab");

    PLayout->addWidget(NumLabel);
    PLayout->addWidget(NameLabel);
    PLayout->addWidget(fButton);

    Layout->addWidget(PWidget);
    if (!hasStretch){
        Layout->addStretch();
        hasStretch = true;
    } else {
        QLayoutItem *item = Layout->itemAt(players - 1);
        Layout->takeAt(players - 1);
        delete item;
        Layout->addStretch();
    }
}

void MainWindow::connectToServer(const QString &host, quint16 port) {
    log(QString("Подключение к серверу %1:%2...").arg(host).arg(port), "INFO");
    m_socket->connectToHost(host, port);
}

void MainWindow::disconnectFromServer() {
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        log("Отключение от сервера...", "INFO");
        m_socket->disconnectFromHost();
    }
}

bool MainWindow::isConnected() const {
    return m_socket->state() == QAbstractSocket::ConnectedState;
}

void MainWindow::onConnected() { log("Подключено к серверу!", "INFO"); }

void MainWindow::onDisconnected() {
    log("Отключено от сервера", "INFO");
    CurrentUser = "";
    players = 0;
    emit disconnected();
}

void MainWindow::onReadyRead() {
    QByteArray data = m_socket->readAll();
    QString message = QString::fromUtf8(data).trimmed();

    if (message.isEmpty()) {
        return;
    }

    log(QString("Получено от сервера: %1").arg(message), "RECV");
    processServerMessage(message);
}

void MainWindow::onError(QAbstractSocket::SocketError error) {
    QString errorString = m_socket->errorString();
    log(QString("ОШИБКА подключения: %1").arg(errorString), "ERROR");
    emit errorOccurred(errorString);
}

void MainWindow::on_start_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_connect_button_clicked(){
    bool ok;
    quint16 port = ui->Port_line->text().toUShort(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Ошибка", "Неверный порт!");
        return;
    }

    connectToServer(ui->Ip_line->text(), port);

    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_Log_button_clicked(){
    QString name = QInputDialog::getText(this, "Вход", "Введите ваше имя: ", QLineEdit::Normal);
    //QString password = QInputDialog::getText(this, "Вход", "Введите ваш пароль: ", QLineEdit::Normal);
    if (name != ""){
        CurrentUser = name;
        ui->UserLb->setText(name);
        ui->stackedWidget->setCurrentIndex(3);
    }
    else {
        QMessageBox* errorMessage = new QMessageBox;
        errorMessage->information(this, "Invalid name", "Введите корректное имя");
        delete errorMessage;
    }
}

void MainWindow::processServerMessage(const QString &message) {
    if (message.startsWith("CONNECTED|")) {
        // Формат: CONNECTED|playerId|totalPlayers
        QStringList parts = message.split("|");
        if (parts.size() >= 3) {
            CurrentUser = parts[1];
            players = parts[2].toInt();
            log(QString("Вы игрок #%1 из %2").arg(CurrentUser).arg(players),
                "INFO");
            emit connected(CurrentUser, players);
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

void MainWindow::log(const QString &message, const QString &level) {
    QString formattedMsg = formatLogMessage(level, message);

    qDebug() << formattedMsg;

    QDir logDir("logs");
    if (!logDir.exists()) {
        logDir.mkpath(".");
    }

    QString logFileName =
        QString("logs/client_%1.log")
            .arg(CurrentUser != "" ? CurrentUser : "unknown");
    QFile logFile(logFileName);
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QTextStream stream(&logFile);
        stream << formattedMsg << "\n";
        logFile.close();
    }

    emit logMessage(formattedMsg);
}

QString MainWindow::formatLogMessage(const QString &level,
                                          const QString &message) const {
    QString timestamp =
        QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    return QString("[%1] [%2] %3").arg(timestamp, level, message);
}
