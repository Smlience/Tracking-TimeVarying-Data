/****************************************************************************
** Meta object code from reading C++ file 'ScatterPlotCalc.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/package/VarSelect/ScatterPlotCalc.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ScatterPlotCalc.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ScatterPlotCalc_t {
    QByteArrayData data[16];
    char stringdata[156];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_ScatterPlotCalc_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_ScatterPlotCalc_t qt_meta_stringdata_ScatterPlotCalc = {
    {
QT_MOC_LITERAL(0, 0, 15),
QT_MOC_LITERAL(1, 16, 7),
QT_MOC_LITERAL(2, 24, 0),
QT_MOC_LITERAL(3, 25, 16),
QT_MOC_LITERAL(4, 42, 11),
QT_MOC_LITERAL(5, 54, 20),
QT_MOC_LITERAL(6, 75, 2),
QT_MOC_LITERAL(7, 78, 18),
QT_MOC_LITERAL(8, 97, 7),
QT_MOC_LITERAL(9, 105, 2),
QT_MOC_LITERAL(10, 108, 7),
QT_MOC_LITERAL(11, 116, 8),
QT_MOC_LITERAL(12, 125, 3),
QT_MOC_LITERAL(13, 129, 9),
QT_MOC_LITERAL(14, 139, 13),
QT_MOC_LITERAL(15, 153, 1)
    },
    "ScatterPlotCalc\0samples\0\0QVector<QPointF>\0"
    "dataChanged\0const unsigned char*\0v1\0"
    "std::pair<int,int>\0v1Range\0v2\0v2Range\0"
    "Vector3i\0dim\0setVolume\0const Volume*\0"
    "v\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ScatterPlotCalc[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x05,
       4,    5,   32,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
      13,    1,   43,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    1,
    QMetaType::Void, 0x80000000 | 5, 0x80000000 | 7, 0x80000000 | 5, 0x80000000 | 7, 0x80000000 | 11,    6,    8,    9,   10,   12,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 14,   15,

       0        // eod
};

void ScatterPlotCalc::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ScatterPlotCalc *_t = static_cast<ScatterPlotCalc *>(_o);
        switch (_id) {
        case 0: _t->samples((*reinterpret_cast< const QVector<QPointF>(*)>(_a[1]))); break;
        case 1: _t->dataChanged((*reinterpret_cast< const unsigned char*(*)>(_a[1])),(*reinterpret_cast< std::pair<int,int>(*)>(_a[2])),(*reinterpret_cast< const unsigned char*(*)>(_a[3])),(*reinterpret_cast< std::pair<int,int>(*)>(_a[4])),(*reinterpret_cast< Vector3i(*)>(_a[5]))); break;
        case 2: _t->setVolume((*reinterpret_cast< const Volume*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<QPointF> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ScatterPlotCalc::*_t)(const QVector<QPointF> & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ScatterPlotCalc::samples)) {
                *result = 0;
            }
        }
        {
            typedef void (ScatterPlotCalc::*_t)(const unsigned char * , std::pair<int,int> , const unsigned char * , std::pair<int,int> , Vector3i );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ScatterPlotCalc::dataChanged)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject ScatterPlotCalc::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ScatterPlotCalc.data,
      qt_meta_data_ScatterPlotCalc,  qt_static_metacall, 0, 0}
};


const QMetaObject *ScatterPlotCalc::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ScatterPlotCalc::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ScatterPlotCalc.stringdata))
        return static_cast<void*>(const_cast< ScatterPlotCalc*>(this));
    return QObject::qt_metacast(_clname);
}

int ScatterPlotCalc::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void ScatterPlotCalc::samples(const QVector<QPointF> & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ScatterPlotCalc::dataChanged(const unsigned char * _t1, std::pair<int,int> _t2, const unsigned char * _t3, std::pair<int,int> _t4, Vector3i _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
