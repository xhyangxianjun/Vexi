#include "DetectThread.h"
#include <QMessageBox>
#include "glasswaredetectsystem.h"
extern GlasswareDetectSystem *pMainFrm;
DetectThread::DetectThread(QObject *parent)
	: QThread(parent)
{
	m_bStopThread = false;
	ThreadNumber = -1;
	dirSaveImagePath = new QDir;
	iMaxErrorType = 0;
	iMaxErrorArea = 0;
	iErrorType = 0;
	for (int i = 0; i<pMainFrm->m_sSystemInfo.iCamCount; i++)
	{
		isShowImage[i] = true;
	}
	for (int i=0;i<256;i++)
	{
		pMainFrm->bIsGetSample[i] = false;
		isShowPicture[i]=0;
	}
	m_lastResult=FALSE;
}

DetectThread::~DetectThread()
{

}

void DetectThread::run()
{
// 	iCamera=ThreadNumber;

	while (!pMainFrm->m_bIsThreadDead)
	{
		WholeThread.StartSpeedTest();
		if(m_bStopThread)
		{
			break;
		}
		if (pMainFrm->m_queue[ThreadNumber].listDetect.size()>0)
		{
			//CLogFile::write(tr("get 3333"),AbnormityLog);
			pMainFrm->mutexListDetect[ThreadNumber].lock();
			DetectElement = pMainFrm->m_queue[ThreadNumber].listDetect.first();
			pMainFrm->m_queue[ThreadNumber].listDetect.removeFirst();
			pMainFrm->mutexListDetect[ThreadNumber].unlock();
			iCamera = DetectElement.iCameraNormal;
			grabImageCount =DetectElement.ImageNormal->nGrabImageCount;
			DetectNormal(DetectElement.ImageNormal);

			if (pMainFrm->m_queue[iCamera].InitID == DetectElement.ImageNormal->initID)
			{
				pMainFrm->m_queue[iCamera].mLocker.lock();
				pMainFrm->m_queue[iCamera].listGrab.push_back(DetectElement.ImageNormal);
				pMainFrm->m_queue[iCamera].mLocker.unlock();
			}
			else
			{
				CLogFile::write(tr("delete a element!"),AbnormityLog);
				delete DetectElement.ImageNormal->myImage;
				DetectElement.ImageNormal = NULL;
				delete DetectElement.ImageNormal;
			}
		}
		else
		{
			Sleep(1);
		}
	}
	delete dirSaveImagePath;
}
void DetectThread::DetectNormal(CGrabElement* pElement)
{
	checkTimecost.StartSpeedTest();

	int iRealCamera = pMainFrm->m_sCarvedCamInfo[iCamera].m_iToRealCamera;
	bCheckResult[iCamera] = false;
	iErrorType = 0;
	iMaxErrorType = 0;
	iMaxErrorArea = 0;
	pElement->cErrorRectList.clear();
			
 	try
 	{
		pMainFrm->m_iDetectSignalNo[iCamera] = pElement->nSignalNo;	
		CountRuningData(iCamera,pElement->nGrabImageCount);
 		rotateImage(pElement);

		if (pMainFrm->m_sRunningInfo.m_bCheck&&pMainFrm->m_sRunningInfo.m_bIsCheck[iCamera])
 		{
 			try
 			{
				checkImage(pElement, 1);
 				if (!getCheckResult(pElement))
 				{
 					return;
 				}
 			}
 			catch (...)
 			{
 				CLogFile::write(tr("check abnormal")+tr("Camera%1").arg(iCamera) + tr("DetectStep%1").arg(pMainFrm->m_iDetectStep[iCamera]), AbnormityLog);
 
 			}
 		}else{
			return;
		}
 	}
 	catch(...)
 	{
		CLogFile::write(tr("Detect abnormal")+tr("Camera%1").arg(iCamera) + tr("DetectStep%1").arg(pMainFrm->m_iDetectStep[iCamera]), AbnormityLog);
 	}
 	try
 	{
 		//�߷�
 		kickOutBad(pElement);
 		//����ͼ��
 		saveImage(pElement);
 		//������ͼ������������
 		if (bCheckResult[iCamera])
 		{
			addErrorImageList(pElement);
 		}
 	}
 	catch (...)
 	{
  	}
	checkTimecost.StopSpeedTest();
	pElement->dCostTime = checkTimecost.dfTime;
	//ˢ��ͼ���״̬
	if (pMainFrm->m_queue[iCamera].InitID == pElement->initID)
	{
		upDateState(pElement->myImage,pElement->nSignalNo,pElement->dCostTime, pElement->nMouldID,pElement->cErrorRectList,pElement->initID);
	}
	//pElement->nStation = 0;
}
void DetectThread::DetectStress(CGrabElement *pElement)
{
	checkTimecost.StartSpeedTest();
	iCamera = iStressCamera;

	int iRealCamera = pMainFrm->m_sCarvedCamInfo[iCamera].m_iToRealCamera;
	bCheckResult[iCamera] = false;
	iErrorType = 0;
	iMaxErrorType = 0;
	iMaxErrorArea = 0;
	pElement->cErrorRectList.clear();

	try
	{
		pMainFrm->m_iDetectSignalNo[iCamera] = pElement->nSignalNo;
		rotateImage(pElement);
		if (0 == pMainFrm->m_sCarvedCamInfo[iCamera].m_iImageType)
		{
			sAlgCInp.fParam = pMainFrm->m_sSystemInfo.fPressScale;
		}
		else if (2 == pMainFrm->m_sCarvedCamInfo[iCamera].m_iImageType)
		{
			sAlgCInp.fParam = pMainFrm->m_sSystemInfo.fBasePressScale;
		}

		pMainFrm->m_cBottleStress[iCamera].Check(sAlgCInp, &pAlgCheckResult);//Ӧ����ǿ

		if (pMainFrm->m_sRunningInfo.m_bCheck)
		{
			try
			{
				checkImage(pElement, 2);
				bool bOK = getCheckResult(pElement);
				if (!bOK)
				{
					return;
				}
			}
			catch (...)
			{
			}
		}
	}
	catch(...)
	{
	}
	try
	{
		//�߷�
		kickOutBad(pElement);
		//����ͼ��
		saveImage(pElement);
		//������ͼ������������
		if (bCheckResult[iCamera])
		{
			addErrorImageList(pElement);
		}
	}
	catch (...)
	{
	}
	checkTimecost.StopSpeedTest();
	pElement->dCostTime = checkTimecost.dfTime;
	int shownSignalNo = pElement->nSignalNo;
	double shownCostTime = pElement->dCostTime;

	//ˢ��ͼ���״̬
	if (pMainFrm->m_queue[iCamera].InitID == pElement->initID)
	{
		upDateState(pElement->myImage,shownSignalNo,shownCostTime,pElement->nMouldID, pElement->cErrorRectList, pElement->initID);
	}
	//pElement->nStation = 0;
}

void DetectThread::WaitThreadStop()
{
	if (isRunning())
	{
		if (!m_bStopThread)
		{
			m_bStopThread = true;
		}
		wait();
	}
}
//��תͼ��
void DetectThread::rotateImage(CGrabElement *pElement)
{

	sAlgCInp.sInputParam.nWidth = pElement->myImage->width();
	sAlgCInp.sInputParam.nHeight = pElement->myImage->height();
	sAlgCInp.sInputParam.pcData = (char*)pElement->myImage->bits();

	int tempWidth = pElement->myImage->width();
	int tempHeight = pElement->myImage->height();

	if(pMainFrm->m_sCarvedCamInfo[iCamera].m_iImageAngle != 0)
	{
		sAlgCInp.nParam = pMainFrm->m_sCarvedCamInfo[iCamera].m_iImageAngle;
		pMainFrm->m_cBottleRotate[iCamera].Check(sAlgCInp, &pAlgCheckResult);
	}

}
//���
void DetectThread::checkImage(CGrabElement *pElement,int iCheckMode)
{
	sAlgCInp.sInputParam.nHeight = pElement->myImage->height();
	sAlgCInp.sInputParam.nWidth = pElement->myImage->width();
	sAlgCInp.sInputParam.nChannel = (pElement->myImage->byteCount()+7)/8;
	sAlgCInp.sInputParam.pcData = (char*)pElement->myImage->bits();

	//�����Ӧ�����ʱ����Ҫ��������
	if (1 == iCheckMode)
	{
		sReturnStatus = pMainFrm->m_cBottleCheck[iCamera].Check(sAlgCInp,&pAlgCheckResult);
		pMainFrm->m_sCarvedCamInfo[iCamera].sImageLocInfo[pElement->nSignalNo].m_AlgImageLocInfos.sLocOri = pAlgCheckResult->sImgLocInfo.sLocOri;
		pMainFrm->m_sCarvedCamInfo[iCamera].sImageLocInfo[pElement->nSignalNo].m_AlgImageLocInfos.sXldPoint.nCount  = pAlgCheckResult->sImgLocInfo.sXldPoint.nCount;

		memcpy(pMainFrm->m_sCarvedCamInfo[iCamera].sImageLocInfo[pElement->nSignalNo].m_AlgImageLocInfos.sXldPoint.nColsAry,pAlgCheckResult->sImgLocInfo.sXldPoint.nColsAry,4*BOTTLEXLD_POINTNUM);														
		memcpy(pMainFrm->m_sCarvedCamInfo[iCamera].sImageLocInfo[pElement->nSignalNo].m_AlgImageLocInfos.sXldPoint.nRowsAry,pAlgCheckResult->sImgLocInfo.sXldPoint.nRowsAry,4*BOTTLEXLD_POINTNUM);
		pMainFrm->m_sCarvedCamInfo[iCamera].sImageLocInfo[pElement->nSignalNo].m_iHaveInfo = 1;
	}
	else if (2 == iCheckMode)
	{
		int normalCamera = pMainFrm->m_sCarvedCamInfo[iCamera].m_iToNormalCamera;
		pElement->sImgLocInfo.sLocOri = pMainFrm->m_sCarvedCamInfo[normalCamera].sImageLocInfo[pElement->nSignalNo].m_AlgImageLocInfos.sLocOri;
		pElement->sImgLocInfo.sXldPoint.nCount = pMainFrm->m_sCarvedCamInfo[normalCamera].sImageLocInfo[pElement->nSignalNo].m_AlgImageLocInfos.sXldPoint.nCount;
		memcpy(pElement->sImgLocInfo.sXldPoint.nColsAry,pMainFrm->m_sCarvedCamInfo[normalCamera].sImageLocInfo[pElement->nSignalNo].m_AlgImageLocInfos.sXldPoint.nColsAry,4*BOTTLEXLD_POINTNUM);							
		memcpy(pElement->sImgLocInfo.sXldPoint.nRowsAry,pMainFrm->m_sCarvedCamInfo[normalCamera].sImageLocInfo[pElement->nSignalNo].m_AlgImageLocInfos.sXldPoint.nRowsAry,4*BOTTLEXLD_POINTNUM);

		sAlgCInp.sImgLocInfo = pElement->sImgLocInfo;
		sReturnStatus = pMainFrm->m_cBottleCheck[iCamera].Check(sAlgCInp,&pAlgCheckResult);
		pMainFrm->m_sCarvedCamInfo[normalCamera].sImageLocInfo[pElement->nSignalNo].m_iHaveInfo = 0;
	}
	else
	{
		sReturnStatus = pMainFrm->m_cBottleCheck[iCamera].Check(sAlgCInp,&pAlgCheckResult);
	}
}
//��ȡ�����
bool DetectThread::getCheckResult(CGrabElement *pElement)
{
	if (sReturnStatus.nErrorID != 0 && sReturnStatus.nErrorID != 10)
	{
		//CLogFile::write(tr("get failed!"),AbnormityLog);
		return false;
	}
	GetModelDotData(pElement);
	if (pAlgCheckResult->nSizeError >0 && pMainFrm->m_sRunningInfo.m_bIsCheck[iCamera]) //�д����Ҵ����δ�رռ��
	{
		//������ƿͳ��
		bCheckResult[iCamera] = true;
		pMainFrm->m_sCarvedCamInfo[iCamera].m_iErrorCount++;
		pElement->cErrorParaList.clear(); //�����
		bool bIsStatic = false;//��ʾ�Ƿ�ͳ��
		for (int j=0;j<pAlgCheckResult->nSizeError;j++) //����㷨���ش����������
		{
			s_ErrorPara  sErrorPara;
			sErrorPara = pAlgCheckResult->vErrorParaAry[j];
			if(sErrorPara.nArea > iMaxErrorArea)
			{
				iMaxErrorArea = sErrorPara.nArea;
				iMaxErrorType = sErrorPara.nErrorType;
			}
			QRect rect(sErrorPara.rRctError.left,\
				sErrorPara.rRctError.top, \
				sErrorPara.rRctError.right - sErrorPara.rRctError.left, \
				sErrorPara.rRctError.bottom - sErrorPara.rRctError.top);
			if (iMaxErrorType > pMainFrm->m_sErrorInfo.m_iErrorTypeCount)
			{
				iMaxErrorType = pMainFrm->m_sErrorInfo.m_iErrorTypeCount+1;
				sErrorPara.nErrorType = pMainFrm->m_sErrorInfo.m_iErrorTypeCount+1;
			}
			//	�Ҳ���ԭ�㲻�߷�
			//if (1 == sErrorPara.nErrorType&&(1 == pMainFrm->m_sSystemInfo.m_iNoRejectIfNoOrigin[iCamera] || 1 == pMainFrm->m_sSystemInfo.m_NoKickIfNoFind ))
			//{
			//	bCheckResult[iCamera] = false;
			//}
			//// Ԥ��������߷�
			//if (2 == sErrorPara.nErrorType&&(1 == pMainFrm->m_sSystemInfo.m_iNoRejectIfROIfail[iCamera] || 1 == pMainFrm->m_sSystemInfo.m_NoKickIfROIFail ))
			//{
			//	bCheckResult[iCamera] = false;
			//}			
			if(sErrorPara.nErrorType==39)
			{
				bCheckResult[iCamera] = true;
			}
			//���㷨���ش�������������
			if (bCheckResult[iCamera])
			{
				pElement->cErrorRectList.append(rect);
				pElement->cErrorParaList.append(sErrorPara);
			}

			//���������ۺ�
			//�Ҳ���ԭ�㲻�ۺ�
			/*if (1 == sErrorPara.nErrorType&&(1 == pMainFrm->m_sSystemInfo.m_iNoRejectIfNoOrigin[iCamera] || 1 == pMainFrm->m_sSystemInfo.m_NoKickIfNoFind ))
			{
			;
			}
			else if (2 == sErrorPara.nErrorType&&(1 == pMainFrm->m_sSystemInfo.m_iNoRejectIfROIfail[iCamera] || 1 == pMainFrm->m_sSystemInfo.m_NoKickIfROIFail ))
			{
			;
			}
			else
			{
			pMainFrm->m_cCombine.m_MutexCombin.lock();
			pMainFrm->m_cCombine.AddError(pElement->nSignalNo,iCamera,sErrorPara);
			pMainFrm->m_cCombine.m_MutexCombin.unlock();
			}*/
			pMainFrm->m_cCombine.m_MutexCombin.lock();
			pMainFrm->m_cCombine.AddError(pElement->nSignalNo,iCamera,sErrorPara);
			pMainFrm->m_cCombine.m_MutexCombin.unlock();
		}

		iErrorType = iMaxErrorType;
		//ȡ��
		/*if (pMainFrm->m_sSampleInfo.m_bSampleType[iErrorType])
		{
			pMainFrm->mutexSetSample.lock();
			pMainFrm->bIsGetSample[pElement->nSignalNo] = true;
			pMainFrm->mutexSetSample.unlock();
		}*/
		pElement->nCheckRet = iErrorType;
	}
	else//û�д������
	{
		//������ƿ����
		pMainFrm->m_sRunningInfo.m_iCamContinueReject[iCamera] = 0;
		s_ErrorPara sErrorPara;
		sErrorPara.nArea = 0;
		sErrorPara.nErrorType = 0;
		pMainFrm->m_cCombine.m_MutexCombin.lock();
		pMainFrm->m_cCombine.AddError(pElement->nSignalNo,iCamera,sErrorPara);
		pMainFrm->m_cCombine.m_MutexCombin.unlock();
	}
	return true;
}
//ȡ��,ֻ�ڽӿڿ�1ȡ��

void DetectThread::GetModelDotData(CGrabElement *pElement)
{
	pElement->nMouldID = pAlgCheckResult->nMouldID;
	if (pAlgCheckResult->nMouldID>0 && pAlgCheckResult->nMouldID < 100)
	{
		pMainFrm->m_sRunningInfo.nModelReadFailureNumber++;
	}
}
void DetectThread::kickOutBad(CGrabElement *pElement)
{
	int tmpResult=0;
	int iKickMode = pMainFrm->m_sRunningInfo.m_iKickMode;
	
	switch (iKickMode)
	{
	case 0:			// ������ 
		tmpResult=1;
		break;
	case 1:			// ��ƿ��
		tmpResult = pElement->nSignalNo%2;
		break;
	case 2:			// ������
		tmpResult=0;
		break;
	case 3:			// ������
		tmpResult=bCheckResult[iCamera];
		break;
	default:			
		break;
	}

	if (!pMainFrm->m_sRunningInfo.m_bCheck)
	{
		tmpResult=0;
	}
	if(tmpResult)
	{
		m_lastResult=1;
	}
	
	if(iCamera == pMainFrm->widget_ellipticity->m_CameraStion-1)
	{
		
		QImage temp_image(*pElement->myImage);
		pMainFrm->widget_ellipticity->insertImage(temp_image,pElement->nSignalNo,grabImageCount);
		
		//������Բ������
		pMainFrm->widget_ellipticity->setData(pAlgCheckResult->sSizeResult.fBodyOvality,grabImageCount);
		if(grabImageCount == pMainFrm->widget_ellipticity->m_CollectNum)
		{
			bool result = pMainFrm->widget_ellipticity->checkData(pMainFrm->widget_ellipticity->m_Templist,pElement->nSignalNo);
			KickOut(pElement->nSignalNo, result, 0);
			pMainFrm->widget_ellipticity->clearData();
		}
	}else{
		if(grabImageCount == pMainFrm->m_sRealCamInfo[iCamera].m_iImageTargetNo)
		{
			KickOut(pElement->nSignalNo, m_lastResult, 0);
			m_lastResult=0;
		}
	}
}
//��ͼ
void DetectThread::saveImage(CGrabElement *pElement)
{
	if (1 == pMainFrm->m_sSystemInfo.m_iSaveNormalErrorImageByTime)
	{
		if (bCheckResult[iCamera] && pMainFrm->m_sCarvedCamInfo[iCamera].m_iStress != 2)
		{
			QDateTime time = QDateTime::currentDateTime();
			QString strSaveImagePath = QString(pMainFrm->m_sConfigInfo.m_strAppPath + tr("SaveImageByTime\\") +\
				tr("normal image\\") + time.date().toString("yyyy-MM-dd") + tr("\\camera%1").arg(iCamera+1)) + "\\" + time.time().toString("hh");
			bool exist = dirSaveImagePath->exists(strSaveImagePath);
			if(!exist)
			{
				bool ok = dirSaveImagePath->mkpath(strSaveImagePath);
				if (!ok)
				{
					pMainFrm->writeLogText(tr("Failure in create Path!"),AbnormityLog);
				}
			}
			QString strSavePath = QString(strSaveImagePath + "/image number%1_%2%3%4_%5.bmp").arg(pElement->nImgSN).arg(time.time().hour()).arg(time.time().minute()).arg(time.time().second()).arg(grabImageCount);
			pElement->myImage->mirrored().save(strSavePath);
		}
	}
	if (1 == pMainFrm->m_sSystemInfo.m_iSaveStressErrorImageByTime)
	{
		if (bCheckResult[iCamera] && pMainFrm->m_sCarvedCamInfo[iCamera].m_iStress == 2)
		{
			QDateTime time = QDateTime::currentDateTime();
			QString strSaveImagePath = QString(pMainFrm->m_sConfigInfo.m_strAppPath + tr("SaveImageByTime\\") +\
				tr("stress image\\") + time.date().toString("yyyy-MM-dd") + tr("\\camera%1").arg(iCamera+1)) + "\\" + time.time().toString("hh");
			bool exist = dirSaveImagePath->exists(strSaveImagePath);
			if(!exist)
			{
				bool ok = dirSaveImagePath->mkpath(strSaveImagePath);
				if (!ok)
				{
					pMainFrm->writeLogText(tr("Failure in create Path!"),AbnormityLog);
				}
			}
			QString strSavePath = QString(strSaveImagePath + "/image number%1_%2%3%4_%5.bmp").arg(pElement->nImgSN).arg(time.time().hour()).arg(time.time().minute()).arg(time.time().second()).arg(grabImageCount);
			pElement->myImage->mirrored().save(strSavePath);
		}

	}
	if (AllImage == pMainFrm->m_sRunningInfo.m_eSaveImageType || AllImageInCount == pMainFrm->m_sRunningInfo.m_eSaveImageType)
	{
		if (0 == pMainFrm->m_sSystemInfo.m_bSaveCamera[iCamera])
		{
			return;
		}
		QTime time = QTime::currentTime();
		QString strSaveImagePath = QString(pMainFrm->m_sConfigInfo.m_strAppPath + "SaveImage/All-image/camera%1/").arg(iCamera+1);
		bool exist = dirSaveImagePath->exists(strSaveImagePath);
		if(!exist)
		{
			bool ok = dirSaveImagePath->mkpath(strSaveImagePath);
			if (!ok)
			{
				pMainFrm->writeLogText(tr("Failure in create Path!"),AbnormityLog);
			}
		}
		if (AllImage == pMainFrm->m_sRunningInfo.m_eSaveImageType)
		{
			QString strSavePath = QString(strSaveImagePath + "image number%1_%2%3%4_%5.bmp").arg(pElement->nImgSN).arg(time.hour()).arg(time.minute()).arg(time.second()).arg(grabImageCount);
			//pElement->myImage.SaveBmpFile(strSavePath.toStdWString().c_str(),pElement->myImage.bitmapInfo(),pElement->myImage.buffAddr());
			pElement->myImage->mirrored().save(strSavePath);
		}
		if (AllImageInCount == pMainFrm->m_sRunningInfo.m_eSaveImageType)
		{
			pMainFrm->m_sRunningInfo.m_mutexRunningInfo.lock();
			if (pMainFrm->m_sRunningInfo.m_iSaveImgCount[iCamera] > 0)
			{
				QString strSavePath = QString(strSaveImagePath + "image number%1_%2%3%4_%5.bmp").arg(pElement->nImgSN).arg(time.hour()).arg(time.minute()).arg(time.second()).arg(grabImageCount);
//				pElement->myImage.SaveBmpFile(strSavePath.toStdWString().c_str(),pElement->myImage.bitmapInfo(),pElement->myImage.buffAddr());
				pElement->myImage->mirrored().save(strSavePath);
				pMainFrm->m_sRunningInfo.m_iSaveImgCount[iCamera]--;
			}
			int itempSavemode = 0;
			for (int i = 0 ; i<pMainFrm->m_sSystemInfo.iCamCount;i++)
			{
				if (pMainFrm->m_sSystemInfo.m_bSaveCamera[i] == 1)
				{
					itempSavemode = 1;
				}
			}
			if (0 == itempSavemode)
			{
				pMainFrm->m_sRunningInfo.m_eSaveImageType = NotSave;
			}
			pMainFrm->m_sRunningInfo.m_mutexRunningInfo.unlock();
		}
	}else if (FailureImage == pMainFrm->m_sRunningInfo.m_eSaveImageType||FailureImageInCount == pMainFrm->m_sRunningInfo.m_eSaveImageType)
	{
		if (0 == pMainFrm->m_sSystemInfo.m_bSaveCamera[iCamera])
		{
			return;
		}
		if (0 == pMainFrm->m_sSystemInfo.m_bSaveErrorType[iErrorType])
		{
			return;
		}
		QTime time = QTime::currentTime();
		QString strSaveImagePath = QString(pMainFrm->m_sConfigInfo.m_strAppPath + "SaveImage/Failure-image/camera%1").arg(iCamera+1);
		bool exist = dirSaveImagePath->exists(strSaveImagePath);
		if(!exist)
		{
			bool ok = dirSaveImagePath->mkpath(strSaveImagePath);
			if (!ok)
			{
				pMainFrm->writeLogText(tr("Failure in create Path!"),AbnormityLog);
			}
		}
		if (FailureImage == pMainFrm->m_sRunningInfo.m_eSaveImageType)
		{
			QString strSavePath = QString(strSaveImagePath + "/image number%1_%2%3%4_%5.bmp").arg(pElement->nImgSN).arg(time.hour()).arg(time.minute()).arg(time.second()).arg(grabImageCount);
			pElement->myImage->mirrored().save(strSavePath);
		}
		if (FailureImageInCount == pMainFrm->m_sRunningInfo.m_eSaveImageType)
		{
			pMainFrm->m_sRunningInfo.m_mutexRunningInfo.lock();
			if (pMainFrm->m_sRunningInfo.m_iSaveImgCount[iCamera] > 0)
			{
				QString strSavePath = QString(strSaveImagePath + "/image number%1_%2%3%4_%5.bmp").arg(pElement->nImgSN).arg(time.hour()).arg(time.minute()).arg(time.second()).arg(grabImageCount);
				pElement->myImage->mirrored().save(strSavePath);
				pMainFrm->m_sRunningInfo.m_iSaveImgCount[iCamera]--;
			}
			if (0 == pMainFrm->m_sRunningInfo.m_iSaveImgCount[iCamera])
			{
				pMainFrm->m_sSystemInfo.m_bSaveCamera[iCamera] = 0;
			}
			int itempSavemode = 0;
			for (int i = 0 ; i<pMainFrm->m_sSystemInfo.iCamCount;i++)
			{
				if (pMainFrm->m_sSystemInfo.m_bSaveCamera[i] == 1)
				{
					itempSavemode = 1;
				}
			}
			if (0 == itempSavemode)
			{
				pMainFrm->m_sRunningInfo.m_eSaveImageType = NotSave;
			}
			pMainFrm->m_sRunningInfo.m_mutexRunningInfo.unlock();
		}
	}

}
//��ȱ��ͼ������������
void DetectThread::addErrorImageList(CGrabElement *pElement)
{
	pMainFrm->m_ErrorList.m_mutexmErrorList.lock();
	CGrabElement *pErrorElement = pMainFrm->m_ErrorList.listError.last();
	pMainFrm->m_ErrorList.listError.removeLast();
	pErrorElement->nCamSN = pElement->nCamSN;
	pErrorElement->dCostTime = pElement->dCostTime;
	pErrorElement->nCheckRet = pElement->nCheckRet;
	pErrorElement->nGrabImageCount = pElement->nGrabImageCount;
	pErrorElement->nImgSN = pElement->nImgSN;
	pErrorElement->nImgType = pElement->nImgType;
	pErrorElement->nSignalNo = pElement->nSignalNo; 
	pErrorElement->cErrorRectList = pElement->cErrorRectList;
	pErrorElement->cErrorParaList = pElement->cErrorParaList;

	pErrorElement->sImgLocInfo.sLocOri = pElement->sImgLocInfo.sLocOri;
	pErrorElement->sImgLocInfo.sXldPoint.nCount = pElement->sImgLocInfo.sXldPoint.nCount;
	memcpy(pErrorElement->sImgLocInfo.sXldPoint.nColsAry,pElement->sImgLocInfo.sXldPoint.nColsAry,4*BOTTLEXLD_POINTNUM);							
	memcpy(pErrorElement->sImgLocInfo.sXldPoint.nRowsAry,pElement->sImgLocInfo.sXldPoint.nRowsAry,4*BOTTLEXLD_POINTNUM);

	pErrorElement->cErrorParaList.clear();
	for (int i =0;i<pElement->cErrorParaList.length();i++)
	{
		s_ErrorPara tempErrorPara = pElement->cErrorParaList.at(i);
		pErrorElement->cErrorParaList.append(tempErrorPara);
	}
	/****************************************************************************/
	if (pErrorElement->myImage != NULL)
	{
		delete pErrorElement->myImage;
		pErrorElement->myImage = NULL;
	}
	pErrorElement->myImage = new QImage(*pElement->myImage);
	
	pMainFrm->m_ErrorList.listError.push_front(pErrorElement);
	pMainFrm->m_ErrorList.m_mutexmErrorList.unlock();
	QString result;
	if(pElement->cErrorParaList.first().nErrorType==8)
	{
		result = pAlgCheckResult->vErrorParaAry->strErroeNm;
	}else{
		result = pMainFrm->m_sErrorInfo.m_vstrErrorType.at(iErrorType);
	}
	emit signals_AddErrorTableView(pElement->nCamSN,pElement->nSignalNo,result);
}
//������ʾ״̬
void DetectThread::upDateState( QImage* myImage, int signalNo,double costTime,int MouldID, QList<QRect> listErrorRectList,int QueenID)
{
	QString camera = QString::number(this->iCamera+1);
	QString imageSN = QString::number(signalNo);
	QString time = QString::number(costTime,'f',2);
	
	QString nMouldID=QString::number(MouldID);
	QString result;

	if(iErrorType==8)
	{
		result = pAlgCheckResult->vErrorParaAry->strErroeNm;
	}else{
		result = pMainFrm->m_sErrorInfo.m_vstrErrorType.at(iErrorType);
	}
	
	if(pMainFrm->number_camera!=-1)
	{
		emit signals_upDateCamera(iCamera,1 );
		emit signals_updateActiveImg(iCamera,signalNo,costTime,iErrorType);//���¼��е�ͼ����ʾ
		emit signals_updateImage(myImage, camera, imageSN, time, result, nMouldID,listErrorRectList, QueenID);
	}else{
		if(isShowPicture[signalNo]==0)
		{
			isShowPicture[signalNo]=1;
			emit signals_upDateCamera(iCamera,1 );
			if(pMainFrm->widget_carveSetting->image_widget->bIsCarveWidgetShow)
			{
				emit signals_updateActiveImg(iCamera,signalNo,costTime,iErrorType);//���¼��е�ͼ����ʾ
			}
			emit signals_updateImage(myImage, camera, imageSN, time, result, nMouldID,listErrorRectList, QueenID);

			if (pMainFrm->m_sRunningInfo.m_checkedNum != 0)
			{
				pMainFrm->m_sRunningInfo.m_iErrorCamRate[iCamera] = 1.0*pMainFrm->m_sRunningInfo.m_iErrorCamCount[iCamera]/pMainFrm->m_sRunningInfo.m_checkedNum * 100;
			}
			else
			{
				pMainFrm->m_sRunningInfo.m_iErrorCamRate[iCamera] = 0;
			}
			emit signals_updateCameraFailureRate();
		}
		if(pAlgCheckResult->nSizeError >0&&isShowPicture[signalNo]!=2)
		{
			isShowPicture[signalNo]=2;
			emit signals_upDateCamera(iCamera,1 );
			if(pMainFrm->widget_carveSetting->image_widget->bIsCarveWidgetShow==false)
			{
				emit signals_updateActiveImg(iCamera,signalNo,costTime,iErrorType);//���¼��е�ͼ����ʾ
			}
			emit signals_updateImage(myImage, camera, imageSN, time, result, nMouldID,listErrorRectList, QueenID);

			if (pMainFrm->m_sRunningInfo.m_checkedNum != 0)
			{
				pMainFrm->m_sRunningInfo.m_iErrorCamRate[iCamera] = 1.0*pMainFrm->m_sRunningInfo.m_iErrorCamCount[iCamera]/pMainFrm->m_sRunningInfo.m_checkedNum * 100;
			}
			else
			{
				pMainFrm->m_sRunningInfo.m_iErrorCamRate[iCamera] = 0;
			}
			emit signals_updateCameraFailureRate();
		}
	}
	QImage temp=QImage(*myImage);
	emit signals_updateMaxImageItem(temp,camera,imageSN,time,result,nMouldID,listErrorRectList,QueenID,grabImageCount);
}

void DetectThread::CountRuningData( int cameraNumber,int nGrabImageCount )
{
	if (cameraNumber == 1&&nGrabImageCount==1)
	{
		static DWORD lastSpeed[CAMERA_MAX_COUNT];
		static int nSpeedCount[CAMERA_MAX_COUNT];
		if(lastSpeed[cameraNumber] == 0)
		{
			lastSpeed[cameraNumber] = timeGetTime();
		}
		else
		{
			nSpeedCount[cameraNumber]++;			
			int nCurTime = timeGetTime();
			//����ƿ�Ӵ����ٶ� = ����/ʱ�䣨min����������1s����һ��
			if (nCurTime-lastSpeed[cameraNumber] > 5000)
			{
				int nCurSpeed = nSpeedCount[cameraNumber]*1.0 / (nCurTime-lastSpeed[cameraNumber]) * 60000;
				//qDebug()<<"ƿ�Ӹ�����"<<nSpeedCount[cameraNumber]<<",ʱ�䣺"<<nCurTime-lastSpeed[cameraNumber]<<",�ٶȣ�"<<nCurSpeed;
				lastSpeed[cameraNumber] = nCurTime;
				nSpeedCount[cameraNumber] = 0;
				pMainFrm->m_sRunningInfo.strSpeed = QString::number(nCurSpeed);
				emit signals_showspeed(nCurSpeed);
			}
		}
	}
}

void DetectThread::KickOut( int glasswareNum, int isKickOut, int ioCardNum )
{
	//pMainFrm->m_cCombine.m_MutexCombin.lock();
	pMainFrm->m_cCombine.AddResult(glasswareNum,iCamera,isKickOut);
	int comResult = -1;
	if (pMainFrm->m_cCombine.ConbineResult(glasswareNum,pMainFrm->m_sSystemInfo.IOCardiCamCount[0],comResult))//ͼ����������ۺ�
	{
		for	(int i = glasswareNum-5; i<glasswareNum ;i++)
		{
			if (!pMainFrm->m_cCombine.IsReject((i+256)%256))
			{
				s_ResultInfo sResultInfo;
				sResultInfo.tmpResult = pMainFrm->m_cCombine.m_Rlts[(i+256)%256].iResult;
				sResultInfo.nImgNo = (i+256)%256;
				sResultInfo.nIOCardNum = 0;

				//pMainFrm->m_mutexmSendResult.lock();
				if (pMainFrm->m_sSystemInfo.m_bIsIOCardOK)
				{
					pMainFrm->m_vIOCard[sResultInfo.nIOCardNum]->SendResult(sResultInfo);
				}
				pMainFrm->m_cCombine.SetReject((i+256)%256);
				//pMainFrm->m_mutexmSendResult.unlock();
			}
		}
		for	(int i = glasswareNum; i<glasswareNum+5;i++)
		{
			pMainFrm->m_cCombine.SetReject(i%256,false);
		}
		for(int ins = 0; ins < pMainFrm->m_sSystemInfo.iRealCamCount; ins++)
		{
			pMainFrm->pdetthread[ins]->isShowPicture[(glasswareNum+10)%256] = 0;
		}
		s_ResultInfo sResultInfo;
		sResultInfo.tmpResult = comResult;
		sResultInfo.nImgNo = glasswareNum;
		sResultInfo.nIOCardNum = 0;
		//pMainFrm->m_mutexmSendResult.lock();
		if (pMainFrm->m_sSystemInfo.m_bIsIOCardOK)
		{
			pMainFrm->m_vIOCard[sResultInfo.nIOCardNum]->SendResult(sResultInfo);
		}

		//pMainFrm->m_mutexmSendResult.unlock();
		pMainFrm->m_cCombine.SetReject(glasswareNum);
		pMainFrm->m_cCombine.RemoveOneResult(glasswareNum);
		//����
		if (pMainFrm->m_sRunningInfo.m_bCheck)	
		{
			int iErrorCamera = pMainFrm->m_cCombine.ErrorCamera(glasswareNum);
			s_ErrorPara sComErrorpara = pMainFrm->m_cCombine.ConbineError(glasswareNum);
			if (pMainFrm->m_sRunningInfo.m_cErrorTypeInfo[iErrorCamera].ErrorTypeJudge(sComErrorpara.nErrorType))
			{
				//pMainFrm->m_sRunningInfo.m_mutexRunningInfo.lock();
				pMainFrm->m_sRunningInfo.m_cErrorTypeInfo[iErrorCamera].iErrorCountByType[sComErrorpara.nErrorType]+=1;
				pMainFrm->m_sRunningInfo.m_iErrorCamCount[iErrorCamera] += 1;
				pMainFrm->m_sRunningInfo.m_iErrorTypeCount[sComErrorpara.nErrorType] +=1;
				//ifSendAret(sComErrorpara.nErrorType);
				//pMainFrm->m_sRunningInfo.m_mutexRunningInfo.unlock();
			}
			else
			{
				//pMainFrm->m_sRunningInfo.m_mutexRunningInfo.lock();
				pMainFrm->m_sRunningInfo.m_cErrorTypeInfo[iErrorCamera].iErrorCountByType[0]+=1;
				pMainFrm->m_sRunningInfo.m_iErrorTypeCount[0] +=1;
				//pMainFrm->m_sRunningInfo.m_mutexRunningInfo.unlock();
			}	
		}
	}
	//pMainFrm->m_cCombine.m_MutexCombin.unlock();
}

void DetectThread::ifSendAret(int i)
{
	i--;
	time_t t;
	t = time(NULL);
	if(pMainFrm->test_widget->iIsTrackErrorType[i])
	{
		pMainFrm->AertNumber[i].push_back(t);
	}
	if(pMainFrm->AertNumber[i].count() >= pMainFrm->test_widget->iStatisMaxNumber[i] && pMainFrm->test_widget->iStatisMaxNumber[i]!=0)
	{
		if(t-pMainFrm->AertNumber[i].at(0)<pMainFrm->test_widget->iStatisMaxTime[i]*60)
		{
			//���ͱ����ź�
			pMainFrm->AertNumber[i].clear();
			if (pMainFrm->m_sSystemInfo.m_bIsIOCardOK)
			{
				pMainFrm->m_vIOCard[0]->m_mutexmIOCard.lock();
				pMainFrm->m_vIOCard[0]->m_Pio24b.setCardOutput(7,1);
				pMainFrm->m_vIOCard[0]->m_mutexmIOCard.unlock();
			}
			//������ʾ��
			pMainFrm->slots_ShowPLCStatus(i+pMainFrm->m_vstrPLCInfoType.size()+2);
			//�ı�ؼ�����ɫ
			pMainFrm->test_widget->ShowAssert(i);
		}
	}
}