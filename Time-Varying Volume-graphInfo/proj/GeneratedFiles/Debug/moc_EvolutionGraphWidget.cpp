/****************************************************************************
** Meta object code from reading C++ file 'EvolutionGraphWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/package/VarSelect/EvolutionGraphWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'EvolutionGraphWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_EvolutionGraphWidget_t {
    QByteArrayData data[21];
    char stringdata[251];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_EvolutionGraphWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_EvolutionGraphWidget_t qt_meta_stringdata_EvolutionGraphWidget = {
    {
QT_MOC_LITERAL(0, 0, 20),
QT_MOC_LITERAL(1, 21, 9),
QT_MOC_LITERAL(2, 31, 0),
QT_MOC_LITERAL(3, 32, 14),
QT_MOC_LITERAL(4, 47, 4),
QT_MOC_LITERAL(5, 52, 8),
QT_MOC_LITERAL(6, 61, 3),
QT_MOC_LITERAL(7, 65, 7),
QT_MOC_LITERAL(8, 73, 6),
QT_MOC_LITERAL(9, 80, 15),
QT_MOC_LITERAL(10, 96, 18),
QT_MOC_LITERAL(11, 115, 5),
QT_MOC_LITERAL(12, 121, 20),
QT_MOC_LITERAL(13, 142, 16),
QT_MOC_LITERAL(14, 159, 19),
QT_MOC_LITERAL(15, 179, 22),
QT_MOC_LITERAL(16, 202, 12),
QT_MOC_LITERAL(17, 215, 1),
QT_MOC_LITERAL(18, 217, 14),
QT_MOC_LITERAL(19, 232, 5),
QT_MOC_LITERAL(20, 238, 11)
    },
    "EvolutionGraphWidget\0tfChanged\0\0"
    "unsigned char*\0data\0Vector2i\0dim\0"
    "changed\0sendTF\0sendClusterInfo\0"
    "sendVoxelCountInfo\0int**\0sendSelectedSequence\0"
    "std::vector<int>\0sendTimeNumSelected\0"
    "sendClusterNumSelected\0colorChanged\0"
    "c\0setFileDirName\0char*\0fileDirName\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_EvolutionGraphWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   64,    2, 0x05,
       7,    0,   69,    2, 0x05,
       8,    1,   70,    2, 0x05,
       9,    1,   73,    2, 0x05,
      10,    1,   76,    2, 0x05,
      12,    1,   79,    2, 0x05,
      14,    1,   82,    2, 0x05,
      15,    1,   85,    2, 0x05,
      16,    1,   88,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
      18,    1,   91,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 11,    2,
    QMetaType::Void, 0x80000000 | 13,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::QColor,   17,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 19,   20,

       0        // eod
};

void EvolutionGraphWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        EvolutionGraphWidget *_t = static_cast<EvolutionGraphWidget *>(_o);
        switch (_id) {
        case 0: _t->tfChanged((*reinterpret_cast< unsigned char*(*)>(_a[1])),(*reinterpret_cast< Vector2i(*)>(_a[2]))); break;
        case 1: _t->changed(); break;
        case 2: _t->sendTF((*reinterpret_cast< unsigned char*(*)>(_a[1]))); break;
        case 3: _t->sendClusterInfo((*reinterpret_cast< unsigned char*(*)>(_a[1]))); break;
        case 4: _t->sendVoxelCountInfo((*reinterpret_cast< int**(*)>(_a[1]))); break;
        case 5: _t->sendSelectedSequence((*reinterpret_cast< std::vector<int>(*)>(_a[1]))); break;
        case 6: _t->sendTimeNumSelected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->sendClusterNumSelected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->colorChanged((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 9: _t->setFileDirName((*reinterpret_cast< char*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (EvolutionGraphWidget::*_t)(unsigned char * , Vector2i );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&EvolutionGraphWidget::tfChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (EvolutionGraphWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&EvolutionGraphWidget::changed)) {
                *result = 1;
            }
        }
        {
            typedef void (EvolutionGraphWidget::*_t)(unsigned char * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&EvolutionGraphWidget::sendTF)) {
                *result = 2;
            }
        }
        {
            typedef void (EvolutionGraphWidget::*_t)(unsigned char * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&EvolutionGraphWidget::sendClusterInfo)) {
                *result = 3;
            }
        }
        {
            typedef void (EvolutionGraphWidget::*_t)(int * * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&EvolutionGraphWidget::sendVoxelCountInfo)) {
                *result = 4;
            }
        }
        {
            typedef void (EvolutionGraphWidget::*_t)(std::vector<int> );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&EvolutionGraphWidget::sendSelectedSequence)) {
                *result = 5;
            }
        }
        {
            typedef void (EvolutionGraphWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&EvolutionGraphWidget::sendTimeNumSelected)) {
                *result = 6;
            }
        }
        {
            typedef void (EvolutionGraphWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&EvolutionGraphWidget::sendClusterNumSelected)) {
                *result = 7;
            }
        }
        {
            typedef void (EvolutionGraphWidget::*_t)(const QColor & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&EvolutionGraphWidget::colorChanged)) {
                *result = 8;
            }
        }
    }
}

const QMetaObject EvolutionGraphWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_EvolutionGraphWidget.data,
      qt_meta_data_EvolutionGraphWidget,  qt_static_metacall, 0, 0}
};


const QMetaObject *EvolutionGraphWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *EvolutionGraphWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EvolutionGraphWidget.stringdata))
        return static_cast<void*>(const_cast< EvolutionGraphWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int EvolutionGraphWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void EvolutionGraphWidget::tfChanged(unsigned char * _t1, Vector2i _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void EvolutionGraphWidget::changed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void EvolutionGraphWidget::sendTF(unsigned char * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void EvolutionGraphWidget::sendClusterInfo(unsigned char * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void EvolutionGraphWidget::sendVoxelCountInfo(int * * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void EvolutionGraphWidget::sendSelectedSequence(std::vector<int> _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void EvolutionGraphWidget::sendTimeNumSelected(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void EvolutionGraphWidget::sendClusterNumSelected(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void EvolutionGraphWidget::colorChanged(const QColor & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_END_MOC_NAMESPACE
