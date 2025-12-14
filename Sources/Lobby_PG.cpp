void MainWindow::sendFightRequest(QString userName)
{
	data.clear();
	QDataStream out(&data, QIODevice::WriteOnly);

	qDebug() << "sendFightRequest: " << CurrentUser.name.c_str() << " | " << userName;
	out << quint16(0) << FIGHT_REQUEST_CODE << QString(CurrentUser.name.c_str()) << userName;

	out.device()->seek(0);
	out << quint16(data.size() - sizeof(quint16));
	m_socket->write(data);

	log(QString("sent fight request to %1").arg(userName));
}

void MainWindow::createPlayer(QString userName)
{
	++players;
	QString strnumber = QString::number(players);
	
	QWidget* PWidget = new QWidget;
	PWidget->setStyleSheet("background-color: BurlyWood; border-radius: 6px; padding: 5px;");
	PWidget->setFixedHeight(60);
	
	QLabel* NumLabel = new QLabel();
	NumLabel->setText(strnumber);
	
	QLabel* NameLabel = new QLabel(userName);
	
	QHBoxLayout* PLayout = new QHBoxLayout(PWidget);
	NumLabel->setStyleSheet("font-weight: bold;");
	
	QPushButton* fButton = new QPushButton("Предложить бой");
	connect(fButton, &QPushButton::clicked, this, [this](){
		sendFightRequest("qw");					// !!!
	});
	fButton->setStyleSheet("background-color: olivedrab");
	
	PLayout->addWidget(NumLabel);
	PLayout->addWidget(NameLabel);
	PLayout->addWidget(fButton);
	
	playersLayout->addWidget(PWidget);
	return;
	if (!hasStretch) {
		playersLayout->addStretch();
		hasStretch = true;
	} else {
		QLayoutItem *item = playersLayout->itemAt(players - 1);
		playersLayout->takeAt(players - 1);
		delete item;
		playersLayout->addStretch();
	}
}

void MainWindow::invokeFightInvintation(QString fInvoker)
{
	QMessageBox* inv {new QMessageBox(this)};
	inv->setWindowTitle("Вызов на бой");
	inv->setText(QString("Игрок %1 вызывает вас на бой").arg(fInvoker));
	inv->setStandardButtons(QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No);
	inv->setIcon(QMessageBox::Icon::Question);
	auto btn {inv->exec()};

	if(btn == QMessageBox::Yes) {
		sendAcceptedInvintation(fInvoker, CurrentUser.name.c_str(), true);
	} else {
		sendAcceptedInvintation(fInvoker, CurrentUser.name.c_str(), false);
	}
	delete inv;
}

void MainWindow::sendAcceptedInvintation(QString fInvoker, QString fReciever, bool accepted)
{
	data.clear();
	QDataStream out(&data, QIODevice::WriteOnly);

	qDebug() << "sendAcceptedInvintation: " << fInvoker << " | " << fReciever;
	out << quint16(0);
	out << ((accepted == true) ? ACCEPTED_INVINTATION_CODE : REJECTED_INVINTATION_CODE);
	out << fInvoker << fReciever;

	out.device()->seek(0);
	out << quint16(data.size() - sizeof(quint16));
	m_socket->write(data);

	if(accepted == true)
		log("accepted invintation");
	else 
		log("rejected invintation");
}

void MainWindow::updateVisualUserInfo()
{
	ui->UserLb->setText(CurrentUser.name.c_str());
	ui->WinLb2->setText(CurrentUser.winCount.c_str());
	ui->LoseLb2->setText(CurrentUser.loseCount.c_str());
	ui->MatchLb_2->setText(CurrentUser.matchCount.c_str());
}
