/****************************************************************************
** Meta object code from reading C++ file 'VolumeRender.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/render/VolumeRender.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VolumeRender.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_VolumeRender_t {
    QByteArrayData data[15];
    char stringdata[147];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_VolumeRender_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_VolumeRender_t qt_meta_stringdata_VolumeRender = {
    {
QT_MOC_LITERAL(0, 0, 12),
QT_MOC_LITERAL(1, 13, 10),
QT_MOC_LITERAL(2, 24, 0),
QT_MOC_LITERAL(3, 25, 13),
QT_MOC_LITERAL(4, 39, 15),
QT_MOC_LITERAL(5, 55, 5),
QT_MOC_LITERAL(6, 61, 11),
QT_MOC_LITERAL(7, 73, 8),
QT_MOC_LITERAL(8, 82, 11),
QT_MOC_LITERAL(9, 94, 8),
QT_MOC_LITERAL(10, 103, 12),
QT_MOC_LITERAL(11, 116, 1),
QT_MOC_LITERAL(12, 118, 7),
QT_MOC_LITERAL(13, 126, 14),
QT_MOC_LITERAL(14, 141, 4)
    },
    "VolumeRender\0dataLoaded\0\0const Volume*\0"
    "sendFileDirName\0char*\0fileDirName\0"
    "loadData\0const char*\0filename\0"
    "onDataLoaded\0v\0set1DTF\0unsigned char*\0"
    "data\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VolumeRender[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x05,
       1,    0,   47,    2, 0x05,
       4,    1,   48,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       7,    1,   51,    2, 0x0a,
      10,    1,   54,    2, 0x0a,
      12,    1,   57,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 3,   11,
    QMetaType::Void, 0x80000000 | 13,   14,

       0        // eod
};

void VolumeRender::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VolumeRender *_t = static_cast<VolumeRender *>(_o);
        switch (_id) {
        case 0: _t->dataLoaded((*reinterpret_cast< const Volume*(*)>(_a[1]))); break;
        case 1: _t->dataLoaded(); break;
        case 2: _t->sendFileDirName((*reinterpret_cast< char*(*)>(_a[1]))); break;
        case 3: _t->loadData((*reinterpret_cast< const char*(*)>(_a[1]))); break;
        case 4: _t->onDataLoaded((*reinterpret_cast< const Volume*(*)>(_a[1]))); break;
        case 5: _t->set1DTF((*reinterpret_cast< unsigned char*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (VolumeRender::*_t)(const Volume * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VolumeRender::dataLoaded)) {
                *result = 0;
            }
        }
        {
            typedef void (VolumeRender::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VolumeRender::dataLoaded)) {
                *result = 1;
            }
        }
        {
            typedef void (VolumeRender::*_t)(char * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VolumeRender::sendFileDirName)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject VolumeRender::staticMetaObject = {
    { &QGLCanvas::staticMetaObject, qt_meta_stringdata_VolumeRender.data,
      qt_meta_data_VolumeRender,  qt_static_metacall, 0, 0}
};


const QMetaObject *VolumeRender::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VolumeRender::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_VolumeRender.stringdata))
        return static_cast<void*>(const_cast< VolumeRender*>(this));
    return QGLCanvas::qt_metacast(_clname);
}

int VolumeRender::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLCanvas::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void VolumeRender::dataLoaded(const Volume * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void VolumeRender::dataLoaded()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void VolumeRender::sendFileDirName(char * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
