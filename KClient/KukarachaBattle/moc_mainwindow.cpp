/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "Headers/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[29];
    char stringdata0[430];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 9), // "connected"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 8), // "QString&"
QT_MOC_LITERAL(4, 31, 8), // "UserName"
QT_MOC_LITERAL(5, 40, 12), // "totalPlayers"
QT_MOC_LITERAL(6, 53, 12), // "disconnected"
QT_MOC_LITERAL(7, 66, 9), // "gameReady"
QT_MOC_LITERAL(8, 76, 20), // "opponentDisconnected"
QT_MOC_LITERAL(9, 97, 13), // "errorOccurred"
QT_MOC_LITERAL(10, 111, 5), // "error"
QT_MOC_LITERAL(11, 117, 10), // "logMessage"
QT_MOC_LITERAL(12, 128, 7), // "message"
QT_MOC_LITERAL(13, 136, 23), // "on_start_button_clicked"
QT_MOC_LITERAL(14, 160, 22), // "on_Exit_button_clicked"
QT_MOC_LITERAL(15, 183, 25), // "on_connect_button_clicked"
QT_MOC_LITERAL(16, 209, 21), // "on_Log_button_clicked"
QT_MOC_LITERAL(17, 231, 21), // "on_Reg_button_clicked"
QT_MOC_LITERAL(18, 253, 11), // "onConnected"
QT_MOC_LITERAL(19, 265, 14), // "onDisconnected"
QT_MOC_LITERAL(20, 280, 11), // "onReadyRead"
QT_MOC_LITERAL(21, 292, 7), // "onError"
QT_MOC_LITERAL(22, 300, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(23, 329, 18), // "onSignalClickedPos"
QT_MOC_LITERAL(24, 348, 16), // "sendFightRequest"
QT_MOC_LITERAL(25, 365, 11), // "onSignalWin"
QT_MOC_LITERAL(26, 377, 12), // "onSignalLose"
QT_MOC_LITERAL(27, 390, 20), // "onSignalReadyToFight"
QT_MOC_LITERAL(28, 411, 18) // "onSignalStartFight"

    },
    "MainWindow\0connected\0\0QString&\0UserName\0"
    "totalPlayers\0disconnected\0gameReady\0"
    "opponentDisconnected\0errorOccurred\0"
    "error\0logMessage\0message\0"
    "on_start_button_clicked\0on_Exit_button_clicked\0"
    "on_connect_button_clicked\0"
    "on_Log_button_clicked\0on_Reg_button_clicked\0"
    "onConnected\0onDisconnected\0onReadyRead\0"
    "onError\0QAbstractSocket::SocketError\0"
    "onSignalClickedPos\0sendFightRequest\0"
    "onSignalWin\0onSignalLose\0onSignalReadyToFight\0"
    "onSignalStartFight"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,  119,    2, 0x06 /* Public */,
       6,    0,  124,    2, 0x06 /* Public */,
       7,    0,  125,    2, 0x06 /* Public */,
       8,    0,  126,    2, 0x06 /* Public */,
       9,    1,  127,    2, 0x06 /* Public */,
      11,    1,  130,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      13,    0,  133,    2, 0x08 /* Private */,
      14,    0,  134,    2, 0x08 /* Private */,
      15,    0,  135,    2, 0x08 /* Private */,
      16,    0,  136,    2, 0x08 /* Private */,
      17,    0,  137,    2, 0x08 /* Private */,
      18,    0,  138,    2, 0x08 /* Private */,
      19,    0,  139,    2, 0x08 /* Private */,
      20,    0,  140,    2, 0x08 /* Private */,
      21,    1,  141,    2, 0x08 /* Private */,
      23,    0,  144,    2, 0x08 /* Private */,
      24,    0,  145,    2, 0x08 /* Private */,
      25,    0,  146,    2, 0x08 /* Private */,
      26,    0,  147,    2, 0x08 /* Private */,
      27,    0,  148,    2, 0x08 /* Private */,
      28,    0,  149,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int,    4,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString,   12,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 22,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->connected((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->disconnected(); break;
        case 2: _t->gameReady(); break;
        case 3: _t->opponentDisconnected(); break;
        case 4: _t->errorOccurred((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->logMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->on_start_button_clicked(); break;
        case 7: _t->on_Exit_button_clicked(); break;
        case 8: _t->on_connect_button_clicked(); break;
        case 9: _t->on_Log_button_clicked(); break;
        case 10: _t->on_Reg_button_clicked(); break;
        case 11: _t->onConnected(); break;
        case 12: _t->onDisconnected(); break;
        case 13: _t->onReadyRead(); break;
        case 14: _t->onError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 15: _t->onSignalClickedPos(); break;
        case 16: _t->sendFightRequest(); break;
        case 17: _t->onSignalWin(); break;
        case 18: _t->onSignalLose(); break;
        case 19: _t->onSignalReadyToFight(); break;
        case 20: _t->onSignalStartFight(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 14:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MainWindow::*)(QString & , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::connected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::disconnected)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::gameReady)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::opponentDisconnected)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::errorOccurred)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::logMessage)) {
                *result = 5;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::connected(QString & _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MainWindow::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void MainWindow::gameReady()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void MainWindow::opponentDisconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void MainWindow::errorOccurred(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void MainWindow::logMessage(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
