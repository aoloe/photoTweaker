/****************************************************************************
** Meta object code from reading C++ file 'photoTweak.h'
**
** Created: Thu Dec 20 18:20:40 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "photoTweak.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'photoTweak.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PhotoTweak[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      24,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_PhotoTweak[] = {
    "PhotoTweak\0\0openMedia()\0showMedia()\0"
};

void PhotoTweak::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PhotoTweak *_t = static_cast<PhotoTweak *>(_o);
        switch (_id) {
        case 0: _t->openMedia(); break;
        case 1: _t->showMedia(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData PhotoTweak::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PhotoTweak::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_PhotoTweak,
      qt_meta_data_PhotoTweak, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PhotoTweak::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PhotoTweak::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PhotoTweak::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PhotoTweak))
        return static_cast<void*>(const_cast< PhotoTweak*>(this));
    if (!strcmp(_clname, "Ui::PhotoTweak"))
        return static_cast< Ui::PhotoTweak*>(const_cast< PhotoTweak*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int PhotoTweak::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
