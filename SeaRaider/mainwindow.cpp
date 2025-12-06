#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

void MainWindow::on_start_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_connect_button_clicked(){
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
