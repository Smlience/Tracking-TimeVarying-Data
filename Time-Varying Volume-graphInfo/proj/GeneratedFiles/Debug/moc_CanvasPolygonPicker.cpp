/****************************************************************************
** Meta object code from reading C++ file 'CanvasPolygonPicker.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/package/VarSelect/CanvasPolygonPicker.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CanvasPolygonPicker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CanvasPolygonPicker_t {
    QByteArrayData data[7];
    char stringdata[113];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CanvasPolygonPicker_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CanvasPolygonPicker_t qt_meta_stringdata_CanvasPolygonPicker = {
    {
QT_MOC_LITERAL(0, 0, 19),
QT_MOC_LITERAL(1, 20, 29),
QT_MOC_LITERAL(2, 50, 0),
QT_MOC_LITERAL(3, 51, 26),
QT_MOC_LITERAL(4, 78, 25),
QT_MOC_LITERAL(5, 104, 1),
QT_MOC_LITERAL(6, 106, 5)
    },
    "CanvasPolygonPicker\0selectedPolygonOpacityChanged\0"
    "\0selectSelectedPolygonColor\0"
    "setSelectedPolygonOpacity\0v\0clear\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CanvasPolygonPicker[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       3,    0,   37,    2, 0x0a,
       4,    1,   38,    2, 0x0a,
       6,    0,   41,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,

       0        // eod
};

void CanvasPolygonPicker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CanvasPolygonPicker *_t = static_cast<CanvasPolygonPicker *>(_o);
        switch (_id) {
        case 0: _t->selectedPolygonOpacityChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->selectSelectedPolygonColor(); break;
        case 2: _t->setSelectedPolygonOpacity((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->clear(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CanvasPolygonPicker::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CanvasPolygonPicker::selectedPolygonOpacityChanged)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject CanvasPolygonPicker::staticMetaObject = {
    { &CanvasPicker::staticMetaObject, qt_meta_stringdata_CanvasPolygonPicker.data,
      qt_meta_data_CanvasPolygonPicker,  qt_static_metacall, 0, 0}
};


const QMetaObject *CanvasPolygonPicker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CanvasPolygonPicker::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CanvasPolygonPicker.stringdata))
        return static_cast<void*>(const_cast< CanvasPolygonPicker*>(this));
    return CanvasPicker::qt_metacast(_clname);
}

int CanvasPolygonPicker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CanvasPicker::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void CanvasPolygonPicker::selectedPolygonOpacityChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
