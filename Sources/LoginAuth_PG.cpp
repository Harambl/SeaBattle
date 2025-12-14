void MainWindow::sendUserInfo(MessageType type, QString name, QString passHash)
{
	data.clear();
	QDataStream out(&data, QIODevice::WriteOnly);
	out << quint16(0);

	switch(type) {
		case MessageType::CODE: break;
		case MessageType::LOGIN: out << LOGIN_CODE; break;
		case MessageType::AUTH: out << AUTH_CODE; break;	 
	};

	out << name << passHash;

	out.device()->seek(0);
	out << quint16(data.size() - sizeof(quint16));
	m_socket->write(data);

	log(QString("sent %1 info").arg(type));
}

// loging in
void MainWindow::on_Reg_button_clicked()
{
	QString name = QInputDialog::getText(this, "Регистрация", "Введите ваше имя: ", QLineEdit::Normal);
	QString password = QInputDialog::getText(this, "Регистрация", "Введите ваш пароль: ", QLineEdit::Normal);
	name = name.trimmed();

	if (name != ""){
		sendUserInfo(MessageType::LOGIN, name, _hashed(password));
	}
	else {
		QMessageBox* errorMessage = new QMessageBox;
		errorMessage->information(this, "Invalid name", "Введите корректное имя");
		delete errorMessage;
	}
}

// authorization
void MainWindow::on_Log_button_clicked()
{
	QString name = QInputDialog::getText(this, "Вход", "Введите ваше имя: ", QLineEdit::Normal);
	QString password = QInputDialog::getText(this, "Вход", "Введите ваш пароль: ", QLineEdit::Normal);
	name = name.trimmed();

	if (name != ""){
		sendUserInfo(MessageType::AUTH, name, _hashed(password));
	}
	else {
		QMessageBox* errorMessage = new QMessageBox;
		errorMessage->information(this, "Invalid name", "Введите корректное имя");
		delete errorMessage;
	}
}

