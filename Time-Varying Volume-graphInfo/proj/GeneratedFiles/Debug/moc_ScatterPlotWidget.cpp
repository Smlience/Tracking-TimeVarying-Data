/****************************************************************************
** Meta object code from reading C++ file 'ScatterPlotWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/package/VarSelect/ScatterPlotWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ScatterPlotWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ScatterPlotWidget_t {
    QByteArrayData data[16];
    char stringdata[153];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_ScatterPlotWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_ScatterPlotWidget_t qt_meta_stringdata_ScatterPlotWidget = {
    {
QT_MOC_LITERAL(0, 0, 17),
QT_MOC_LITERAL(1, 18, 9),
QT_MOC_LITERAL(2, 28, 0),
QT_MOC_LITERAL(3, 29, 14),
QT_MOC_LITERAL(4, 44, 4),
QT_MOC_LITERAL(5, 49, 8),
QT_MOC_LITERAL(6, 58, 3),
QT_MOC_LITERAL(7, 62, 7),
QT_MOC_LITERAL(8, 70, 20),
QT_MOC_LITERAL(9, 91, 2),
QT_MOC_LITERAL(10, 94, 18),
QT_MOC_LITERAL(11, 113, 7),
QT_MOC_LITERAL(12, 121, 2),
QT_MOC_LITERAL(13, 124, 7),
QT_MOC_LITERAL(14, 132, 8),
QT_MOC_LITERAL(15, 141, 10)
    },
    "ScatterPlotWidget\0tfChanged\0\0"
    "unsigned char*\0data\0Vector2i\0dim\0"
    "setData\0const unsigned char*\0v1\0"
    "std::pair<int,int>\0v1Range\0v2\0v2Range\0"
    "Vector3i\0emitTFData\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ScatterPlotWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   29,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       7,    5,   34,    2, 0x0a,
      15,    0,   45,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 8, 0x80000000 | 10, 0x80000000 | 8, 0x80000000 | 10, 0x80000000 | 14,    9,   11,   12,   13,    6,
    QMetaType::Void,

       0        // eod
};

void ScatterPlotWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ScatterPlotWidget *_t = static_cast<ScatterPlotWidget *>(_o);
        switch (_id) {
        case 0: _t->tfChanged((*reinterpret_cast< unsigned char*(*)>(_a[1])),(*reinterpret_cast< Vector2i(*)>(_a[2]))); break;
        case 1: _t->setData((*reinterpret_cast< const unsigned char*(*)>(_a[1])),(*reinterpret_cast< std::pair<int,int>(*)>(_a[2])),(*reinterpret_cast< const unsigned char*(*)>(_a[3])),(*reinterpret_cast< std::pair<int,int>(*)>(_a[4])),(*reinterpret_cast< Vector3i(*)>(_a[5]))); break;
        case 2: _t->emitTFData(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ScatterPlotWidget::*_t)(unsigned char * , Vector2i );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ScatterPlotWidget::tfChanged)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject ScatterPlotWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ScatterPlotWidget.data,
      qt_meta_data_ScatterPlotWidget,  qt_static_metacall, 0, 0}
};


const QMetaObject *ScatterPlotWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ScatterPlotWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ScatterPlotWidget.stringdata))
        return static_cast<void*>(const_cast< ScatterPlotWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int ScatterPlotWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void ScatterPlotWidget::tfChanged(unsigned char * _t1, Vector2i _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
