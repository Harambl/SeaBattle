#include "mainwindow.h"

#include "Menu_PG.cpp"
#include "Connect_PG.cpp"
#include "LoginAuth_PG.cpp"
#include "Lobby_PG.cpp"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow),
	m_socket(new QTcpSocket(this)), players(0)
{
	ui->setupUi(this);
	
	ui->stackedWidget->setCurrentIndex(Page::MENU);
	
	playersLayout = new QVBoxLayout();
	playersLayout->setContentsMargins(0, 0, 0, 0);
	playersLayout->setSpacing(2);
	playersLayout->stretch(0);
	ui->WidgetContents->setLayout(playersLayout);

	connect(m_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}

MainWindow::~MainWindow()
{
    disconnectFromServer();
    delete ui;
}

void MainWindow::processServerCode(quint8 code)
{
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

void MainWindow::changePage(int ind)
{
	ui->stackedWidget->setCurrentIndex(ind);
}

QString MainWindow::_hashed(QString password)
{
	// unsafe
	size_t _passHash;
	string _passw, _strPassHash;
	_passw = string(password.toUtf8().constData());
	_passHash = std::hash<string>{}(_passw);
	_strPassHash = std::to_string(_passHash);
	return QString(_strPassHash.c_str());
}


void MainWindow::connectToServer(const QString &host, quint16 port)
{
	log(QString("Подключение к серверу %1:%2...").arg(host).arg(port), "INFO");
	m_socket->connectToHost(host, port);
}

void MainWindow::disconnectFromServer()
{
	if (m_socket->state() == QAbstractSocket::ConnectedState) {
		log("Отключение от сервера...", "INFO");
		m_socket->disconnectFromHost();
	}
}

bool MainWindow::isConnected() const
{
	return m_socket->state() == QAbstractSocket::ConnectedState;
}

void MainWindow::onConnected() { log("Подключено к серверу!", "INFO"); }

void MainWindow::onDisconnected()
{
	log("Отключено от сервера", "INFO");
	emit disconnected();
}

void MainWindow::onReadyRead()
{
	QDataStream in(m_socket);
	in.setVersion(QDataStream::Qt_5_2);
	User user;
	quint8 code;

	if(m_socket->bytesAvailable() == 1) {
		log("Получен код от сервера", "INFO");
		in >> code; processServerCode(code);
		return;
	}

	dataSize = 0;

	if(in.status() == QDataStream::Ok) {
		for(;;) {
			if(dataSize == 0) {
				if(m_socket->bytesAvailable() < 2) continue;
				in >> dataSize;
			}
			if(m_socket->bytesAvailable() < dataSize) {
				log(QString("data loosed: recieved %1 / %2")
						.arg(m_socket->bytesAvailable()).arg(dataSize), "INFO");
				continue;
			}
			in >> code;
			// process server data
			
			if(code == OK_AUTH_CODE) {

				log("authorized successfully", "INFO");
				qDebug() << "OK_AUTH_CODE";
				QString name, winCount, loseCount, matchCount;
				in >> name >> winCount >> loseCount >> matchCount;
				qDebug() << name << winCount << loseCount << matchCount;
				CurrentUser = User({
							name.toUtf8().constData(),
							string(""),
							winCount.toUtf8().constData(),
							loseCount.toUtf8().constData(),
							matchCount.toUtf8().constData(),
						});
				updateVisualUserInfo();
				ui->stackedWidget->setCurrentIndex(Page::LOBBY);
				break;
			}

			else if(code == USERNAME_CODE) {

				log("recieved player name to playersLayout", "INFO");
				vector<QString> Names; quint16 size;
				QString otherUserName;
				in >> size;
				for(quint16 i {0}; i < size; ++i) {
					in >> otherUserName;
					createPlayer(otherUserName);
				}
				break;
			}

			else if(code == FIGHT_INVINTATION_CODE) {

				QString fInvoker; in >> fInvoker;
				invokeFightInvintation(fInvoker);
				break;

			} else if(code == START_FIGHT_CODE) {

				in >> enemy;
				log(QString("Начался бой с %1").arg(enemy), "INFO");
				changePage(Page::SHIPS_PLANNING);
				break;

			} else if(code == REJECTED_FIGHT_CODE) {
					
				in >> enemy;
				log(QString("Отклонен бой с %1").arg(enemy), "INFO");
				if(enemy != QString(CurrentUser.name.c_str())) {
					QMessageBox* errorMessage = new QMessageBox;
					errorMessage->information(this, 
							"Opponent anwer", "Противние отклонил вызов");
					delete errorMessage;
				}
				break;

			}
		}	
		log("Data reading finished", "INFO");
	} else
		log("Error reading data", "INFO");
}

void MainWindow::onError(QAbstractSocket::SocketError error)
{
	QString errorString = m_socket->errorString();
	log(QString("ОШИБКА подключения: %1").arg(errorString), "ERROR");
	emit errorOccurred(errorString);
}

void MainWindow::log(const QString &message, const QString &level) {
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
	QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
	return QString("[%1] [%2] %3").arg(timestamp, level, message);
}
