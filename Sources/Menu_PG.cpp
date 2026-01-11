#include "mainwindow.h"

void MainWindow::on_start_button_clicked()
{
	// Переход на страницу подключения
	changePage(Page::CONNECT);
}

void MainWindow::on_Exit_button_clicked()
{
	// Выход
	log("выход из приложения");
	disconnectFromServer();
	QApplication::quit();	
}
