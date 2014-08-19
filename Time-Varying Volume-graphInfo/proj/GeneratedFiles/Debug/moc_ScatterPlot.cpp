/****************************************************************************
** Meta object code from reading C++ file 'ScatterPlot.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/package/VarSelect/ScatterPlot.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ScatterPlot.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ScatterPlot_t {
    QByteArrayData data[11];
    char stringdata[97];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_ScatterPlot_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_ScatterPlot_t qt_meta_stringdata_ScatterPlot = {
    {
QT_MOC_LITERAL(0, 0, 11),
QT_MOC_LITERAL(1, 12, 7),
QT_MOC_LITERAL(2, 20, 0),
QT_MOC_LITERAL(3, 21, 20),
QT_MOC_LITERAL(4, 42, 2),
QT_MOC_LITERAL(5, 45, 18),
QT_MOC_LITERAL(6, 64, 7),
QT_MOC_LITERAL(7, 72, 2),
QT_MOC_LITERAL(8, 75, 7),
QT_MOC_LITERAL(9, 83, 8),
QT_MOC_LITERAL(10, 92, 3)
    },
    "ScatterPlot\0setData\0\0const unsigned char*\0"
    "v1\0std::pair<int,int>\0v1Range\0v2\0"
    "v2Range\0Vector3i\0dim\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ScatterPlot[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    5,   19,    2, 0x0a,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5, 0x80000000 | 3, 0x80000000 | 5, 0x80000000 | 9,    4,    6,    7,    8,   10,

       0        // eod
};

void ScatterPlot::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ScatterPlot *_t = static_cast<ScatterPlot *>(_o);
        switch (_id) {
        case 0: _t->setData((*reinterpret_cast< const unsigned char*(*)>(_a[1])),(*reinterpret_cast< std::pair<int,int>(*)>(_a[2])),(*reinterpret_cast< const unsigned char*(*)>(_a[3])),(*reinterpret_cast< std::pair<int,int>(*)>(_a[4])),(*reinterpret_cast< Vector3i(*)>(_a[5]))); break;
        default: ;
        }
    }
}

const QMetaObject ScatterPlot::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ScatterPlot.data,
      qt_meta_data_ScatterPlot,  qt_static_metacall, 0, 0}
};


const QMetaObject *ScatterPlot::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ScatterPlot::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ScatterPlot.stringdata))
        return static_cast<void*>(const_cast< ScatterPlot*>(this));
    return QWidget::qt_metacast(_clname);
}

int ScatterPlot::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
