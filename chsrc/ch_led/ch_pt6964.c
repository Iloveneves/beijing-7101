/*******************�ļ�˵��ע��************************************/
/*��˾��Ȩ˵������Ȩ��2006���鳤������Ƽ����޹�˾���С�           */
/*�ļ�����ch_pt6964.c                                              */
/*�汾�ţ�V1.0                                                     */
/*���ߣ�  �����                                                   */
/*�������ڣ�2006-02-08                                             */
/*ģ�鹦��:ͨ��PT6964 ���LED��ǰ��尴������                      */
/*��Ҫ����������:                                                  */
/*                                                                 */
/*�޸���ʷ���޸�ʱ��    ����      �޸Ĳ���       ԭ��              */
/*  
				  ***1***
                  6      2
                  *      *
                  ***7****
                  5      3
                  *      *
                  ***4****  .8      

  Modifiction: 
  Date:2006-05-03 by ������ 
  Ŀ�ģ���ATMEL���Ƶ�LED ���ݣ����̶ȱ���Ӧ�ó��򲻱䡣
  1���޸ĺ���BOOL DisplayTimeLED(void)�����Ӳ���TDTTIME DisTime��
  2���޸ĺ���boolean CH_PUB_PTinit(void)���������ĳ�ʼ������������
  3������ȫ�ֱ���IsTunerLigthOn�����޸ĺ���BOOL StandbyLedOn()��BOOL StandbyLedOff(),���Լ�¼��ǰ
		TUNER �Ƶ�״̬��

  4�����������к���BOOL LEDDisplay(U8* Content)��BOOL FrontKeybordInit(void)��
	 BOOL DisplayFreqLED(int Freq)��BOOL GetTunerLightStatus(void)


                                                                    */
/*******************************************************************/

/**************** #include ����*************************************/
#include "stddefs.h"  /* standard definitions           */
#include "stevt.h"
#include "stlite.h"   /* os20 for standard definitions, */

#include "initterm.h"
#include "stddefs.h"
#include "stcommon.h"
#include "appltype.h"

#include "keymap.h"
#include "ch_time.h"
#include "ch_pt6964.h"

void CH_PUB_ClearPTSTB(void);
void CH_PUB_Senddata(U8 ruc_Data); 
void CH_PUB_PTSetData(U8 ruc_command,U8 *rp_Data,int ri_Len);
void CH_PUB_Scan(void);

/*******************************************************************/
/**************** #define ����**************************************/
#define PT_REFRESH_MAX         10
#define PT_MODE                0x02
#define PT_DUTY_CYCLE            /*0x8f��������*/0x8A
#define PT_ADDRESS_INC         0x40
#define PT_ADDRESS_FIX         0x44
#define PT_ADDRESS_MASK        0xc0
#define PT_READ_KEY            0x42
#define PT_SW_KEY              0x43
#define PT_READ_SW             0x43

#define TASK_DELAY              ST_GetClocksPerSecond()/1000000

#define PT_STB_BIT            0x02
#define PT_CLK_BIT            0x04
#define PT_DAINOUT_BIT        0x08
#define PT_YLED_BIT           0x10

#define MAX_PT_CHARINDEX      9      /*���������ַ�����*/
#define FIRST_ADDR            0x06   /*ǧλ��ַ*/
#define TWO_ADDR              0x04   /*��λ��ַ*/
#define THREE_ADDR            0x02   /*ʮλ��ַ*/
#define FOUR_ADDR             0x00   /*��λ��ַ*/
#define GREENLED_ADDR         0x05   /*GREEN LED ��ַ*/
#define REDLED_ADDR           0x09   /*RED LED ��ַ*/
#define LED_ADDR              0x05
#define PT_KEYSCAN_LEN        0x02   /*����ɨ����*/
/*******************************************************************/

/***************  ȫ�ֱ������岿�� *********************************/
STPIO_Handle_t gST_PTHandle;
STPIO_Handle_t gST_PTHandleDataInOut;


U8             *gp_PTtask_stack;
semaphore_t*    pgST_PTSema;
/*��Ӧ��0~9�ַ�����*/
U8             guc_LedData[]=
{
 0x3F,/*0x30*/0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F
};
static      BYTE suc_LastKey=0xFF;            /*�ϴεļ�ֵ*/
static      int  si_SamekeyCount=0;        /*��ͬ�ļ�ֵ����*/


#if 0 /*yxl 2007-03-07 move this variable to file ch_LEDmid.c*/  
static boolean IsTunerLigthOn=false;/*yxl 2006-05-03 add this variable,
	false stand for tuner light has been turned off,
	false stand for tuner light has been turned on*/
#endif

static message_queue_t  *pstUsifMsgQueue;/*yxl 2006-05-05 add this line*/

/*******************************************************************/
/************************����˵��***********************************/
/*������:BYTE CH_PUB_PTNumToSeg (int ri_index)              */
/*�����˺Ϳ���ʱ��:zengxianggen 2006-02-08                         */
/*������������:������ת��Ϊ��Ӧ��SegMent����                       */
/*����ԭ��˵��:                                                    */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                       */
/*���õ���Ҫ�����б�                                             */
/*����ֵ˵������                  */

BYTE CH_PUB_PTNumToSeg (int ri_index)
/*int ri_index����Ҫת��������*/
{
	if(ri_index > MAX_PT_CHARINDEX)  
		ri_index = MAX_PT_CHARINDEX;
	return guc_LedData[ri_index];
}


/************************����˵��***********************************/
/*������:staic void CH_PUB_PTProcess(void *rp_vParam)              */
/*�����˺Ϳ���ʱ��:zengxianggen 2006-02-08                         */
/*������������:PT6964�������                                       */
/*����ԭ��˵��:                                                    */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                       */
/*���õ���Ҫ�����б�                                             */
/*����ֵ˵������                  */
static void CH_PUB_PTProcess(void *rp_vParam)  
{
#if 0 /*yxl 2006-05-03 cancel below section*/
    int i;
	int j;
	U8  uc_Data1[1]={0x00};
	U8  uc_Data2[1]={0xFF};
	semaphore_wait(pgST_PTSema);
	/*��������Ϊ��ʾģʽ�����ҵ�ַ�̶�*/
	CH_PUB_PTSetData(PT_ADDRESS_FIX,NULL,0);
	/*ѭ�����������ʾ����*/
    for(i=0;i<PT_REFRESH_MAX;i++)
	{
      CH_PUB_PTSetData(PT_ADDRESS_MASK|i,uc_Data1,1);/*���ö�Ӧ�ĵ�ַ,������ո��ڴ�����*/
	}
    /*������ʾģʽ*/
   	CH_PUB_PTSetData(PT_MODE,NULL,0);
	/*������ʾ��������,0x88~0x8F,*/
    CH_PUB_PTSetData(PT_DUTY_CYCLE,NULL,0)/*PULS WIDTH=1/16,DISPLAY ON*/;
	semaphore_signal(pgST_PTSema);
    
#endif   /*end yxl 2006-05-03 cancel below section*/

	while(true)
	{
		/*ѭ��ɨ��KEY*/
         task_delay(ST_GetClocksPerSecond()/20);/*50ms*/
		 /*��ʼ��*/
         semaphore_wait(pgST_PTSema);
         /*����Ϊ��ģʽ*/
		 CH_PUB_ClearPTSTB();
         CH_PUB_Senddata(PT_READ_KEY);
         task_delay(1000);
         CH_PUB_Scan();
		 semaphore_signal(pgST_PTSema);

	}
}

/************************����˵��***********************************/
/*������:boolean CH_PUB_PTinit(void)                               */
/*�����˺Ϳ���ʱ��:zengxianggen 2006-02-08                         */
/*������������:PT6964��ʼ��                                        */
/*����ԭ��˵��:                                                    */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                       */
/*���õ���Ҫ�����б�                                             */
/*����ֵ˵����false����ʼ���ɹ���true����ʼ��ʧ��                  */
boolean CH_PUB_PTinit(void) 
{
    ST_ErrorCode_t ErrCode;
    boolean b_result=false;
  	STPIO_OpenParams_t    ST_OpenParams;

	ST_OpenParams.ReservedBits      = PIO_BIT_1|PIO_BIT_2|PIO_BIT_4;
	ST_OpenParams.BitConfigure[1]  = STPIO_BIT_OUTPUT;           /*STB*/
	ST_OpenParams.BitConfigure[2]  = STPIO_BIT_OUTPUT;           /*CLK*/
	ST_OpenParams.BitConfigure[4]  = STPIO_BIT_OUTPUT;           /*YELLOW LED*/
	ST_OpenParams.IntHandler       = NULL;         

	ErrCode=STPIO_Open(PIO_DeviceName[5], &ST_OpenParams, &gST_PTHandle );
	if(ErrCode!=ST_NO_ERROR)
	{
		STTBX_Print(("\n STPIO_Open()=s",GetErrorText(ErrCode)));
		return TRUE;
	}


	ST_OpenParams.ReservedBits      =PIO_BIT_3;
	ST_OpenParams.BitConfigure[3]  =/* STPIO_BIT_OUTPUT|STPIO_BIT_INPUT*/STPIO_BIT_BIDIRECTIONAL;          /*DIN_OUT*/
	ST_OpenParams.IntHandler       = NULL;     
	ErrCode=STPIO_Open(PIO_DeviceName[5], &ST_OpenParams, &gST_PTHandleDataInOut );
	if(ErrCode!=ST_NO_ERROR)
	{
		STTBX_Print(("\n STPIO_Open()=s",GetErrorText(ErrCode)));
		return TRUE;
	}

	#if 1 /*yxl 2006-05-03 add below section*/
	{
		int i;
		int j;
		U8  uc_Data1[1]={0x00};
		U8  uc_Data2[1]={0xFF};
		
		/*��������Ϊ��ʾģʽ�����ҵ�ַ�̶�*/
		CH_PUB_PTSetData(PT_ADDRESS_FIX,NULL,0);
		/*ѭ�����������ʾ����*/
		for(i=0;i<PT_REFRESH_MAX;i++)
		{
			CH_PUB_PTSetData(PT_ADDRESS_MASK|i,uc_Data1,1);/*���ö�Ӧ�ĵ�ַ,������ո��ڴ�����*/
		}
		/*������ʾģʽ*/
		CH_PUB_PTSetData(PT_MODE,NULL,0);
		/*������ʾ��������,0x88~0x8F,*/
		CH_PUB_PTSetData(PT_DUTY_CYCLE,NULL,0)/*PULS WIDTH=1/16,DISPLAY ON*/;

	}

#endif   /*end yxl 2006-05-03 add below section*/


     pgST_PTSema=semaphore_create_fifo(1);




	/*******************/
	return b_result;
}



/************************����˵��***********************************/
/*������:void CH_PUB_ClearPTSTB(void)                              */
/*�����˺Ϳ���ʱ��:zengxianggen 2006-02-08                         */
/*������������:PT6964 STB�õ�λ                                    */
/*����ԭ��˵��:                                                    */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                       */
/*���õ���Ҫ�����б�                                             */
/*����ֵ˵������                                                   */
void CH_PUB_ClearPTSTB(void) 
{
    ST_ErrorCode_t ErrCode;
	ErrCode=STPIO_Clear(gST_PTHandle,PT_STB_BIT);
   	if(ErrCode!=ST_NO_ERROR)
	{
		STTBX_Print(("\n STPIO_Clear()=s",GetErrorText(ErrCode)));
	}
}

/************************����˵��***********************************/
/*������:void CH_PUB_SetPTSTB(void)                                */
/*�����˺Ϳ���ʱ��:zengxianggen 2006-02-08                         */
/*������������:PT6964 STB�ø�λ                                    */
/*����ԭ��˵��:                                                    */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                       */
/*���õ���Ҫ�����б�                                             */
/*����ֵ˵������                                                   */
void CH_PUB_SetPTSTB(void) 
{
  ST_ErrorCode_t ErrCode;
	ErrCode=STPIO_Set(gST_PTHandle,PT_STB_BIT);
  	if(ErrCode!=ST_NO_ERROR)
	{
		STTBX_Print(("\n STPIO_Clear()=s",GetErrorText(ErrCode)));
	}
   task_delay(TASK_DELAY);
}

/************************����˵��***********************************/
/*������:void CH_PUB_ClearPTCLK(void)                              */
/*�����˺Ϳ���ʱ��:zengxianggen 2006-02-08                         */
/*������������:PT6964 CLK�õ�λ                                    */
/*����ԭ��˵��:                                                    */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                       */
/*���õ���Ҫ�����б�                                             */
/*����ֵ˵������                                                   */
void CH_PUB_ClearPTCLK(void) 
{
     ST_ErrorCode_t ErrCode;
	ErrCode=STPIO_Clear(gST_PTHandle,PT_CLK_BIT);
   if(ErrCode!=ST_NO_ERROR)
   {
	   STTBX_Print(("\n STPIO_Clear()=s",GetErrorText(ErrCode)));
   }
   task_delay(TASK_DELAY);
}

/************************����˵��***********************************/
/*������:void CH_PUB_SetPTCLK(void)                                */
/*�����˺Ϳ���ʱ��:zengxianggen 2006-02-08                         */
/*������������:PT6964 CLK�ø�λ                                    */
/*����ԭ��˵��:                                                    */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                       */
/*���õ���Ҫ�����б�                                             */
/*����ֵ˵������                                                   */
void CH_PUB_SetPTCLK(void) 
{
     ST_ErrorCode_t ErrCode;
	ErrCode=STPIO_Set(gST_PTHandle,PT_CLK_BIT);
   if(ErrCode!=ST_NO_ERROR)
   {
	   STTBX_Print(("\n STPIO_Clear()=s",GetErrorText(ErrCode)));
   }
   task_delay(TASK_DELAY);
}
/************************����˵��***********************************/
/*������:void CH_PUB_ClearPTDAINOUT(void)                             */
/*�����˺Ϳ���ʱ��:zengxianggen 2006-02-08                         */
/*������������:PT6964 DAIN�õ�λ                                   */
/*����ԭ��˵��:                                                    */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                       */
/*���õ���Ҫ�����б�                                             */
/*����ֵ˵������                                                   */
void CH_PUB_ClearPTDAINOUT(void) 
{
    ST_ErrorCode_t ErrCode;
	ErrCode=STPIO_Clear(gST_PTHandleDataInOut,PT_DAINOUT_BIT);
   if(ErrCode!=ST_NO_ERROR)
   {
	   STTBX_Print(("\n STPIO_Clear()=s",GetErrorText(ErrCode)));
   }
   task_delay(TASK_DELAY);
}

/************************����˵��***********************************/
/*������:void CH_PUB_SetPTDAINOUT(void)                               */
/*�����˺Ϳ���ʱ��:zengxianggen 2006-02-08                         */
/*������������:PT6964 DAINOUT�ø�λ                                   */
/*����ԭ��˵��:                                                    */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                       */
/*���õ���Ҫ�����б�                                             */
/*����ֵ˵������                                                   */
void CH_PUB_SetPTDAINOUT(void) 
{
    ST_ErrorCode_t ErrCode;
	ErrCode=STPIO_Set(gST_PTHandleDataInOut,PT_DAINOUT_BIT);
   if(ErrCode!=ST_NO_ERROR)
   {
	   STTBX_Print(("\n STPIO_Clear()=s",GetErrorText(ErrCode)));
   }
   task_delay(TASK_DELAY);
}

/************************����˵��***********************************/
/*������:void CH_PUB_ClearYLED(void)                               */
/*�����˺Ϳ���ʱ��:zengxianggen 2006-02-08                         */
/*������������: YELLOW LED �õ�λ                                  */
/*����ԭ��˵��:                                                    */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                       */
/*���õ���Ҫ�����б�                                             */
/*����ֵ˵������                                                   */
void CH_PUB_ClearYLED(void) 
{
    ST_ErrorCode_t ErrCode;
	ErrCode=STPIO_Clear(gST_PTHandle,PT_YLED_BIT);
   if(ErrCode!=ST_NO_ERROR)
   {
	   STTBX_Print(("\n STPIO_Clear()=s",GetErrorText(ErrCode)));
   }
}

/************************����˵��***********************************/
/*������:void CH_PUB_SetYLED(void)                                 */
/*�����˺Ϳ���ʱ��:zengxianggen 2006-02-08                         */
/*������������:YELLOW LED�ø�λ                                    */
/*����ԭ��˵��:                                                    */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                       */
/*���õ���Ҫ�����б�                                             */
/*����ֵ˵������                                                   */
void CH_PUB_SetYLED(void) 
{
    ST_ErrorCode_t ErrCode;
	ErrCode=STPIO_Set(gST_PTHandle,PT_YLED_BIT);
   if(ErrCode!=ST_NO_ERROR)
   {
	   STTBX_Print(("\n STPIO_Clear()=s",GetErrorText(ErrCode)));
   }
}

/************************����˵��***********************************/
/*������:void CH_PUB_Senddata(U8 ruc_Data)                         */
/*�����˺Ϳ���ʱ��:zengxianggen 2006-02-09                         */
/*������������:����һ�����ݻ�����                                  */
/*����ԭ��˵��:                                                    */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                       */
/*���õ���Ҫ�����б�                                             */
/*����ֵ˵������                                                   */
void CH_PUB_Senddata(U8 ruc_Data) 
/*U8 ruc_Data,Ҫ���͵����ݻ�����*/
{
	int i; 
	BYTE uc_bTmp = ruc_Data; 
    for (i = 0; i < 8; i++)
	{
		CH_PUB_ClearPTCLK();
		if(uc_bTmp & 0x1)
		{
           CH_PUB_SetPTDAINOUT() ;
		}
		else
		{
           CH_PUB_ClearPTDAINOUT() ;
		}
		uc_bTmp >>= 1;
		CH_PUB_SetPTCLK(); 
	} 
    CH_PUB_SetPTDAINOUT() ; 
}

/************************����˵��***********************************/
/*������:void CH_PUB_STBSenddata(U8 ruc_Data)                         */
/*�����˺Ϳ���ʱ��:zengxianggen 2006-02-09                         */
/*������������:����һ��STB����                                  */
/*����ԭ��˵��:                                                    */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                       */
/*���õ���Ҫ�����б�                                             */
/*����ֵ˵������                                                   */
void CH_PUB_STBSenddata(U8 ruc_Data) 
/*U8 ruc_Data,Ҫ���͵�STB����*/
{
   CH_PUB_ClearPTSTB();
   CH_PUB_Senddata(ruc_Data); 
   CH_PUB_SetPTSTB();
}




/************************����˵��***********************************/
/*������:void CH_PUB_PTSetData(U8 ruc_command,U8 *rp_Data,int ri_Len)*/
/*�����˺Ϳ���ʱ��:zengxianggen 2006-02-08                         */
/*������������:������Ӧ���������                                  */
/*����ԭ��˵��:                                                    */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                       */
/*���õ���Ҫ�����б�                                             */
/*����ֵ˵������                                                   */
void CH_PUB_PTSetData(U8 ruc_command,U8 *rp_Data,int ri_Len)
/*************************����ע��˵��*******************************/
/*U8 ruc_command:Ҫ���͵�����
  U8 *rp_Data:Ҫ���͵�����ָ��
  int ri_Len: Ҫ���͵����ݳ���*/
{
	int i;
	int i_tempdata;
	U8  uc_Tmp;

	/*���ȴ�������,STB*/
	CH_PUB_ClearPTSTB();
    CH_PUB_Senddata(ruc_command);
	/*����ri_Len������*/
	while(rp_Data!=NULL&&ri_Len>0)
	{        
		uc_Tmp=*rp_Data;
		CH_PUB_Senddata(uc_Tmp);
		ri_Len--;
		rp_Data++;
	}
    CH_PUB_SetPTSTB();
	

}

/************************����˵��***********************************/
/*������:BYTE CH_PT_Recv(void) */
/*�����˺Ϳ���ʱ��:zengxianggen 2006-02-08                         */
/*������������:������Ӧ���������                                  */
/*����ԭ��˵��:                                                    */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                       */
/*���õ���Ҫ�����б�                                             */
/*����ֵ˵�������յ���ɨ��ֵ                                       */
BYTE CH_PT_Recv(void) 
{
	BYTE i, uc_KeyData,uc_tempdata;
	
	uc_KeyData = 0;
	
	for(i = 0; i < 8; i++)
	{
		CH_PUB_ClearPTCLK();
		uc_KeyData <<= 1;
		CH_PUB_SetPTCLK();		
		/* delay for data output ready */
		/*task_delay(ST_GetClocksPerSecond)/5000;*/
		STPIO_Read(gST_PTHandleDataInOut, &uc_tempdata);
		if (uc_tempdata&PT_DAINOUT_BIT)
		{
			uc_KeyData |= 1;
		}
	}
	
	return (uc_KeyData);
}
/************************����˵��***********************************/
/*������:void CH_PUB_Scan(void)*/
/*�����˺Ϳ���ʱ��:zengxianggen 2006-02-08                         */
/*������������:ɨ���                                  */
/*����ԭ��˵��:                                                    */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                       */
/*���õ���Ҫ�����б�                                             */
/*����ֵ˵�������յ���ɨ��ֵ                                       */

void CH_PUB_Scan(void)
{
	BYTE i, j,  uc_RecvByte,uc_key;
	usif_cmd_t* str_pMsg;
	clock_t     st_timeout;
	
	uc_key = 0xFF;
	
	for (j=PT_KEYSCAN_LEN;j>0;j--)
	{
		uc_RecvByte = CH_PT_Recv();
		if (uc_RecvByte != 0)
		{
			for(i = 0; i < 8; i++)
			{
				if (uc_RecvByte & 0x1)
				{
					uc_key = i + ((PT_KEYSCAN_LEN - j) * 8);
					break;
				}
				uc_RecvByte >>= 1;
			}
		}
	}
	
	CH_PUB_ClearPTCLK(); /* ? */
	CH_PUB_SetPTSTB();
	
	if (uc_key == 0xFF)
	{
		si_SamekeyCount=0;
		suc_LastKey=0xFF;
	/*	do_report(0,"no Key pressed\n");*/
	}
	else/*�м�ֵ*/
	{
		if(suc_LastKey!=uc_key)
		{
			st_timeout = ST_GetClocksPerSecond()*2 ;
			si_SamekeyCount=0;
			if ((str_pMsg = (usif_cmd_t *)message_claim_timeout(pstUsifMsgQueue,/*TIMEOUT_INFINITY*/&st_timeout)) != NULL)
			{
				str_pMsg->from_which_module = KEYBOARD_MODULE;
				str_pMsg->contents.keyboard.scancode = uc_key;
				str_pMsg->contents.keyboard.device = FRONT_PANEL_KEYBOARD;
				str_pMsg->contents.keyboard.repeat_status =FALSE;
				message_send(pstUsifMsgQueue, str_pMsg);
				suc_LastKey=uc_key;
/*				do_report(0,"have Key=%d\n",uc_key);	  */
			}
		}
		else
		{
			if(si_SamekeyCount>3)
			{
				st_timeout = ST_GetClocksPerSecond()*2 ;
				si_SamekeyCount=0;
				if ((str_pMsg = (usif_cmd_t *)message_claim_timeout(pstUsifMsgQueue,/*TIMEOUT_INFINITY*/&st_timeout)) != NULL)
				{
					str_pMsg->from_which_module = KEYBOARD_MODULE;
					str_pMsg->contents.keyboard.scancode = uc_key;
					str_pMsg->contents.keyboard.device = FRONT_PANEL_KEYBOARD;
					str_pMsg->contents.keyboard.repeat_status =FALSE;
					message_send(pstUsifMsgQueue, str_pMsg);
					suc_LastKey=uc_key;
/*					do_report(0,"have Key=%d\n",uc_key);*/	  
				}
			}
			si_SamekeyCount++;
		}
	}
}

/*���к�������д��ʽ��Ϊ�˼����ϵĳ���*/

void DisplayLEDMode(U32 num,int mode)
{
	U8 TempMode=mode;
	U8 uc_WriteData[4];
	int i;
	semaphore_wait(pgST_PTSema);
	switch( TempMode)
	{
	case 0: /*display ProgNo*/
		{
			uc_WriteData[0]	 = CH_PUB_PTNumToSeg ( num / 1000 );          /*ǧλ*/

			uc_WriteData[1]	 = CH_PUB_PTNumToSeg ( (num % 1000) / 100 );  /*��λ*/
			uc_WriteData[2]	 = CH_PUB_PTNumToSeg ( (num % 100) / 10 );    /*ʮλ*/

			uc_WriteData[3]    = CH_PUB_PTNumToSeg ( num % 10 );          /*��λ*/
			CH_PUB_PTSetData(PT_MODE,NULL,0);
			CH_PUB_PTSetData(PT_ADDRESS_MASK|FIRST_ADDR,&uc_WriteData[0],1);
			CH_PUB_PTSetData(PT_ADDRESS_MASK|TWO_ADDR,  &uc_WriteData[1],1);
			CH_PUB_PTSetData(PT_ADDRESS_MASK|THREE_ADDR,&uc_WriteData[2],1);
			CH_PUB_PTSetData(PT_ADDRESS_MASK|FOUR_ADDR, &uc_WriteData[3],1);
		}
		break;
	case 1: /*display SN */
		{
			uc_WriteData[0]	 = CH_PUB_PTNumToSeg ( num / 1000 );        /*ǧλ*/
			uc_WriteData[1]	 = CH_PUB_PTNumToSeg ( (num % 1000) / 100 );/*��λ*/
			uc_WriteData[2]	 = CH_PUB_PTNumToSeg ( (num % 100) / 10 );  /*ʮλ*/
			uc_WriteData[3]  = CH_PUB_PTNumToSeg ( num % 10 );          /*��λ*/
			CH_PUB_PTSetData(PT_MODE,NULL,0);
			CH_PUB_PTSetData(PT_ADDRESS_MASK|FIRST_ADDR,&uc_WriteData[0],1);
			CH_PUB_PTSetData(PT_ADDRESS_MASK|TWO_ADDR,  &uc_WriteData[1],1);
			CH_PUB_PTSetData(PT_ADDRESS_MASK|THREE_ADDR,&uc_WriteData[2],1);
			CH_PUB_PTSetData(PT_ADDRESS_MASK|FOUR_ADDR, &uc_WriteData[3],1);
		}
		break;
	case 2: /*display time */
		{
			uc_WriteData[0]	 = CH_PUB_PTNumToSeg ( num / 1000 );          /*ǧλ*/

			uc_WriteData[1]	 = CH_PUB_PTNumToSeg ( (num % 1000) / 100 )|0x80;  /*��λ*/
			uc_WriteData[2]	 = CH_PUB_PTNumToSeg ( (num % 100) / 10 )|0x80;    /*ʮλ*/

			uc_WriteData[3]    = CH_PUB_PTNumToSeg ( num % 10 );          /*��λ*/
			CH_PUB_PTSetData(PT_MODE,NULL,0);
			CH_PUB_PTSetData(PT_ADDRESS_MASK|FIRST_ADDR,&uc_WriteData[0],1);
			CH_PUB_PTSetData(PT_ADDRESS_MASK|TWO_ADDR,  &uc_WriteData[1],1);
			CH_PUB_PTSetData(PT_ADDRESS_MASK|THREE_ADDR,&uc_WriteData[2],1);
			CH_PUB_PTSetData(PT_ADDRESS_MASK|FOUR_ADDR, &uc_WriteData[3],1);
			
		}
		break;
	case 3:	/* display search freq */
		{
			uc_WriteData[0]	 = 0x71;/*F�ַ�*/                            /*ǧλ*/
			uc_WriteData[1]	 = CH_PUB_PTNumToSeg ( (num % 1000) / 100 ); /*��λ*/
			uc_WriteData[2]	 = CH_PUB_PTNumToSeg ( (num % 100) / 10 );   /*ʮλ*/
			uc_WriteData[3]    = CH_PUB_PTNumToSeg ( num % 10 );         /*��λ*/
			CH_PUB_PTSetData(PT_MODE,NULL,0);
			CH_PUB_PTSetData(PT_ADDRESS_MASK|FIRST_ADDR,&uc_WriteData[0],1);
			CH_PUB_PTSetData(PT_ADDRESS_MASK|TWO_ADDR,  &uc_WriteData[1],1);
			CH_PUB_PTSetData(PT_ADDRESS_MASK|THREE_ADDR,&uc_WriteData[2],1);
			CH_PUB_PTSetData(PT_ADDRESS_MASK|FOUR_ADDR, &uc_WriteData[3],1);
		}
		break;
	}
	semaphore_signal(pgST_PTSema);
}

#if 0 /*yxl 2007-03-07 move this function to file ch_LEDmid.c*/  
/*��ʾ��Ŀ��*/
void DisplayLED(U32 num)
{
#if 0 /*yxl 2007-02-21 temp cancel for debug*/
	DisplayLEDMode(num,0);
#else
	return;
#endif
}
#endif /*end yxl 2007-03-07 move this function to file ch_LEDmid.c*/  


#if 0 /*yxl 2006-05-03 modify below function*/
/*��ʾʱ��*/
BOOL DisplayTimeLED(void)
{
	TDTTIME TempTime;
	U32  hm = 0;
	GetCurrentTime(&TempTime);		
	hm = (TempTime.ucHours* 100) + TempTime.ucMinutes;
	if(hm ==0xffffffff)
		return TRUE;
	
	DisplayLEDMode(hm,2);	
	return FALSE;	
}

#else
#if 0 /*yxl 2007-03-07 move this function to file ch_LEDmid.c*/  
boolean DisplayTimeLED(TDTTIME TempTime)
{
#if 0 /*yxl 2007-02-21 temp cancel for debug*/
	U32  hm = 0;

	hm = (TempTime.ucHours* 100) + TempTime.ucMinutes;
	if(hm ==0xffffffff)
		return TRUE;
	
	DisplayLEDMode(hm,2);	
#else
#endif
	return FALSE;	
}
#endif  /*yxl 2007-03-07 move this function to file ch_LEDmid.c*/  
#endif /*end yxl 2006-05-03 modify below function,Add para 	TDTTIME DisTime*/


#if 0 /*yxl 2007-03-07 move this function to file ch_LEDmid.c*/  
/*������*/
BOOL StandbyLedOn()
{  
	U8 uc_WriteData[1]={0x01};
	semaphore_wait(pgST_PTSema);
    CH_PUB_PTSetData(PT_MODE,NULL,0);
	CH_PUB_PTSetData(PT_ADDRESS_MASK|LED_ADDR,&uc_WriteData[0],1);/*�̵���*/
	semaphore_signal(pgST_PTSema);
	return true;
	
}
/*������*/
BOOL StandbyLedOff()
{
	U8 uc_WriteData[1]={0x02};
	semaphore_wait(pgST_PTSema);
    CH_PUB_PTSetData(PT_MODE,NULL,0);
	CH_PUB_PTSetData(PT_ADDRESS_MASK|LED_ADDR,&uc_WriteData[0],1);/*�̵���*/
	semaphore_signal(pgST_PTSema);
	
	return true;/*for test*/		
}

/*��Դָʾ��*/
BOOL PowerLightOff(void)
{
	U8 uc_WriteData[1]={0x01};
	semaphore_wait(pgST_PTSema);
    CH_PUB_PTSetData(PT_MODE,NULL,0);
	CH_PUB_PTSetData(PT_ADDRESS_MASK|LED_ADDR,&uc_WriteData[0],1);/*�����*/
	semaphore_signal(pgST_PTSema);
	return TRUE;
}
/*��Դָʾ��*/
BOOL PowerLightOn()
{
	U8 uc_WriteData[1]={0x02};
	semaphore_wait(pgST_PTSema);
    CH_PUB_PTSetData(PT_MODE,NULL,0);
	CH_PUB_PTSetData(PT_ADDRESS_MASK|LED_ADDR,&uc_WriteData[0],1);/*�����*/
	semaphore_signal(pgST_PTSema);
	return TRUE;
}
/*TUNERָʾ��*/
BOOL TunerLightOff()
{

	CH_PUB_ClearYLED();
	IsTunerLigthOn=FALSE;/*yxl 2006-05-03 add this line*/
	return TRUE;
}
/*TUNERָʾ��*/
BOOL TunerLightOn()
{
	CH_PUB_SetYLED();
	IsTunerLigthOn=TRUE;/*yxl 2006-05-03 add this line*/
	return TRUE;
}

BOOL DisplayLEDString(char* pDisString)
{	
	return FALSE;
}


/*yxl 2006-05-03 add below function*/

BOOL LEDDisplay(U8* Content)
{
	U8 uc_WriteData[4];
	int i;

	memcpy((void*)uc_WriteData,(const void*)Content,4);
	semaphore_wait(pgST_PTSema);
	CH_PUB_PTSetData(PT_MODE,NULL,0);
	CH_PUB_PTSetData(PT_ADDRESS_MASK|FIRST_ADDR,&uc_WriteData[0],1);
	CH_PUB_PTSetData(PT_ADDRESS_MASK|TWO_ADDR,  &uc_WriteData[1],1);
	CH_PUB_PTSetData(PT_ADDRESS_MASK|THREE_ADDR,&uc_WriteData[2],1);
	CH_PUB_PTSetData(PT_ADDRESS_MASK|FOUR_ADDR, &uc_WriteData[3],1);
	semaphore_signal(pgST_PTSema);

	return FALSE;
}

#endif  /*yxl 2007-03-07 move this function to file ch_LEDmid.c*/  

/*end yxl 2006-05-03 add below function*/

/*yxl 2006-05-01 add below fuctions*/
BOOL FrontKeybordInit(void)
{

	int i_result;
	int i_stacksize=1024;
#if 0
	/*yxl 2006-05-05 add below section*/
	if ( symbol_enquire_value ( "usif_queue", ( void ** ) &pstUsifMsgQueue ) )
	{
   
		do_report ( 0 , "Cant find USIF message queue\n" );

		return   TRUE;
	}
	/*end yxl 2006-05-05 add below section*/
#endif 
	gp_PTtask_stack=memory_allocate(CHSysPartition,i_stacksize);
	if(gp_PTtask_stack==NULL)
	{
		return true;
	}
/*	i_result = task_init( CH_PUB_PTProcess,NULL,gp_PTtask_stack, i_stacksize,&gST_PTtask, &gST_desc_pt,5,"pt_process", 0);	
	
	if(i_result!=0)
	{		
		STTBX_Print(("Failed to create pt process task\n"));
		return true ;
	}
	else
		do_report( 0,"pt process ok\n" );
*/
	return false;
}
/*end yxl 2006-05-01 add below fuctions*/


#if 0 /*yxl 2007-03-07 move this function to file ch_LEDmid.c*/  

/*yxl 2006-05-01 add below fuctions*/
BOOL DisplayFreqLED(int Freq)
{
#if 0 /*yxl2007-02-21 temp cancel for debug*/
	DisplayLEDMode(Freq,3);
#endif 
	return FALSE;
}
/*end yxl 2006-05-01 add below fuctions*/

/*yxl 2006-05-01 add below fuctions*/
BOOL GetTunerLightStatus(void)
{

	return IsTunerLigthOn;
}
/*end yxl 2006-05-01 add below fuctions*/

#endif  /*yxl 2007-03-07 move this function to file ch_LEDmid.c*/  

/*********************************************/



