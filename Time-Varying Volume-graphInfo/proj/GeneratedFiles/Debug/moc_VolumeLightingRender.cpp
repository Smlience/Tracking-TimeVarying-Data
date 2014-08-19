/****************************************************************************
** Meta object code from reading C++ file 'VolumeLightingRender.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/render/VolumeLightingRender.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VolumeLightingRender.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_VolumeLightingRender_t {
    QByteArrayData data[12];
    char stringdata[108];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_VolumeLightingRender_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_VolumeLightingRender_t qt_meta_stringdata_VolumeLightingRender = {
    {
QT_MOC_LITERAL(0, 0, 20),
QT_MOC_LITERAL(1, 21, 5),
QT_MOC_LITERAL(2, 27, 0),
QT_MOC_LITERAL(3, 28, 1),
QT_MOC_LITERAL(4, 30, 5),
QT_MOC_LITERAL(5, 36, 5),
QT_MOC_LITERAL(6, 42, 12),
QT_MOC_LITERAL(7, 55, 11),
QT_MOC_LITERAL(8, 67, 8),
QT_MOC_LITERAL(9, 76, 3),
QT_MOC_LITERAL(10, 80, 12),
QT_MOC_LITERAL(11, 93, 13)
    },
    "VolumeLightingRender\0setKa\0\0v\0setKs\0"
    "setKd\0setShininess\0setLightPos\0Vector3d\0"
    "pos\0onDataLoaded\0const Volume*\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VolumeLightingRender[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x0a,
       4,    1,   47,    2, 0x0a,
       5,    1,   50,    2, 0x0a,
       6,    1,   53,    2, 0x0a,
       7,    1,   56,    2, 0x0a,
      10,    1,   59,    2, 0x0a,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 11,    3,

       0        // eod
};

void VolumeLightingRender::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VolumeLightingRender *_t = static_cast<VolumeLightingRender *>(_o);
        switch (_id) {
        case 0: _t->setKa((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->setKs((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->setKd((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->setShininess((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->setLightPos((*reinterpret_cast< Vector3d(*)>(_a[1]))); break;
        case 5: _t->onDataLoaded((*reinterpret_cast< const Volume*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject VolumeLightingRender::staticMetaObject = {
    { &VolumeRender::staticMetaObject, qt_meta_stringdata_VolumeLightingRender.data,
      qt_meta_data_VolumeLightingRender,  qt_static_metacall, 0, 0}
};


const QMetaObject *VolumeLightingRender::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VolumeLightingRender::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_VolumeLightingRender.stringdata))
        return static_cast<void*>(const_cast< VolumeLightingRender*>(this));
    return VolumeRender::qt_metacast(_clname);
}

int VolumeLightingRender::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = VolumeRender::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
