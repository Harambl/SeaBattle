#include <QCoreApplication>
#include "OceanServer.h"
#include <QDebug>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationName("OceanServer");
    app.setApplicationVersion("1.0");
    
    QCommandLineParser parser;
    parser.setApplicationDescription("Сервер для игры Морской бой");
    parser.addHelpOption();
    parser.addVersionOption();
    
    QCommandLineOption ipOption(QStringList() << "i" << "ip",
                                "IP адрес для прослушивания",
                                "ip", "127.0.0.1");
    QCommandLineOption portOption(QStringList() << "p" << "port",
                                   "Порт для прослушивания",
                                   "port", "12345");
    
    parser.addOption(ipOption);
    parser.addOption(portOption);
    parser.process(app);
    
    QString ip = parser.value(ipOption);
    quint16 port = parser.value(portOption).toUShort();
    
    OceanServer server;
    
    // Подключаем сигналы для логирования
    QObject::connect(&server, &OceanServer::clientConnected,
                     [](int id, const QString &address) {
        qDebug() << QString(">>> Игрок #%1 подключен: %2").arg(id).arg(address);
    });
    
    QObject::connect(&server, &OceanServer::clientDisconnected,
                     [](int id) {
        qDebug() << QString(">>> Игрок #%1 отключен").arg(id);
    });
    
    if (!server.startServer(ip, port)) {
        return 1;
    }
    
    qDebug() << "\n=== Сервер запущен ===";
    qDebug() << QString("IP: %1").arg(ip);
    qDebug() << QString("Порт: %1").arg(port);
    qDebug() << "Логи сохраняются в logs/server.log";
    qDebug() << "Нажмите Ctrl+C для остановки\n";
    
    return app.exec();
}

