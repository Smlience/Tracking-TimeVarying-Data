/****************************************************************************
** Meta object code from reading C++ file 'TF1DWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/tf/1dtf/TF1DWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TF1DWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TF1DWidget_t {
    QByteArrayData data[18];
    char stringdata[232];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_TF1DWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_TF1DWidget_t qt_meta_stringdata_TF1DWidget = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 12),
QT_MOC_LITERAL(2, 24, 0),
QT_MOC_LITERAL(3, 25, 1),
QT_MOC_LITERAL(4, 27, 9),
QT_MOC_LITERAL(5, 37, 7),
QT_MOC_LITERAL(6, 45, 6),
QT_MOC_LITERAL(7, 52, 14),
QT_MOC_LITERAL(8, 67, 22),
QT_MOC_LITERAL(9, 90, 31),
QT_MOC_LITERAL(10, 122, 17),
QT_MOC_LITERAL(11, 140, 16),
QT_MOC_LITERAL(12, 157, 4),
QT_MOC_LITERAL(13, 162, 18),
QT_MOC_LITERAL(14, 181, 18),
QT_MOC_LITERAL(15, 200, 9),
QT_MOC_LITERAL(16, 210, 13),
QT_MOC_LITERAL(17, 224, 6)
    },
    "TF1DWidget\0colorChanged\0\0c\0tfChanged\0"
    "tfReset\0sendTF\0unsigned char*\0"
    "on_canvas_colorChanged\0"
    "on_canvas_resetTransferFunction\0"
    "on_canvas_changed\0on_canvas_sendTF\0"
    "data\0on_btnSave_clicked\0on_btnLoad_clicked\0"
    "setVolume\0const Volume*\0volume\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TF1DWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x05,
       4,    0,   72,    2, 0x05,
       5,    0,   73,    2, 0x05,
       6,    1,   74,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       8,    1,   77,    2, 0x0a,
       9,    0,   80,    2, 0x0a,
      10,    0,   81,    2, 0x0a,
      11,    1,   82,    2, 0x0a,
      13,    0,   85,    2, 0x0a,
      14,    0,   86,    2, 0x0a,
      15,    1,   87,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, QMetaType::QColor,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QColor,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 16,   17,

       0        // eod
};

void TF1DWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TF1DWidget *_t = static_cast<TF1DWidget *>(_o);
        switch (_id) {
        case 0: _t->colorChanged((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 1: _t->tfChanged(); break;
        case 2: _t->tfReset(); break;
        case 3: _t->sendTF((*reinterpret_cast< unsigned char*(*)>(_a[1]))); break;
        case 4: _t->on_canvas_colorChanged((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 5: _t->on_canvas_resetTransferFunction(); break;
        case 6: _t->on_canvas_changed(); break;
        case 7: _t->on_canvas_sendTF((*reinterpret_cast< unsigned char*(*)>(_a[1]))); break;
        case 8: _t->on_btnSave_clicked(); break;
        case 9: _t->on_btnLoad_clicked(); break;
        case 10: _t->setVolume((*reinterpret_cast< const Volume*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TF1DWidget::*_t)(const QColor & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TF1DWidget::colorChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (TF1DWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TF1DWidget::tfChanged)) {
                *result = 1;
            }
        }
        {
            typedef void (TF1DWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TF1DWidget::tfReset)) {
                *result = 2;
            }
        }
        {
            typedef void (TF1DWidget::*_t)(unsigned char * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TF1DWidget::sendTF)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject TF1DWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_TF1DWidget.data,
      qt_meta_data_TF1DWidget,  qt_static_metacall, 0, 0}
};


const QMetaObject *TF1DWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TF1DWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TF1DWidget.stringdata))
        return static_cast<void*>(const_cast< TF1DWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int TF1DWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void TF1DWidget::colorChanged(const QColor & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TF1DWidget::tfChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void TF1DWidget::tfReset()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void TF1DWidget::sendTF(unsigned char * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
