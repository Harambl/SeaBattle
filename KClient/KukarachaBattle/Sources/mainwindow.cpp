#include "mainwindow.h"

#include "Menu_PG.cpp"
#include "Connect_PG.cpp"
#include "LoginAuth_PG.cpp"
#include "Lobby_PG.cpp"
#include "Game_PG.cpp"
#include "Select_PG.cpp"
#include "onReadyRead.cpp"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow),
	m_socket(new QTcpSocket(this)), players(0), Players {}, fButtons {}
{
	ui->setupUi(this);
	
	// Переключение на начальную страницу при создании окна
	changePage(Page::MENU);

	mainGameUI = new GameUI(ui->Game_pg);
	shipsPlanningUI = new ShipsPlanningUI(ui->Select_pg);
	
	playersLayout = new QVBoxLayout();
	playersLayout->setContentsMargins(0, 0, 0, 0);
	playersLayout->setSpacing(2);
	playersLayout->stretch(0);
	ui->WidgetContents->setLayout(playersLayout);

	// Задание фона всем страницам
	for(QWidget* page : {ui->Menu_pg, ui->Connect_pg,
			ui->RegLog_pg, ui->Lobby_pg, ui->Select_pg, ui->Game_pg}) {
		QPalette pal = page->palette();
		pal.setBrush(QPalette::Window, QBrush(QPixmap("./SeaBattlePapers.jpg")));
		page->setPalette(pal);
		page->setAutoFillBackground(true);
	}

	connect(m_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
	connect(ui->Quit_button, SIGNAL(clicked()), this, SLOT(on_Exit_button_clicked()));

	connect(mainGameUI, SIGNAL(signalClickedPos()), this, SLOT(onSignalClickedPos()));
	connect(mainGameUI, SIGNAL(signalWin()), this, SLOT(onSignalWin()));
	connect(mainGameUI, SIGNAL(signalLose()), this, SLOT(onSignalLose()));

	connect(shipsPlanningUI, SIGNAL(signalReadyToFight()), this, SLOT(onSignalReadyToFight()));
	connect(shipsPlanningUI, SIGNAL(signalStartFight()), this, SLOT(onSignalStartFight()));
}

MainWindow::~MainWindow()
{
	if(mainGameUI) delete mainGameUI;
	if(shipsPlanningUI) delete shipsPlanningUI;

	disconnectFromServer();
	delete ui;
}

template <class... Ts>
void MainWindow::sendDataToServer(quint16 code, Ts&&... Args)
{
	// Основная шаблонная функция отправки информации на сервер
	
	data.clear();
	QDataStream out(&data, QIODevice::WriteOnly);

	out << quint16(0) << code;
	(..., (out << std::forward<Ts>(Args)));

	out.device()->seek(0);
	out << quint16(data.size() - sizeof(quint16));
	m_socket->write(data);
	QThread::sleep(0.3);
	m_socket->flush();

	log(QString("sent code %1").arg(code), "INFO");
}

void MainWindow::resetGameUI()
{
	// Сброс игрового интерфейса
	mainGameUI->reset();
}

void MainWindow::processServerCode(quint8 code)
{
	// Обработка результата входа / регистрации
	
	if(code == OK_CODE) {
		log("Подключение установлено", "INFO");
		state = State::CONNECTED;
		changePage(Page::LOGIN_AUTH);
	} else if(code == WRONG_AUTH_CODE) {
		log("Неверное имя пользователя или пароль", "INFO");
		QMessageBox* errorMessage = new QMessageBox;
		errorMessage->information(this, "Неверное имя пользователя или пароль", "Попробуйте снова");
		delete errorMessage;
	} else if(code == WRONG_LOGIN_CODE) {
		log("Пользователь с таким именем уже существует", "INFO");
		QMessageBox* errorMessage = new QMessageBox;
		errorMessage->information(this, "Пользователь с таким именем уже существует", "Попробуйте снова");
		delete errorMessage;
	}
}

void MainWindow::processShotResult(ShotResult result)
{
	mainGameUI->processShotRes(result);
}

void MainWindow::changePage(int ind)
{
	// Переключение на виджет-страницу по индексу.
	// Зачастую в коде используются свойства enum,
	// каждому индексу страницы соответствует свое имя в enum Page
	ui->stackedWidget->setCurrentIndex(ind);
}

void MainWindow::changeUserMatchesInfo(int winDelta, int loseDelta, int mCountDelta)
{
	// Изменение количества побед, поражений, матчей на заданные значения
	
	string newWinCountString {}, newLoseCountString {}, newMatchCountString {};

	newWinCountString = std::to_string(stoi(CurrentUser.winCount) + winDelta);
	newLoseCountString = std::to_string(stoi(CurrentUser.loseCount) + loseDelta);
	newMatchCountString = std::to_string(stoi(CurrentUser.matchCount) + mCountDelta);

	std::cout << newWinCountString << " " << newLoseCountString;
	std::cout << " " << newMatchCountString << std::endl;
	
	CurrentUser.winCount = newWinCountString;
	CurrentUser.loseCount = newLoseCountString;
	CurrentUser.matchCount = newMatchCountString;

	updateVisualUserInfo();
}

QString MainWindow::_hashed(QString password)
{
	// Хэширование пароля
	// (ограничимся не самым безопасным std::hash ввиду учебной цели проекта)
	// (лучше было использовать другие виды хэша)
	
	size_t _passHash;
	string _passw, _strPassHash;
	_passw = string(password.toUtf8().constData());

	// unfase
	_passHash = std::hash<string>{}(_passw);

	_strPassHash = std::to_string(_passHash);

	return QString(_strPassHash.c_str());
}

void MainWindow::sendNameAndCodeToServer(quint16 Code)
{
	// Общая функция отправления имени и кода на сервер
	sendDataToServer(Code, QString(CurrentUser.name.c_str()));
}

void MainWindow::connectToServer(const QString &host, quint16 port)
{
	// Подключение к серверу
	log(QString("Подключение к серверу %1:%2...").arg(host).arg(port), "INFO");
	m_socket->connectToHost(host, port);
}

void MainWindow::disconnectFromServer()
{
	// Отключение от сервера
	if (m_socket->state() == QAbstractSocket::ConnectedState) {
		log("Отключение от сервера...", "INFO");
		m_socket->disconnectFromHost();
	}
}

bool MainWindow::isConnected() const
{
	// Получение состояния сокета
	return m_socket->state() == QAbstractSocket::ConnectedState;
}

void MainWindow::onConnected()
{
	log("Подключено к серверу!", "INFO");
}

void MainWindow::onDisconnected()
{
	log("Отключено от сервера", "INFO");
	emit disconnected();
}

void MainWindow::onError(QAbstractSocket::SocketError error)
{
	QString errorString = m_socket->errorString();
	log(QString("ОШИБКА подключения: %1").arg(errorString), "ERROR");
	emit errorOccurred(errorString);
}

void MainWindow::log(const QString &message, const QString &level) {
	// Логирование
	
	QString formattedMsg = formatLogMessage(level, message);

	qDebug() << formattedMsg;

	QDir logDir(".logs");
	if (!logDir.exists()) {
		logDir.mkpath(".");
	}

	QString logFileName = QString("logs/client_%1.log")
		.arg(CurrentUser.name != "" ? CurrentUser.name.c_str() : "unknownUser");
	QFile logFile(logFileName);
	if (logFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
		QTextStream stream(&logFile);
		stream << formattedMsg << "\n";
		logFile.close();
 	}

	emit logMessage(formattedMsg);
}

QString MainWindow::formatLogMessage(const QString &level, const QString &message) const {
	// Формирование лога
	QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
	return QString("[%1] [%2] %3").arg(timestamp, level, message);
}

void MainWindow::changeFButton(QString figthsUserName, bool disable)
{
	// Поиск кнопки и изменение ее кликабельности
	for(FightButton* fButton : fButtons) {
		if(fButton->getUserName() == figthsUserName)	{
			fButton->change(disable);
			break;
		}
	}
}
