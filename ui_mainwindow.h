/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QStackedWidget *stackedWidget;
    QWidget *Menu_pg;
    QLabel *Name;
    QPushButton *start_button;
    QPushButton *Exit_button;
    QWidget *Connect_pg;
    QLabel *Name_c;
    QLineEdit *Ip_line;
    QLineEdit *Port_line;
    QLabel *IpLab;
    QLabel *PortLab;
    QPushButton *connect_button;
    QWidget *RegLog_pg;
    QLabel *Name_rl;
    QPushButton *Reg_button;
    QPushButton *Log_button;
    QWidget *Lobby_pg;
    QLabel *PlayersLb;
    QPushButton *Quit_button;
    QScrollArea *scrollArea;
    QWidget *WidgetContents;
    QLabel *YourLb;
    QLabel *UserLb;
    QLabel *WinLb;
    QLabel *WinLb2;
    QLabel *LoseLb;
    QLabel *LoseLb2;
    QLabel *MatchLb;
    QLabel *MatchLb_2;
    QWidget *Select_pg;
    QWidget *Game_pg;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        MainWindow->setMinimumSize(QSize(800, 600));
        MainWindow->setMaximumSize(QSize(800, 600));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        stackedWidget->setGeometry(QRect(0, -20, 800, 600));
        stackedWidget->setContextMenuPolicy(Qt::NoContextMenu);
        Menu_pg = new QWidget();
        Menu_pg->setObjectName(QString::fromUtf8("Menu_pg"));
        Name = new QLabel(Menu_pg);
        Name->setObjectName(QString::fromUtf8("Name"));
        Name->setGeometry(QRect(300, 200, 231, 51));
        QFont font;
        font.setFamily(QString::fromUtf8("Terminal"));
        font.setPointSize(23);
        font.setBold(true);
        font.setItalic(false);
        Name->setFont(font);
        start_button = new QPushButton(Menu_pg);
        start_button->setObjectName(QString::fromUtf8("start_button"));
        start_button->setGeometry(QRect(300, 290, 191, 41));
        QFont font1;
        font1.setPointSize(12);
        start_button->setFont(font1);
        Exit_button = new QPushButton(Menu_pg);
        Exit_button->setObjectName(QString::fromUtf8("Exit_button"));
        Exit_button->setGeometry(QRect(330, 360, 131, 31));
        stackedWidget->addWidget(Menu_pg);
        Connect_pg = new QWidget();
        Connect_pg->setObjectName(QString::fromUtf8("Connect_pg"));
        Name_c = new QLabel(Connect_pg);
        Name_c->setObjectName(QString::fromUtf8("Name_c"));
        Name_c->setGeometry(QRect(300, 159, 211, 31));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Terminal"));
        font2.setPointSize(24);
        font2.setBold(true);
        font2.setItalic(false);
        Name_c->setFont(font2);
        Ip_line = new QLineEdit(Connect_pg);
        Ip_line->setObjectName(QString::fromUtf8("Ip_line"));
        Ip_line->setGeometry(QRect(310, 250, 171, 21));
        QFont font3;
        font3.setPointSize(14);
        Ip_line->setFont(font3);
        Port_line = new QLineEdit(Connect_pg);
        Port_line->setObjectName(QString::fromUtf8("Port_line"));
        Port_line->setGeometry(QRect(310, 320, 171, 21));
        Port_line->setFont(font3);
        IpLab = new QLabel(Connect_pg);
        IpLab->setObjectName(QString::fromUtf8("IpLab"));
        IpLab->setGeometry(QRect(360, 220, 81, 21));
        IpLab->setFont(font3);
        PortLab = new QLabel(Connect_pg);
        PortLab->setObjectName(QString::fromUtf8("PortLab"));
        PortLab->setGeometry(QRect(370, 290, 51, 20));
        PortLab->setFont(font3);
        connect_button = new QPushButton(Connect_pg);
        connect_button->setObjectName(QString::fromUtf8("connect_button"));
        connect_button->setGeometry(QRect(320, 370, 151, 24));
        stackedWidget->addWidget(Connect_pg);
        RegLog_pg = new QWidget();
        RegLog_pg->setObjectName(QString::fromUtf8("RegLog_pg"));
        Name_rl = new QLabel(RegLog_pg);
        Name_rl->setObjectName(QString::fromUtf8("Name_rl"));
        Name_rl->setGeometry(QRect(300, 210, 200, 31));
        Name_rl->setFont(font);
        Reg_button = new QPushButton(RegLog_pg);
        Reg_button->setObjectName(QString::fromUtf8("Reg_button"));
        Reg_button->setGeometry(QRect(286, 290, 221, 31));
        QFont font4;
        font4.setPointSize(10);
        Reg_button->setFont(font4);
        Log_button = new QPushButton(RegLog_pg);
        Log_button->setObjectName(QString::fromUtf8("Log_button"));
        Log_button->setGeometry(QRect(310, 350, 171, 31));
        Log_button->setFont(font4);
        stackedWidget->addWidget(RegLog_pg);
        Lobby_pg = new QWidget();
        Lobby_pg->setObjectName(QString::fromUtf8("Lobby_pg"));
        PlayersLb = new QLabel(Lobby_pg);
        PlayersLb->setObjectName(QString::fromUtf8("PlayersLb"));
        PlayersLb->setGeometry(QRect(40, 50, 431, 31));
        QFont font5;
        font5.setPointSize(16);
        PlayersLb->setFont(font5);
        PlayersLb->setAlignment(Qt::AlignCenter);
        Quit_button = new QPushButton(Lobby_pg);
        Quit_button->setObjectName(QString::fromUtf8("Quit_button"));
        Quit_button->setGeometry(QRect(530, 499, 201, 31));
        scrollArea = new QScrollArea(Lobby_pg);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setGeometry(QRect(40, 100, 431, 441));
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        scrollArea->setWidgetResizable(true);
        WidgetContents = new QWidget();
        WidgetContents->setObjectName(QString::fromUtf8("WidgetContents"));
        WidgetContents->setGeometry(QRect(0, 0, 429, 439));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(WidgetContents->sizePolicy().hasHeightForWidth());
        WidgetContents->setSizePolicy(sizePolicy);
        WidgetContents->setLayoutDirection(Qt::LeftToRight);
        scrollArea->setWidget(WidgetContents);
        YourLb = new QLabel(Lobby_pg);
        YourLb->setObjectName(QString::fromUtf8("YourLb"));
        YourLb->setGeometry(QRect(560, 90, 161, 51));
        QFont font6;
        font6.setPointSize(13);
        font6.setItalic(false);
        font6.setStrikeOut(false);
        YourLb->setFont(font6);
        YourLb->setLayoutDirection(Qt::RightToLeft);
        YourLb->setAutoFillBackground(false);
        YourLb->setAlignment(Qt::AlignCenter);
        UserLb = new QLabel(Lobby_pg);
        UserLb->setObjectName(QString::fromUtf8("UserLb"));
        UserLb->setGeometry(QRect(510, 130, 261, 31));
        QFont font7;
        font7.setPointSize(11);
        UserLb->setFont(font7);
        UserLb->setAlignment(Qt::AlignCenter);
        WinLb = new QLabel(Lobby_pg);
        WinLb->setObjectName(QString::fromUtf8("WinLb"));
        WinLb->setGeometry(QRect(560, 220, 61, 31));
        WinLb->setFont(font4);
        WinLb2 = new QLabel(Lobby_pg);
        WinLb2->setObjectName(QString::fromUtf8("WinLb2"));
        WinLb2->setGeometry(QRect(680, 220, 71, 31));
        WinLb2->setFont(font4);
        LoseLb = new QLabel(Lobby_pg);
        LoseLb->setObjectName(QString::fromUtf8("LoseLb"));
        LoseLb->setGeometry(QRect(560, 274, 101, 31));
        LoseLb->setFont(font4);
        LoseLb2 = new QLabel(Lobby_pg);
        LoseLb2->setObjectName(QString::fromUtf8("LoseLb2"));
        LoseLb2->setGeometry(QRect(680, 270, 61, 41));
        LoseLb2->setFont(font4);
        MatchLb = new QLabel(Lobby_pg);
        MatchLb->setObjectName(QString::fromUtf8("MatchLb"));
        MatchLb->setGeometry(QRect(560, 320, 101, 41));
        MatchLb->setFont(font4);
        MatchLb_2 = new QLabel(Lobby_pg);
        MatchLb_2->setObjectName(QString::fromUtf8("MatchLb_2"));
        MatchLb_2->setGeometry(QRect(680, 320, 49, 41));
        MatchLb_2->setFont(font4);
        stackedWidget->addWidget(Lobby_pg);
        Select_pg = new QWidget();
        Select_pg->setObjectName(QString::fromUtf8("Select_pg"));
        stackedWidget->addWidget(Select_pg);
        Game_pg = new QWidget();
        Game_pg->setObjectName(QString::fromUtf8("Game_pg"));
        stackedWidget->addWidget(Game_pg);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 23));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
        QWidget::setTabOrder(Exit_button, Ip_line);
        QWidget::setTabOrder(Ip_line, Port_line);
        QWidget::setTabOrder(Port_line, Reg_button);
        QWidget::setTabOrder(Reg_button, Log_button);
        QWidget::setTabOrder(Log_button, Quit_button);
        QWidget::setTabOrder(Quit_button, start_button);
        QWidget::setTabOrder(start_button, scrollArea);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        Name->setText(QCoreApplication::translate("MainWindow", "Sea Raider", nullptr));
        start_button->setText(QCoreApplication::translate("MainWindow", "\320\235\320\260\321\207\320\260\321\202\321\214 \320\270\320\263\321\200\321\203", nullptr));
        Exit_button->setText(QCoreApplication::translate("MainWindow", "\320\222\321\213\320\271\321\202\320\270", nullptr));
        Name_c->setText(QCoreApplication::translate("MainWindow", "Sea Raider", nullptr));
        Ip_line->setText(QCoreApplication::translate("MainWindow", "127.0.0.1", nullptr));
        Port_line->setText(QCoreApplication::translate("MainWindow", "55555", nullptr));
        IpLab->setText(QCoreApplication::translate("MainWindow", "Ip \320\260\320\264\321\200\320\265\321\201", nullptr));
        PortLab->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\321\200\321\202", nullptr));
        connect_button->setText(QCoreApplication::translate("MainWindow", "\320\237\321\200\320\270\321\201\320\276\320\265\320\264\320\270\320\275\320\270\321\202\321\214\321\201\321\217", nullptr));
        Name_rl->setText(QCoreApplication::translate("MainWindow", "Sea Raider", nullptr));
        Reg_button->setText(QCoreApplication::translate("MainWindow", "\320\227\320\260\321\200\320\265\320\263\320\270\321\201\321\202\321\200\320\270\321\200\320\276\320\262\320\260\321\202\321\214\321\201\321\217", nullptr));
        Log_button->setText(QCoreApplication::translate("MainWindow", "\320\222\320\276\320\271\321\202\320\270", nullptr));
        PlayersLb->setText(QCoreApplication::translate("MainWindow", "\320\230\320\263\321\200\320\276\320\272\320\270 \320\262 \320\273\320\276\320\261\320\261\320\270", nullptr));
        Quit_button->setText(QCoreApplication::translate("MainWindow", "\320\236\321\202\321\201\320\276\320\265\320\264\320\270\320\275\320\270\321\202\321\214\321\201\321\217 \320\270 \320\262\321\213\320\271\321\202\320\270", nullptr));
        YourLb->setText(QCoreApplication::translate("MainWindow", "\320\222\320\260\321\210\320\260 \321\201\321\202\320\260\321\202\320\270\321\201\321\202\320\270\320\272\320\260", nullptr));
        UserLb->setText(QString());
        WinLb->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\261\320\265\320\264:", nullptr));
        WinLb2->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        LoseLb->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\321\200\320\260\320\266\320\265\320\275\320\270\320\271:", nullptr));
        LoseLb2->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        MatchLb->setText(QCoreApplication::translate("MainWindow", "\320\222\321\201\320\265\320\263\320\276 \320\274\320\260\321\202\321\207\320\265\320\271:", nullptr));
        MatchLb_2->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
