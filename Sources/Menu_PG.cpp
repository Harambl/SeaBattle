void MainWindow::on_start_button_clicked()
{
	ui->stackedWidget->setCurrentIndex(Page::CONNECT);
}

void MainWindow::on_Exit_button_clicked()
{
	log("выход из приложения");
	QApplication::quit();	
}
