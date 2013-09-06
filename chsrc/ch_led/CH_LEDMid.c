/*
(c) Copyright changhong
  Name:CH_LEDmid.c
  Description: ʵ�ֳ���LED��ʾ��ָʾ�ƿ��Ƶ��м�ӿں�����������Ӧ�ú�Ӳ��LED��ָʾ�ƾ���ʵ�ַ�ʽ�佨������  
  Authors:yixiaoli
  Date          Remark
  2007-03-07    Created
  2007-05-08    change  zxg
Modifiction:


*/

#include "..\main\initterm.h"
#include "..\util\ch_time.h"
#include "..\dbase\vdbase.h"
#include "ch_vfd.h"

void CH_LED_DisplayStandby(void);

static boolean IsTunerLigthOn=false;/*	false stand for tuner light has been turned off,
	false stand for tuner light has been turned on*/
/*20070822 add*/
Semaphore_t * gp_SemVFD;


BOOL CH_LEDInit(void)
{
	BOOL res;
#ifndef PT6964
	res=CH_VFD_Init(I2C_DeviceName[2]);
	if(res==TRUE)
	{
		STTBX_Print(("\nCH_LEDInit() isn't successful"));
	}
	   gp_SemVFD = semaphore_create_fifo( 1 );
#endif
    
	return res;

}

/*��ʾ��Ŀ��*/
void DisplayLED(U32 num)
{
#ifdef PT6964 /*yxl 2007-02-21 temp cancel for debug*/
	DisplayLEDMode(num,0);
#else
        semaphore_wait(gp_SemVFD);
	CH_VFD_DisplayProgNumber(num);
	 semaphore_signal(gp_SemVFD);
	return;
#endif
}


BOOL DisplayTimeLED(TDTTIME TempTime)
{
#ifdef PT6964  /*yxl 2007-02-21 temp cancel for debug*/
	U32  hm = 0;

	hm = (TempTime.ucHours* 100) + TempTime.ucMinutes;
	if(hm ==0xffffffff)
		return TRUE;
	
	DisplayLEDMode(hm,2);	
#else
	U32  hm = 0;

	hm = (TempTime.ucHours* 100) + TempTime.ucMinutes;
	if(hm ==0xffffffff)
		return TRUE;
	semaphore_wait(gp_SemVFD);
	CH_VFD_DisplayTime(hm);
	semaphore_signal(gp_SemVFD);
#endif
	return FALSE;	
}


BOOL DisplayFreqLED(int Freq)
{
#ifdef PT6964 /*yxl2007-02-21 temp cancel for debug*/
	DisplayLEDMode(Freq,3);
#else
	semaphore_wait(gp_SemVFD);
	CH_VFD_ShowChar(0,0,"F");
	CH_VFD_WriteNumber(1,3,Freq);\
       semaphore_signal(gp_SemVFD);
#endif 
	return FALSE;
}


BOOL LEDDisplay(U8* Content)
{
	U8 uc_WriteData[4];
	int i;
#ifdef PT6964 /*yxl 2007-03-07 temp cancel*/
	memcpy((void*)uc_WriteData,(const void*)Content,4);
	semaphore_wait(pgST_PTSema);
	CH_PUB_PTSetData(PT_MODE,NULL,0);
	CH_PUB_PTSetData(PT_ADDRESS_MASK|FIRST_ADDR,&uc_WriteData[0],1);
	CH_PUB_PTSetData(PT_ADDRESS_MASK|TWO_ADDR,  &uc_WriteData[1],1);
	CH_PUB_PTSetData(PT_ADDRESS_MASK|THREE_ADDR,&uc_WriteData[2],1);
	CH_PUB_PTSetData(PT_ADDRESS_MASK|FOUR_ADDR, &uc_WriteData[3],1);
	semaphore_signal(pgST_PTSema);
#endif 

	return FALSE;
}


/*������*/
BOOL StandbyLedOn()
{  
#ifdef PT6964 /*yxl 2007-03-07 temp cancel*/
	U8 uc_WriteData[1]={0x01};
	semaphore_wait(pgST_PTSema);
    CH_PUB_PTSetData(PT_MODE,NULL,0);
	CH_PUB_PTSetData(PT_ADDRESS_MASK|LED_ADDR,&uc_WriteData[0],1);/*�̵���*/
	semaphore_signal(pgST_PTSema);

#else

    CH_LED_DisplayStandby();

#endif


	return true;
	
}
/*������*/
BOOL StandbyLedOff()
{
#ifdef PT6964 /*yxl 2007-03-07 temp cancel*/
	U8 uc_WriteData[1]={0x02};
	semaphore_wait(pgST_PTSema);
    CH_PUB_PTSetData(PT_MODE,NULL,0);
	CH_PUB_PTSetData(PT_ADDRESS_MASK|LED_ADDR,&uc_WriteData[0],1);/*�̵���*/
	semaphore_signal(pgST_PTSema);
#else
	semaphore_wait(gp_SemVFD);
	CH_CellControl(CELL_POWER,DISPLAY_OFF);
        CH_VFD_ShowChar(0,8,"         ");
        semaphore_signal(gp_SemVFD);
#endif	
	return true;/*for test*/		
}

/*��Դָʾ��*/
BOOL PowerLightOff(void)
{
#ifdef PT6964 /*yxl 2007-03-07 temp cancel*/
	U8 uc_WriteData[1]={0x01};
	semaphore_wait(pgST_PTSema);
    CH_PUB_PTSetData(PT_MODE,NULL,0);
	CH_PUB_PTSetData(PT_ADDRESS_MASK|LED_ADDR,&uc_WriteData[0],1);/*�����*/
	semaphore_signal(pgST_PTSema);
#endif
	return TRUE;
}
/*��Դָʾ��*/
BOOL PowerLightOn()
{
#ifdef PT6964 /*yxl 2007-03-07 temp cancel*/
	U8 uc_WriteData[1]={0x02};
	semaphore_wait(pgST_PTSema);
    CH_PUB_PTSetData(PT_MODE,NULL,0);
	CH_PUB_PTSetData(PT_ADDRESS_MASK|LED_ADDR,&uc_WriteData[0],1);/*�����*/
	semaphore_signal(pgST_PTSema);
#endif
	return TRUE;
}
/*TUNERָʾ��*/
BOOL TunerLightOff()
{
#ifdef PT6964 /*yxl 2007-03-07 temp cancel*/
	CH_PUB_ClearYLED();
	IsTunerLigthOn=FALSE;
#else
	semaphore_wait(gp_SemVFD);
     CH_CellControl(CELL_TUNER0,DISPLAY_OFF);
	     semaphore_signal(gp_SemVFD);
#endif
	return TRUE;
}
/*TUNERָʾ��*/
BOOL TunerLightOn()
{
#ifdef PT6964 /*yxl 2007-03-07 temp cancel*/
	CH_PUB_SetYLED();
	IsTunerLigthOn=TRUE;/*yxl 2006-05-03 add this line*/
#else
	semaphore_wait(gp_SemVFD);
    CH_CellControl(CELL_TUNER0,DISPLAY_ON);
	     semaphore_signal(gp_SemVFD);
#endif


	return TRUE;
}

BOOL GetTunerLightStatus(void)
{

	return IsTunerLigthOn;
}

/******************************************************************/
/*������:BOvoid CH_DisplayCurrentTimeLed(void)                    */
/*�����˺Ϳ���ʱ��:zengxianggen 2007-04-25                        */
/*������������:��LED����ʾ��ǰ��ϵͳʱ��                          */
/*����ԭ��˵��:                                                   */
/*�����������                                                    */
/*�������: ��                                                    */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                      */
/*���õ���Ҫ�����б�                                            */
/*����ֵ˵����TRUE, ʧ��,FALSE,�ɹ�                               */   
/*����ע������:                                                   */
/*����˵��:                                                       */
void CH_DisplayCurrentTimeLed(void)
{
	TDTTIME  ch_currenttime;
    GetCurrentTime(&ch_currenttime);
    DisplayTimeLED(ch_currenttime);

}

/*********************************************/

/***************************�����嶨��********************************/ 
/******************************************************************/
/*������:void CH_LED_DisplayHDMode(void)                         */
/*�����˺Ϳ���ʱ��:zengxianggen 2007-05-08                        */
/*������������:��ʾ��ǰ�����ģʽ                                 */
/*����ԭ��˵��:                                                   */
/*�����������                                                    */
/*�������: ��                                                    */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��
 pstBoxInfo->HDVideoTimingMode,��ǰ�������ģʽ                   */
/*���õ���Ҫ�����б�                                            */
/*����ֵ˵������                                                  */   
/*����ע������:                                                   */
/*����˵��:                                                       */
void CH_LED_DisplayHDMode( void )
{
		semaphore_wait(gp_SemVFD);
    if(pstBoxInfo->HDVideoTimingMode == MODE_576I50)
	{
      CH_CellControl(CELL_1080I,DISPLAY_OFF);
      CH_CellControl(CELL_720P,DISPLAY_OFF);
	  CH_CellControl(CELL_576P,DISPLAY_OFF);
      CH_CellControl(CELL_576I,DISPLAY_ON);
	}
	else if(pstBoxInfo->HDVideoTimingMode == MODE_576P50)
	{
      CH_CellControl(CELL_1080I,DISPLAY_OFF);
      CH_CellControl(CELL_720P,DISPLAY_OFF);
	  CH_CellControl(CELL_576P,DISPLAY_ON);
      CH_CellControl(CELL_576I,DISPLAY_OFF);
	}
	else if(pstBoxInfo->HDVideoTimingMode == MODE_720P50||
	        pstBoxInfo->HDVideoTimingMode == MODE_720P60)
	{
      CH_CellControl(CELL_1080I,DISPLAY_OFF);
      CH_CellControl(CELL_720P,DISPLAY_ON);
	  CH_CellControl(CELL_576P,DISPLAY_OFF);
      CH_CellControl(CELL_576I,DISPLAY_OFF);
	}
	else /*1080I*/
	{
         CH_CellControl(CELL_1080I,DISPLAY_ON);
         CH_CellControl(CELL_720P,DISPLAY_OFF);
	  CH_CellControl(CELL_576P,DISPLAY_OFF);
         CH_CellControl(CELL_576I,DISPLAY_OFF);
	}
	     semaphore_signal(gp_SemVFD);
}
/******************************************************************/
/*������:void CH_LED_DiplayStandby(void)                          */
/*�����˺Ϳ���ʱ��:zengxianggen 2007-05-08                        */
/*������������:VFD��ʾSTANDBY��Ϣ                                 */
/*����ԭ��˵��:                                                   */
/*�����������                                                    */
/*�������: ��                                                    */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                      */
/*���õ���Ҫ�����б�                                            */
/*����ֵ˵������                                                  */   
/*����ע������:                                                   */
/*����˵��:                                                       */
void CH_LED_DisplayStandby(void)
{

#if 0
  static int si_standbypos=0;
  static int si_direction=1;
  semaphore_wait(gp_SemVFD);
  CH_CellControl(CELL_POINTB,DISPLAY_OFF);
  CH_CellControl(CELL_POINTA,DISPLAY_OFF);
  CH_CellControl(CELL_CH,DISPLAY_OFF);
   CH_VFD_ShowChar(0,8,"          ");
  /* CH_VFD_ShowChar(0,3,"          ");
   CH_VFD_ShowChar(4,8,"          ");*/
  CH_VFD_ShowChar(si_standbypos,si_standbypos+6,"STANDBY");
     semaphore_signal(gp_SemVFD);
  si_standbypos+=si_direction;
  if(si_standbypos>=3)
  {
	 si_direction=-1;
	 si_standbypos+=si_direction;
  }
  if(si_standbypos<0)
  {
     si_direction=1;
	 si_standbypos+=si_direction;
  }
#else
	static boolean glance=true;
	U32  hm = 0;
	TDTTIME  ch_currenttime;
	GetCurrentTime(&ch_currenttime);
	hm = (ch_currenttime.ucHours* 100) + ch_currenttime.ucMinutes;
	if(hm ==0xffffffff)
	return;
	semaphore_wait(gp_SemVFD);
	CH_CellControl(CELL_POINTA,DISPLAY_OFF);
	CH_CellControl(CELL_CH,DISPLAY_OFF);
	CH_VFD_ShowChar(0,3,"    ");
	CH_CellControl(CELL_POWER,DISPLAY_ON);
	CH_VFD_WriteNumber(5,8,hm);
	if (glance == true)
	CH_CellControl(CELL_POINTB,DISPLAY_ON);
	else
	CH_CellControl(CELL_POINTB,DISPLAY_OFF);
	glance = !glance;
	semaphore_signal(gp_SemVFD);

#endif
}


void CH_LEDDis4ByteStr(char *LedStr)
{
	CH_VFD_ShowChar ( 1, 5, LedStr );
}

void CH_LedDisNvod()
{
	char s[]={'N','v','o','d'};
	CH_LEDDis4ByteStr(s);
}


/*End of file*/

