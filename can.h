
#ifndef EXPORTS
	#ifdef __cplusplus
		#define EXPORTS extern "C" __declspec(dllimport)
	#else
		#define EXPORTS __declspec(dllimport)
	#endif
#endif


#ifndef __PISO_CAN_HEADER__
#define __PISO_CAN_HEADER__

// return code
#ifndef DWORD
#define DWORD unsigned long
#endif

#ifndef WORD
#define WORD unsigned short
#endif

#ifndef BYTE
#define BYTE unsigned char
#endif

BYTE B0 = 0;
BYTE P1 = 1,P2 = 2;

//====================
#define CAN_NoError                    0
#define CAN_DriverError                1
#define CAN_ActiveBoardError           2
#define CAN_BoardNumberError           3
#define CAN_PortNumberError            4 
#define CAN_ResetError                 5
#define CAN_SoftResetError             6
#define CAN_InitError                  7       
#define CAN_ConfigError                8
#define CAN_SetACRError                9
#define CAN_SetAMRError                10
#define CAN_SetBaudRateError           11

#define CAN_EnableRxIrqFailure         12
#define CAN_DisableRxIrqFailure        13
#define CAN_InstallIrqFailure          14 
#define CAN_RemoveIrqFailure           15

#define CAN_TransmitBufferLocked       16
#define CAN_TransmitIncomplete         17
#define CAN_ReceiveBufferEmpty         18
#define CAN_DataOverrun                19
#define CAN_ReceiveError               20
#define CAN_SoftBufferIsEmpty          21
#define CAN_SoftBufferIsFull           22
#define CAN_TimeOut                    23

#define CAN_InstallIsrError			   24

#define CAN_BusOffError				   25

typedef struct config
{
	BYTE AccCode[4];
	BYTE AccMask[4];
	BYTE BaudRate;
	BYTE BT0,BT1;
} ConfigStruct;

//BaudRate 0:user define 1=10K   2=20K   3=50K  4=125K  5=250K  6=500K 7=800K  8=1Mbps

typedef struct packet
{	
	LONGLONG MsgTimeStamps;
	BYTE  mode;
	DWORD id;
	BYTE  rtr;
	BYTE  len;
	BYTE  data[8];
} PacketStruct;

//mode 0 -> 11 bits ID    1 -> 29 bits ID 

EXPORTS WORD CALLBACK CAN_GetDllVersion();
EXPORTS int CALLBACK CAN_TotalBoard();
EXPORTS int CALLBACK CAN_GetBoardInf(BYTE BoardNo, DWORD *dwVID, DWORD *dwDID,
									DWORD *dwSVID, DWORD *dwSDID,DWORD *dwSAuxID,
						            DWORD* dwIrqNo);
EXPORTS int CALLBACK CAN_GetCardPortNum(BYTE BoardNo, BYTE *bGetPortNum);

EXPORTS int CALLBACK CAN_ActiveBoard(BYTE BoardNo);
EXPORTS int CALLBACK CAN_CloseBoard(BYTE BoardNo);
EXPORTS int CALLBACK CAN_BoardIsActive(BYTE BoardNo);

EXPORTS int CALLBACK CAN_Reset(BYTE BoardNo,BYTE Port);
EXPORTS int CALLBACK CAN_Init(BYTE BoardNo,BYTE Port);
EXPORTS int CALLBACK CAN_Config(BYTE BoardNo, BYTE Port,ConfigStruct *CanConfig);
EXPORTS int CALLBACK CAN_ConfigWithoutStruct(BYTE BoardNo, BYTE Port, DWORD AccCode, DWORD AccMask, BYTE BaudRate, BYTE BT0, BYTE BT1);

EXPORTS int CALLBACK CAN_EnableRxIrq(BYTE BoardNo,BYTE Port);
EXPORTS int CALLBACK CAN_DisableRxIrq(BYTE BoardNo,BYTE Port);
EXPORTS int CALLBACK CAN_RxIrqStatus(BYTE BoardNo,BYTE Port,BYTE *bStatus);
EXPORTS int CALLBACK CAN_InstallIrq(BYTE BoardNo);
EXPORTS int CALLBACK CAN_RemoveIrq(BYTE BoardNo);
EXPORTS int CALLBACK CAN_IrqStatus(BYTE BoardNo,BYTE *bStatus);

EXPORTS int CALLBACK CAN_Status(BYTE BoardNo, BYTE Port,BYTE *bStatus);

EXPORTS int CALLBACK CAN_SendMsg(BYTE BoardNo, BYTE Port, PacketStruct *CanPacket);
EXPORTS int CALLBACK CAN_SendWithoutStruct(BYTE BoardNo, BYTE Port, BYTE Mode, DWORD Id, BYTE Rtr, BYTE Dlen, BYTE *Data);
EXPORTS int CALLBACK CAN_SendWithoutStruct_NoChk(BYTE BoardNo, BYTE Port, BYTE Mode, DWORD Id, BYTE Rtr, BYTE Dlen, BYTE *Data);

EXPORTS int CALLBACK CAN_RxMsgCount(BYTE BoardNo, BYTE Port);
EXPORTS int CALLBACK CAN_ReceiveMsg(BYTE BoardNo, BYTE Port,PacketStruct *CanPacket);
EXPORTS int CALLBACK CAN_ReceiveWithoutStruct(BYTE BoardNo, BYTE Port, BYTE *Mode, DWORD *Id, BYTE *Rtr, BYTE *Dlen, BYTE *Data, DWORD *H_MsgTimeStamps ,DWORD *L_MsgTimeStamps);

EXPORTS int CALLBACK CAN_ClearSoftBuffer(BYTE BoardNo, BYTE Port);
EXPORTS int CALLBACK CAN_ClearDataOverrun(BYTE BoardNo, BYTE Port);

EXPORTS void CALLBACK CAN_OutputByte(BYTE BoardNo,BYTE Port,WORD wOffset,BYTE bValue);
EXPORTS BYTE CALLBACK CAN_InputByte(BYTE BoardNo,BYTE Port,WORD wOffset);
EXPORTS LONGLONG CALLBACK CAN_GetSystemFreq(void);
EXPORTS int CALLBACK CAN_BusErrorCode(BYTE BoardNo, BYTE Port,BYTE *bErrorCode);

EXPORTS int CALLBACK CAN_InstallUserIsr(BYTE BoardNo, void (*UserISR)(BYTE BoardNo));
EXPORTS int CALLBACK CAN_RemoveUserIsr(BYTE BoardNo);

#endif // end define __PISO_CAN_HEADER__

