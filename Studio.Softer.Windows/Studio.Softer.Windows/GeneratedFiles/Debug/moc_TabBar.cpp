/****************************************************************************
** Meta object code from reading C++ file 'TabBar.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../TabBar.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TabBar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Studio__Softer__Windows__TabItem_t {
    QByteArrayData data[1];
    char stringdata0[33];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Studio__Softer__Windows__TabItem_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Studio__Softer__Windows__TabItem_t qt_meta_stringdata_Studio__Softer__Windows__TabItem = {
    {
QT_MOC_LITERAL(0, 0, 32) // "Studio::Softer::Windows::TabItem"

    },
    "Studio::Softer::Windows::TabItem"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Studio__Softer__Windows__TabItem[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void Studio::Softer::Windows::TabItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject Studio::Softer::Windows::TabItem::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Studio__Softer__Windows__TabItem.data,
      qt_meta_data_Studio__Softer__Windows__TabItem,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Studio::Softer::Windows::TabItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Studio::Softer::Windows::TabItem::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Studio__Softer__Windows__TabItem.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Studio::Softer::Windows::TabItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_Studio__Softer__Windows__TabBar_t {
    QByteArrayData data[12];
    char stringdata0[115];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Studio__Softer__Windows__TabBar_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Studio__Softer__Windows__TabBar_t qt_meta_stringdata_Studio__Softer__Windows__TabBar = {
    {
QT_MOC_LITERAL(0, 0, 31), // "Studio::Softer::Windows::TabBar"
QT_MOC_LITERAL(1, 32, 8), // "tabMoved"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 10), // "tabClicked"
QT_MOC_LITERAL(4, 53, 6), // "addTab"
QT_MOC_LITERAL(5, 60, 4), // "icon"
QT_MOC_LITERAL(6, 65, 7), // "toolTip"
QT_MOC_LITERAL(7, 73, 9), // "insertTab"
QT_MOC_LITERAL(8, 83, 5), // "index"
QT_MOC_LITERAL(9, 89, 9), // "removeTab"
QT_MOC_LITERAL(10, 99, 7), // "hideTab"
QT_MOC_LITERAL(11, 107, 7) // "showTab"

    },
    "Studio::Softer::Windows::TabBar\0"
    "tabMoved\0\0tabClicked\0addTab\0icon\0"
    "toolTip\0insertTab\0index\0removeTab\0"
    "hideTab\0showTab"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Studio__Softer__Windows__TabBar[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   59,    2, 0x06 /* Public */,
       3,    1,   64,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    2,   67,    2, 0x0a /* Public */,
       4,    1,   72,    2, 0x2a /* Public | MethodCloned */,
       7,    3,   75,    2, 0x0a /* Public */,
       7,    2,   82,    2, 0x2a /* Public | MethodCloned */,
       9,    1,   87,    2, 0x0a /* Public */,
      10,    1,   90,    2, 0x0a /* Public */,
      11,    1,   93,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QIcon, QMetaType::QString,    5,    6,
    QMetaType::Void, QMetaType::QIcon,    5,
    QMetaType::Void, QMetaType::QIcon, QMetaType::Int, QMetaType::QString,    5,    8,    6,
    QMetaType::Void, QMetaType::QIcon, QMetaType::Int,    5,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,

       0        // eod
};

void Studio::Softer::Windows::TabBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TabBar *_t = static_cast<TabBar *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->tabMoved((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->tabClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->addTab((*reinterpret_cast< QIcon(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: _t->addTab((*reinterpret_cast< QIcon(*)>(_a[1]))); break;
        case 4: _t->insertTab((*reinterpret_cast< QIcon(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 5: _t->insertTab((*reinterpret_cast< QIcon(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->removeTab((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->hideTab((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->showTab((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TabBar::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TabBar::tabMoved)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (TabBar::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TabBar::tabClicked)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject Studio::Softer::Windows::TabBar::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Studio__Softer__Windows__TabBar.data,
      qt_meta_data_Studio__Softer__Windows__TabBar,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Studio::Softer::Windows::TabBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Studio::Softer::Windows::TabBar::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Studio__Softer__Windows__TabBar.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Studio::Softer::Windows::TabBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void Studio::Softer::Windows::TabBar::tabMoved(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Studio::Softer::Windows::TabBar::tabClicked(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
