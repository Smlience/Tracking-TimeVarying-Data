/****************************************************************************
** Meta object code from reading C++ file 'VolumeLightingRender2DTF.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/package/VarSelect/VolumeLightingRender2DTF.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VolumeLightingRender2DTF.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_VolumeLightingRender2DTF_t {
    QByteArrayData data[28];
    char stringdata[256];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_VolumeLightingRender2DTF_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_VolumeLightingRender2DTF_t qt_meta_stringdata_VolumeLightingRender2DTF = {
    {
QT_MOC_LITERAL(0, 0, 24),
QT_MOC_LITERAL(1, 25, 5),
QT_MOC_LITERAL(2, 31, 0),
QT_MOC_LITERAL(3, 32, 1),
QT_MOC_LITERAL(4, 34, 5),
QT_MOC_LITERAL(5, 40, 5),
QT_MOC_LITERAL(6, 46, 12),
QT_MOC_LITERAL(7, 59, 11),
QT_MOC_LITERAL(8, 71, 8),
QT_MOC_LITERAL(9, 80, 3),
QT_MOC_LITERAL(10, 84, 12),
QT_MOC_LITERAL(11, 97, 13),
QT_MOC_LITERAL(12, 111, 7),
QT_MOC_LITERAL(13, 119, 14),
QT_MOC_LITERAL(14, 134, 4),
QT_MOC_LITERAL(15, 139, 8),
QT_MOC_LITERAL(16, 148, 3),
QT_MOC_LITERAL(17, 152, 7),
QT_MOC_LITERAL(18, 160, 7),
QT_MOC_LITERAL(19, 168, 20),
QT_MOC_LITERAL(20, 189, 2),
QT_MOC_LITERAL(21, 192, 18),
QT_MOC_LITERAL(22, 211, 7),
QT_MOC_LITERAL(23, 219, 2),
QT_MOC_LITERAL(24, 222, 7),
QT_MOC_LITERAL(25, 230, 8),
QT_MOC_LITERAL(26, 239, 10),
QT_MOC_LITERAL(27, 250, 4)
    },
    "VolumeLightingRender2DTF\0setKa\0\0v\0"
    "setKs\0setKd\0setShininess\0setLightPos\0"
    "Vector3d\0pos\0onDataLoaded\0const Volume*\0"
    "set2DTF\0unsigned char*\0data\0Vector2i\0"
    "dim\0set1DTF\0setV1V2\0const unsigned char*\0"
    "v1\0std::pair<int,int>\0v1Range\0v2\0"
    "v2Range\0Vector3i\0setCurTime\0time\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VolumeLightingRender2DTF[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x0a,
       4,    1,   67,    2, 0x0a,
       5,    1,   70,    2, 0x0a,
       6,    1,   73,    2, 0x0a,
       7,    1,   76,    2, 0x0a,
      10,    1,   79,    2, 0x0a,
      12,    2,   82,    2, 0x0a,
      17,    1,   87,    2, 0x0a,
      18,    5,   90,    2, 0x0a,
      26,    1,  101,    2, 0x0a,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 11,    3,
    QMetaType::Void, 0x80000000 | 13, 0x80000000 | 15,   14,   16,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void, 0x80000000 | 19, 0x80000000 | 21, 0x80000000 | 19, 0x80000000 | 21, 0x80000000 | 25,   20,   22,   23,   24,   16,
    QMetaType::Void, QMetaType::Int,   27,

       0        // eod
};

void VolumeLightingRender2DTF::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VolumeLightingRender2DTF *_t = static_cast<VolumeLightingRender2DTF *>(_o);
        switch (_id) {
        case 0: _t->setKa((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->setKs((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->setKd((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->setShininess((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->setLightPos((*reinterpret_cast< Vector3d(*)>(_a[1]))); break;
        case 5: _t->onDataLoaded((*reinterpret_cast< const Volume*(*)>(_a[1]))); break;
        case 6: _t->set2DTF((*reinterpret_cast< unsigned char*(*)>(_a[1])),(*reinterpret_cast< Vector2i(*)>(_a[2]))); break;
        case 7: _t->set1DTF((*reinterpret_cast< unsigned char*(*)>(_a[1]))); break;
        case 8: _t->setV1V2((*reinterpret_cast< const unsigned char*(*)>(_a[1])),(*reinterpret_cast< std::pair<int,int>(*)>(_a[2])),(*reinterpret_cast< const unsigned char*(*)>(_a[3])),(*reinterpret_cast< std::pair<int,int>(*)>(_a[4])),(*reinterpret_cast< Vector3i(*)>(_a[5]))); break;
        case 9: _t->setCurTime((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject VolumeLightingRender2DTF::staticMetaObject = {
    { &VolumeRender::staticMetaObject, qt_meta_stringdata_VolumeLightingRender2DTF.data,
      qt_meta_data_VolumeLightingRender2DTF,  qt_static_metacall, 0, 0}
};


const QMetaObject *VolumeLightingRender2DTF::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VolumeLightingRender2DTF::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_VolumeLightingRender2DTF.stringdata))
        return static_cast<void*>(const_cast< VolumeLightingRender2DTF*>(this));
    return VolumeRender::qt_metacast(_clname);
}

int VolumeLightingRender2DTF::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = VolumeRender::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
