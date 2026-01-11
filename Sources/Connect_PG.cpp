#include "mainwindow.h"

void MainWindow::on_connect_button_clicked()
{
	if(isConnected()) return;

	bool ok;
	quint16 port = ui->Port_line->text().toUShort(&ok);
	if (!ok || !is_valid_port(port)) {
		QMessageBox::warning(this, "Ошибка", "Неверный порт!");
		return;
	}
	QString ip = ui->Ip_line->text();
	if (!is_valid_ip(ip)) {
		QMessageBox::warning(this, "Ошибка", "Неверный IP!");
		return;
	}
	connectToServer(ip, port);
}

bool MainWindow::is_valid_port(quint16 port)
{
	return true;	
}

bool MainWindow::is_valid_ip(QString ip)
{
	return true;	
}
