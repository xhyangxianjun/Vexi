/****************************************************************************
** Meta object code from reading C++ file 'widget_test.h'
**
** Created: Fri Nov 6 10:17:22 2020
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../widget_test.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'widget_test.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_WidgetTest[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      44,   14, // methods
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
     229,   11,   11,   11, 0x0a,
     249,   11,   11,   11, 0x0a,
     272,   11,   11,   11, 0x0a,
     287,   11,   11,   11, 0x0a,
     304,   11,   11,   11, 0x0a,
     333,   11,   11,   11, 0x0a,
     344,   11,   11,   11, 0x0a,
     365,  359,   11,   11, 0x0a,
     392,  359,   11,   11, 0x0a,
     420,  359,   11,   11, 0x0a,
     457,  451,   11,   11, 0x0a,
     485,  451,   11,   11, 0x0a,
     519,  513,   11,   11, 0x0a,
     546,  513,   11,   11, 0x0a,
     575,   11,   11,   11, 0x0a,
     593,   11,   11,   11, 0x0a,
     611,   11,   11,   11, 0x0a,
     629,   11,   11,   11, 0x0a,
     648,   11,   11,   11, 0x0a,
     667,   11,   11,   11, 0x0a,
     686,   11,   11,   11, 0x0a,
     705,   11,   11,   11, 0x0a,
     733,   11,   11,   11, 0x0a,
     761,   11,   11,   11, 0x0a,
     778,   11,   11,   11, 0x0a,
     795,   11,   11,   11, 0x0a,
     818,   11,   11,   11, 0x0a,
     842,   11,   11,   11, 0x0a,
     868,   11,   11,   11, 0x0a,
     895,   11,   11,   11, 0x0a,
     917,   11,   11,   11, 0x0a,
     945,   11,   11,   11, 0x0a,
     967,   11,   11,   11, 0x0a,
     990,   11,   11,   11, 0x0a,
    1016,   11,   11,   11, 0x0a,
    1041,   11,   11,   11, 0x0a,
    1068,   11,   11,   11, 0x0a,
    1094,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_WidgetTest[] = {
    "WidgetTest\0\0iStatisMode,iMinute,iStatisNumber\0"
    "signals_changeStatisMode(int,int,int)\0"
    "signals_ReadDistance1()\0signals_ReadDistance2()\0"
    "warningType,warningInfo\0"
    "signals_sendAlarm(int,QString)\0"
    "slots_intoWidget()\0slots_CameraOffAreet()\0"
    "slots_ChoseCamera()\0slots_ChoseErrorType()\0"
    "slots_OKSave()\0slots_OKStatis()\0"
    "slots_OKCameraSurveillance()\0slots_OK()\0"
    "slots_Cancel()\0iMode\0slots_KickModeChanged(int)\0"
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
        case 5: _t->slots_CameraOffAreet(); break;
        case 6: _t->slots_ChoseCamera(); break;
        case 7: _t->slots_ChoseErrorType(); break;
        case 8: _t->slots_OKSave(); break;
        case 9: _t->slots_OKStatis(); break;
        case 10: _t->slots_OKCameraSurveillance(); break;
        case 11: _t->slots_OK(); break;
        case 12: _t->slots_Cancel(); break;
        case 13: _t->slots_KickModeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->slots_KickModeChanged2((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->slots_KickModeChangedVEXI((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->slots_setIOCard1OutPut((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->slots_setIOCard2OutPut((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->slots_SaveModeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->slots_StatisModeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->slots_readDelay(); break;
        case 21: _t->slots_setToCard(); break;
        case 22: _t->slots_setToFile(); break;
        case 23: _t->slots_setToCard1(); break;
        case 24: _t->slots_setToCard2(); break;
        case 25: _t->slots_setToFile1(); break;
        case 26: _t->slots_setToFile2(); break;
        case 27: _t->slots_readIOCard1Distance(); break;
        case 28: _t->slots_readIOCard2Distance(); break;
        case 29: _t->slots_advance1(); break;
        case 30: _t->slots_advance2(); break;
        case 31: _t->slots_choseAllCamera(); break;
        case 32: _t->slots_choseNoneCamera(); break;
        case 33: _t->slots_choseAllErrorType(); break;
        case 34: _t->slots_choseNoneErrorType(); break;
        case 35: _t->slots_ShowErrorType(); break;
        case 36: _t->slots_updateIOcardCounter(); break;
        case 37: _t->slots_VEXIIOCardSet(); break;
        case 38: _t->slots_VEXIIOCardSave(); break;
        case 39: _t->slots_VEXIIOCardAdvance(); break;
        case 40: _t->slots_VEXIIOCardCancel(); break;
        case 41: _t->slots_CheckIsSendKickOut(); break;
        case 42: _t->slots_onStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 43: _t->CloseAssert(); break;
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
        if (_id < 44)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 44;
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
