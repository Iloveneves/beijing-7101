/*******************�ļ�˵��ע��************************************/
/*��˾��Ȩ˵������Ȩ��2007���鳤������Ƽ����޹�˾���С�           */
/*�ļ�����ch_chard.c                                               */
/*�汾�ţ�V1.0                                                     */
/*���ߣ�  �����                                                   */
/*�������ڣ�2007-04-13                                             */
/*ģ�鹦��:���SMSMART CARD��ʼ������������                        */
/*��Ҫ����������:                                                  */
/*                                                                 */
/*�޸���ʷ���޸�ʱ��    ����           �޸Ĳ���        ԭ��        */             
/*          20070429    zxg       �����Ϲ淶����   ���㹫˾����淶*/               
/*******************************************************************/

/**************** #include ����*************************************/
#include "stddefs.h"  /* standard definitions           */
#include "stevt.h"
#include "stlite.h"   /* os20 for standard definitions, */

#include "..\main\initterm.h"

#include "stddefs.h"
#include "stcommon.h"
#include "appltype.h"
#include "stuart.h"
#include "stsmart.h"
#include "stevt.h"
#include "..\report\report.h"

/*******************************************************************/
/**************** #define ����**************************************/
#define SYSTEM_CONFIG_BASE               (U32)0xB9001000                     /*ϵͳ����ַ*/
#define SYS_CONFIG_7_OFFSET              0x11C                               /*ƫ�Ƶ�ַ*/               
#define SYSTEM_CONFIG_7                  (SYSTEM_CONFIG_BASE + SYS_CONFIG_7_OFFSET)  /*SYSTEM_CONFIG_7��ַ*/

/***************  ȫ�ֱ������岿�� *********************************/
ST_DeviceName_t				gST_SCEventDeviceName[1] = {"event_sc"};
/* ���ܣ�SC�����¼���������;                                       */
/* ֵ��[x-xxxxxx....],xλ�ַ�                                    */
/* Ӧ�û�����1: SMATT CARD��ʼ��ʱSTSMART_InitParams_t����         */
/*           2: STEVT STEVT_Init ������ڲ���                      */
/*           3: STEVT STEVT_Open ������ڲ���                      */
/* ע�����1,2,3���ò���������ͬ                                 */
STSMART_Handle_t            gST_SCHandle;
/* ���ܣ� �ݴ�STSMART_Open���handle,������STSMARTģ�麯��ʹ��     */
/* ֵ�� [0-0XFFFFFFFF]                                            */
/* Ӧ�û�����1: ����STSMART�ӿں���                                */
/* ע�����                                                      */
char                        gc_Cardstatus = 0;		
/* ���ܣ��ݴ浱ǰ���ܿ���״̬,
 0:not inserted	1:inserted	2:OK	other:error;                   */
/* ֵ��[-128-127]                                                */
/* Ӧ�û�����1: ���ܿ����ģ��                                     */
/* ע�����                                                      */        
semaphore_t                 *gpST_SemSCCheck = NULL;
/* ���ܣ����ܿ�״̬�仯��⴦��ͬ��                                */
/* ֵ��[0x80000000-0x80400000]                                   */
/* Ӧ�û�����1:���ܿ�״̬�仯����
             2:���ܿ����ģ��                                      */
/* ע�����                                                      */
task_t                      *gpST_SCTask = NULL;
/* ���ܣ�ռ�����ܿ����ģ������ָ��                                */
/* ֵ��[0x80000000-0x80400000]                                   */
/* Ӧ�û�����1����Ҫ�������ܿ����ģ������״̬��ģ��               */
/* ע�����                                                      */
/*******************************************************************/
/***********************�ⲿ��������********************************/

extern ST_Partition_t *SystemPartition;

/************************��������***********************************/

void  CH_SC_EvtNotifyFunc(STEVT_CallReason_t rST_Reason, STEVT_EventConstant_t rST_Event, const void *rp_EventData);
static void CH_SC_Monitor(void *pvParam);

/***************************�����嶨��********************************/ 
/******************************************************************/
/*������:BOOL CH_SC_HwInit��void��                                 */
/*�����˺Ϳ���ʱ��:zengxianggen 2007-04-13                        */
/*������������:��ʼ��SMART CARDģ��                               */
/*����ԭ��˵��:                                                   */
/*�����������                                                    */
/*�������: ��                                                    */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                      */
/*���õ���Ҫ�����б�                                            */
/*����ֵ˵����TRUE, ʧ��,FALSE,�ɹ�                               */   
/*����ע������:                                                   */
/*����˵��:                                                       */                                             
BOOL CH_SC_HwInit(void)
{
    #define CARD_INDEX                     0   /*ʹ�� SMART CRAD ��������Ӳ����� */
	ST_ErrorCode_t st_ErrCode = ST_NO_ERROR;     
	STUART_InitParams_t  st_UartInitParams[4];    /*UART��ʼ������*/  
	STSMART_InitParams_t st_SmartInitParams[2];  /*SMART CARD��ʼ������*/
	STSMART_OpenParams_t st_SmartOpenParams;     /*SMART CARD OPEN����*/
    U32 ui_CFG7Value;
    STEVT_InitParams_t st_EVTInitParams;
	STUART_Params_t    st_UartDefaultParams; 	

	STEVT_OpenParams_t st_EVTOpenParams;       
	STEVT_SubscribeParams_t st_SubscribeParams = { CH_SC_EvtNotifyFunc};   
	ST_DeviceName_t st_SmartDeviceName[] = {"SMART0","SMART1","SMART2"};
	STEVT_Handle_t st_EVTHandle;	
	ST_ClockInfo_t st_ClockInfo;


	STSYS_WriteRegDev32LE(SYSTEM_CONFIG_7,0x1B0  );/*zxg 20070417 change*/
    do_report(0,"CFG7Value2=%x\n",ui_CFG7Value);
	ST_GetClockInfo(&st_ClockInfo);

   	st_UartDefaultParams.RxMode.DataBits = STUART_8BITS_EVEN_PARITY;
    st_UartDefaultParams.RxMode.StopBits = STUART_STOP_0_5;
    st_UartDefaultParams.RxMode.FlowControl = STUART_NO_FLOW_CONTROL/*STUART_SW_FLOW_CONTROL*/;
    st_UartDefaultParams.RxMode.BaudRate = /*9600*/115200;
    st_UartDefaultParams.RxMode.TermString = NULL;
    st_UartDefaultParams.RxMode.NotifyFunction = NULL;
    st_UartDefaultParams.TxMode.DataBits = STUART_8BITS_EVEN_PARITY;
    st_UartDefaultParams.TxMode.StopBits = STUART_STOP_0_5;
    st_UartDefaultParams.TxMode.FlowControl =/* STUART_NO_FLOW_CONTROL*/STUART_SW_FLOW_CONTROL;
    st_UartDefaultParams.TxMode.BaudRate =115200;
    st_UartDefaultParams.TxMode.TermString = NULL;
    st_UartDefaultParams.TxMode.NotifyFunction = NULL;
    st_UartDefaultParams.SmartCardModeEnabled = TRUE;
    st_UartDefaultParams.GuardTime = 22;
    st_UartDefaultParams.Retries = 6;
    st_UartDefaultParams.NACKSignalDisabled = FALSE;
    st_UartDefaultParams.HWFifoDisabled = FALSE;
    st_UartDefaultParams.SmartCardModeEnabled = TRUE;

    st_SmartOpenParams.NotifyFunction = NULL;
    st_SmartOpenParams.BlockingIO = TRUE;

    /*UartInitParams set */
    st_UartInitParams[CARD_INDEX].UARTType = STUART_ISO7816;  
    st_UartInitParams[CARD_INDEX].DriverPartition = SystemPartition;
    if(CARD_INDEX == 0)
	{
       st_UartInitParams[CARD_INDEX].BaseAddress = (U32 *)ASC_0_BASE_ADDRESS;
       st_UartInitParams[CARD_INDEX].InterruptNumber = ASC_0_INTERRUPT;
       st_UartInitParams[CARD_INDEX].InterruptLevel = ASC_0_INTERRUPT_LEVEL;
	}
    else if(CARD_INDEX == 1)
	{
       st_UartInitParams[CARD_INDEX].BaseAddress = (U32 *)ASC_1_BASE_ADDRESS;
       st_UartInitParams[CARD_INDEX].InterruptNumber = ASC_1_INTERRUPT;
       st_UartInitParams[CARD_INDEX].InterruptLevel = ASC_1_INTERRUPT_LEVEL;
	}
    st_UartInitParams[CARD_INDEX].ClockFrequency = 66500000 /*zxg 20070417 change ClockInfo.ckga.com_clk*/;
    st_UartInitParams[CARD_INDEX].SwFIFOEnable = TRUE;
    st_UartInitParams[CARD_INDEX].FIFOLength = 256;
    strcpy(st_UartInitParams[CARD_INDEX].RXD.PortName, PIO_DeviceName[CARD_INDEX]);
    st_UartInitParams[CARD_INDEX].RXD.BitMask = PIO_BIT_1;        /*���ݽ���λ*/
    strcpy(st_UartInitParams[CARD_INDEX].TXD.PortName, PIO_DeviceName[CARD_INDEX]);
    st_UartInitParams[CARD_INDEX].TXD.BitMask = PIO_BIT_0;        /*���ݴ���λ*/
    strcpy(st_UartInitParams[CARD_INDEX].RTS.PortName, "\0");
    st_UartInitParams[CARD_INDEX].RTS.BitMask = 0;
    strcpy(st_UartInitParams[CARD_INDEX].CTS.PortName, "\0");
    st_UartInitParams[CARD_INDEX].CTS.BitMask = 0;
    st_UartInitParams[CARD_INDEX].DefaultParams = NULL;

    /* Smartcard  initialization params */
    strcpy(st_SmartInitParams[CARD_INDEX].UARTDeviceName, UART_DeviceName[CARD_INDEX]);
    st_SmartInitParams[CARD_INDEX].DeviceType = STSMART_ISO;
    st_SmartInitParams[CARD_INDEX].DriverPartition = SystemPartition;
    if(CARD_INDEX == 0)
	{
		st_SmartInitParams[CARD_INDEX].BaseAddress = (U32 *)SMARTCARD0_BASE_ADDRESS;
	}
	else if(CARD_INDEX == 1)
	{
		st_SmartInitParams[CARD_INDEX].BaseAddress = (U32 *)SMARTCARD1_BASE_ADDRESS;
	}
	strcpy(st_SmartInitParams[CARD_INDEX].ClkGenExtClk.PortName, "\0");
    st_SmartInitParams[CARD_INDEX].ClkGenExtClk.BitMask = 0;
    strcpy(st_SmartInitParams[CARD_INDEX].Clk.PortName, PIO_DeviceName[CARD_INDEX]);
    st_SmartInitParams[CARD_INDEX].Clk.BitMask = PIO_BIT_3;             /*ʱ��λ*/    
    strcpy(st_SmartInitParams[CARD_INDEX].Reset.PortName,  PIO_DeviceName[CARD_INDEX]);
    st_SmartInitParams[CARD_INDEX].Reset.BitMask = PIO_BIT_4;          /*��λλ*/
    strcpy(st_SmartInitParams[CARD_INDEX].CmdVcc.PortName, PIO_DeviceName[CARD_INDEX]);
    st_SmartInitParams[CARD_INDEX].CmdVcc.BitMask = PIO_BIT_5;         /*��Դλ*/
    strcpy(st_SmartInitParams[CARD_INDEX].CmdVpp.PortName, "\0");
    st_SmartInitParams[CARD_INDEX].CmdVpp.BitMask = 0;
    strcpy(st_SmartInitParams[CARD_INDEX].Detect.PortName, PIO_DeviceName[CARD_INDEX]);
    st_SmartInitParams[CARD_INDEX].Detect.BitMask = PIO_BIT_7;         /*���λ*/
    st_SmartInitParams[CARD_INDEX].ClockSource = STSMART_CPU_CLOCK;
    st_SmartInitParams[CARD_INDEX].ClockFrequency = 66500000 /*zxg 20070417 change ClockInfo.ckga.com_clk*/;


    st_SmartInitParams[CARD_INDEX].MaxHandles = 32;
    strcpy(st_SmartInitParams[CARD_INDEX].EVTDeviceName, gST_SCEventDeviceName[CARD_INDEX] );       

	st_SmartInitParams[CARD_INDEX].IsInternalSmartcard = false;

    /* Event initialization params */
    st_EVTInitParams.MemoryPartition = SystemPartition;
    st_EVTInitParams.EventMaxNum = STSMART_NUMBER_EVENTS * 2;
    st_EVTInitParams.ConnectMaxNum = 3;
    st_EVTInitParams.SubscrMaxNum = STSMART_NUMBER_EVENTS * 2;    
   
    /* UART initialization */
    st_ErrCode = STUART_Init(UART_DeviceName[CARD_INDEX], &st_UartInitParams[CARD_INDEX]);
    if (st_ErrCode != ST_NO_ERROR)
     	{
     	do_report(0,"\n[IRDETO_SC]Unable to initialize UART0\n");
        return st_ErrCode;
        }

     /* EVT_SC  initialization */
     st_ErrCode = STEVT_Init(gST_SCEventDeviceName[CARD_INDEX], &st_EVTInitParams);
     if (st_ErrCode != ST_NO_ERROR)
        {
        do_report(0,"\n[IRDETO_SC]Unable to initialize EVT_SC0  device\n");
        return st_ErrCode;
        }
     
     /* SC  initialization */
     st_ErrCode = STSMART_Init(st_SmartDeviceName[CARD_INDEX], &st_SmartInitParams[CARD_INDEX]);
     if (st_ErrCode != ST_NO_ERROR)
        {
        do_report(0,"\n[IRDETO_SC]Unable to initialize SC0\n");
        return st_ErrCode; 
        }      

      /* SC  Open */  
     st_ErrCode = STSMART_Open(st_SmartDeviceName[CARD_INDEX], &st_SmartOpenParams, &gST_SCHandle);
     if (st_ErrCode != ST_NO_ERROR)
    	{
    	do_report(0, "\nUnable to open SC\n" );  	
    	return st_ErrCode; 	
        }
     else
    	{
    	do_report(0,"\nOpen Smartcard slot successfully\n");
    	
    	}
     /* EVT_SC Open and Subscribe Smart_card Event */  
     st_ErrCode = STEVT_Open( gST_SCEventDeviceName[CARD_INDEX], &st_EVTOpenParams, &st_EVTHandle);                        
     st_ErrCode = STEVT_Subscribe(st_EVTHandle,
            	STSMART_EV_CARD_INSERTED,
            	&st_SubscribeParams);            
     st_ErrCode = STEVT_Subscribe(st_EVTHandle,
            	STSMART_EV_CARD_REMOVED,
            	&st_SubscribeParams);    
	 gpST_SemSCCheck   =semaphore_create_fifo(0);	 
	 if(gpST_SemSCCheck==NULL)
	 {
		 do_report ( 100 , "Failed to init sc semphore\n" );
		 return true;
	 }
	 gpST_SCTask=Task_Create ( CH_SC_Monitor, NULL, 8192,8+3, "CHCA_SCMonitor", 0 );
	 if (gpST_SCTask == NULL )
	 {
		 do_report ( 100 , "Failed to start CHCA_SCManager process\n" );
		 return true;
	 } 
    return st_ErrCode;
}
/*******************************************************************/
/************************����˵��***********************************/
/*������:void CH_SC_EvtNotifyFunc(STEVT_CallReason_t rST_Reason, STEVT_EventConstant_t rST_Event, const void *rp_EventData)*/
/*�����˺Ϳ���ʱ��:zengxianggen 2007-04-13                        */
/*������������:SMART CARD�¼�֪ͨ                                 */
/*����ԭ��˵��:                                                   */
/*���������
STEVT_CallReason_t rST_Reason,   �����¼���ԭ��,                        
STEVT_EventConstant_t rST_Event ,�����¼�������,
const void *rp_EventData ,       �����¼�������                   */
/*�������: ��                                                    */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                      */
/*���õ���Ҫ�����б�                                            */
/*����ֵ˵������                                                  */ 
/*����ע������:                                                   */
/*����˵��:                                                       */                                             
                                                
void  CH_SC_EvtNotifyFunc
    (STEVT_CallReason_t rST_Reason, STEVT_EventConstant_t rST_Event, const void *rp_EventData)
{
	
	switch(rST_Event)
	{
	case STSMART_EV_CARD_INSERTED:
		do_report(severity_info,"THE SMART CARD HAS BEEN INSERTED!!!\n");
		gc_Cardstatus = 1;
		semaphore_signal(gpST_SemSCCheck);

		break;
		
	case STSMART_EV_CARD_REMOVED:
		gc_Cardstatus = 0;
		do_report(severity_info,"THE SMART CARD HAS BEEN REMOVED!!!\n");
		semaphore_signal(gpST_SemSCCheck);

		break;
	}
	return;	
}
/*******************************************************************/
/************************����˵��***********************************/
/*������:BOOL CH_SC_Transfer(U8 *rpuc_Command_p,
                    U32 rui_NumberToWrite,
                    U32 *rpui_NumberWritten_p,
                    U8 *rpuc_Response_p,
                    U32 rui_NumberToRead,
                    U32 *rpui_NumberRead_p)                        */
/*�����˺Ϳ���ʱ��:zengxianggen 2007-04-13                         */
/*������������:                                                    */
/*����ԭ��˵��:        ���ܿ�����ͨѶ                              */
/*���������
U8 *rpuc_Command_p,   ���͸���������BUFFER                       
U32 rui_NumberToWrite,����д�뿨�����ݴ�С
U8 *rpuc_Response_p,���ܿ����ص�����BUFFER
U32 rui_NumberToRead,���ܿ����ص����ݴ�С                         */
/*�������: U32 *rpui_NumberWritten_p,ʵ��д�뿨�����ݴ�С
            U32 *rpui_NumberRead_p,ʵ�ʴ����ܿ����ص����ݴ�С     */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                      */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                      */
/*���õ���Ҫ�����б�                                            */
/*����ֵ˵����TRUE, ʧ��,FALSE,�ɹ�                               */  
/*����ע������:                                                   */
/*����˵��:                                                       */                                             
                                               
BOOL CH_SC_Transfer(U8 *rpuc_Command_p,
                    U32 rui_NumberToWrite,
                    U32 *rpui_NumberWritten_p,
                    U8 *rpuc_Response_p,
                    U32 rui_NumberToRead,
                    U32 *rpui_NumberRead_p)
{
	ST_ErrorCode_t    st_ErrCode = -1; 		
	STSMART_Status_t  st_Status; 
	U32	ui_chWantLen=0;
	U32 ui_NumberWritten;
	U8  i=0,ui_SW1,ui_SW2;
	st_ErrCode = STSMART_Transfer(gST_SCHandle, (U8*)rpuc_Command_p, rui_NumberToWrite, 		
	rpui_NumberWritten_p,rpuc_Response_p, rui_NumberToRead, rpui_NumberRead_p, &st_Status);    
	if (st_ErrCode == ST_NO_ERROR)
	{
		ui_SW1=st_Status.StatusBlock.T0.PB[0];
		ui_SW2=st_Status.StatusBlock.T0.PB[1];
		return false;
	}
	else
		return true;	
}

/*******************************************************************/
/************************����˵��***********************************/
/*������:BOOL CH_SC_Reset(void)                                     */
/*�����˺Ϳ���ʱ��:zengxianggen 2007-04-13                         */
/*������������:                                                    */
/*����ԭ��˵��:        ���ܿ���λ                                  */
/*�����������                                                     */
/*�������: ��                                                     */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                       */
/*���õ���Ҫ�����б�                                             */
/*����ֵ˵����TRUE, ʧ��,FALSE,�ɹ�                                */                                                 
BOOL CH_SC_Reset(void)
{
    #define SC_REPEATE_TIMES  3

	ST_ErrorCode_t  st_err = ST_NO_ERROR;
	U8 uc_ATRbuf[33],i=0;
	U32 ui_ATRLength;
    STSMART_Status_t st_Status;
    
#if 0
	 /* Check that a card is inserted before commencing */
    {
        st_err = STSMART_GetStatus(gST_SCHandle, &st_Status);
        if (st_err == STSMART_ERROR_NOT_INSERTED)
        {
            STTBX_Print(("Please insert a smartcard to proceed...\n"));
            do
            {
               task_delay(ST_GetClocksPerSecond());
            } while (STSMART_GetStatus(gST_SCHandle, &st_Status) == STSMART_ERROR_NOT_INSERTED);
        }
    }
#endif
	while( i++ < SC_REPEATE_TIMES )
	{
		st_err = STSMART_Reset(gST_SCHandle, uc_ATRbuf, &ui_ATRLength);
		
		do_report(0,"STSMART_Reset(%d) = %s\n", i,GetErrorText(st_err));
		
		if(st_err == ST_NO_ERROR)
		{
			return false;
		}
		else if(st_err == STSMART_ERROR_NOT_INSERTED)
		{
			i = SC_REPEATE_TIMES;
		}
		else
			task_delay(300);
	}

	return true;
}
/************************����˵��***********************************/
/*������:static  void CH_SC_Monitor(void *pvParam)                */
/*�����˺Ϳ���ʱ��:zengxianggen 2007-04-13                         */
/*������������:                                                    */
/*����ԭ��˵��:        ���ܿ��������                              */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                       */
/*���õ���Ҫ�����б�                                             */
/*����ֵ˵������                                                   */
/*����ע������:                                                    */
/*����˵��:                                                        */                                             
                                                 
static void CH_SC_Monitor(void *pvParam)
{ 
	ST_ErrorCode_t st_ErrCode = ST_NO_ERROR; 	
	while(TRUE)
	{
		semaphore_wait(gpST_SemSCCheck);
		if(gc_Cardstatus == 1)
		{
			/*���и�λ����*/
            if(CH_SC_Reset() == false)
			{
               do_report(0,"��λ�ɹ�");
			}
			else
			{
              do_report(0,"��λʧ��");
			}
		}
		else if(gc_Cardstatus == 0)
		{
			 /*ֻ�ܰε�*/
		}
	}
}





