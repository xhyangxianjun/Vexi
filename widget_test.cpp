#include "widget_test.h"
#include "glasswaredetectsystem.h"

extern GlasswareDetectSystem *pMainFrm;

WidgetTest::WidgetTest(QWidget *parent)
	: QWidget(parent)
{
	m_lastAllNumber=0;
	iSaveMode = -1;
	ui.setupUi(this);
	ui.checkBox_saveFailureNormalImage->setEnabled(ui.widget_StatisErrorType);
	ui.checkBox_saveFailureStressImage->setEnabled(ui.widget_StatisErrorType);
 	checkBoxAll = new QCheckBox(ui.widget_StatisErrorType);
	checkBoxAll->setText(QString::fromLocal8Bit("ȫѡ"));
	connect(checkBoxAll,SIGNAL(stateChanged(int)),this,SLOT(slots_onStateChanged(int)));
 	QLabel* labelMin = new QLabel(ui.widget_StatisErrorType);
 	labelMin->setText(QString::fromLocal8Bit("��󱨾�����:"));
 	QLabel* labelMax = new QLabel(ui.widget_StatisErrorType);
 	labelMax->setText(QString::fromLocal8Bit("����ʱ����(m):"));
 	ui.layoutStatisErrorType->addWidget(checkBoxAll,0,0,1,1);
 	ui.layoutStatisErrorType->addWidget(labelMin,0,1,1,1);
 	ui.layoutStatisErrorType->addWidget(labelMax,0,2,1,1);


	for (int i=1; i <= pMainFrm->m_sErrorInfo.m_iErrorTypeCount;i++)
	{
		QCheckBox* checkBoxError = new QCheckBox(ui.widget_StatisErrorType);
		checkBoxError->setText(pMainFrm->m_sErrorInfo.m_vstrErrorType.at(i));

		QSpinBox *spinboxMin = new QSpinBox();
		QSpinBox *spinboxMax = new QSpinBox();
		spinboxMax->setMaximum(999);
		spinboxMin->setMaximum(20);
		spinboxMax->setAlignment(Qt::AlignRight);
		spinboxMin->setAlignment(Qt::AlignRight);
		ui.layoutStatisErrorType->addWidget(checkBoxError,i,0,1,1);
		ui.layoutStatisErrorType->addWidget(spinboxMin,i,1,1,1,Qt::AlignLeft);
		ui.layoutStatisErrorType->addWidget(spinboxMax,i,2,1,1,Qt::AlignLeft);
		listCheckBoxErrorType.append(checkBoxError);
		listSpinboxStatisMaxNumber.append(spinboxMin);
		listSpinboxStatisMaxTime.append(spinboxMax);
		
	}
	ui.radioButton_alternate->setVisible(false);
	ui.radioButton_alternate_2->setVisible(false);
	if (pMainFrm->m_sSystemInfo.m_iIs3Sensor)
	{
		ui.label_senser2to3distance->setText(tr("photosensor1-3distance:"));
	}
	int widgetWidth = pMainFrm->statked_widget->geometry().width();
	//setMinimumSize(screenRect.width(),screenRect.height());

	init();
	initWidgetName();

	ui.label_14->setVisible(false);
	ui.Distance4to5->setVisible(false);
	if (!pMainFrm->m_sSystemInfo.iIsSample)
	{
		widget_Sample->setVisible(false);
		ui.widget_sampleIOCardSet->setVisible(false);
	} 
	else
	{
		widget_Sample->setVisible(true);
		ui.widget_sampleIOCardSet->setVisible(true);
	}
	if (0 == pMainFrm->m_sSystemInfo.m_iSystemType||4 == pMainFrm->m_sSystemInfo.m_iSystemType)
	{
		ui.Delay7->setVisible(false);
		ui.Delay8->setVisible(false);
	}
	ui.btnChoseCamera->setVisible(false);
	ui.btnChoseErrorType->setVisible(false);
	timerUpdateIOCardCounter = new QTimer(this);
	timerUpdateIOCardCounter->setInterval(100);//ÿ����ˢ��һ�μ���
 	connect(timerUpdateIOCardCounter, SIGNAL(timeout()), this, SLOT(slots_updateIOcardCounter()));   
	bIsShowStatisErrorType = false;
	ui.widget_StatisErrorType->setVisible(false);
	ui.checkBox_DoubleReject->setVisible(false);
	ui.label_31->setVisible(false);
	ui.lineEdit_DoubleRejectDelay->setVisible(false);
	ui.label_35->setVisible(false);
	ui.lineEdit_DoubleRejectWidth->setVisible(false);
	//Ϊ��̩�ͻ����ӱ�������
	kickOutTimer = new QTimer(this);
	ui.label_alarmInterval->setText(QString::fromLocal8Bit("���౨�����(m):"));
	ui.comboBox_StatisMode->insertItem(3,QString::fromLocal8Bit("���߷���ͳ��"));
	connect(kickOutTimer, SIGNAL(timeout()), this, SLOT(slots_CheckIsSendKickOut()));


}
WidgetTest::~WidgetTest()
{

}
void WidgetTest::slots_intoWidget()
{		
	MinRate=pMainFrm->MinRate;
	MaxRate=pMainFrm->MaxRate;
	resultTime=iStatisNumber;
	timerUpdateIOCardCounter->stop();
	Sleep(10);
	timerUpdateIOCardCounter->start();
//	initInformation();

	ui.comboBox_StatisMode->setCurrentIndex(pMainFrm->m_sSystemInfo.m_iIsTrackStatistics);
// 	ui.comboBox_StatisErrorType->setCurrentIndex(pMainFrm->m_sSystemInfo.m_iIsTrackErrorType);
	ui.spinBox_minute->setValue(pMainFrm->m_sSystemInfo.m_iTrackTime);
	ui.spinBox_Number_2->setValue(pMainFrm->m_sSystemInfo.m_iTrackNumber);
// 	ui.spinBox_MaxRate->setValue(pMainFrm->m_sSystemInfo.m_iTrackAlertRateMax[0]);
// 	ui.spinBox_MinRate->setValue(pMainFrm->m_sSystemInfo.m_iTrackAlertRateMin[0]);
	slots_StatisModeChanged(pMainFrm->m_sSystemInfo.m_iIsTrackStatistics);

// 	ui.checkBox_CameraOffLine->setChecked(pMainFrm->m_sSystemInfo.bCameraOffLineSurveillance);
// 	ui.checkBox_CameraContinueReject->setChecked(pMainFrm->m_sSystemInfo.bCameraContinueRejectSurveillance);
// 	ui.spinBox_OffLineNumber->setValue(pMainFrm->m_sSystemInfo.iCamOfflineNo);
// 	ui.spinBox_RejectNo->setValue(pMainFrm->m_sSystemInfo.iCamContinueRejectNumber);

	if (pMainFrm->m_sSystemInfo.m_iSaveNormalErrorImageByTime)
	{
		ui.checkBox_saveFailureNormalImage->setChecked(true);
	}
	else{
		ui.checkBox_saveFailureNormalImage->setChecked(false);
	}
	if (pMainFrm->m_sSystemInfo.m_iSaveStressErrorImageByTime)
	{
		ui.checkBox_saveFailureStressImage->setChecked(true);
	}
	else{
		ui.checkBox_saveFailureStressImage->setChecked(false);
	}

	switch (pMainFrm->m_sRunningInfo.m_eSaveImageType)
	{
	case NotSave:
		ui.comboBox_SaveMode->setCurrentIndex(0);
		iLastSaveMode = 0;
		iSaveMode = 0;
		break;
	case AllImageInCount:
		ui.comboBox_SaveMode->setCurrentIndex(1);
		iLastSaveMode = 1;
		iSaveMode = 1;
		break;
	case FailureImageInCount:
		ui.comboBox_SaveMode->setCurrentIndex(2);
		iLastSaveMode = 2;
		iSaveMode = 2;
		break;
	}
	slots_SaveModeChanged(iSaveMode);

	iSaveImgCount = 0;
	for(int i = 0; i<pMainFrm->m_sSystemInfo.iCamCount; i++)
	{
		if (iSaveImgCount < pMainFrm->m_sRunningInfo.m_iSaveImgCount[i])
		{		
			iSaveImgCount = pMainFrm->m_sRunningInfo.m_iSaveImgCount[i];
		}
	}
	ui.spinBox_Number->setValue(iSaveImgCount);

	widget_Sample->lineEdit_sampleCount->setText(QString::number(pMainFrm->m_sSampleInfo.m_iSampleCount));
	widget_Sample->changeSimpleCount();

	initInformation();//���½ӿڿ�����
	for (int i = 0;i<pMainFrm->m_sErrorInfo.m_iErrorTypeCount;i++)
	{
		//�ֱ��ʾ���ı��������ʹ������
		iIsTrackErrorType[i] = pMainFrm->m_sSystemInfo.m_iIsTrackErrorType[i];
		iStatisMaxTime[i] = pMainFrm->m_sSystemInfo.m_iTrackAlertRateMax[i];
		iStatisMaxNumber[i] = pMainFrm->m_sSystemInfo.m_iTrackAlertRateMin[i];
		listCheckBoxErrorType.at(i)->setChecked(iIsTrackErrorType[i]);
		listSpinboxStatisMaxTime.at(i)->setValue(iStatisMaxTime[i]);
		listSpinboxStatisMaxNumber.at(i)->setValue(iStatisMaxNumber[i]);
	}


}
bool WidgetTest::leaveWidget()
{
	//disconnect(timerUpdateIOCardCounter, SIGNAL(timeout()), this, SLOT(slots_updateIOcardCounter()));   
	timerUpdateIOCardCounter->stop();

	return true;
}
void WidgetTest::init()
{
	iChoseCamera = 1;
	iChoseErrorType = 1;
	iSaveMode = 0;
	iLastSaveMode = 0;
	iKickMode = 3;
	iLastKickMode = 3;
	iSaveImgCount = 0;
	iLastSaveImgCount = 0;
	iStatisMode = pMainFrm->m_sSystemInfo.m_iIsTrackStatistics;
// 	iLastStatisMode = iStatisMode;
	for (int i = 0;i<=pMainFrm->m_sErrorInfo.m_iErrorTypeCount;i++)
	{
		iIsTrackErrorType[i] = pMainFrm->m_sSystemInfo.m_iIsTrackErrorType[i];
// 		iLastIsTrackErrorType[i] = iIsTrackErrorType[i];
		iStatisMaxNumber[i]=0;
	}
	iMinute = pMainFrm->m_sSystemInfo.m_iTrackTime;
// 	iLastMinute = iMinute;
	iStatisNumber = pMainFrm->m_sSystemInfo.m_iTrackNumber;
// 	iLastStatisNumber = iStatisNumber;

	m_nDelay1 = 0;
	m_nDelay2 = 0;
	m_nDelay3 = 0;
	m_nDelay4 = 0;
	m_nDelay5 = 0;
	m_nDelay6 = 0;
	m_nDelay7 = 0;
	m_nDelay8 = 0;

	m_nDistinct1to4 = 0;
	m_nDistinct4to5 = 0;
	m_nDistinct2to3 = 0;
	m_nDistinct3to4 = 0;

	m_nKickDelay = 0;
	m_nKickWidth = 0;
	m_nSampleDelay = 0;
	m_nSampleWidth = 0;

	if (0 == iStatisMode)
	{
		ui.label_minute->setVisible(false);
		ui.spinBox_minute->setVisible(false);
		ui.label_Number_2->setVisible(false);
		ui.spinBox_Number_2->setVisible(false);
	}

	if (1 == iStatisMode)
	{
		ui.label_minute->setVisible(true);
		ui.spinBox_minute->setVisible(true);
		ui.spinBox_minute->setValue(iMinute);

		ui.label_Number_2->setVisible(false);
		ui.spinBox_Number_2->setVisible(false);
	}
	if (2 == iStatisMode)
	{
		ui.label_minute->setVisible(false);
		ui.spinBox_minute->setVisible(false);
		ui.label_Number_2->setVisible(true);
		ui.spinBox_Number_2->setVisible(true);
		ui.spinBox_Number_2->setValue(iStatisNumber);
	}

	ui.read->setVisible(false);
	ui.label_Number->setEnabled(false);
	ui.spinBox_Number->setEnabled(false);
	ui.comboBox_SaveMode->setCurrentIndex(0);

	connect(ui.comboBox_SaveMode, SIGNAL(currentIndexChanged(int)), this, SLOT(slots_SaveModeChanged(int)));
	connect(ui.comboBox_StatisMode, SIGNAL(currentIndexChanged(int)), this, SLOT(slots_StatisModeChanged(int)));

	QButtonGroup * buttonGroup  = new QButtonGroup(this);
	buttonGroup->addButton(ui.radioButton_bad,0);
	buttonGroup->addButton(ui.radioButton_alternate,1);
	buttonGroup->addButton(ui.radioButton_good,2);
	buttonGroup->addButton(ui.radioButton_normal,3);
	connect(buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(slots_KickModeChanged(int)));
	ui.radioButton_normal->setChecked(true);

	QButtonGroup * buttonGroup2  = new QButtonGroup(this);
	buttonGroup2->addButton(ui.radioButton_bad_2,0);
	buttonGroup2->addButton(ui.radioButton_alternate_2,1);
	buttonGroup2->addButton(ui.radioButton_good_2,2);
	buttonGroup2->addButton(ui.radioButton_normal_2,3);
	connect(buttonGroup2, SIGNAL(buttonClicked(int)), this, SLOT(slots_KickModeChanged2(int)));
	ui.radioButton_normal_2->setChecked(true);

	QButtonGroup * buttonGroup3  = new QButtonGroup(this);
	buttonGroup->addButton(ui.radioButton_bad_3,0);
	buttonGroup->addButton(ui.radioButton_alternate_3,1);
	buttonGroup->addButton(ui.radioButton_good_3,2);
	buttonGroup->addButton(ui.radioButton_normal_3,3);
	connect(buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(slots_KickModeChanged(int)));
	ui.radioButton_normal_3->setChecked(true);

	QButtonGroup * buttonGroupIOCard1  = new QButtonGroup(this);
	buttonGroupIOCard1->addButton(ui.btnIOCard1Out0,0);
	buttonGroupIOCard1->addButton(ui.btnIOCard1Out1,1);
	buttonGroupIOCard1->addButton(ui.btnIOCard1Out2,2);
	buttonGroupIOCard1->addButton(ui.btnIOCard1Out3,3);
	buttonGroupIOCard1->addButton(ui.btnIOCard1Out4,4);
	buttonGroupIOCard1->addButton(ui.btnIOCard1Out5,5);
	buttonGroupIOCard1->addButton(ui.btnIOCard1Out6,6);
	connect(buttonGroupIOCard1, SIGNAL(buttonClicked(int)), this, SLOT(slots_setIOCard1OutPut(int)));

	QButtonGroup * buttonGroupIOCard2  = new QButtonGroup(this);
	buttonGroupIOCard2->addButton(ui.btnIOCard2Out0,0);
	buttonGroupIOCard2->addButton(ui.btnIOCard2Out1,1);
	buttonGroupIOCard2->addButton(ui.btnIOCard2Out2,2);
	buttonGroupIOCard2->addButton(ui.btnIOCard2Out3,3);
	buttonGroupIOCard2->addButton(ui.btnIOCard2Out4,4);
	buttonGroupIOCard2->addButton(ui.btnIOCard2Out5,5);
	buttonGroupIOCard2->addButton(ui.btnIOCard2Out6,6);
	connect(buttonGroupIOCard2, SIGNAL(buttonClicked(int)), this, SLOT(slots_setIOCard2OutPut(int)));

	widget_Sample = new Widget_Sample();
	ui.layoutSample->addWidget(widget_Sample);
	widget_Camera = new Widget_Camera();
	ui.layoutChoseCamera->addWidget(widget_Camera);
	widget_ErrorType = new Widget_ErrorType();
	ui.layoutChoseErrorType->addWidget(widget_ErrorType);

	ui.widget_IOCounter->setWidgetName(tr("Counter"));
	ui.namelayout_IOCardCounter->addWidget(ui.widget_IOCounter->widgetName);

	if (pMainFrm->m_sSystemInfo.iIOCardCount<2)
	{
		ui.groupBox_IOCard2IN->setVisible(false);
		ui.widget_IOCardTest2->setVisible(false);
	}

	ui.btnOK->setVisible(false);
	ui.btnCancel->setVisible(false);

	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(CloseAssert()));
	connect(ui.btnChoseCamera, SIGNAL(clicked()), this, SLOT(slots_ChoseCamera()));
	connect(ui.btnChoseErrorType, SIGNAL(clicked()), this, SLOT(slots_ChoseErrorType()));
	connect(ui.btnOK_Save, SIGNAL(clicked()), this, SLOT(slots_OKSave()));
	connect(ui.btnOK_Statis, SIGNAL(clicked()), this, SLOT(slots_OKStatis()));
	connect(ui.btnOK_CameraSurveillance, SIGNAL(clicked()), this, SLOT(slots_OKCameraSurveillance()));

	connect(ui.read, SIGNAL(clicked()), this, SLOT(slots_readDelay()));
	connect(ui.settocard, SIGNAL(clicked()), this, SLOT(slots_setToCard()));
	connect(ui.settofile, SIGNAL(clicked()), this, SLOT(slots_setToFile()));

	connect(ui.read1, SIGNAL(clicked()), this, SLOT(slots_readIOCard1Distance()));
	connect(ui.read2, SIGNAL(clicked()), this, SLOT(slots_readIOCard2Distance()));
	connect(ui.advance1, SIGNAL(clicked()), this, SLOT(slots_advance1()));
	connect(ui.advance2, SIGNAL(clicked()), this, SLOT(slots_advance2()));
	connect(ui.settocard_1, SIGNAL(clicked()), this, SLOT(slots_setToCard1()));
	connect(ui.settocard_2, SIGNAL(clicked()), this, SLOT(slots_setToCard2()));
	connect(ui.settofile_1, SIGNAL(clicked()), this, SLOT(slots_setToFile1()));
	connect(ui.settofile_2, SIGNAL(clicked()), this, SLOT(slots_setToFile2()));

	connect(ui.pushButton_choseAllCamera, SIGNAL(clicked()), this, SLOT(slots_choseAllCamera()));
	connect(ui.pushButton_choseNoneCamera, SIGNAL(clicked()), this, SLOT(slots_choseNoneCamera()));

	connect(ui.pushButton_choseAllErrorType, SIGNAL(clicked()), this, SLOT(slots_choseAllErrorType()));
	connect(ui.pushButton_choseNoneErrorType, SIGNAL(clicked()), this, SLOT(slots_choseNoneErrorType()));
	connect(ui.pushButton_ShowErrorType, SIGNAL(clicked()), this, SLOT(slots_ShowErrorType()));

	connect(widget_Sample->button_OK, SIGNAL(clicked()), this, SLOT(slots_OK()));

	connect(ui.pushButton_Set, SIGNAL(clicked()), this, SLOT(slots_VEXIIOCardSet()));
	connect(ui.pushButton_Save, SIGNAL(clicked()), this, SLOT(slots_VEXIIOCardSave()));
	connect(ui.pushButton_Advance, SIGNAL(clicked()), this, SLOT(slots_VEXIIOCardAdvance()));
	connect(ui.pushButton_Cancel, SIGNAL(clicked()), this, SLOT(slots_VEXIIOCardCancel()));


	connect(this,SIGNAL(signals_ReadDistance1()),this,SLOT(slots_readIOCard1Distance()));
	connect(this,SIGNAL(signals_ReadDistance2()),this,SLOT(slots_readIOCard2Distance()));
	initInformation();
}
void WidgetTest::initInformation()
{
	switch(pMainFrm->m_sSystemInfo.m_iSystemType)
	{
	case 0:
		ui.widget_IOCard2Set->setVisible(false);
		ui.label_3->setVisible(false);
		ui.Distance1to4->setVisible(false);
		ui.read1->setVisible(false);
		ui.widget_IOCardSetVEXI->setVisible(false);
		break;
	case 1:
		ui.widget_IOCard2Set->setVisible(false);
		ui.label_3->setText(tr("photosensor1-2distance:"));
		ui.widget_IOCardSetVEXI->setVisible(false);
		break;
	case 2:
		ui.widget_IOCardSetVEXI->setVisible(false);
		break;
	case 3:
		ui.widget_IOCardSetVEXI->setVisible(false);
		break;
	case 4:
		ui.widget_IOCard2Set->setVisible(false);
		ui.label_3->setText(tr("photosensor1-2distance:"));
		ui.widget_IOCardSetVEXI->setVisible(false);
		break;
	case 5:
		ui.widget_IOCardSetVEXI->setVisible(false);
		break;
	case 6:
		ui.widget_IOCardSetVEXI->setVisible(false);
		break;
	case 7:
		ui.widget_IOCard1set->setVisible(false);
		ui.widget_IOCard2Set->setVisible(false);
		ui.widget_IOCardSet->setVisible(false);

		sVEXIIOCardInfo.bCleanLightSource = pMainFrm->m_sSystemInfo.bVEXICleanLightSource;
		sVEXIIOCardInfo.nCleanLightSourceInterval = pMainFrm->m_sSystemInfo.nVEXICleanLightSourceInterval;
		sVEXIIOCardInfo.nCleanLightSourceWidth = pMainFrm->m_sSystemInfo.nVEXICleanLightSourceWidth;
		break;
	case 8:
		ui.widget_IOCard1set->setVisible(false);
		ui.widget_IOCard2Set->setVisible(false);
		ui.widget_IOCardSet->setVisible(false);

		sVEXIIOCardInfo.bCleanLightSource = pMainFrm->m_sSystemInfo.bVEXICleanLightSource;
		sVEXIIOCardInfo.nCleanLightSourceInterval = pMainFrm->m_sSystemInfo.nVEXICleanLightSourceInterval;
		sVEXIIOCardInfo.nCleanLightSourceWidth = pMainFrm->m_sSystemInfo.nVEXICleanLightSourceWidth;

		break;
	default:
		CLogFile::write(tr("No Available System Type"),AbnormityLog);
	}
	if (pMainFrm->m_sSystemInfo.m_bIsIOCardOK)
	{
		if(0 == pMainFrm->m_sSystemInfo.m_iSystemType)
		{
			m_nDelay1 = pMainFrm->m_vIOCard[0]->readParam(32);
			m_nDelay2 = pMainFrm->m_vIOCard[0]->readParam(57);
			m_nDelay3 = pMainFrm->m_vIOCard[0]->readParam(59);
			m_nDelay4 = pMainFrm->m_vIOCard[0]->readParam(61);
			m_nDelay5 = pMainFrm->m_vIOCard[0]->readParam(65);
			m_nDelay6 = pMainFrm->m_vIOCard[0]->readParam(90);
		}

		if((2 == pMainFrm->m_sSystemInfo.m_iSystemType||6 == pMainFrm->m_sSystemInfo.m_iSystemType))
		{
			m_nDelay1 = pMainFrm->m_vIOCard[0]->readParam(32);
			m_nDelay2 = pMainFrm->m_vIOCard[0]->readParam(95);
			m_nDelay3 = pMainFrm->m_vIOCard[0]->readParam(65);
			m_nDelay4 = pMainFrm->m_vIOCard[1]->readParam(32);
			m_nDelay5 = pMainFrm->m_vIOCard[1]->readParam(57);
			m_nDelay6 = pMainFrm->m_vIOCard[0]->readParam(57);
			m_nDelay7 = pMainFrm->m_vIOCard[0]->readParam(59);
			m_nDelay8 = pMainFrm->m_vIOCard[0]->readParam(90);
		}
		if(3 == pMainFrm->m_sSystemInfo.m_iSystemType)
		{
			m_nDelay1 = pMainFrm->m_vIOCard[0]->readParam(32);
			m_nDelay2 = pMainFrm->m_vIOCard[0]->readParam(95);
			m_nDelay3 = pMainFrm->m_vIOCard[1]->readParam(32);
			m_nDelay4 = pMainFrm->m_vIOCard[1]->readParam(57);
			m_nDelay5 = pMainFrm->m_vIOCard[0]->readParam(57);
			m_nDelay6 = pMainFrm->m_vIOCard[0]->readParam(59);
		}
		if(4 == pMainFrm->m_sSystemInfo.m_iSystemType)
		{
			m_nDelay1 = pMainFrm->m_vIOCard[0]->readParam(32);
			m_nDelay2 = pMainFrm->m_vIOCard[0]->readParam(65);
			m_nDelay3 = pMainFrm->m_vIOCard[0]->readParam(95);
			m_nDelay4 = pMainFrm->m_vIOCard[0]->readParam(57);
			m_nDelay5 = pMainFrm->m_vIOCard[0]->readParam(59);
			m_nDelay6 = pMainFrm->m_vIOCard[0]->readParam(90);
		}
		if(5 == pMainFrm->m_sSystemInfo.m_iSystemType)
		{
			m_nDelay1 = pMainFrm->m_vIOCard[0]->readParam(32);
			m_nDelay2 = pMainFrm->m_vIOCard[0]->readParam(57);
			m_nDelay3 = pMainFrm->m_vIOCard[0]->readParam(59);
			m_nDelay4 = pMainFrm->m_vIOCard[0]->readParam(61);
			m_nDelay5 = pMainFrm->m_vIOCard[0]->readParam(65);
			m_nDelay6 = pMainFrm->m_vIOCard[0]->readParam(90);
			m_nDelay7 = pMainFrm->m_vIOCard[1]->readParam(32);
			m_nDelay8 = pMainFrm->m_vIOCard[1]->readParam(57);
		}
		if(VexiFinishButtom == pMainFrm->m_sSystemInfo.m_iSystemType || VexiButtom == pMainFrm->m_sSystemInfo.m_iSystemType)
		{
			sVEXIIOCardInfo.nCamera1Interval = pMainFrm->m_vIOCard[0]->readParam(43);
			sVEXIIOCardInfo.nCamera2Interval = pMainFrm->m_vIOCard[0]->readParam(72);
			sVEXIIOCardInfo.nCamera3Interval = pMainFrm->m_vIOCard[0]->readParam(73);
			sVEXIIOCardInfo.nCamera4Interval = pMainFrm->m_vIOCard[0]->readParam(74);

			sVEXIIOCardInfo.nCamera1TrggerTimes = pMainFrm->m_vIOCard[0]->readParam(44);
			sVEXIIOCardInfo.nCamera2TrggerTimes = pMainFrm->m_vIOCard[0]->readParam(68);
			sVEXIIOCardInfo.nCamera3TrggerTimes = pMainFrm->m_vIOCard[0]->readParam(69);
			sVEXIIOCardInfo.nCamera4TrggerTimes = pMainFrm->m_vIOCard[0]->readParam(70);

			sVEXIIOCardInfo.nCamera1TrggerDelay = pMainFrm->m_vIOCard[0]->readParam(32);
			sVEXIIOCardInfo.nCamera2TrggerDelay = pMainFrm->m_vIOCard[0]->readParam(57);
			sVEXIIOCardInfo.nCamera3TrggerDelay = pMainFrm->m_vIOCard[0]->readParam(59);
			sVEXIIOCardInfo.nCamera4TrggerDelay = pMainFrm->m_vIOCard[0]->readParam(61);

			sVEXIIOCardInfo.nCamera1Frame = pMainFrm->m_vIOCard[0]->readParam(33);
			sVEXIIOCardInfo.nCamera2Frame = pMainFrm->m_vIOCard[0]->readParam(58);
			sVEXIIOCardInfo.nCamera3Frame = pMainFrm->m_vIOCard[0]->readParam(60);
			sVEXIIOCardInfo.nCamera4Frame = pMainFrm->m_vIOCard[0]->readParam(62);

			sVEXIIOCardInfo.nRejectDelay = pMainFrm->m_vIOCard[0]->readParam(49);
			sVEXIIOCardInfo.nRejectWidth = pMainFrm->m_vIOCard[0]->readParam(46);
			sVEXIIOCardInfo.bDoubleReject = pMainFrm->m_sSystemInfo.bVEXIDoubleReject;
			sVEXIIOCardInfo.nDoubleRejectDelay = pMainFrm->m_vIOCard[0]->readParam(76);
			sVEXIIOCardInfo.nDoubleRejectWidth = pMainFrm->m_vIOCard[0]->readParam(92);

			sVEXIIOCardInfo.bCleanLightSource = pMainFrm->m_sSystemInfo.bVEXICleanLightSource;
			sVEXIIOCardInfo.nCleanLightSourceInterval = pMainFrm->m_sSystemInfo.nVEXICleanLightSourceInterval;
			sVEXIIOCardInfo.nCleanLightSourceWidth = pMainFrm->m_sSystemInfo.nVEXICleanLightSourceWidth;

		}

		m_nKickDelay = pMainFrm->m_vIOCard[0]->readParam(49);
		m_nKickWidth = pMainFrm->m_vIOCard[0]->readParam(46);
		if (pMainFrm->m_sSystemInfo.iIsSample)
		{
			m_nSampleDelay = pMainFrm->m_vIOCard[0]->readParam(76);
			m_nSampleWidth = pMainFrm->m_vIOCard[0]->readParam(92);
		}

		m_nDistinct1to4 = pMainFrm->m_vIOCard[0]->readParam(61);
		//m_nDistinct4to5 = pMainFrm->m_pio24b.readParam(49);
		if (pMainFrm->m_sSystemInfo.iIOCardCount>1)
		{
			m_nDistinct2to3 = pMainFrm->m_vIOCard[1]->readParam(61);
			m_nDistinct3to4 = pMainFrm->m_vIOCard[1]->readParam(49);
		}

		QString str;
		str = str.setNum(m_nDistinct1to4,10);
		ui.Distance1to4->setText(str);
		str = str.setNum(m_nDistinct4to5,10);
		ui.Distance4to5->setText(str);
		str = str.setNum(m_nDistinct2to3,10);
		ui.Distance2to3->setText(str);
		str = str.setNum(m_nDistinct3to4,10);
		ui.Distance3to4->setText(str);
	}
	if(VexiFinishButtom == pMainFrm->m_sSystemInfo.m_iSystemType || VexiButtom == pMainFrm->m_sSystemInfo.m_iSystemType)
	{
		updateIOCardParamVEXI();
	}
	else
	{
		updateIOCardParam();
	}

}
void WidgetTest::initWidgetName()
{
	ui.widget_saveImageSet->setWidgetName(tr("Save Mode"));
	ui.widget_saveImageSet->widgetName->setMaximumHeight(25);
	ui.namelayout_saveImage->addWidget(ui.widget_saveImageSet->widgetName);//,Qt::AlignTop);

	ui.widget_statisticsSet->setWidgetName(tr("Statistics Mode"));
	ui.namelayout_statisticsSet->addWidget(ui.widget_statisticsSet->widgetName);

	ui.widget_CameraSurveillance->setWidgetName(tr("Camera Surveillance"));
	ui.namelayout_CameraSurveillance->addWidget(ui.widget_CameraSurveillance->widgetName);

	ui.widget_IOCard1set->setWidgetName(tr("IOCard1 Set"));
	ui.namelayout_IOCard1Set->addWidget(ui.widget_IOCard1set->widgetName);

	ui.widget_IOCard2Set->setWidgetName(tr("IOCard2 Set"));
	ui.namelayout_IOCard2Set->addWidget(ui.widget_IOCard2Set->widgetName);

	ui.widget_IOCardTest->setWidgetName(tr("IOCard Test"));
	ui.namelayout_IOCardTest->addWidget(ui.widget_IOCardTest->widgetName);

	ui.widget_IOCardSet->setWidgetName(tr("IOCard Delay"));
	ui.namelayout_IOCardDelaySet->addWidget(ui.widget_IOCardSet->widgetName);

	ui.widget_IOCardSetVEXI->setWidgetName(tr("IOCard Information"));
	ui.namelayout_IOCardSetVEXI->addWidget(ui.widget_IOCardSetVEXI->widgetName);
}
void WidgetTest::slots_ChoseCamera()
{
	if (0 == iChoseCamera)
	{
		ui.groupBox_ChoseCamera->setVisible(true);
		iChoseCamera = 1;
	}
	else
	{
		ui.groupBox_ChoseCamera->setVisible(false);
		iChoseCamera = 0;
	}
}
void WidgetTest::slots_ChoseErrorType()
{
	if (0 == iChoseErrorType)
	{
		ui.groupBox_ChoseErrorType->setVisible(true);
		iChoseErrorType = 1;
	}
	else
	{
		ui.groupBox_ChoseErrorType->setVisible(false);
		iChoseErrorType = 0;
	}
}
void WidgetTest::slots_OKSave()
{	
	QSettings settingSave(pMainFrm->m_sConfigInfo.m_strConfigPath,QSettings::IniFormat);
	settingSave.setIniCodec(QTextCodec::codecForName("GBK"));
	settingSave.beginGroup("system");
	if (ui.checkBox_saveFailureNormalImage->isChecked())
	{
		pMainFrm->m_sSystemInfo.m_iSaveNormalErrorImageByTime = 1;
		settingSave.setValue("SaveNormalErrorImageByTime",1);
	}
	else{
		pMainFrm->m_sSystemInfo.m_iSaveNormalErrorImageByTime = 0;
		settingSave.setValue("SaveNormalErrorImageByTime",0);
	}
	if (ui.checkBox_saveFailureStressImage->isChecked())
	{
		pMainFrm->m_sSystemInfo.m_iSaveStressErrorImageByTime = 1;
		settingSave.setValue("SaveStressErrorImageByTime",1);
	}
	else{
		pMainFrm->m_sSystemInfo.m_iSaveStressErrorImageByTime = 0;
		settingSave.setValue("SaveStressErrorImageByTime",0);
	}
	settingSave.endGroup();

	if	(iLastSaveMode != iSaveMode||iSaveImgCount != ui.spinBox_Number->value())
	{
		iLastSaveMode = iSaveMode;
		iLastSaveImgCount = iSaveImgCount;

		iSaveImgCount = ui.spinBox_Number->value();
		for (int i = 0;i<pMainFrm->m_sSystemInfo.iCamCount;i++ )
		{
			if (widget_Camera->bIsChosed[i])
			{
				pMainFrm->m_sRunningInfo.m_iSaveImgCount[i] = iSaveImgCount;
			}
			else
			{
				pMainFrm->m_sRunningInfo.m_iSaveImgCount[i] = 0;
			}
		}
		switch (iSaveMode)
		{
		case 0:
			pMainFrm->m_sRunningInfo.m_eSaveImageType = NotSave;
			break;
		//case 1:
		//	pMainFrm->m_sRunningInfo.m_eSaveImageType = AllImage;
		//	break;
		//case 2:
		//	pMainFrm->m_sRunningInfo.m_eSaveImageType = FailureImage;
		//	break;
		case 1:
			pMainFrm->m_sRunningInfo.m_eSaveImageType = AllImageInCount;
			break;
		case 2:
			pMainFrm->m_sRunningInfo.m_eSaveImageType = FailureImageInCount;
			break;
		}
	}
	for (int i = 0; i<CAMERA_MAX_COUNT;i++)
	{
		pMainFrm->m_sSystemInfo.m_bSaveCamera[i] = widget_Camera->bIsChosed[i];
	}
	for (int i = 0; i<ERRORTYPE_MAX_COUNT;i++)
	{
		pMainFrm->m_sSystemInfo.m_bSaveErrorType[i] = widget_ErrorType->bIsChosed[i];
	}
}
void WidgetTest::slots_OKStatis()
{
	iMinute = ui.spinBox_minute->value();
	iStatisNumber = ui.spinBox->value();
	for (int i = 0;i<pMainFrm->m_sErrorInfo.m_iErrorTypeCount;i++)
	{
		iIsTrackErrorType[i] = listCheckBoxErrorType.at(i)->isChecked();
		iStatisMaxTime[i] = listSpinboxStatisMaxTime.at(i)->value();
		iStatisMaxNumber[i] = listSpinboxStatisMaxNumber.at(i)->value();
		pMainFrm->m_sSystemInfo.m_iIsTrackErrorType[i] = iIsTrackErrorType[i];
		pMainFrm->m_sSystemInfo.m_iTrackAlertRateMax[i] = iStatisMaxTime[i];
		pMainFrm->m_sSystemInfo.m_iTrackAlertRateMin[i] = iStatisMaxNumber[i];
	}

	pMainFrm->m_sSystemInfo.m_mutexSystemInfo.lock();
	pMainFrm->m_sSystemInfo.m_iIsTrackStatistics = iStatisMode;
	pMainFrm->m_sSystemInfo.m_iTrackTime = iMinute;
	pMainFrm->m_sSystemInfo.m_iTrackNumber = iStatisNumber;
	pMainFrm->m_sSystemInfo.m_mutexSystemInfo.unlock();

//		emit signals_changeStatisMode(iStatisMode,iMinute,iStatisNumber);
	QSettings iniStatisSet(pMainFrm->m_sConfigInfo.m_strConfigPath,QSettings::IniFormat);
	iniStatisSet.setIniCodec(QTextCodec::codecForName("GBK"));

	iniStatisSet.setValue("/system/isTrackStatistics",iStatisMode);
	iniStatisSet.setValue("/system/TrackTime",iMinute);	//����ͳ��ʱ��
	iniStatisSet.setValue("/system/TrackNumber",iStatisNumber);	//����ͳ�Ƹ���
//		iniStatisSet.setValue("/system/m_iIsTrackErrorType",1);	//��������
	QString strSession;
	for (int i = 0;i<pMainFrm->m_sErrorInfo.m_iErrorTypeCount;i++)
	{
		strSession = QString("/TrackErrorType/IsTrack%1").arg(i);
		iniStatisSet.setValue(strSession,iIsTrackErrorType[i]);
		strSession = QString("/TrackErrorType/MaxRate%1").arg(i);
		iniStatisSet.setValue(strSession,iStatisMaxTime[i]);	//��������
		strSession = QString("/TrackErrorType/MinRate%1").arg(i);
		iniStatisSet.setValue(strSession,iStatisMaxNumber[i]);	//��������
		pMainFrm->AertNumber[i].clear();
	}
	int temp = GetKickOutInterval().toInt();
	if (temp>0 && ui.comboBox_StatisMode->currentIndex() == 3)
	{
		kickOutTimer->stop();
		MaxRate=GetMaxKickOutRate();
		MinRate=GetMinKickOutRate();
		resultTime=temp;
		ui.spinBox_Number_2->setValue(temp);

		pMainFrm->MaxRate=MaxRate;
		pMainFrm->MinRate=MinRate;
		kickOutTimer->setInterval(10*1000);
		kickOutTimer->start();
	}
}
void WidgetTest::slots_OKCameraSurveillance()
{
// 	pMainFrm->m_sSystemInfo.bCameraOffLineSurveillance = ui.checkBox_CameraOffLine->isChecked();
// 	pMainFrm->m_sSystemInfo.bCameraContinueRejectSurveillance = ui.checkBox_CameraContinueReject->isChecked();
// 	pMainFrm->m_sSystemInfo.iCamOfflineNo = ui.spinBox_OffLineNumber->value();
// 	pMainFrm->m_sSystemInfo.iCamContinueRejectNumber = ui.spinBox_RejectNo->value();

// 
// 	QSettings iniStatisSet(pMainFrm->m_sConfigInfo.m_strConfigPath,QSettings::IniFormat);
// 	iniStatisSet.setIniCodec(QTextCodec::codecForName("GBK"));
// 
// 	iniStatisSet.setValue("/system/bCameraOffLineSurveillance",pMainFrm->m_sSystemInfo.bCameraOffLineSurveillance);
// 	iniStatisSet.setValue("/system/bCameraContinueRejectSurveillance",pMainFrm->m_sSystemInfo.bCameraContinueRejectSurveillance);	
// 	iniStatisSet.setValue("/system/iCamOfflineNo",pMainFrm->m_sSystemInfo.iCamOfflineNo);
// 	iniStatisSet.setValue("/system/iCamContinueRejectNumber",pMainFrm->m_sSystemInfo.iCamContinueRejectNumber);

}
void WidgetTest::slots_OK()
{ 
// 	if (widget_Sample->isChanged())
	{
		saveSampleInfo();
		widget_Sample->recoverChanged();
	}
}
void WidgetTest::slots_Cancel()
{
	//�ָ��߷�
	iKickMode = iLastKickMode;
	switch (iLastKickMode)
	{
	case 0:  
		ui.radioButton_bad->setChecked(true);
		break;	
	case 1:
		ui.radioButton_alternate->setChecked(true);
		break;	
	case 2:
		ui.radioButton_good->setChecked(true);
		break;	
	case 3:
		ui.radioButton_normal->setChecked(true);
		break;
	}
	//�ָ���ͼ
	if (iSaveMode != iLastSaveMode)
	{
		iSaveMode = iLastSaveMode;
		ui.comboBox_SaveMode->setCurrentIndex(iLastSaveMode);
		iSaveImgCount = iLastSaveImgCount;
		ui.spinBox_Number->setValue(iSaveImgCount);
	}
// 	if (iStatisMode != iLastStatisMode)
// 	{
// 		iStatisMode = iLastStatisMode;
// 		iMinute = iLastMinute;
// 		iStatisNumber = iLastStatisNumber;
// 	}
// 	if (iIsTrackErrorType != iLastIsTrackErrorType)
// 	{
// 		iIsTrackErrorType = iLastIsTrackErrorType;
// 	}
}
void WidgetTest::slots_KickModeChanged(int iMode)
{
	iKickMode = iMode;
	if (iKickMode != iLastKickMode)
	{
		iLastKickMode = iKickMode;
		pMainFrm->m_sRunningInfo.m_iKickMode = iKickMode;
	}
}
void WidgetTest::slots_KickModeChanged2(int iMode)
{
	iKickMode = iMode;
	if (iKickMode != iLastKickMode)
	{
		iLastKickMode = iKickMode;
		pMainFrm->m_sRunningInfo.m_iKickMode = iKickMode;
	}
}
void WidgetTest::slots_KickModeChangedVEXI(int iMode)
{
	iKickMode2 = iMode;
	if (iKickMode2 != iLastKickMode2)
	{
		iLastKickMode2 = iKickMode2;
		pMainFrm->m_sRunningInfo.m_iKickMode2 = iKickMode2;
	}

}
void WidgetTest::slots_setIOCard1OutPut(int ComNo)
{
	if(pMainFrm->m_sSystemInfo.m_bIsIOCardOK)
	{
		pMainFrm->m_vIOCard[0]->TestOutPut(ComNo);
	}
}
void WidgetTest::slots_setIOCard2OutPut(int ComNo)
{
	if(pMainFrm->m_sSystemInfo.m_bIsIOCardOK)
	{
		pMainFrm->m_vIOCard[1]->TestOutPut(ComNo);
	}
}
void WidgetTest::slots_SaveModeChanged(int index)
{
	iSaveMode = index;
	if (1 == iSaveMode)
	{
		ui.label_Number->setEnabled(true);
		ui.spinBox_Number->setEnabled(true);
		if (1 == iChoseErrorType)
		{
			slots_ChoseErrorType();
		}
		if (0 == iChoseCamera)
		{
			slots_ChoseCamera();
		}
	}
	else if (2 == iSaveMode)
	{
		ui.label_Number->setEnabled(true);
		ui.spinBox_Number->setEnabled(true);
		if (0 == iChoseErrorType)
		{
			slots_ChoseErrorType();
		}
		if (0 == iChoseCamera)
		{
			slots_ChoseCamera();
		}

	}
	else
	{
		ui.label_Number->setEnabled(false);
		ui.spinBox_Number->setEnabled(false);
		if (1 == iChoseErrorType)
		{
			slots_ChoseErrorType();
		}
		if (1 == iChoseCamera)
		{
			slots_ChoseCamera();
		}

	}
}
void WidgetTest::slots_StatisModeChanged(int index)
{
	iStatisMode = index;
	if (0 == iStatisMode)
	{
		ui.label_minute->setVisible(false);
		ui.spinBox_minute->setVisible(false);
		ui.label_Number_2->setVisible(false);
		ui.spinBox_Number_2->setVisible(false);
		ui.pushButton_ShowErrorType->setVisible(true);
		kickOutTimer->stop();
		ui.doubleSpinBox->setVisible(false);
		ui.doubleSpinBox_2->setVisible(false);
		ui.label_36->setVisible(false);
		ui.label_37->setVisible(false);
	}
	else if (1 == iStatisMode)
	{
		ui.label_minute->setVisible(true);
		ui.spinBox_minute->setVisible(true);
		ui.label_Number_2->setVisible(false);
		ui.spinBox_Number_2->setVisible(false);
		ui.pushButton_ShowErrorType->setVisible(true);
		kickOutTimer->stop();
		ui.doubleSpinBox->setVisible(false);
		ui.doubleSpinBox_2->setVisible(false);
		ui.label_36->setVisible(false);
		ui.label_37->setVisible(false);
	}
	else if (2 == iStatisMode)
	{
		ui.label_minute->setVisible(false);
		ui.spinBox_minute->setVisible(false);
		ui.label_Number_2->setText(QApplication::translate("WidgetTest", "Number:", 0, QApplication::UnicodeUTF8));
		ui.label_Number_2->setVisible(true);
		ui.spinBox_Number_2->setVisible(true);
		ui.pushButton_ShowErrorType->setVisible(true);
		kickOutTimer->stop();
		ui.doubleSpinBox->setVisible(false);
		ui.doubleSpinBox_2->setVisible(false);
		ui.label_36->setVisible(false);
		ui.label_37->setVisible(false);
	}
	else if (3 == iStatisMode)
	{
		ui.label_minute->setVisible(false);
		ui.spinBox_minute->setVisible(false);
		ui.label_Number_2->setVisible(false);
		ui.spinBox_Number_2->setVisible(false);
		ui.pushButton_ShowErrorType->setVisible(true);
		ui.label_Number_2->setText(QString::fromLocal8Bit("�߷ϰٷ���:"));
		ui.doubleSpinBox->setVisible(true);
		ui.doubleSpinBox_2->setVisible(true);
		ui.label_36->setVisible(true);
		ui.label_37->setVisible(true);
		ui.label_36->setText(QString::fromLocal8Bit("������Сֵ(%):"));
		ui.label_37->setText(QString::fromLocal8Bit("�������ֵ(%):"));
		ui.doubleSpinBox->setValue(MinRate);
		ui.doubleSpinBox_2->setValue(MaxRate);
		ui.pushButton_ShowErrorType->setVisible(true);
		ui.spinBox->setValue(resultTime);
	}
	else
	{
		ui.label_minute->setVisible(false);
		ui.spinBox_minute->setVisible(false);
		ui.label_Number_2->setVisible(false);
		ui.spinBox_Number_2->setVisible(false);
		ui.spinBox_Number_2->setVisible(false);
		ui.pushButton_ShowErrorType->setVisible(false);
		kickOutTimer->stop();
	}
}
void WidgetTest::saveSampleInfo()
{
//  s_SampleInfo *tempSampleInfo = &widget_Sample->sSampleInfo;
	pMainFrm->mutexSetSample.lock();
	pMainFrm->m_sSampleInfo.m_iSampleTypeCount = widget_Sample->sSampleInfo.m_iSampleTypeCount;
	pMainFrm->m_sSampleInfo.m_iSampleCount = widget_Sample->sSampleInfo.m_iSampleCount;
	pMainFrm->mutexSetSample.unlock();

	for (int i = 0; i < ERRORTYPE_MAX_COUNT ;i++)
	{
		pMainFrm->m_sSampleInfo.m_bSampleType[i] = widget_Sample->sSampleInfo.m_bSampleType[i];
	}
}
void WidgetTest::slots_readDelay()
{
	QMessageBox::information(this,"Info","��ֹ��ȡ��");
	return ;
	if (pMainFrm->m_sSystemInfo.m_bIsIOCardOK)
	{
		if(0 == pMainFrm->m_sSystemInfo.m_iSystemType)//ҩ��ƿ��
		{
			m_nDelay1 = pMainFrm->m_vIOCard[0]->readParam(32);
			m_nDelay2 = pMainFrm->m_vIOCard[0]->readParam(57);
			m_nDelay3 = pMainFrm->m_vIOCard[0]->readParam(59);
			m_nDelay4 = pMainFrm->m_vIOCard[0]->readParam(61);
			m_nDelay5 = pMainFrm->m_vIOCard[0]->readParam(65);
			m_nDelay6 = pMainFrm->m_vIOCard[0]->readParam(90);

		}
		else if ((2 == pMainFrm->m_sSystemInfo.m_iSystemType||6 == pMainFrm->m_sSystemInfo.m_iSystemType))//һ���
		{
			m_nDelay1 = pMainFrm->m_vIOCard[0]->readParam(32);
			m_nDelay2 = pMainFrm->m_vIOCard[0]->readParam(95);
			m_nDelay3 = pMainFrm->m_vIOCard[0]->readParam(65);
			m_nDelay4 = pMainFrm->m_vIOCard[1]->readParam(32);
			m_nDelay5 = pMainFrm->m_vIOCard[1]->readParam(57);
			m_nDelay6 = pMainFrm->m_vIOCard[0]->readParam(57);
			m_nDelay7 = pMainFrm->m_vIOCard[0]->readParam(59);
			m_nDelay8 = pMainFrm->m_vIOCard[0]->readParam(90);
		}

		else if (3 == pMainFrm->m_sSystemInfo.m_iSystemType)//һ���
		{
			m_nDelay1 = pMainFrm->m_vIOCard[0]->readParam(32);
			m_nDelay2 = pMainFrm->m_vIOCard[0]->readParam(95);
			m_nDelay3 = pMainFrm->m_vIOCard[1]->readParam(32);
			m_nDelay4 = pMainFrm->m_vIOCard[1]->readParam(57);
			m_nDelay5 = pMainFrm->m_vIOCard[0]->readParam(57);
			m_nDelay6 = pMainFrm->m_vIOCard[0]->readParam(59);
		}

		else if (4 == pMainFrm->m_sSystemInfo.m_iSystemType)//ƿ��
		{
			m_nDelay1 = pMainFrm->m_vIOCard[0]->readParam(32);
			m_nDelay2 = pMainFrm->m_vIOCard[0]->readParam(65);
			m_nDelay3 = pMainFrm->m_vIOCard[0]->readParam(95);
			m_nDelay4 = pMainFrm->m_vIOCard[0]->readParam(57);
			m_nDelay5 = pMainFrm->m_vIOCard[0]->readParam(59);
			m_nDelay6 = pMainFrm->m_vIOCard[0]->readParam(90);

		}
		else if (5 == pMainFrm->m_sSystemInfo.m_iSystemType)//ҩ��һ���
		{
			m_nDelay1 = pMainFrm->m_vIOCard[0]->readParam(32);
			m_nDelay2 = pMainFrm->m_vIOCard[0]->readParam(57);
			m_nDelay3 = pMainFrm->m_vIOCard[0]->readParam(59);
			m_nDelay4 = pMainFrm->m_vIOCard[0]->readParam(61);
			m_nDelay5 = pMainFrm->m_vIOCard[0]->readParam(65);
			m_nDelay6 = pMainFrm->m_vIOCard[0]->readParam(90);
			if (pMainFrm->m_sSystemInfo.iIOCardCount > 1)
			{
				m_nDelay7 = pMainFrm->m_vIOCard[1]->readParam(32);
				m_nDelay8 = pMainFrm->m_vIOCard[1]->readParam(57);
			}
		}		
		m_nKickDelay = pMainFrm->m_vIOCard[0]->readParam(49);
		m_nKickWidth = pMainFrm->m_vIOCard[0]->readParam(46);
		if (pMainFrm->m_sSystemInfo.iIsSample)
		{
			m_nSampleDelay = pMainFrm->m_vIOCard[0]->readParam(76);
			m_nSampleWidth = pMainFrm->m_vIOCard[0]->readParam(92);
		}

	}
	CLogFile::write(tr("readDelay"),OperationLog,0);

	updateIOCardParam();
}
void WidgetTest::slots_setToCard()
{
	getIOCardParam();
	if (m_nDistinct1to4>0xFFFF||m_nDistinct2to3>0xFFFF||m_nDistinct3to4>0xFFFF||m_nDistinct4to5>0xFFFF\
		||m_nDelay1>0xFFFF||m_nDelay2>0xFFFF||m_nDelay3>0xFFFF||m_nDelay4>0xFFFF\
		||m_nDelay5>0xFFFF||m_nDelay6>0xFFFF||m_nDelay7>0xFFFF||m_nDelay8>0xFFFF\
		||m_nKickWidth>0xFFFF||m_nKickDelay>0xFFFF)
	{
		QString str(tr("The set value is out of range!"));
		QMessageBox::information(this,"Error",str);	
		return; 
	}
//	if (pMainFrm->m_sSystemInfo.m_bIsIOCardOK)
	{
		if (0 == pMainFrm->m_sSystemInfo.m_iSystemType)//ƿ��
		{
			pMainFrm->m_vIOCard[0]->writeParam(32,m_nDelay1);
			pMainFrm->m_vIOCard[0]->writeParam(57,m_nDelay2);
			pMainFrm->m_vIOCard[0]->writeParam(59,m_nDelay3);
			pMainFrm->m_vIOCard[0]->writeParam(61,m_nDelay4);
			pMainFrm->m_vIOCard[0]->writeParam(65,m_nDelay5);
			pMainFrm->m_vIOCard[0]->writeParam(90,m_nDelay6);
		}
		else if ((2 == pMainFrm->m_sSystemInfo.m_iSystemType||6 == pMainFrm->m_sSystemInfo.m_iSystemType))//һ���
		{
			pMainFrm->m_vIOCard[0]->writeParam(32,m_nDelay1);
			pMainFrm->m_vIOCard[0]->writeParam(95,m_nDelay2);
			pMainFrm->m_vIOCard[0]->writeParam(65,m_nDelay3);
			pMainFrm->m_vIOCard[1]->writeParam(32,m_nDelay4);
			pMainFrm->m_vIOCard[1]->writeParam(57,m_nDelay5);
			pMainFrm->m_vIOCard[0]->writeParam(57,m_nDelay6);
			pMainFrm->m_vIOCard[0]->writeParam(59,m_nDelay7);
			pMainFrm->m_vIOCard[0]->writeParam(90,m_nDelay8);

			pMainFrm->m_vIOCard[1]->writeParam(42,m_nKickDelay);
			pMainFrm->m_vIOCard[1]->writeParam(46,m_nKickWidth);
		}
		else if (3 == pMainFrm->m_sSystemInfo.m_iSystemType)//һ���
		{
			pMainFrm->m_vIOCard[0]->writeParam(32,m_nDelay1);
			pMainFrm->m_vIOCard[0]->writeParam(95,m_nDelay2);
			pMainFrm->m_vIOCard[1]->writeParam(32,m_nDelay3);
			pMainFrm->m_vIOCard[1]->writeParam(57,m_nDelay4);
			pMainFrm->m_vIOCard[0]->writeParam(57,m_nDelay5);
			pMainFrm->m_vIOCard[0]->writeParam(59,m_nDelay6);

			pMainFrm->m_vIOCard[1]->writeParam(42,m_nKickDelay);
			pMainFrm->m_vIOCard[1]->writeParam(46,m_nKickWidth);
		}
		else if (4 == pMainFrm->m_sSystemInfo.m_iSystemType)//һ���
		{
			pMainFrm->m_vIOCard[0]->writeParam(32,m_nDelay1);
			pMainFrm->m_vIOCard[0]->writeParam(65,m_nDelay2);
			pMainFrm->m_vIOCard[0]->writeParam(95,m_nDelay3);
			pMainFrm->m_vIOCard[0]->writeParam(57,m_nDelay4);
			pMainFrm->m_vIOCard[0]->writeParam(59,m_nDelay5);
			pMainFrm->m_vIOCard[0]->writeParam(90,m_nDelay6);
		}

		else if (5 == pMainFrm->m_sSystemInfo.m_iSystemType)//ҩ��һ���
		{
			pMainFrm->m_vIOCard[0]->writeParam(32,m_nDelay1);
			pMainFrm->m_vIOCard[0]->writeParam(57,m_nDelay2);
			pMainFrm->m_vIOCard[0]->writeParam(59,m_nDelay3);
			pMainFrm->m_vIOCard[0]->writeParam(61,m_nDelay4);
			pMainFrm->m_vIOCard[0]->writeParam(65,m_nDelay5);
			pMainFrm->m_vIOCard[0]->writeParam(90,m_nDelay6);

			if (pMainFrm->m_sSystemInfo.iIOCardCount > 1)
			{
				pMainFrm->m_vIOCard[1]->writeParam(32,m_nDelay7);
				pMainFrm->m_vIOCard[1]->writeParam(57,m_nDelay8);
			}

		}
		pMainFrm->m_vIOCard[0]->writeParam(49,m_nKickDelay);
		pMainFrm->m_vIOCard[0]->writeParam(46,m_nKickWidth);
	}	
	if (pMainFrm->m_sSystemInfo.iIsSample)
	{
		if (m_nSampleDelay>0xFFFF||m_nSampleWidth>0xFFFF)
		{
			QString str(tr("The Sample value is out of range!"));
			QMessageBox::information(this,"Error",str);	
			return; 
		}
		else
		{
			pMainFrm->m_vIOCard[0]->writeParam(76,m_nSampleDelay);
			pMainFrm->m_vIOCard[0]->writeParam(92,m_nSampleWidth);
		}
	}
	CLogFile::write(tr("Set to IOCard"),OperationLog,0);
}
void WidgetTest::slots_setToFile()
{
	getIOCardParam();
	if (m_nDistinct1to4>0xFFFF||m_nDistinct2to3>0xFFFF||m_nDistinct3to4>0xFFFF||m_nDistinct4to5>0xFFFF\
		||m_nDelay1>0xFFFF||m_nDelay2>0xFFFF||m_nDelay3>0xFFFF||m_nDelay4>0xFFFF\
		||m_nDelay5>0xFFFF||m_nDelay6>0xFFFF||m_nDelay7>0xFFFF||m_nDelay8>0xFFFF\
		||m_nKickWidth>0xFFFF||m_nKickDelay>0xFFFF||m_nSampleDelay>0xFFFF||m_nSampleWidth>0xFFFF)
	{
		QString str(tr("The set value is out of range!"));
		QMessageBox::information(this,"Error",str);	
		return;
	}

	if (0 == pMainFrm->m_sSystemInfo.m_iSystemType)//ƿ��
	{
		QString strBakFile = QString("./pio24b/body/PIO24B_reg_init.txt");
		QString strIniName1 = QString("./PIO24B_reg_init.txt");
		QString strValue,strPara;
		strValue = strValue.setNum(m_nDelay1,10);
		strPara = strPara.setNum(32,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nDelay2,10);
		strPara = strPara.setNum(57,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nDelay3,10);
		strPara = strPara.setNum(59,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nDelay4,10);
		strPara = strPara.setNum(61,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nDelay5,10);
		strPara = strPara.setNum(65,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nDelay6,10);
		strPara = strPara.setNum(90,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);


		strValue = strValue.setNum(m_nKickDelay,10);
		strPara = strPara.setNum(49,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nKickWidth,10);
		strPara = strPara.setNum(46,10);

		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);
		if (pMainFrm->m_sSystemInfo.iIsSample)
		{
			QString strIniName1 = QString("./PIO24B_reg_init.txt");
			QString strValue,strPara;

			strValue = strValue.setNum(m_nSampleDelay,10);
			strPara = strPara.setNum(76,10);
			StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
			StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

			strValue = strValue.setNum(m_nSampleWidth,10);
			strPara = strPara.setNum(92,10);
			StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
			StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);
		}
	}
	else if ((2 == pMainFrm->m_sSystemInfo.m_iSystemType||6 == pMainFrm->m_sSystemInfo.m_iSystemType))//һ���
	{
		QString strBakFile = QString("./pio24b/ALLINONE/14Camera/PIO24B_reg_init.txt");
		QString strIniName1 = QString("./PIO24B_reg_init.txt");
		QString strValue,strPara;
		strValue = strValue.setNum(m_nDelay1,10);
		strPara = strPara.setNum(32,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nDelay2,10);
		strPara = strPara.setNum(95,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nDelay3,10);
		strPara = strPara.setNum(65,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nDelay6,10);
		strPara = strPara.setNum(57,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nDelay7,10);
		strPara = strPara.setNum(59,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nDelay8,10);
		strPara = strPara.setNum(90,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);


		strValue = strValue.setNum(m_nKickDelay,10);
		strPara = strPara.setNum(49,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nKickWidth,10);
		strPara = strPara.setNum(46,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strBakFile = QString("./pio24b//ALLINONE/14Camera/PIO24B_reg_init1.txt");
		strIniName1 = QString("./PIO24B_reg_init1.txt");

		strValue = strValue.setNum(m_nDelay4,10);
		strPara = strPara.setNum(32,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nDelay5,10);
		strPara = strPara.setNum(57,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nKickDelay,10);
		strPara = strPara.setNum(42,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nKickWidth,10);
		strPara = strPara.setNum(46,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);
		if (pMainFrm->m_sSystemInfo.iIsSample)
		{
			QString strIniName1 = QString("./PIO24B_reg_init.txt");
			QString strValue,strPara;

			strValue = strValue.setNum(m_nSampleDelay,10);
			strPara = strPara.setNum(76,10);
			StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
			StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

			strValue = strValue.setNum(m_nSampleWidth,10);
			strPara = strPara.setNum(92,10);
			StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
			StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);
		}
	}
	else if (3 == pMainFrm->m_sSystemInfo.m_iSystemType)//һ���
	{
		QString strBakFile = QString("./pio24b//ALLINONE/10Camera/PIO24B_reg_init.txt");
		QString strIniName1 = QString("./PIO24B_reg_init.txt");
		QString strValue,strPara;
		strValue = strValue.setNum(m_nDelay1,10);
		strPara = strPara.setNum(32,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nDelay2,10);
		strPara = strPara.setNum(95,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nDelay5,10);
		strPara = strPara.setNum(57,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nDelay6,10);
		strPara = strPara.setNum(59,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nKickDelay,10);
		strPara = strPara.setNum(49,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nKickWidth,10);
		strPara = strPara.setNum(46,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strBakFile = QString("./pio24b//ALLINONE/10Camera/PIO24B_reg_init1.txt");
		strIniName1 = QString("./PIO24B_reg_init1.txt");

		strValue = strValue.setNum(m_nDelay3,10);
		strPara = strPara.setNum(32,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nDelay4,10);
		strPara = strPara.setNum(57,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nKickDelay,10);
		strPara = strPara.setNum(42,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nKickWidth,10);
		strPara = strPara.setNum(46,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);
		if (pMainFrm->m_sSystemInfo.iIsSample)
		{
			QString strIniName1 = QString("./PIO24B_reg_init.txt");
			QString strValue,strPara;

			strValue = strValue.setNum(m_nSampleDelay,10);
			strPara = strPara.setNum(76,10);
			StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
			StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

			strValue = strValue.setNum(m_nSampleWidth,10);
			strPara = strPara.setNum(92,10);
			StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
			StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);
		}
	}
	else if (4 == pMainFrm->m_sSystemInfo.m_iSystemType)//ƿ��2���
	{
		QString strBakFile = QString("./pio24b/BodySystem(2 Sensor)/PIO24B_reg_init.txt");
		QString strIniName1 = QString("./PIO24B_reg_init.txt");
		QString strValue,strPara;
		strValue = strValue.setNum(m_nDelay1,10);
		strPara = strPara.setNum(32,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nDelay2,10);
		strPara = strPara.setNum(65,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nDelay3,10);
		strPara = strPara.setNum(95,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nDelay4,10);
		strPara = strPara.setNum(57,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nDelay5,10);
		strPara = strPara.setNum(59,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nDelay6,10);
		strPara = strPara.setNum(90,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nKickDelay,10);
		strPara = strPara.setNum(49,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nKickWidth,10);
		strPara = strPara.setNum(46,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);
		if (pMainFrm->m_sSystemInfo.iIsSample)
		{
			QString strIniName1 = QString("./PIO24B_reg_init.txt");
			QString strValue,strPara;

			strValue = strValue.setNum(m_nSampleDelay,10);
			strPara = strPara.setNum(76,10);
			StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
			StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

			strValue = strValue.setNum(m_nSampleWidth,10);
			strPara = strPara.setNum(92,10);
			StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
			StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);
		}
	}
	else if (5 == pMainFrm->m_sSystemInfo.m_iSystemType)//ҩ��һ���
	{
		QString strBakFile = QString("./pio24b/ALLINONE (ҩ��)/PIO24B_reg_init.txt");
		QString strIniName1 = QString("./PIO24B_reg_init.txt");
		QString strValue,strPara;
		strValue = strValue.setNum(m_nDelay1,10);
		strPara = strPara.setNum(32,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nDelay2,10);
		strPara = strPara.setNum(57,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nDelay3,10);
		strPara = strPara.setNum(59,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nDelay4,10);
		strPara = strPara.setNum(61,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nDelay5,10);
		strPara = strPara.setNum(65,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nDelay6,10);
		strPara = strPara.setNum(90,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);
		
		strValue = strValue.setNum(m_nKickDelay,10);
		strPara = strPara.setNum(49,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nKickWidth,10);
		strPara = strPara.setNum(46,10);

		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strBakFile = QString("./pio24b/ALLINONE (ҩ��)/PIO24B_reg_init1.txt");
		strIniName1 = QString("./PIO24B_reg_init1.txt");
		strValue = strValue.setNum(m_nDelay7,10);
		strPara = strPara.setNum(32,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nDelay8,10);
		strPara = strPara.setNum(57,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);
		if (pMainFrm->m_sSystemInfo.iIsSample)
		{
			QString strIniName1 = QString("./PIO24B_reg_init.txt");
			QString strValue,strPara;

			strValue = strValue.setNum(m_nSampleDelay,10);
			strPara = strPara.setNum(76,10);
			StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
			StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

			strValue = strValue.setNum(m_nSampleWidth,10);
			strPara = strPara.setNum(92,10);
			StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
			StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);
		}
	}
	CLogFile::write(tr("Write to TXT"),OperationLog,0);
}
void WidgetTest::slots_readIOCard1Distance()
{
	if (pMainFrm->m_sSystemInfo.m_bIsIOCardOK)
	{
		if(0 == pMainFrm->m_sSystemInfo.m_iSystemType)
		{

		}
		else if(1 == pMainFrm->m_sSystemInfo.m_iSystemType)
		{
			int i,j;
			i=pMainFrm->m_vIOCard[0]->readStatus(13);
			j=pMainFrm->m_vIOCard[0]->readStatus(14); 
			m_nDistinct1to4 = j - i;
			QString str;
			str = str.setNum(m_nDistinct1to4,10);
			ui.Distance1to4->setText(str);
		}
		else if((2 == pMainFrm->m_sSystemInfo.m_iSystemType||6 == pMainFrm->m_sSystemInfo.m_iSystemType))
		{
			int i,j,K;
			i=pMainFrm->m_vIOCard[0]->readStatus(13);
			j=pMainFrm->m_vIOCard[0]->readStatus(14); 
			K=pMainFrm->m_vIOCard[0]->readStatus(15); 
			m_nDistinct1to4 = j - i;
			m_nDistinct4to5 = K - j;
			QString str;
			str = str.setNum(m_nDistinct1to4,10);
			ui.Distance1to4->setText(str);
			str = str.setNum(m_nDistinct4to5,10);
			ui.Distance4to5->setText(str);

		}
		else if(3 == pMainFrm->m_sSystemInfo.m_iSystemType)
		{
			int i,j,K;
			i=pMainFrm->m_vIOCard[0]->readStatus(13);
			j=pMainFrm->m_vIOCard[0]->readStatus(14); 
			K=pMainFrm->m_vIOCard[0]->readStatus(15); 
			m_nDistinct1to4 = j - i;
			m_nDistinct4to5 = K - j;
			QString str;
			str = str.setNum(m_nDistinct1to4,10);
			ui.Distance1to4->setText(str);
			str = str.setNum(m_nDistinct4to5,10);
			ui.Distance4to5->setText(str);

		}
		else if(4 == pMainFrm->m_sSystemInfo.m_iSystemType)
		{
			int i,j;
			i=pMainFrm->m_vIOCard[0]->readStatus(13);
			j=pMainFrm->m_vIOCard[0]->readStatus(14); 
			m_nDistinct1to4 = j - i;
			QString str;
			str = str.setNum(m_nDistinct1to4,10);
			ui.Distance1to4->setText(str);
		}
		else if(5 == pMainFrm->m_sSystemInfo.m_iSystemType)
		{
			int i,j,K;
			i=pMainFrm->m_vIOCard[0]->readStatus(13);
			j=pMainFrm->m_vIOCard[0]->readStatus(14); 
			K=pMainFrm->m_vIOCard[0]->readStatus(15); 
			m_nDistinct1to4 = j - i;
			m_nDistinct4to5 = K - j;
			QString str;
			str = str.setNum(m_nDistinct1to4,10);
			ui.Distance1to4->setText(str);
			str = str.setNum(m_nDistinct4to5,10);
			ui.Distance4to5->setText(str);
		}
	}
	CLogFile::write(tr("Read IOCard1 photosensor distance"),OperationLog,0);

}
void WidgetTest::slots_readIOCard2Distance()
{
	if (pMainFrm->m_sSystemInfo.m_bIsIOCardOK)
	{
		int i,j,K;
		i=pMainFrm->m_vIOCard[1]->readStatus(13);
		j=pMainFrm->m_vIOCard[1]->readStatus(14); 
		K=pMainFrm->m_vIOCard[1]->readStatus(15); 
		m_nDistinct2to3 = j - i;
		m_nDistinct3to4 = K - j;
		QString str;
		str = str.setNum(m_nDistinct2to3,10);
		ui.Distance2to3->setText(str);
		str = str.setNum(m_nDistinct3to4,10);
		ui.Distance3to4->setText(str);
	}	
	CLogFile::write(tr("Read IOCard2 photosensor distance"),OperationLog,0);


}
void WidgetTest::slots_advance1()
{
	if (pMainFrm->m_sSystemInfo.m_bIsIOCardOK)
	{
		pMainFrm->m_vIOCard[0]->Show_PIO24B_DebugDialog(this);//((this,"\\Config\\PIO24B_reg_init.txt_1", "PIO24B"));
	}
	else
	{
		QMessageBox::information(this,"ERROR","IOCard is not available");
	}

}
void WidgetTest::slots_advance2()
{
	if (pMainFrm->m_sSystemInfo.m_bIsIOCardOK)
	{
		pMainFrm->m_vIOCard[1]->Show_PIO24B_DebugDialog(this);//((this,"\\Config\\PIO24B_reg_init.txt_1", "PIO24B"));
	}
	else
	{
		QMessageBox::information(this,"ERROR","IOCard is not available");
	}

}
void WidgetTest::slots_setToCard1()
{
	getIOCardParam();
	if (m_nDistinct1to4>0xFFFF||m_nDistinct4to5>0xFFFF)
	{
		QString str(tr("The set value is out of range!"));
		QMessageBox::information(this,"Error",str);	
		return;
	}
	if (pMainFrm->m_sSystemInfo.m_bIsIOCardOK)
	{
		pMainFrm->m_vIOCard[0]->writeParam(61,m_nDistinct1to4);
//		pMainFrm->m_vIOCard[0]->writeParam(49,m_nDistinct4to5);
	}
	CLogFile::write(tr("SetToIOCard1"),OperationLog,0);

}
void WidgetTest::slots_setToCard2()
{
	getIOCardParam();
	if (m_nDistinct2to3>0xFFFF||m_nDistinct3to4>0xFFFF)
	{
		QString str(tr("The set value is out of range!"));
		QMessageBox::information(this,"Error",str);	
		return;
	}
	if (pMainFrm->m_sSystemInfo.m_bIsIOCardOK)
	{
		pMainFrm->m_vIOCard[1]->writeParam(61,m_nDistinct2to3);
		pMainFrm->m_vIOCard[1]->writeParam(49,m_nDistinct3to4);

	}
	CLogFile::write(tr("SetToIOCard2"),OperationLog,0);


}
void WidgetTest::slots_setToFile1()
{
	getIOCardParam();
	if (m_nDistinct1to4>0xFFFF||m_nDistinct4to5>0xFFFF)
	{
		QString str(tr("The set value is out of range!"));
		QMessageBox::information(this,"Error",str);	
		return;
	}

	if (0 == pMainFrm->m_sSystemInfo.m_iSystemType)//ƿ��
	{
		QString strBakFile = QString("./pio24b/��ϵͳ����/ƿ��ϵͳ/PIO24B_reg_init.txt");
		QString strIniName1 = QString("./PIO24B_reg_init.txt");
		QString strValue,strPara;
		strValue = strValue.setNum(m_nDistinct1to4,10);
		strPara = strPara.setNum(61,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		//strValue = strValue.setNum(m_nDistinct4to5,10);
		//strPara = strPara.setNum(49,10);
		//StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		//StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

	}
	else if (2 == pMainFrm->m_sSystemInfo.m_iSystemType || 6 == pMainFrm->m_sSystemInfo.m_iSystemType)//һ���
	{
		QString strBakFile = QString("./pio24b/��ϵͳ����/һ���/PIO24B_reg_init.txt");
		QString strIniName1 = QString("./PIO24B_reg_init.txt");
		QString strValue,strPara;
		strValue = strValue.setNum(m_nDistinct1to4,10);
		strPara = strPara.setNum(61,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		//strValue = strValue.setNum(m_nDistinct4to5,10);
		//strPara = strPara.setNum(49,10);
		//StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		//StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

	}
	else if (3 == pMainFrm->m_sSystemInfo.m_iSystemType)//һ���
	{
		QString strBakFile = QString("./pio24b/��ϵͳ����/һ���/PIO24B_reg_init.txt");
		QString strIniName1 = QString("./PIO24B_reg_init.txt");
		QString strValue,strPara;
		strValue = strValue.setNum(m_nDistinct1to4,10);
		strPara = strPara.setNum(61,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		//strValue = strValue.setNum(m_nDistinct4to5,10);
		//strPara = strPara.setNum(49,10);
		//StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		//StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

	}
	else if (4 == pMainFrm->m_sSystemInfo.m_iSystemType)//ƿ��2���
	{
		QString strBakFile = QString("./pio24b/��ϵͳ����/ƿ��2���/PIO24B_reg_init.txt");
		QString strIniName1 = QString("./PIO24B_reg_init.txt");
		QString strValue,strPara;
		strValue = strValue.setNum(m_nDistinct1to4,10);
		strPara = strPara.setNum(61,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		//strValue = strValue.setNum(m_nDistinct4to5,10);
		//strPara = strPara.setNum(49,10);
		//StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		//StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

	}
	else if (5 == pMainFrm->m_sSystemInfo.m_iSystemType)//ҩ��һ���
	{
		QString strBakFile = QString("./pio24b/��ϵͳ����/ҩ��һ���/PIO24B_reg_init1.txt");
		QString strIniName1 = QString("./PIO24B_reg_init.txt");
		QString strValue,strPara;
		strValue = strValue.setNum(m_nDistinct1to4,10);
		strPara = strPara.setNum(61,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		//strValue = strValue.setNum(m_nDistinct4to5,10);
		//strPara = strPara.setNum(49,10);
		//StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		//StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);
	}
	CLogFile::write(tr("WriteToTXT1"),OperationLog,0);

}
void WidgetTest::slots_setToFile2()
{
	getIOCardParam();
	if (m_nDistinct2to3>0xFFFF||m_nDistinct3to4>0xFFFF)
	{
		QString str(tr("The set value is out of range!"));
		QMessageBox::information(this,"Error",str);	
		return;
	}

	if ((2 == pMainFrm->m_sSystemInfo.m_iSystemType || 6 == pMainFrm->m_sSystemInfo.m_iSystemType))//һ���
	{
		QString strBakFile = QString("./pio24b/��ϵͳ����/һ���/PIO24B_reg_init1.txt");
		QString strIniName1 = QString("./PIO24B_reg_init1.txt");
		QString strValue,strPara;
		strValue = strValue.setNum(m_nDistinct2to3,10);
		strPara = strPara.setNum(61,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nDistinct3to4,10);
		strPara = strPara.setNum(49,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);
	}
	else if (3 == pMainFrm->m_sSystemInfo.m_iSystemType)//һ���
	{
		QString strBakFile = QString("./pio24b/��ϵͳ����/һ���/PIO24B_reg_init1.txt");
		QString strIniName1 = QString("./PIO24B_reg_init1.txt");
		QString strValue,strPara;
		strValue = strValue.setNum(m_nDistinct2to3,10);
		strPara = strPara.setNum(61,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nDistinct3to4,10);
		strPara = strPara.setNum(49,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);
	}
	else if (5 == pMainFrm->m_sSystemInfo.m_iSystemType)//ҩ��һ���
	{
		QString strBakFile = QString("./pio24b/��ϵͳ����/ҩ��һ���/PIO24B_reg_init1.txt");
		QString strIniName1 = QString("./PIO24B_reg_init1.txt");
		QString strValue,strPara;
		strValue = strValue.setNum(m_nDistinct2to3,10);
		strPara = strPara.setNum(61,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

		strValue = strValue.setNum(m_nDistinct3to4,10);
		strPara = strPara.setNum(32,10);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
		StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);
	}
	CLogFile::write(tr("WriteToTXT2"),OperationLog,0);

}
void WidgetTest::updateIOCardParam()
{
	QString str;

	str = str.setNum(m_nDelay1,10);
	ui.Delay1->setText(str);
	str = str.setNum(m_nDelay2,10);
	ui.Delay2->setText(str);
	str = str.setNum(m_nDelay3,10);
	ui.Delay3->setText(str);
	str = str.setNum(m_nDelay4,10);
	ui.Delay4->setText(str);
	str = str.setNum(m_nDelay5,10);
	ui.Delay5->setText(str);
	str = str.setNum(m_nDelay6,10);
	ui.Delay6->setText(str);
	str = str.setNum(m_nDelay7,10);
	ui.Delay7->setText(str);
	str = str.setNum(m_nDelay8,10);
	ui.Delay8->setText(str);

	str = str.setNum(m_nDistinct1to4,10);
	ui.Distance1to4->setText(str);
	str = str.setNum(m_nDistinct4to5,10);
	ui.Distance4to5->setText(str);
	str = str.setNum(m_nDistinct2to3,10);
	ui.Distance2to3->setText(str);
	str = str.setNum(m_nDistinct3to4,10);
	ui.Distance3to4->setText(str);

	str = str.setNum(m_nKickDelay,10);
	ui.KickDelay->setText(str);
	str = str.setNum(m_nKickWidth,10);
	ui.KickWidth->setText(str);
	if (pMainFrm->m_sSystemInfo.iIsSample)
	{
		str = str.setNum(m_nSampleDelay,10);
		ui.SampleDelay->setText(str);
		str = str.setNum(m_nSampleWidth,10);
		ui.SamplWidth->setText(str);
	}
}
void WidgetTest::getIOCardParam()
{
	m_nDelay1 = ui.Delay1->text().toInt();
	m_nDelay2 = ui.Delay2->text().toInt();
	m_nDelay3 = ui.Delay3->text().toInt();
	m_nDelay4 = ui.Delay4->text().toInt();
	m_nDelay5 = ui.Delay5->text().toInt();
	m_nDelay6 = ui.Delay6->text().toInt();
	m_nDelay7 = ui.Delay7->text().toInt();
	m_nDelay8 = ui.Delay8->text().toInt();

	m_nDistinct1to4 = ui.Distance1to4->text().toInt();
	m_nDistinct4to5 = ui.Distance4to5->text().toInt();
	m_nDistinct2to3 = ui.Distance2to3->text().toInt();
	m_nDistinct3to4 = ui.Distance3to4->text().toInt();

	m_nKickDelay = ui.KickDelay->text().toInt();
	m_nKickWidth = ui.KickWidth->text().toInt();
	if (pMainFrm->m_sSystemInfo.iIsSample)
	{
		m_nSampleDelay = ui.SampleDelay->text().toInt();
		m_nSampleWidth = ui.SamplWidth->text().toInt();
	}
}
void WidgetTest::updateIOCardParamVEXI()
{
	QString str;

	str = str.setNum(sVEXIIOCardInfo.nCamera1Interval,10);
	ui.lineEdit_Camera1TrggerInterval->setText(str);
	str = str.setNum(sVEXIIOCardInfo.nCamera2Interval,10);
	ui.lineEdit_Camera2TrggerInterval->setText(str);
	str = str.setNum(sVEXIIOCardInfo.nCamera3Interval,10);
	ui.lineEdit_Camera3TrggerInterval->setText(str);
	str = str.setNum(sVEXIIOCardInfo.nCamera4Interval,10);
	ui.lineEdit_Camera4TrggerInterval->setText(str);

	str = str.setNum(sVEXIIOCardInfo.nCamera1TrggerTimes,10);
	ui.lineEdit_Camera1TrggerTimes->setText(str);
	str = str.setNum(sVEXIIOCardInfo.nCamera2TrggerTimes,10);
	ui.lineEdit_Camera2TrggerTimes->setText(str);
	str = str.setNum(sVEXIIOCardInfo.nCamera3TrggerTimes,10);
	ui.lineEdit_Camera3TrggerTimes->setText(str);
	str = str.setNum(sVEXIIOCardInfo.nCamera4TrggerTimes,10);
	ui.lineEdit_Camera4TrggerTimes->setText(str);

	str = str.setNum(sVEXIIOCardInfo.nCamera1TrggerDelay,10);
	ui.lineEdit_Camera1TrggerDelay->setText(str);
	str = str.setNum(sVEXIIOCardInfo.nCamera2TrggerDelay,10);
	ui.lineEdit_Camera2TrggerDelay->setText(str);
	str = str.setNum(sVEXIIOCardInfo.nCamera3TrggerDelay,10);
	ui.lineEdit_Camera3TrggerDelay->setText(str);
	str = str.setNum(sVEXIIOCardInfo.nCamera4TrggerDelay,10);
	ui.lineEdit_Camera4TrggerDelay->setText(str);

	ui.checkBox_DoubleReject->setChecked(sVEXIIOCardInfo.bDoubleReject);	
	str = str.setNum(sVEXIIOCardInfo.nDoubleRejectDelay,10);
	ui.lineEdit_DoubleRejectDelay->setText(str);	
	str = str.setNum(sVEXIIOCardInfo.nDoubleRejectWidth,10);
	ui.lineEdit_DoubleRejectWidth->setText(str);
	str = str.setNum(sVEXIIOCardInfo.nRejectDelay,10);
	ui.lineEdit_RejectDelay->setText(str);	
	str = str.setNum(sVEXIIOCardInfo.nRejectWidth,10);
	ui.lineEdit_RejectWidth->setText(str);

//	str = str.setNum(sVEXIIOCardInfo.bCleanLightSource,10);
	ui.checkBox_CleanBaseLightSource->setChecked(sVEXIIOCardInfo.bCleanLightSource);
	str = str.setNum(sVEXIIOCardInfo.nCleanLightSourceInterval,10);
	ui.lineEdit_CleanBaseInterval->setText(str);
	str = str.setNum(sVEXIIOCardInfo.nCleanLightSourceWidth,10);
	ui.lineEdit_CleanBaseWidth->setText(str);

}
void WidgetTest::getIOCardParamVEXI()
{
	sVEXIIOCardInfo.nCamera1Interval = ui.lineEdit_Camera1TrggerInterval->text().toInt();
	sVEXIIOCardInfo.nCamera2Interval = ui.lineEdit_Camera2TrggerInterval->text().toInt();
	sVEXIIOCardInfo.nCamera3Interval = ui.lineEdit_Camera3TrggerInterval->text().toInt();
	sVEXIIOCardInfo.nCamera4Interval = ui.lineEdit_Camera4TrggerInterval->text().toInt();

	sVEXIIOCardInfo.nCamera1TrggerTimes = ui.lineEdit_Camera1TrggerTimes->text().toInt();
	sVEXIIOCardInfo.nCamera2TrggerTimes = ui.lineEdit_Camera2TrggerTimes->text().toInt();
	sVEXIIOCardInfo.nCamera3TrggerTimes = ui.lineEdit_Camera3TrggerTimes->text().toInt();
	sVEXIIOCardInfo.nCamera4TrggerTimes = ui.lineEdit_Camera4TrggerTimes->text().toInt();

	sVEXIIOCardInfo.nCamera1TrggerDelay = ui.lineEdit_Camera1TrggerDelay->text().toInt();
	sVEXIIOCardInfo.nCamera2TrggerDelay = ui.lineEdit_Camera2TrggerDelay->text().toInt();
	sVEXIIOCardInfo.nCamera3TrggerDelay = ui.lineEdit_Camera3TrggerDelay->text().toInt();
	sVEXIIOCardInfo.nCamera4TrggerDelay = ui.lineEdit_Camera4TrggerDelay->text().toInt();

	sVEXIIOCardInfo.bDoubleReject = ui.checkBox_DoubleReject->isChecked();
	sVEXIIOCardInfo.nDoubleRejectDelay = ui.lineEdit_DoubleRejectDelay->text().toInt();
	sVEXIIOCardInfo.nDoubleRejectWidth = ui.lineEdit_DoubleRejectWidth->text().toInt();
	sVEXIIOCardInfo.nRejectDelay = ui.lineEdit_RejectDelay->text().toInt();
	sVEXIIOCardInfo.nRejectWidth = ui.lineEdit_RejectWidth->text().toInt();

	sVEXIIOCardInfo.bCleanLightSource = ui.checkBox_CleanBaseLightSource->isChecked();
	sVEXIIOCardInfo.nCleanLightSourceInterval = ui.lineEdit_CleanBaseInterval->text().toInt();
	sVEXIIOCardInfo.nCleanLightSourceWidth = ui.lineEdit_CleanBaseWidth->text().toInt();

}
void WidgetTest::slots_choseAllCamera()
{
	for (int i=0;i<widget_Camera->listCheckBox.length();i++)
	{
		widget_Camera->listCheckBox.at(i)->setChecked(true);
	}
}
void WidgetTest::slots_choseNoneCamera()
{
	for (int i=0;i<widget_Camera->listCheckBox.length();i++)
	{
		widget_Camera->listCheckBox.at(i)->setChecked(false);
	}
}
void WidgetTest::slots_choseAllErrorType()
{
	for (int i=0;i<widget_ErrorType->listCheckBox.length();i++)
	{
		widget_ErrorType->listCheckBox.at(i)->setChecked(true);
	}
}
void WidgetTest::slots_choseNoneErrorType()
{
	for (int i=0;i<widget_ErrorType->listCheckBox.length();i++)
	{
		widget_ErrorType->listCheckBox.at(i)->setChecked(false);
	}
}
void WidgetTest::slots_ShowErrorType()
{
	if (bIsShowStatisErrorType)
	{
		bIsShowStatisErrorType = false;
		ui.widget_StatisErrorType->setVisible(false);
		ui.pushButton_ShowErrorType->setText(tr("Show error type"));
	}
	else
	{
		bIsShowStatisErrorType = true;
		ui.widget_StatisErrorType->setVisible(true);
		ui.pushButton_ShowErrorType->setText(tr("Hide error type"));
	}
	
}
void WidgetTest::slots_updateIOcardCounter()
{
	if (pMainFrm->m_sSystemInfo.m_bIsIOCardOK)
	{
		int iCounter;
		iCounter = pMainFrm->m_vIOCard[0]->ReadCounter(16);
		ui.label_frequency->setText(tr("Frequency:")+QString::number(iCounter));


		iCounter = pMainFrm->m_vIOCard[0]->ReadCounter(0);
		ui.label_IN0->setText(tr("IN0:")+QString::number(iCounter));
		iCounter = pMainFrm->m_vIOCard[0]->ReadCounter(1);
		ui.label_IN1->setText(tr("IN1:")+QString::number(iCounter));
		iCounter = pMainFrm->m_vIOCard[0]->ReadCounter(2);
		ui.label_IN2->setText(tr("IN2:")+QString::number(iCounter));
		iCounter = pMainFrm->m_vIOCard[0]->ReadCounter(3);
		ui.label_IN3->setText(tr("IN3:")+QString::number(iCounter));


		iCounter = pMainFrm->m_vIOCard[0]->ReadCounter(4);
		ui.label_OUT0->setText(tr("OUT0:")+QString::number(iCounter));
		iCounter = pMainFrm->m_vIOCard[0]->ReadCounter(5);
		ui.label_OUT1->setText(tr("OUT1:")+QString::number(iCounter));
		iCounter = pMainFrm->m_vIOCard[0]->ReadCounter(6);
		ui.label_OUT2->setText(tr("OUT2:")+QString::number(iCounter));
		iCounter = pMainFrm->m_vIOCard[0]->ReadCounter(7);
		ui.label_OUT3->setText(tr("OUT3:")+QString::number(iCounter));
		iCounter = pMainFrm->m_vIOCard[0]->ReadCounter(9)%0x100;
		ui.label_OUT4->setText(tr("OUT4:")+QString::number(iCounter));
		iCounter = pMainFrm->m_vIOCard[0]->ReadCounter(9)/0x100;
		ui.label_OUT5->setText(tr("OUT5:")+QString::number(iCounter));
		iCounter = pMainFrm->m_vIOCard[0]->ReadCounter(10)%0x100;
		ui.label_OUT6->setText(tr("OUT6:")+QString::number(iCounter));
		iCounter = pMainFrm->m_vIOCard[0]->ReadCounter(10)/0x100;
		ui.label_OUT7->setText(tr("OUT7:")+QString::number(iCounter));

		if (2 == pMainFrm->m_sSystemInfo.iIOCardCount)
		{
			iCounter = pMainFrm->m_vIOCard[0]->ReadCounter(16);
			ui.label_frequency_2->setText(tr("Frequency:")+QString::number(iCounter));

			iCounter = pMainFrm->m_vIOCard[1]->ReadCounter(0);
			ui.label_IN0_2->setText(tr("IN0:")+QString::number(iCounter));
			iCounter = pMainFrm->m_vIOCard[1]->ReadCounter(1);
			ui.label_IN1_2->setText(tr("IN1:")+QString::number(iCounter));
			iCounter = pMainFrm->m_vIOCard[1]->ReadCounter(2);
			ui.label_IN2_2->setText(tr("IN2:")+QString::number(iCounter));
			iCounter = pMainFrm->m_vIOCard[1]->ReadCounter(3);
			ui.label_IN3_2->setText(tr("IN3:")+QString::number(iCounter));


			iCounter = pMainFrm->m_vIOCard[1]->ReadCounter(4);
			ui.label_OUT0_2->setText(tr("OUT0:")+QString::number(iCounter));
			iCounter = pMainFrm->m_vIOCard[1]->ReadCounter(5);
			ui.label_OUT1_2->setText(tr("OUT1:")+QString::number(iCounter));
			iCounter = pMainFrm->m_vIOCard[1]->ReadCounter(6);
			ui.label_OUT2_2->setText(tr("OUT2:")+QString::number(iCounter));
			iCounter = pMainFrm->m_vIOCard[1]->ReadCounter(7);
			ui.label_OUT3_2->setText(tr("OUT3:")+QString::number(iCounter));
			iCounter = pMainFrm->m_vIOCard[1]->ReadCounter(9)%0x100;
			ui.label_OUT4_2->setText(tr("OUT4:")+QString::number(iCounter));
			iCounter = pMainFrm->m_vIOCard[1]->ReadCounter(9)/0x100;
			ui.label_OUT5_2->setText(tr("OUT5:")+QString::number(iCounter));
			iCounter = pMainFrm->m_vIOCard[1]->ReadCounter(10)%0x100;
			ui.label_OUT6_2->setText(tr("OUT6:")+QString::number(iCounter));
			iCounter = pMainFrm->m_vIOCard[1]->ReadCounter(10)/0x100;
			ui.label_OUT7_2->setText(tr("OUT7:")+QString::number(iCounter));
		}
	}
}
void WidgetTest::slots_VEXIIOCardSet()
{
	getIOCardParamVEXI();

	pMainFrm->m_vIOCard[0]->writeParam(43,sVEXIIOCardInfo.nCamera1Interval);
	pMainFrm->m_vIOCard[0]->writeParam(72,sVEXIIOCardInfo.nCamera2Interval);
	pMainFrm->m_vIOCard[0]->writeParam(73,sVEXIIOCardInfo.nCamera3Interval);
	pMainFrm->m_vIOCard[0]->writeParam(74,sVEXIIOCardInfo.nCamera4Interval);

	pMainFrm->m_vIOCard[0]->writeParam(44,sVEXIIOCardInfo.nCamera1TrggerTimes);
	pMainFrm->m_vIOCard[0]->writeParam(68,sVEXIIOCardInfo.nCamera2TrggerTimes);
	pMainFrm->m_vIOCard[0]->writeParam(69,sVEXIIOCardInfo.nCamera3TrggerTimes);
	pMainFrm->m_vIOCard[0]->writeParam(70,sVEXIIOCardInfo.nCamera4TrggerTimes);

	pMainFrm->m_vIOCard[0]->writeParam(32,sVEXIIOCardInfo.nCamera1TrggerDelay);
	pMainFrm->m_vIOCard[0]->writeParam(57,sVEXIIOCardInfo.nCamera2TrggerDelay);
	pMainFrm->m_vIOCard[0]->writeParam(59,sVEXIIOCardInfo.nCamera3TrggerDelay);
	pMainFrm->m_vIOCard[0]->writeParam(61,sVEXIIOCardInfo.nCamera4TrggerDelay);

	pMainFrm->m_vIOCard[0]->writeParam(33,sVEXIIOCardInfo.nCamera1Frame);
	pMainFrm->m_vIOCard[0]->writeParam(58,sVEXIIOCardInfo.nCamera2Frame);
	pMainFrm->m_vIOCard[0]->writeParam(60,sVEXIIOCardInfo.nCamera3Frame);
	pMainFrm->m_vIOCard[0]->writeParam(62,sVEXIIOCardInfo.nCamera4Frame);

	pMainFrm->m_sSystemInfo.bVEXIDoubleReject = sVEXIIOCardInfo.bDoubleReject;
	pMainFrm->m_vIOCard[0]->writeParam(49,sVEXIIOCardInfo.nRejectDelay);
	pMainFrm->m_vIOCard[0]->writeParam(46,sVEXIIOCardInfo.nRejectWidth);
	pMainFrm->m_vIOCard[0]->writeParam(76,sVEXIIOCardInfo.nDoubleRejectDelay);
	pMainFrm->m_vIOCard[0]->writeParam(92,sVEXIIOCardInfo.nDoubleRejectWidth);

	pMainFrm->m_sSystemInfo.bVEXICleanLightSource = sVEXIIOCardInfo.bCleanLightSource;
	pMainFrm->m_sSystemInfo.nVEXICleanLightSourceInterval = sVEXIIOCardInfo.nCleanLightSourceInterval;
	pMainFrm->m_sSystemInfo.nVEXICleanLightSourceWidth = sVEXIIOCardInfo.nCleanLightSourceWidth;
	
	pMainFrm->pThreadBaseClean->setBaseCleanParam(pMainFrm->m_sSystemInfo.bVEXICleanLightSource, 
	pMainFrm->m_sSystemInfo.nVEXICleanLightSourceInterval,
	pMainFrm->m_sSystemInfo.nVEXICleanLightSourceWidth);
	if (pMainFrm->m_sSystemInfo.m_bIsIOCardOK)
	{
		pMainFrm->pThreadBaseClean->start();
	}


}
void WidgetTest::slots_VEXIIOCardSave()
{
	getIOCardParamVEXI();
	QString strBakFile = QString("./pio24b/Vexi/PIO24B_reg_init.txt");
	QString strIniName1 = QString("./PIO24B_reg_init.txt");
	QString strValue,strPara;
	strValue = strValue.setNum(sVEXIIOCardInfo.nCamera1Interval,10);
	strPara = strPara.setNum(43,10);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);
	strValue = strValue.setNum(sVEXIIOCardInfo.nCamera2Interval,10);
	strPara = strPara.setNum(72,10);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);
	strValue = strValue.setNum(sVEXIIOCardInfo.nCamera3Interval,10);
	strPara = strPara.setNum(73,10);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);
	strValue = strValue.setNum(sVEXIIOCardInfo.nCamera4Interval,10);
	strPara = strPara.setNum(74,10);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

	strValue = strValue.setNum(sVEXIIOCardInfo.nCamera1TrggerTimes,10);
	strPara = strPara.setNum(44,10);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);
	strValue = strValue.setNum(sVEXIIOCardInfo.nCamera2TrggerTimes,10);
	strPara = strPara.setNum(68,10);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);
	strValue = strValue.setNum(sVEXIIOCardInfo.nCamera3TrggerTimes,10);
	strPara = strPara.setNum(69,10);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);
	strValue = strValue.setNum(sVEXIIOCardInfo.nCamera4TrggerTimes,10);
	strPara = strPara.setNum(70,10);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);

	strValue = strValue.setNum(sVEXIIOCardInfo.nCamera1TrggerDelay,10);
	strPara = strPara.setNum(32,10);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);
	strValue = strValue.setNum(sVEXIIOCardInfo.nCamera2TrggerDelay,10);
	strPara = strPara.setNum(57,10);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);
	strValue = strValue.setNum(sVEXIIOCardInfo.nCamera3TrggerDelay,10);
	strPara = strPara.setNum(59,10);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);
	strValue = strValue.setNum(sVEXIIOCardInfo.nCamera4TrggerDelay,10);
	strPara = strPara.setNum(61,10);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);	

	strValue = strValue.setNum(sVEXIIOCardInfo.nCamera1Frame,10);
	strPara = strPara.setNum(33,10);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);
	strValue = strValue.setNum(sVEXIIOCardInfo.nCamera2Frame,10);
	strPara = strPara.setNum(58,10);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);
	strValue = strValue.setNum(sVEXIIOCardInfo.nCamera3Frame,10);
	strPara = strPara.setNum(60,10);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);
	strValue = strValue.setNum(sVEXIIOCardInfo.nCamera4Frame,10);
	strPara = strPara.setNum(62,10);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);	

	strValue = strValue.setNum(sVEXIIOCardInfo.nRejectDelay,10);
	strPara = strPara.setNum(49,10);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);
	strValue = strValue.setNum(sVEXIIOCardInfo.nRejectWidth,10);
	strPara = strPara.setNum(46,10);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);
	strValue = strValue.setNum(sVEXIIOCardInfo.nDoubleRejectDelay,10);
	strPara = strPara.setNum(76,10);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);
	strValue = strValue.setNum(sVEXIIOCardInfo.nDoubleRejectWidth,10);
	strPara = strPara.setNum(92,10);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strIniName1);
	StateTool::WritePrivateProfileQString("PIO24B",strPara,strValue,strBakFile);	

	QSettings iniStatisSet(pMainFrm->m_sConfigInfo.m_strConfigPath,QSettings::IniFormat);
	iniStatisSet.setIniCodec(QTextCodec::codecForName("GBK"));

	iniStatisSet.setValue("/system/bVEXIDoubleReject", sVEXIIOCardInfo.bDoubleReject);
	iniStatisSet.setValue("/system/bVEXICleanLightSource",sVEXIIOCardInfo.bCleanLightSource);
	iniStatisSet.setValue("/system/nVEXICleanLightSourceInterval",sVEXIIOCardInfo.nCleanLightSourceInterval);
	iniStatisSet.setValue("/system/nVEXICleanLightSourceWidth",sVEXIIOCardInfo.nCleanLightSourceWidth);

	pMainFrm->m_sSystemInfo.bVEXIDoubleReject = sVEXIIOCardInfo.bDoubleReject;
	pMainFrm->m_sSystemInfo.bVEXICleanLightSource = sVEXIIOCardInfo.bCleanLightSource;
	pMainFrm->m_sSystemInfo.nVEXICleanLightSourceInterval = sVEXIIOCardInfo.nCleanLightSourceInterval;
	pMainFrm->m_sSystemInfo.nVEXICleanLightSourceWidth = sVEXIIOCardInfo.nCleanLightSourceWidth;

	/*pMainFrm->pThreadBaseClean->setBaseCleanParam(pMainFrm->m_sSystemInfo.bVEXICleanLightSource, 
	pMainFrm->m_sSystemInfo.nVEXICleanLightSourceInterval,
	pMainFrm->m_sSystemInfo.nVEXICleanLightSourceWidth);
	if (pMainFrm->m_sSystemInfo.m_bIsIOCardOK)
	{
	pMainFrm->pThreadBaseClean->start();
	}*/
}
void WidgetTest::slots_VEXIIOCardAdvance()
{
	if (pMainFrm->m_sSystemInfo.m_bIsIOCardOK)
	{
		pMainFrm->m_vIOCard[0]->Show_PIO24B_DebugDialog(this);//((this,"\\Config\\PIO24B_reg_init.txt_1", "PIO24B"));
	}
	else
	{
		QMessageBox::information(this,"ERROR","IOCard is not available");
	}

}
void WidgetTest::slots_VEXIIOCardCancel()
{
	initInformation();
}
QString WidgetTest::CompareDoubleString(QString temp)
{
	temp=temp.replace(QString("."),QString(""));
	if(temp.length()==3)
	{
		return "0"+temp;
	}else{
		return temp;
	}
}
void WidgetTest::slots_CheckIsSendKickOut()
{
	double total = pMainFrm->m_sRunningInfo.m_checkedNum;
	double failur = pMainFrm->m_sRunningInfo.m_failureNumFromIOcard;

	if (total != 0&&MaxRate>MinRate)
	{
		if(m_lastAllNumber==total)
		{
			return;
		}
		
		QTime time = QTime::currentTime();
		int CurrentTemp=time.hour()*60+time.minute();
		int shift1Time=pMainFrm->widget_count->shift1.hour()*60+pMainFrm->widget_count->shift1.minute();
		int shift2Time=pMainFrm->widget_count->shift2.hour()*60+pMainFrm->widget_count->shift2.minute();
		int shift3Time=pMainFrm->widget_count->shift3.hour()*60+pMainFrm->widget_count->shift3.minute();
		int IntelTime=ui.spinBox->value();


		if (pMainFrm->widget_count->isClear1&&(CurrentTemp-shift1Time<IntelTime))
		{
			return;
		}
		if (pMainFrm->widget_count->isClear2&&(CurrentTemp-shift2Time<IntelTime))
		{
			return;
		}
		if (pMainFrm->widget_count->isClear3&&(CurrentTemp-shift3Time<IntelTime))
		{
			return;
		}
		
		m_lastAllNumber=total;
		double SRate=failur/total*100;
		QString temp=CompareDoubleString(QString::number(SRate,10,2));
		QString b_minrate=CompareDoubleString(QString::number(MinRate,10,2));
		QString c_maxrate=CompareDoubleString(QString::number(MaxRate,10,2));

		if(temp>b_minrate && c_maxrate>temp && b_minrate<c_maxrate)
		{

		}else
		{
			if (pMainFrm->m_sSystemInfo.m_bIsIOCardOK)
			{
				pMainFrm->m_vIOCard[0]->m_mutexmIOCard.lock();
				pMainFrm->m_vIOCard[0]->m_Pio24b.setCardOutput(3,1);
				pMainFrm->m_vIOCard[0]->m_mutexmIOCard.unlock();
			}
			pMainFrm->slots_ShowPLCStatus(8);
		}
	}
}
void WidgetTest::slots_onStateChanged(int)
{
	bool m_temp=checkBoxAll->isChecked();
	for (int i = 0;i<pMainFrm->m_sErrorInfo.m_iErrorTypeCount;i++)
	{
		listCheckBoxErrorType.at(i)->setChecked(m_temp);
	}
}
void WidgetTest::ShowAssert(int i)
{
	listCheckBoxErrorType.at(i)->setStyleSheet("color:red;");
}
void WidgetTest::CloseAssert()
{
	if (pMainFrm->m_sSystemInfo.m_bIsIOCardOK)
	{
		pMainFrm->m_vIOCard[0]->m_mutexmIOCard.lock();
		pMainFrm->m_vIOCard[0]->m_Pio24b.setCardOutput(7,0);
		pMainFrm->m_vIOCard[0]->m_mutexmIOCard.unlock();
	}
	for (int i = 0;i<pMainFrm->m_sErrorInfo.m_iErrorTypeCount;i++)
	{
		if(pMainFrm->test_widget->iIsTrackErrorType[i])
		{
			listCheckBoxErrorType.at(i)->setStyleSheet("color:black");
		}
	}
}