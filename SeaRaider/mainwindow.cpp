#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createPlayer(){
    players++;
    QString strnumber = QString::number(players);

    QWidget* PWidget = new QWidget;
    PWidget->setStyleSheet("background-color: peru; border-radius: 6px; padding: 5px;");
    PWidget->setMaximumHeight(80);

    QLabel* NumLabel = new QLabel();
    NumLabel->setText(strnumber);

    QLabel* NameLabel = new QLabel();

    QHBoxLayout* PLayout = new QHBoxLayout(PWidget);
    NumLabel->setStyleSheet("font-weight: bold;");

    QPushButton* compButton = new QPushButton;
    connect(compButton, &QPushButton::clicked, this, [this](){

    });

    PLayout->addWidget(NumLabel);
    PLayout->addWidget(NameLabel);
    PLayout->addWidget(compButton);

    ui->PlayersLayout->addWidget(PWidget);
}
