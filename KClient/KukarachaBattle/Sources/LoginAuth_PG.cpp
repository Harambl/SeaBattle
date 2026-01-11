#include "mainwindow.h"

void MainWindow::sendUserInfo(MessageType type, QString name, QString passHash)
{
	// Отправление введенных пользователем имени и пароля на сервер
	switch(type) {
		case MessageType::CODE: break;
		case MessageType::LOGIN: sendDataToServer(LOGIN_CODE, name, passHash); break;
		case MessageType::AUTH: sendDataToServer(AUTH_CODE, name, passHash); break;	 
	};
}

void MainWindow::on_Reg_button_clicked()
{
	// Получение данных для регистрации
	
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

void MainWindow::on_Log_button_clicked()
{
	// Получение данных для входа
	
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

