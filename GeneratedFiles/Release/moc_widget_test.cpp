/****************************************************************************
** Meta object code from reading C++ file 'widget_test.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../widget_test.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'widget_test.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_WidgetTest[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      43,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      46,   12,   11,   11, 0x05,
      84,   11,   11,   11, 0x05,
     108,   11,   11,   11, 0x05,
     156,  132,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
     187,   11,   11,   11, 0x0a,
     206,   11,   11,   11, 0x0a,
     226,   11,   11,   11, 0x0a,
     249,   11,   11,   11, 0x0a,
     264,   11,   11,   11, 0x0a,
     281,   11,   11,   11, 0x0a,
     310,   11,   11,   11, 0x0a,
     321,   11,   11,   11, 0x0a,
     342,  336,   11,   11, 0x0a,
     369,  336,   11,   11, 0x0a,
     397,  336,   11,   11, 0x0a,
     434,  428,   11,   11, 0x0a,
     462,  428,   11,   11, 0x0a,
     496,  490,   11,   11, 0x0a,
     523,  490,   11,   11, 0x0a,
     552,   11,   11,   11, 0x0a,
     570,   11,   11,   11, 0x0a,
     588,   11,   11,   11, 0x0a,
     606,   11,   11,   11, 0x0a,
     625,   11,   11,   11, 0x0a,
     644,   11,   11,   11, 0x0a,
     663,   11,   11,   11, 0x0a,
     682,   11,   11,   11, 0x0a,
     710,   11,   11,   11, 0x0a,
     738,   11,   11,   11, 0x0a,
     755,   11,   11,   11, 0x0a,
     772,   11,   11,   11, 0x0a,
     795,   11,   11,   11, 0x0a,
     819,   11,   11,   11, 0x0a,
     845,   11,   11,   11, 0x0a,
     872,   11,   11,   11, 0x0a,
     894,   11,   11,   11, 0x0a,
     922,   11,   11,   11, 0x0a,
     944,   11,   11,   11, 0x0a,
     967,   11,   11,   11, 0x0a,
     993,   11,   11,   11, 0x0a,
    1018,   11,   11,   11, 0x0a,
    1045,   11,   11,   11, 0x0a,
    1071,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_WidgetTest[] = {
    "WidgetTest\0\0iStatisMode,iMinute,iStatisNumber\0"
    "signals_changeStatisMode(int,int,int)\0"
    "signals_ReadDistance1()\0signals_ReadDistance2()\0"
    "warningType,warningInfo\0"
    "signals_sendAlarm(int,QString)\0"
    "slots_intoWidget()\0slots_ChoseCamera()\0"
    "slots_ChoseErrorType()\0slots_OKSave()\0"
    "slots_OKStatis()\0slots_OKCameraSurveillance()\0"
    "slots_OK()\0slots_Cancel()\0iMode\0"
    "slots_KickModeChanged(int)\0"
    "slots_KickModeChanged2(int)\0"
    "slots_KickModeChangedVEXI(int)\0ComNo\0"
    "slots_setIOCard1OutPut(int)\0"
    "slots_setIOCard2OutPut(int)\0index\0"
    "slots_SaveModeChanged(int)\0"
    "slots_StatisModeChanged(int)\0"
    "slots_readDelay()\0slots_setToCard()\0"
    "slots_setToFile()\0slots_setToCard1()\0"
    "slots_setToCard2()\0slots_setToFile1()\0"
    "slots_setToFile2()\0slots_readIOCard1Distance()\0"
    "slots_readIOCard2Distance()\0"
    "slots_advance1()\0slots_advance2()\0"
    "slots_choseAllCamera()\0slots_choseNoneCamera()\0"
    "slots_choseAllErrorType()\0"
    "slots_choseNoneErrorType()\0"
    "slots_ShowErrorType()\0slots_updateIOcardCounter()\0"
    "slots_VEXIIOCardSet()\0slots_VEXIIOCardSave()\0"
    "slots_VEXIIOCardAdvance()\0"
    "slots_VEXIIOCardCancel()\0"
    "slots_CheckIsSendKickOut()\0"
    "slots_onStateChanged(int)\0CloseAssert()\0"
};

void WidgetTest::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        WidgetTest *_t = static_cast<WidgetTest *>(_o);
        switch (_id) {
        case 0: _t->signals_changeStatisMode((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->signals_ReadDistance1(); break;
        case 2: _t->signals_ReadDistance2(); break;
        case 3: _t->signals_sendAlarm((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 4: _t->slots_intoWidget(); break;
        case 5: _t->slots_ChoseCamera(); break;
        case 6: _t->slots_ChoseErrorType(); break;
        case 7: _t->slots_OKSave(); break;
        case 8: _t->slots_OKStatis(); break;
        case 9: _t->slots_OKCameraSurveillance(); break;
        case 10: _t->slots_OK(); break;
        case 11: _t->slots_Cancel(); break;
        case 12: _t->slots_KickModeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->slots_KickModeChanged2((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->slots_KickModeChangedVEXI((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->slots_setIOCard1OutPut((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->slots_setIOCard2OutPut((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->slots_SaveModeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->slots_StatisModeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->slots_readDelay(); break;
        case 20: _t->slots_setToCard(); break;
        case 21: _t->slots_setToFile(); break;
        case 22: _t->slots_setToCard1(); break;
        case 23: _t->slots_setToCard2(); break;
        case 24: _t->slots_setToFile1(); break;
        case 25: _t->slots_setToFile2(); break;
        case 26: _t->slots_readIOCard1Distance(); break;
        case 27: _t->slots_readIOCard2Distance(); break;
        case 28: _t->slots_advance1(); break;
        case 29: _t->slots_advance2(); break;
        case 30: _t->slots_choseAllCamera(); break;
        case 31: _t->slots_choseNoneCamera(); break;
        case 32: _t->slots_choseAllErrorType(); break;
        case 33: _t->slots_choseNoneErrorType(); break;
        case 34: _t->slots_ShowErrorType(); break;
        case 35: _t->slots_updateIOcardCounter(); break;
        case 36: _t->slots_VEXIIOCardSet(); break;
        case 37: _t->slots_VEXIIOCardSave(); break;
        case 38: _t->slots_VEXIIOCardAdvance(); break;
        case 39: _t->slots_VEXIIOCardCancel(); break;
        case 40: _t->slots_CheckIsSendKickOut(); break;
        case 41: _t->slots_onStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 42: _t->CloseAssert(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData WidgetTest::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject WidgetTest::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_WidgetTest,
      qt_meta_data_WidgetTest, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &WidgetTest::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *WidgetTest::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *WidgetTest::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WidgetTest))
        return static_cast<void*>(const_cast< WidgetTest*>(this));
    return QWidget::qt_metacast(_clname);
}

int WidgetTest::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 43)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 43;
    }
    return _id;
}

// SIGNAL 0
void WidgetTest::signals_changeStatisMode(int _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void WidgetTest::signals_ReadDistance1()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void WidgetTest::signals_ReadDistance2()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void WidgetTest::signals_sendAlarm(int _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
