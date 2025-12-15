#include "GameWidget.h"
#include "GameLogic.h"
#include "../../common/GameTypes.h"
#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <QMessageBox>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("SeaBattle Demo");
    
    QWidget window;
    window.setWindowTitle("Морской бой - Демонстрация поля");
    window.resize(800, 600);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(&window);
    
    // Заголовок
    QLabel *titleLabel = new QLabel("Демонстрация игрового поля");
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; padding: 10px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    
    // Горизонтальный layout для полей
    QHBoxLayout *boardsLayout = new QHBoxLayout;
    
    // Левое поле (свое поле с кораблями)
    QVBoxLayout *leftLayout = new QVBoxLayout;
    QLabel *leftLabel = new QLabel("Ваше поле");
    leftLabel->setStyleSheet("font-size: 14px; font-weight: bold;");
    leftLabel->setAlignment(Qt::AlignCenter);
    leftLayout->addWidget(leftLabel);
    
    GameWidget *playerBoardWidget = new GameWidget;
    playerBoardWidget->setShowShips(true);
    leftLayout->addWidget(playerBoardWidget);
    
    QLabel *leftInfoLabel = new QLabel("Корабли: 10");
    leftInfoLabel->setAlignment(Qt::AlignCenter);
    leftLayout->addWidget(leftInfoLabel);
    
    boardsLayout->addLayout(leftLayout);
    
    // Правое поле (поле противника - только результаты выстрелов)
    QVBoxLayout *rightLayout = new QVBoxLayout;
    QLabel *rightLabel = new QLabel("Поле противника");
    rightLabel->setStyleSheet("font-size: 14px; font-weight: bold;");
    rightLabel->setAlignment(Qt::AlignCenter);
    rightLayout->addWidget(rightLabel);
    
    GameWidget *enemyBoardWidget = new GameWidget;
    enemyBoardWidget->setShowShips(false);
    rightLayout->addWidget(enemyBoardWidget);
    
    QLabel *rightInfoLabel = new QLabel("Корабли противника: 10");
    rightInfoLabel->setAlignment(Qt::AlignCenter);
    rightLayout->addWidget(rightInfoLabel);
    
    boardsLayout->addLayout(rightLayout);
    
    mainLayout->addLayout(boardsLayout);
    
    // Кнопки управления
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    
    QPushButton *resetBtn = new QPushButton("Новая расстановка");
    QPushButton *testShotBtn = new QPushButton("Тестовый выстрел");
    
    buttonLayout->addWidget(resetBtn);
    buttonLayout->addWidget(testShotBtn);
    buttonLayout->addStretch();
    
    mainLayout->addLayout(buttonLayout);
    
    // Игровая логика
    GameLogic *gameLogic = new GameLogic;
    
    // Автоматическая расстановка кораблей при запуске
    if (!gameLogic->autoPlaceShips()) {
        QMessageBox::critical(&window, "Ошибка", "Не удалось расставить корабли!");
        return 1;
    }
    
    // Обновление отображения
    auto updateBoards = [&]() {
        playerBoardWidget->setBoard(gameLogic->getPlayerBoard());
        enemyBoardWidget->setBoard(gameLogic->getEnemyBoard());
        leftInfoLabel->setText(QString("Корабли: %1").arg(gameLogic->getAliveShipsCount()));
    };
    
    updateBoards();
    
    // Обработка кнопки "Новая расстановка"
    QObject::connect(resetBtn, &QPushButton::clicked, [&]() {
        if (gameLogic->autoPlaceShips()) {
            updateBoards();
        } else {
            QMessageBox::warning(&window, "Ошибка", "Не удалось расставить корабли!");
        }
    });
    
    // Обработка тестового выстрела
    QObject::connect(testShotBtn, &QPushButton::clicked, [&]() {
        // Делаем случайный выстрел для демонстрации
        static int shotCount = 0;
        shotCount++;
        
        if (shotCount <= 100) {
            // Простой способ: стреляем по порядку
            int row = (shotCount - 1) / BOARD_SIZE;
            int col = (shotCount - 1) % BOARD_SIZE;
            
            if (row < BOARD_SIZE) {
                QPoint pos(col, row);
                ShotResult result = gameLogic->shoot(pos);
                
                QString resultText;
                switch (result) {
                    case ShotResult::Miss:
                        resultText = "Мимо";
                        break;
                    case ShotResult::Hit:
                        resultText = "Ранил";
                        break;
                    case ShotResult::Destroyed:
                        resultText = "Убил!";
                        break;
                    case ShotResult::Invalid:
                        resultText = "Некорректный выстрел";
                        break;
                }
                
                updateBoards();
                
                if (result == ShotResult::Destroyed) {
                    QMessageBox::information(&window, "Результат", 
                        QString("Выстрел в %1%2: %3")
                            .arg(QChar('A' + col))
                            .arg(row + 1)
                            .arg(resultText));
                }
                
                if (gameLogic->areAllShipsDestroyed()) {
                    QMessageBox::information(&window, "Победа!", "Все корабли уничтожены!");
                    shotCount = 0;
                    gameLogic->autoPlaceShips();
                    updateBoards();
                }
            }
        } else {
            shotCount = 0;
            gameLogic->autoPlaceShips();
            updateBoards();
        }
    });
    
    // Обработка клика по полю противника (для интерактивности)
    QObject::connect(enemyBoardWidget, &GameWidget::cellClicked, [&](const QPoint& pos) {
        ShotResult result = gameLogic->shoot(pos);
        
        QString resultText;
        switch (result) {
            case ShotResult::Miss:
                resultText = "Мимо";
                break;
            case ShotResult::Hit:
                resultText = "Ранил";
                break;
            case ShotResult::Destroyed:
                resultText = "Убил!";
                break;
            case ShotResult::Invalid:
                return; // Игнорируем некорректные выстрелы
        }
        
        updateBoards();
        
        if (result == ShotResult::Destroyed || result == ShotResult::Hit) {
            QMessageBox::information(&window, "Результат", 
                QString("Выстрел в %1%2: %3")
                    .arg(QChar('A' + pos.x()))
                    .arg(pos.y() + 1)
                    .arg(resultText));
        }
        
        if (gameLogic->areAllShipsDestroyed()) {
            QMessageBox::information(&window, "Победа!", "Все корабли уничтожены!");
            gameLogic->autoPlaceShips();
            updateBoards();
        }
    });
    
    window.show();
    return app.exec();
}

