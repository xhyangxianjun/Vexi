/****************************************************************************
** Meta object code from reading C++ file 'widget_count.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../widget_count.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'widget_count.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Widget_Count[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      19,   13,   14,   13, 0x0a,
      38,   13,   14,   13, 0x0a,
      60,   13,   14,   13, 0x0a,
      85,   13,   13,   13, 0x08,
     113,   13,   13,   13, 0x08,
     132,   13,   13,   13, 0x08,
     153,   13,   14,   13, 0x08,
     185,   13,   14,   13, 0x08,
     208,   13,   13,   13, 0x08,
     229,   13,   13,   13, 0x08,
     254,   13,   13,   13, 0x08,
     270,   13,   13,   13, 0x08,
     290,   13,   13,   13, 0x08,
     303,   13,   13,   13, 0x08,
     322,   13,   14,   13, 0x08,
     352,   13,   14,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Widget_Count[] = {
    "Widget_Count\0\0bool\0slots_intoWidget()\0"
    "slots_SaveCountInfo()\0slots_SaveALLCountInfo()\0"
    "slots_timerClearCountInfo()\0"
    "slots_OpenRecord()\0slots_DeleteRecord()\0"
    "slots_SaveCountHourInfoBytime()\0"
    "slots_ClearCountInfo()\0slots_saveRecordOK()\0"
    "slots_saveRecordCancel()\0slots_shiftOK()\0"
    "slots_shiftCancel()\0slots_turn()\0"
    "slots_updateInfo()\0slots_InsertCountCameraInfo()\0"
    "slots_InsertCountHourInfo()\0"
};

void Widget_Count::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Widget_Count *_t = static_cast<Widget_Count *>(_o);
        switch (_id) {
        case 0: { bool _r = _t->slots_intoWidget();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 1: { bool _r = _t->slots_SaveCountInfo();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 2: { bool _r = _t->slots_SaveALLCountInfo();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 3: _t->slots_timerClearCountInfo(); break;
        case 4: _t->slots_OpenRecord(); break;
        case 5: _t->slots_DeleteRecord(); break;
        case 6: { bool _r = _t->slots_SaveCountHourInfoBytime();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 7: { bool _r = _t->slots_ClearCountInfo();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 8: _t->slots_saveRecordOK(); break;
        case 9: _t->slots_saveRecordCancel(); break;
        case 10: _t->slots_shiftOK(); break;
        case 11: _t->slots_shiftCancel(); break;
        case 12: _t->slots_turn(); break;
        case 13: _t->slots_updateInfo(); break;
        case 14: { bool _r = _t->slots_InsertCountCameraInfo();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 15: { bool _r = _t->slots_InsertCountHourInfo();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Widget_Count::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Widget_Count::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Widget_Count,
      qt_meta_data_Widget_Count, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Widget_Count::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Widget_Count::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Widget_Count::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Widget_Count))
        return static_cast<void*>(const_cast< Widget_Count*>(this));
    return QWidget::qt_metacast(_clname);
}

int Widget_Count::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}
QT_END_MOC_NAMESPACE