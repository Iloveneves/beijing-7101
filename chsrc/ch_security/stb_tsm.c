
#include "stdarg.h"  
#include "stddefs.h"
#include "string.h"
#include "..\dbase\vdbase.h"
#include "..\util\ch_time.h"
#include "stb_tsm.h"
#include "..\key\RC6_keymap.h"
#include "..\main\initterm.h"
#include "..\eis\eis_api_osd.h"

#if 0
#define _SUMA_DEBUG_
#endif
#ifdef  _SUMA_DEBUG_
static char suma_debug_buff[1024*4];
#endif
#if 0
#define stb_scms_debug
#endif

/*�����������Ŀǰֻ�õ���һ��*/
#define MAX_SUMA_TASK_NUM			5
static task_t *gs_SumaTaskIDNeedDelete[MAX_SUMA_TASK_NUM] = {NULL, NULL};
/*�źŻ���*/
static semaphore_t *gs_SumaTaskLock = NULL;

extern   ST_Partition_t* CHSysPartition;  /*ϵͳ�ڴ���*/

static U8 *SumaSecure_memory =NULL;
static U8 SumaChipID[20] = {0};/*�ݴ�chip id sqzow20100707*/
semaphore_t *p_SumaMemorySema = NULL; /*��ȫ�ֱ���SumaSecure_memory���ʵ��ź���*/

#if 1
static boolean gb_SetUserStatus = false;

boolean CH_GetUserIDStatus(void)
{
	return gb_SetUserStatus;
}

void  CH_SetUserIDStatus(boolean val)
{
	gb_SetUserStatus = val;
}

#endif
/*===================== �����д洢��غ��� ========================*/


/*
1.	�ӿڶ���
	int  SumaSTBSecure_GetFlashAddr(long* plSize, long* plBaseAddr);
2.	����˵��
	��û����з������"�ն˰�ȫģ��"��ʹ�õ�Flash(�����������ݣ��Ǵ���)�Ļ���ַ�ʹ�С��
3.	����˵��
	�����������
	���������
        plSize��Flash�ռ��С
        plBaseAddr|��Flash�ռ���׵�ַ
	����ֵ��
       		 �ɹ���0�����ɹ�������ֵ������ţ�
4.	Ӧ�ó���
	�ն˰�ȫģ����Ҫд���ݵ�Flash��ʱ����Ҫ��û����з�����ն˰�ȫģ���Flash�ռ���׵�ַ�Ϳռ��С��
*/
int SumaSTBSecure_GetFlashAddr(int* plSize, int* plBaseAddr)
{
#ifdef stb_scms_debug
	do_report(0,"SumaSTBSecure_GetFlashAddr\n");
#endif
	p_SumaMemorySema=semaphore_create_fifo(1);
	SumaSecure_memory = memory_allocate(CHSysPartition,FLASH_SECURITYDATA_LEN);
	if(SumaSecure_memory == NULL)
	{
		do_report(0,"SumaSTBSecure_GetFlashAddr err!!\n");
		*plBaseAddr = 0;
		*plSize = 0;
		return -1;
	}
	else
	{
		memset(SumaSecure_memory,0,FLASH_SECURITYDATA_LEN);
		
		if(CH_GetSecurityFlashStatus() != 0X8A)
		{
			do_report(0,"SumaSTBSecure_ReadFlash������Ч��Ҫ�ָ�\n");
			CH_CheckSecurityDataStatus();
		}

		if(CH_GetSecurityFlashStatus() == 0X8A)
		{
			
			ipanel_porting_nvram_read(FLASH_SECURITY_ADDR,SumaSecure_memory,FLASH_SECURITYDATA_LEN);
		
		}

			*plBaseAddr = (int)SumaSecure_memory;
			*plSize = FLASH_SECURITYDATA_LEN;

			return 0;

	}
}

/*
1.	�ӿڶ���
	int  SumaSTBSecure_ReadFlash(long lBeginAddr, long* plDataLen, BYTE* pData);
2.	����˵��
	��ȡFlash�б�������ݵ��ڴ档
3.	����˵��
	���������
	lBeginAddr���������ݵ�Flash�ĵ�ַ�����Ե�ַ��
		         plDataLen��	��Ҫ��ȡ�����ݵĳ���
	        ��������� 
	pData��		��ȡ������
		         plDataLen��	ʵ�ʶ�ȡ�����ݵĳ���
        ����ֵ��
                �ɹ���0�����ɹ�������ֵ������ţ�
4.	Ӧ�ó���
	�ն˰�ȫģ���ȡ������Flashָ��λ��ָ����С�����ݵ������ڴ棬�����ն˰�ȫģ���ȡ������Flash�е��ն�֤������ݣ�

*/
int SumaSTBSecure_ReadFlash(int lBeginAddr, int* plDataLen, BYTE* pData)
{
	int len = *plDataLen;

#ifdef stb_scms_debug
	do_report(0,"SumaSTBSecure_ReadFlash 0x%x len=0x%x\n",lBeginAddr,len);
#endif
  	semaphore_wait(p_SumaMemorySema);
	memcpy((BYTE *)pData,(BYTE *)lBeginAddr,len);	
	semaphore_signal(p_SumaMemorySema);
	return 0;
}

/*
1.	�ӿڶ���
	int  SumaSTBSecure_WriteFlash(long lBeginAddr, long* plDataLen, const BYTE* pData);
2.	����˵��
	�����ڴ������ݵ�Flash�С�
3.	����˵��
	���������
 	lBeginAddr���������ݵ�Flash�ĵ�ַ�����Ե�ַ��
	 plDataLen�� ��Ҫ��������ݵĳ���
	  pData��	  ���������
����ֵ��
     	�ɹ���0�����ɹ�������ֵ������ţ�
4.	Ӧ�ó���
	�ն˰�ȫģ�鱣��ָ����С�����ݵ�Flashָ��λ�á������ն˰�ȫģ�鱣���ն�֤������ݵ�Flash��
*/
int SumaSTBSecure_WriteFlash(int lBeginAddr, int* plDataLen, const BYTE* pData)
{
	int i;
	int writelen = *plDataLen;
	
	int i_offset = 0;
	
#ifdef stb_scms_debug
	do_report(0,"SumaSTBSecure_WriteFlash 0x%x len 0x%x\n",lBeginAddr,writelen);
#endif	
	
	/* �������ݵ��ڴ���*/
	i_offset = lBeginAddr- (int)SumaSecure_memory;
	memcpy((BYTE *)((int)SumaSecure_memory+i_offset),pData,writelen);


	

	CH_SetSecurityFlashStatus(0x8B);

	CH_Security_Flash_burn(FLASH_SECURITY_ADDR,SumaSecure_memory,FLASH_SECURITYDATA_LEN);
		

	/*FLASH����*/
	CH_SetSpareSecurityFlashStatus(0x8B);

	CH_Security_Flash_burn(FLASH_SECURITY_ADDR_SPARE,SumaSecure_memory,FLASH_SECURITYDATA_LEN);
		
	
	return 0;


}

/*
1.	�ӿڶ���
	int   SumaSTBSecure_GetOTPAddr(long* plSize, long* plBaseAddr);
2.	����˵��
	��û����з������"�ն˰�ȫģ��"��ʹ�õ�OTP�Ļ���ַ�ʹ�С��
3.	����˵��
        �����������
	���������
   		 plSize��OTP�ռ��С
   	 	plBaseAddr��OTP�ռ���׵�ַ
	����ֵ��
    		�ɹ���0�����ɹ�������ֵ������ţ�
4.	Ӧ�ó���
	�ն˰�ȫģ����Ҫ��ȡ������OTP�еĸ���Կ��Ϣʱ����Ҫ���Ȼ�û����з�����ն˰�ȫģ���OTP�ռ���׵�ַ�ʹ�С��

*/
int SumaSTBSecure_GetOTPAddr(int* plSize, int* plBaseAddr)
{
#ifdef stb_scms_debug
	do_report(0,"SumaSTBSecure_GetOTPAddr \n");
#endif
	if(plSize ==  NULL || plBaseAddr==NULL)
		return -1;
	* plSize = 128*1024;
	* plBaseAddr = 0xA1FC0000;
	return 0;

}

/*
1.	�ӿڶ���
	int   SumaSTBSecure_ReadOTP(long lBeginAddr, long* plDataLen, BYTE* pData);
2.	����˵��
	��ȡOTP���ݵ��ڴ档
3.	����˵��
     	���������
		 lBeginAddr���������ݵ�OTP��ַ�����Ե�ַ��
		  plDataLen��	��Ҫ��ȡ�����ݵĳ���
	��������� 
		pData��		��ȡ������
		plDataLen��	ʵ�ʶ�ȡ�����ݵĳ���
      ����ֵ��
         �ɹ���0�����ɹ�������ֵ������ţ�
4.	Ӧ�ó���
	�ն˰�ȫģ����Ҫ��ȡ������OTP�еĸ���Կ

*/
int SumaSTBSecure_ReadOTP(int lBeginAddr, int* plDataLen, BYTE* pData)
{
#ifdef stb_scms_debug
	do_report(0,"SumaSTBSecure_ReadOTP \n");
#endif

	if(plDataLen ==  NULL || pData==NULL)
		return -1;
	
	ipanel_porting_nvram_read(lBeginAddr,pData,*plDataLen);
	return 0;
}

/*
1.	�ӿڶ���
	int   SumaSTBSecure_GetEepromAddr(long* plSize, long* plBaseAddr);
2.	����˵��
	��û����з������"�ն˰�ȫģ��"��ʹ�õ�EEPROM(�����������ݣ��Ǵ���)�Ļ���ַ�ʹ�С��
3.	����˵��
	�����������
	���������
        	plSize��EEPROM�ռ��С
        	plBaseAddr��EEPROM�ռ���׵�ַ
	����ֵ��
        	�ɹ���0�����ɹ�������ֵ������ţ�
4.	Ӧ�ó���
	�ն˰�ȫģ�鱣���ն˼���˽Կ�����ݵ�EEPROMʱ����Ҫ��ȡEEPROM���׵�ַ�ʹ�С��
*/
int SumaSTBSecure_GetEepromAddr(int* plSize, int* plBaseAddr)
{
#ifdef stb_scms_debug
	do_report(0,"SumaSTBSecure_GetEepromAddr \n");
#endif

	if(plSize ==  NULL || plBaseAddr==NULL)
		return -1;
	
	*plBaseAddr = START_E2PROM_SECURITYDATA;
	*plSize = E2PROM_SECURITYDATA_LEN;

	return 0;

}
/*
	1.	�ӿڶ���
	int   SumaSTBSecure_ReadEeprom(long lBeginAddr, long* plDataLen, BYTE* pData);
	2.	����˵��
	��EEPROM���ݵ��ڴ档
	3.	����˵��
	���������
 		 lBeginAddr���������ݵ�EEPROM�ĵ�ַ�����Ե�ַ��
		 plDataLen��	��Ҫ��ȡ�����ݵĳ���
	��������� 
		pData��		��ȡ������
		plDataLen��	ʵ�ʶ�ȡ�����ݵĳ���
   	����ֵ��
             �ɹ���0�����ɹ�������ֵ������ţ�
	4.	Ӧ�ó���
		�ն˰�ȫģ���EEPROM��ȡ����˽Կ���ڴ档

*/
int SumaSTBSecure_ReadEeprom(int lBeginAddr, int* plDataLen, BYTE* pData)
{
#ifdef stb_scms_debug
	do_report(0,"SumaSTBSecure_ReadEeprom adr %d len %d pdata 0x%x\n",lBeginAddr,*plDataLen,pData);
#endif
	if(plDataLen ==  NULL || pData==NULL)
		return -1;
	if((int)(lBeginAddr + (int)*plDataLen) >= START_E2PROM_SECURITYDATA_SPARE )/*д��Խ��*/
	{
		*plDataLen = 0;
		do_report(0,"ERR!!!SumaSTBSecure_ReadEeprom err!! %d len %d\n",lBeginAddr,*plDataLen);
		return -1;
	}
	if(CH_ReadSecurityData(lBeginAddr, (int)*plDataLen, (void *) pData) == TRUE)/*�����ݷ�������*/
	{
		*plDataLen = 0;
		do_report(0,"ERR!!!SumaSTBSecure_ReadEeprom->CH_ReadSecurityData err!! \n");
		return -1;	
	}
	else
	{
		return 0;
	}

}
/*	
	1.	�ӿڶ���
		int SumaSTBSecure_WriteEeprom(long lBeginAddr, long* plDataLen, const BYTE* pData);
	2.	����˵��
		��ָ�����ȵ��ڴ����ݼ�¼��EEPROM��ָ��λ�á�
	3.	����˵��
		���������
			lBeginAddr�� EEPROM�ĵ�ַ��
			plDataLen���������ݳ��ȵ�ָ��
			pData���������ݵ�ָ��
		�����������
		���أ�
		�ɹ���0�����ɹ�������ֵ������ţ�
		4.	Ӧ�ó���
		���ն�֤������ɹ�����Ҫ��֤���Ӧ��˽Կ���ܴ洢��EEPROM�С�
*/
int SumaSTBSecure_WriteEeprom(int lBeginAddr, int* plDataLen, const BYTE* pData)
{
#ifdef stb_scms_debug
	do_report(0,"SumaSTBSecure_WriteEeprom adr %d len %d pdata 0x%x \n",lBeginAddr,*plDataLen,pData);
#endif
	if(plDataLen ==  NULL || pData==NULL)
		return -1;
	if((int)(lBeginAddr + (int)*plDataLen) >= START_E2PROM_SECURITYDATA_SPARE )/*д��Խ��*/
	{
		*plDataLen = 0;
		do_report(0,"ERR!!!SumaSTBSecure_WriteEeprom err!! %d len %d\n",lBeginAddr,*plDataLen);
		return -1;
	}

	if(CH_WriteSecurityData(lBeginAddr, (int)*plDataLen, (void *) pData) == TRUE)
	{
		*plDataLen = 0;
		do_report(0,"ERR!!!SumaSTBSecure_WriteEeprom->CH_WriteSecurityData err!!\n ");
		return -1;	
	}
	else
	{
		return 0;
	}

}
/****************************************************/
/*
1.	�ӿڶ���
	Int   SumaSTBSecure_SemaphoreInit( long * pSemaphore long lInitCount);
2.	����˵����
	��ʼ���ź�����
3.	����˵��
	���������
	pSemaphoreָ���ź�����ָ�룻
	lInitCount����ʼ���ź���ֵ?
4.	Ӧ�ó���
*/
int SumaSTBSecure_SemaphoreInit( U32* pSemaphore ,U32 lInitCount)
{
	semaphore_t* TempSem  = 0;
#ifdef stb_scms_debug
	do_report(0,"SumaSTBSecure_SemaphoreInit %d \n",lInitCount);
#endif
	if(pSemaphore ==  NULL )
		return -1;
	TempSem =semaphore_create_fifo_timeout(lInitCount);

	*pSemaphore = (U32)TempSem;

	return 0;

}
/*
	1.	�ӿڶ���
		Int   SumaSTBSecure_Semaphore Release ( long lSemaphore );
	2.	����˵����
		�ͷ��ź�����
	3.	����˵��
		���������
		pSemaphoreָ���ź�����ָ�룻
	4.	Ӧ�ó���

*/
int   SumaSTBSecure_SemaphoreRelease ( U32 pSemaphore )
{

#ifdef stb_scms_debug
	do_report(0,"SumaSTBSecure_SemaphoreRelease 0x%x \n",pSemaphore);
#endif
	    if(pSemaphore ==0)
	    {
		 return -1;
	    }
	
	semaphore_delete((semaphore_t*)pSemaphore);
	return 0;

}

/*
	1.	�ӿڶ���
		int  SumaSTBSecure_SemaphoreSignal( long lSemaphore);
	2.	����˵��
		Ϊ�ź������ź�
	3.	����˵��
		���������pSemaphoreָ���ź�����ָ�룻
	4.	Ӧ�ó���

*/

int SumaSTBSecure_SemaphoreSignal( U32 lSemaphore)
{
#ifdef stb_scms_debug
	do_report(0,"SumaSTBSecure_SemaphoreSignal  0x%x \n",lSemaphore);
#endif
	if(lSemaphore ==0)
	{
	 return -1;
	}

	semaphore_signal((semaphore_t*)lSemaphore);
	return 0;
}

/*
	1.	�ӿڶ���
		int  SumaSTBSecure_SemaphoreTimedWait(long lSemaphore ��long lWaitCount);
	2.	����˵��
		�ȴ��ź���,�ȴ��ɹ�֮��,�ź���Ϊ���ź�
	3.	����˵��
		���������
			lSemaphoreָ���ź�����ָ�룻
			lWaitCount���ȴ���ʱ����, ��λ��ms��-1��ʾ���õȴ���
4.	Ӧ�ó���

*/
int  SumaSTBSecure_SemaphoreTimedWait(U32 lSemaphore ,U32 lWaitCount)
{
	int ret;
	clock_t  time;
	int val;
#ifdef stb_scms_debug
	do_report(0,"SumaSTBSecure_SemaphoreTimedWait  0x%x  %d \n",lSemaphore,lWaitCount);
#endif
   	 if(lSemaphore ==0)
	    {
		 return -1;
	    }
	

	if(lWaitCount == 0)
	{
    		val = semaphore_wait_timeout((semaphore_t*)lSemaphore,TIMEOUT_IMMEDIATE);
		
	}
	else if(lWaitCount == -1)
	{
		
		val =  semaphore_wait_timeout((semaphore_t*)lSemaphore,TIMEOUT_INFINITY);	

	}
	else if(lWaitCount >0)
	{
		
		time = time_plus(time_now(), ST_GetClocksPerSecond()*lWaitCount/1000);
		val =  semaphore_wait_timeout((semaphore_t*)lSemaphore,&time);	
	}
	else
	{

		return -1;

	}
	
	if(val == 0)
	{

		return 0;
	}
	else
	{

		return -1;
	}
	
 
}

/*
	1.	�ӿڶ���
		Int SumaSTBSecure_ CreateThread (const char * szName, pThreadFunc pTaskFun, Int priority, void * param, int StackSize��unsigened long *plThreadID);
	2.	����˵��
		�������̡�
	3.	����˵��
		���������
			szName�������Ľ�������
         		pTaskFun�������Ľ��̶�Ӧ�ĺ���
			priority�����ȼ���0-15��normal Ϊ7 8�������ȼ�Ϊ12
			param�����̺�������
			StackSize������ջ��С��������ѡ��
         	���������
              		PlThreadID �����Ľ���ID
	����ֵ��
       		�ɹ���0�����ɹ��������
	4.	Ӧ�ó���

*/
int SumaSTBSecure_CreateThread (const char * szName, pThreadFunc pTaskFun, int priority, void * param, int StackSize, U32*plThreadID)
{
       task_t* SecurityTask;

#ifdef stb_scms_debug
	do_report(0,"SumaSTBSecure_CreateThread %s priority %d  stacksize %d ",szName,priority,StackSize);
#endif
	StackSize *=2;/*��ջ����һ�㣬���֤�����ʱ���������� sqzow20100706*/
   	 if( pTaskFun == NULL)
	    {
		 return -1;
	    }

	   SecurityTask = Task_Create (pTaskFun,param,StackSize,priority,szName,0);

	   *plThreadID = (U32)SecurityTask;
	   
	   return 0;

}

/*-------------------------------��������ע��ͷ--------------------------------------
    FuncName:SumaSTB_DeleteThread
    Purpose: �첽ɾ����ȡ֤����ӵ�����
    Reference: 
    in:
        void >> 
    out:
        none
    retval:
        success >> 
        fail >> 
   
notice : 
    sqZow 2010/07/06 19:31:00 
---------------------------------�����嶨��----------------------------------------*/
void SumaSTB_DeleteThread(void)
{
	int i;

	if(gs_SumaTaskLock == NULL)
	{
		return;
	}
	semaphore_wait(gs_SumaTaskLock);
	for(i = 0; i < MAX_SUMA_TASK_NUM; i++)
	{
		if(gs_SumaTaskIDNeedDelete[i])
		{
			task_wait(&gs_SumaTaskIDNeedDelete[i], 1, TIMEOUT_INFINITY);
			task_delete(gs_SumaTaskIDNeedDelete[i]);
			gs_SumaTaskIDNeedDelete[i] = NULL;
		}
	}
	semaphore_signal(gs_SumaTaskLock);
}

/*
	1.	�ӿڶ���
		int  SumaSTBSecure_TerminateThread(unsigened long lThreadID);
	2.	����˵��
		ǿ���˳��̡߳�
	3.	����˵��
		���������
		hThread���������̵ľ�� 
	4.	Ӧ�ó���

*/
int  SumaSTBSecure_TerminateThread(U32 lThreadID)
{

#if 1 	/*sqzow20100706*/
	int i;

	if(gs_SumaTaskLock == NULL)
	{
		gs_SumaTaskLock = semaphore_create_fifo(0);
		if(gs_SumaTaskLock == NULL)
		{
			do_report(0,"error !!SumaSTBSecure_TerminateThread create sem fail! ");
		}
	}
	else
	{
		semaphore_wait(gs_SumaTaskLock);
	}
	for(i = 0; i < MAX_SUMA_TASK_NUM; i++)
	{
		if(gs_SumaTaskIDNeedDelete[i] == NULL)
		{
			gs_SumaTaskIDNeedDelete[i] = (task_t*)lThreadID;
			break;
		}
	}
	if(i == MAX_SUMA_TASK_NUM)
	{
		do_report(0,"SumaSTBSecure_TerminateThread, task num too large! ");
	}
	if(gs_SumaTaskLock)
	{
		semaphore_signal(gs_SumaTaskLock);
	}
	MakeVirtualkeypress(VIRKEY_CLOSESUMA_KEY);
#else
	task_delete((task_t*)lThreadID);
#endif

#ifdef stb_scms_debug
	do_report(0,"SumaSTBSecure_TerminateThread ");
#endif
	return 0;
}

/*===================== ��ȡ�����������Ϣ��غ��� ========================*/
/*
1.	�ӿڶ���
	Int   SumaSTBSecure_GetSTBID(char* szSTBID, int * piLen);
2.	����˵��
	��ȡ������ID��
3.	����˵��
	���������szSTBID�������б��
	piLen����ų���
	����ֵ��
		�ɹ���0�����ɹ�������ֵ������ţ�
4.	Ӧ�ó���
		���ն�֤���������£����߹����ն���ݵ�ǩ�����ƾ֤ʱ����Ҫ�������ṩ�ӿڻ�ȡ��ǰ�����е������Ϣ������STBID��ChipID��CardID��MacAddr	��Ϣ��

*/
int SumaSTBSecure_GetSTBID(char* szSTBID, int* piLen)
{
				
	U8 STB_SN[8];
	boolean GetSTB=true;
	U8 ManuID;
	U8 ProductModel;
	int ProYear;
	U8 ProMonth;
	U8 ProDay;
	long int STB_SNnum;
	 char strSTBInfo[20];
	 int len;
#ifdef stb_scms_debug
	 do_report(0,"SumaSTBSecure_GetSTBID buf=%x len=%d\n",szSTBID,* piLen);
#endif
	do_report(0,"SumaSTBSecure_GetSTBID buf=%x len=%d\n",szSTBID,*piLen);

	if(szSTBID ==  NULL || piLen==NULL)
		return -3;
	
	memset(strSTBInfo,0,20);
	
	GetSTB=CHCA_GetStbID(STB_SN);                /*Get STB serial number*/

	STB_SNnum=STB_SN[5]|(STB_SN[4]<<8)|(STB_SN[3]&0xF)<<16;
	ManuID=STB_SN[7];
	ProductModel=STB_SN[6];
	ProYear=((STB_SN[1]&0xf)<<3)|((STB_SN[2]&0xe0)>>5);
	ProMonth=((STB_SN[2]&0x1e)>>1)&0xF;
	ProDay=((STB_SN[2]&0x1)<<4)|((STB_SN[3]>>4)&0xf);

	if(STB_SNnum==0xFFFFF||GetSTB==true)
	{	
		*piLen = 0;
		do_report(0,"SumaSTBSecure_GetSTBID  Failed,num=0x%x, getstb=%d \n",
			STB_SNnum,
			GetSTB);

		return -3;
	}
	else
	{
	       sprintf(strSTBInfo,"%02d%02d%02d%02d%02d%07d",ManuID,ProductModel,ProYear,ProMonth,ProDay,STB_SNnum);
		len = strlen(strSTBInfo);

		{
			*piLen = len;
			memcpy(szSTBID,strSTBInfo,len);
			do_report(0,"SumaSTBSecure_GetSTBID %s \n",szSTBID);

			return 0;
		}	
	}


}
/*
	1.	�ӿڶ���
		Int   SumaSTBSecure_ GetChipID (char* szChipID, int * piLen);
	2.	����˵��
		��ȡ������оƬID��
	3.	����˵��
		���������szChipID��������оƬ���
		piLen����ų���
		����ֵ��
		�ɹ���0�����ɹ�������ֵ�������
	4.	Ӧ�ó���
		���ն�֤���������£����߹����ն���ݵ�ǩ�����ƾ֤ʱ����Ҫ�������ṩ�ӿڻ�ȡ��ǰ�����е������Ϣ������STBID��ChipID��CardID��MacAddr	��Ϣ��

*/
int SumaSTBSecure_GetChipID(char* szChipID, int* piLen)
{
	if(szChipID && piLen)
	{
		strcpy(szChipID, SumaChipID);
		*piLen = strlen(SumaChipID);
		return 0;
	}
	return -1;
}

/*-------------------------------��������ע��ͷ--------------------------------------
    FuncName:SumaSTBSecure_GetChipIDPrevate
    Purpose: �ڲ�Ԥ��chip id���������������̻�ȡ���޷�����
    Reference: 
    in:
        piLen >> 
        szChipID >> 
    out:
        none
    retval:
        success >> 
        fail >> 
   
notice : 
    sqZow 2010/07/07 22:23:57 
---------------------------------�����嶨��----------------------------------------*/
int SumaSTBSecure_GetChipIDPrevate(void)
{
	U8 NuIDKey[16];
	U32 NuICRC;
	U32 DevPubID;
	char NuIDInfo[10];
	int len;
         ST_ErrorCode_t Error;
#ifdef stb_scms_debug
	// do_report(0,"SumaSTBSecure_GetChipID buf=%x len=%d\n",szChipID,* piLen);
#endif

	
	memset(NuIDKey,0,16);
	memset(NuIDInfo,0,10);
	
#if 0 	/*<!-- ZQ 2009/9/6 16:14:18 --!>*/
	CH_GetNUID((U8 *)NuIDKey,(U32 *)&NuICRC); 
#endif 	/*<!-- ZQ 2009/9/6 16:14:18 --!>*/

        Error = CH_GetNUIDFormTKDMA(NuIDKey);
        if(Error != ST_NO_ERROR)
        {
#ifdef stb_scms_debug        
             //do_report(0,"Error in Get ChipID.\n");
#endif
            return -1;
        }
        
	DevPubID=(U32)(NuIDKey[3] << 24) | \
				(NuIDKey[2] << 16) | \
				(NuIDKey[1] << 8)  |  \
				(NuIDKey[0]);

	sprintf(NuIDInfo,"%x",DevPubID);
	len = strlen(NuIDInfo);
	/*	if( len > *piLen)
	{
		do_report(0,"SumaSTBSecure_GetChipID len %d С��оƬID����%d",*piLen,len);
	
		memcpy(szChipID,NuIDInfo,*piLen);
		return -1;
	}
	else*/
	{
		memcpy(SumaChipID,NuIDInfo,len);
		return 0;
	}

}
/*
	1.	�ӿڶ���
		Int   INT   SumaSTBSecure_GetCardID(char* szCardID, int * piLen);
	2.	����˵��
		��ȡ��ǰ���ܿ����š�
	3.	����˵��
		���������szCardID����ǰ���ܿ����
		piLen����ų���
		����ֵ��
		�ɹ���0�����ɹ�������ֵ�������
	4.	Ӧ�ó���
		���ն�֤���������£����߹����ն���ݵ�ǩ�����ƾ֤ʱ����Ҫ�������ṩ�ӿڻ�ȡ��ǰ�����е������Ϣ������STBID��ChipID��CardID��MacAddr	��Ϣ��

*/
int SumaSTBSecure_GetCardID(char* szCardID, int* piLen)
{
    	U8 Card_SN[6];
	long int Tmepcardnum1;
	long int Tmepcardnum2;
	char strCardInfo[20];
	int len;

#ifdef stb_scms_debug
	do_report(0,"\nSumaSTBSecure_GetCardID  buf %x len %d \n",szCardID,*piLen);
#endif
	if(szCardID ==  NULL || piLen==NULL)
		return -3;
	
	memset(strCardInfo,0,20);
		
	if(CHCA_GetCardContent(Card_SN) == TRUE)
	{
		*piLen = 0;
		do_report(0,"SumaSTBSecure_GetCardID ERR\n");
		return -3;
	}
	else
	{

		Tmepcardnum1=Card_SN[0]*256+Card_SN[1];
		Tmepcardnum2=Card_SN[2]*16777216+Card_SN[3]*65536+Card_SN[4]*256+Card_SN[5];
		if(Tmepcardnum1==0)
		{
			sprintf(strCardInfo,"%d",Tmepcardnum2);
		}
		else
		{
			sprintf(strCardInfo,"%d%d",Tmepcardnum1,Tmepcardnum2);
		}

		len = strlen(strCardInfo);
		if( len > *piLen)
		{
			do_report(0,"SumaSTBSecure_GetCardID len %d С�ڿ��ų���%d",*piLen,len);
	
			memcpy(szCardID,strCardInfo,*piLen);
			return -1;
		}
		else
		{
			*piLen = len;
			memcpy(szCardID,strCardInfo,len);
			return 0;
		}
			
	

	}

	}
/*
	1.	�ӿڶ���
		int  SumaSTBSecure_GetMacAddr(char* szMacAddr, int * piLen);
	2.	����˵��
		��ȡ��ǰ������MAC��ַ��
	3.	����˵��
		���������szMacAddr:������MAC��ַ
						piLen:MAC	��ַ����
		����ֵ��
			�ɹ���0�����ɹ�������ֵ�������
	4.	Ӧ�ó���
   	 ���ն�֤���������£����߹����ն���ݵ�ǩ�����ƾ֤ʱ����Ҫ�������ṩ�ӿڻ�ȡ��ǰ�����е������Ϣ������STBID��ChipID��CardID��MacAddr	��Ϣ��

*/
int  SumaSTBSecure_GetMacAddr(char* szMacAddr, int* piLen)
{
#define MAC_ADDRESS_LENGTH 6
	U8  mac[MAC_ADDRESS_LENGTH];
	char s[20];
	int len;
#ifdef stb_scms_debug
	do_report(0,"SumaSTBSecure_GetMacAddr  buf %x len %d \n",szMacAddr,*piLen);
#endif
	if(szMacAddr ==  NULL || piLen==NULL)
		return -3;
	
	memset(s,0,20);
	
	CH_ReadSTBMac(mac);
	
	sprintf(s,"%02x%02x%02x%02x%02x%02x\0",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
	len = strlen(s);
/*	if( len > *piLen)
	{
		do_report(0,"SumaSTBSecure_GetMacAddr len %d С��MAC����%d",*piLen,len);
		memcpy(szMacAddr,s,*piLen);
		return -1;
	}
	else*/
	{
		*piLen = len;
		memcpy(szMacAddr,s,len);
		return 0;
	}
	
	
}

/*====================�ڴ����ؽӿ�===================================*/
/*
1.	�ӿڶ���
void*  SumaSTBSecure_malloc(int size);
2.	����˵��
�������ָ����С���ڴ�顣�ڴ���亯����Ҫ��ÿ�ε��øú�����ͳ������ڴ�ռ�����������������ڴ档
3.	����˵����
���������
size:������ڴ��С�����ֽ�Ϊ��λ������0��Ч��С�ڵ���0��Ч��
�����������
�� �أ�
�ɹ���������Ч�ڴ�����ʼ��ַ��ʧ�ܣ�����NULL����ָ�룩��
4.	Ӧ�ó���
*/

void*  SumaSTBSecure_malloc(int size)
{
	U8 * buf = NULL;

	if(size > 0)
	{
		buf = memory_allocate(CHSysPartition,size);
#ifdef stb_scms_debug
	do_report(0,"<<<<SumaSTBSecure_malloc 0x%x size %d \n",buf,size);
#endif
		return buf;
	}
	else
	{
		return NULL;
	}

}

/*
	1.	�ӿڶ���
	void  SumaSTBSecure_free(void *ptr);
	2.	����˵��
	�ͷ�ָ��ptr ָ�����ڴ�ռ䣬���Ҹ��ڴ�ռ��������SumaSTBSecure_malloc ����ġ�
	3.	����˵����
	���������
	ptr ��ָ��Ҫ�ͷŵ��ڴ��ָ�롣
	�����������
	��    �أ���
	4.	Ӧ�ó���
	
*/

void  SumaSTBSecure_free(void *ptr)
{
#ifdef stb_scms_debug
	do_report(0,"<<<<<SumaSTBSecure_free 0x%x\n",ptr);
#endif

	if(ptr != NULL)
	{
		memory_deallocate(CHSysPartition,ptr);
		ptr = NULL;
	}
}

/*

*/
boolean SumaSTBSecure_GetSeeds(BYTE * pbyTime,BYTE * pbyTickCount,BYTE * pbyBuf,int nBufLen)
{
	TDTTIME   curtime;
	clock_t   timenow;

	if(pbyTime == NULL || pbyTickCount == NULL || pbyBuf == NULL || nBufLen <= 0)
		return false;

	GetCurrentTime(&curtime);
  
	pbyTime[0] = curtime.ucHours;
	pbyTime[1] = curtime.ucMinutes;
	pbyTime[2] = curtime.ucSeconds;
	
	timenow = time_now();
 
	memcpy(pbyBuf,(char *)&curtime,nBufLen);

	*pbyTickCount = time_now();


	return true;
    
}

/*======================== ��Ҫ�����н���֧�ֵ���غ���================================*/


/*
	1.	�ӿڶ���
	int  SumaSTBSecure_GetUserPIN(char* szUserPIN);
	2.	��������
	��ȡ˽Կ�������
	3.	����˵����
	�����������
	���������
		szUserPIN��˽Կ�������
	�� �أ�
	0��ʾ�ɹ���������ʾʧ�ܡ�
	4.	Ӧ�ó���
	����Ҫϵͳҵ��������Ҫ�û�����˽Կ��������ʱ�����ô˽ӿڡ�


*/
int SumaSTBSecure_GetUserPIN(char* szUserPIN)
{	
	
#if 1
#define MAX_PIN_LEN 16
	boolean                    focus = true;

	int 		  iKeyScanCode;
	boolean      NOEXIT_TAG=true;

	char           tempchar;
	char           retbuf[MAX_PIN_LEN+1];
	int              curinputpos =0;

	int              x,y,w,h,text_x,text_y,char_width;
	int              max_string_len = 0;

	U8            *tempbuf = NULL;

	CH_RESOLUTION_MODE_e enum_Mode;

	CH_SetUserIDStatus(TRUE);

	 enum_Mode = CH_GetReSolutionRate() ;
	 

	 if(enum_Mode == CH_1280X720_MODE)
    	{
  			x = 400 ;
			y = 200;
			w= 420;
			h = 288;
			text_x  = x + 105+10;
			text_y = y+ 144+10;
        }
	else
	{
		x = 246;
		y = 170;
		w= 289;
		h = 198;
		text_x  = x + 46;
		text_y = y+ 108+5;
	 }


	
	max_string_len = MAX_PIN_LEN+1;
	memset(retbuf,0,MAX_PIN_LEN+1);


	
	tempbuf = CH_Eis_GetRectangle(x, y,w, h);
	if(tempbuf == NULL)
	{
		CH_SetUserIDStatus(FALSE);
		return -1;
	}
	 if(enum_Mode == CH_1280X720_MODE)
 	{
		CH_Eis_DisplayPicdata(x, y,w,h ,(U8*)BIG_PINPIC_ADDR);
 	}
	 else
 	{
		CH_Eis_DisplayPicdata(x, y,w,h,(U8*)SMALL_PINPIC_ADDR);
 	}


	while(NOEXIT_TAG)
	{
		   
		iKeyScanCode=GetMilliKeyFromKeyQueue (500 );

               switch(iKeyScanCode)
		{
			case OK_KEY:

				memcpy(szUserPIN,retbuf,MAX_PIN_LEN+1);
		 		NOEXIT_TAG=false;
		   		do_report(0,"<<<<SumaSTBSecure_GetUserPIN %s \n",szUserPIN);
		   		break;
			case EXIT_KEY:

				szUserPIN = "\0";

		  	 	NOEXIT_TAG=false;
		 
		   		break;
			case LEFT_ARROW_KEY:
				if(curinputpos == 0)
				{
					break;
				}
				curinputpos --;
				retbuf[curinputpos] = '\0';

				SumaSecure_SKB_fillBox(text_x+ 11*curinputpos, text_y,11,12, 0x80E6EBF1);
				ipanel_porting_graphics_draw_image( text_x+ 11*curinputpos, text_y,11, 12, NULL, EIS_REGION_WIDTH);
				/* ����һ��*��*/
				break;
			default:  
				if((iKeyScanCode>=KEY_DIGIT0)&&(iKeyScanCode<=KEY_DIGIT9))
				{
					if(curinputpos <max_string_len)
					{
						tempchar = Convert2Char(iKeyScanCode);
						retbuf[curinputpos] = tempchar;
						
						CH_Eis_DisplayPicdata(text_x+ 11*curinputpos, text_y, 10,11,BIG_PINSTAR_ADDR);
						curinputpos++;
					}

				}
				
		   		break;
		}
		
	   }

		CH_Eis_PutRectangle(x, y, w, h, tempbuf);

	ipanel_porting_graphics_draw_image( x, y, w, h, NULL, EIS_REGION_WIDTH);

		CH_SetUserIDStatus(FALSE);
	   return  0;


EXIT_ERR:
		CH_SetUserIDStatus(FALSE);
		return -1;
#endif	   
	
}
#if 0
int SumaSTBSecure_GetUserID(char* szUserID)
{
#ifdef stb_scms_debug
	do_report(0,"SumaSTBSecure_GetUserID \n");
#endif
	CH_Security_InputDialog("��ʾ��Ϣ","���������ʶ���:(��ͬ��)",szUserID,0);
}
/*
���ܣ���ȡ�û��ĵ�¼ID�����ͬ�ţ���PIN�룬
�����
szUserID:	�û�ID��Ϣ 
szPassword:�û�����
ʹ�ó�����
1���û�ʹ��uKeyʱ����Ҫ������ID�Ϳ���
2���û�ʹ����֤��ʱ�����ݱ�־λ�����Ƿ���Ҫ�û�����id�Ϳ���
*/
//�������ҳ��ʾ�������Ϣ��������ҳ����ģ�����ǻ������ṩ
//����Ҫ�ṩ������Ĳ�������
int SumaSTBSecure_GetLogonInfo(char* szUserIDInfo,char * szPIN)
{
#ifdef stb_scms_debug
	do_report(0,"SumaSTBSecure_GetLogonInfo \n");
#endif
	CH_Security_UkeyDialog("������UKEY��Ϣ","�û���","����", szUserIDInfo, szPIN);
}

/*
���ܣ���ʾ�û�ѡ��
�����0 :yes   others: no
szMsg: ������Ϣ
*/
//ɶ��˼?
int SumaSTBSecure_YesNoBox(const char* szMsg)
{
	int result = 2;
#ifdef stb_scms_debug
	do_report(0,"SumaSTBSecure_YesNoBox \n");
#endif
	result = CH_Security_PopupDialog("��ѡ��",szMsg,NULL,3,-1, 1);

	if(result == 1)
	{
		result = 0;
	}
	

	return result;

}

/*===================== ��ȡ�û�������Ϣ��ͼ�ν�����ʾ��غ��� ========================*/
/*
���ܣ���ͼ�ν������û������ʾ��Ϣ
		�û�ȷ���󣬴�ͼ�ν���Ӧ��ʧ��
���룺szTitle:	��ʾ���ڵı��⡣�硰�����ɹ�����"����ʧ��"��"���������У���ȴ�"��
�����szPromptMsg:	��ʾ���ݡ������ʧ��ԭ��� 

*/
/* �Ƿ�֧�ֳ�ʱʱ�䣬��ʱʱ����٣���ʾ�Ƿ���ø軪���еĶԻ����ʽ*/
int SumaSTBSecure_ShowPromptMsg(const char* szTitle, const char* szPromptMsg,int waitsecond)
{
#ifdef stb_scms_debug
	do_report(0,"SumaSTBSecure_ShowPromptMsg \n");
#endif
	CH_Security_PopupDialog(szTitle,szPromptMsg,"ȷ��",1,waitsecond, 1);
	return 0;

}

#endif
/*===================== ����&��ʾ��Ϣ��غ��� =======================*/
/*
	1.	�ӿڶ���
	void SumaSTBSecure_AddDebugMsg(const char * szMsg);
	2.	��������
	��������ֲʱ��ӡ������Ϣʹ�á���ֲ��Ϻ󣬸ú���ֱ�ӷ��ز����κδ���
	3.	����˵��
	�����������
	���������
		szMsg��������Ϣ��
	�� �أ�
	0��ʾ�ɹ���������ʾʧ�ܡ�
	4.	Ӧ�ó���
	�����м���ʱ��Ϊ����õ�����ĵ�����Ϣ�����Ե��ô˽ӿڡ�
*/
void SumaSTBSecure_AddDebugMsg(const char * szMsg, ... )
{
#ifdef _SUMA_DEBUG_
	va_list Argument;

	va_start( Argument, szMsg );
	vsprintf( suma_debug_buff, szMsg, Argument );
	va_end( Argument );
	do_report(0,"%s", suma_debug_buff );
	
#endif


}
#if 0
/*=====================UKeyʹ�õ���غ���===============================*/
/*
	1.	�ӿڶ���
		HANDLE SumaSTBSecure_OpenUKey(void);
	2.	��������
		��UKey�����UKey�ľ�������return NULL�����ʾû��UKEY�����򣬷���Ukey�ľ����
	3.	����˵����
			�����������
			�����������
		�� �أ�
			USBKey�����
	4.	Ӧ�ó���

*/
HANDLE SumaSecure_OpenUKey(void)
{


}
/*
	1.	�ӿڶ���
		int  SumaSTBSecure_WriteDataToUKey(HANDLE hUkey,BYTE * pbyinData,WORD *pwLen);
	2.	��������
		��ȡ��������ʾʱ�䡢������cpu����������������һ���ڴ�����ݡ�
	3.	����˵����
		���������
			hUkey��USBKey���
			pbyinData����д������ݵĻ�����
			pwLen��д������ݵĳ���		  
		���������
		pwLen��ʵ��д������ݵĳ���
		�� �أ�
			0��ʾ�ɹ���������ʾʧ�ܡ�
	4.	Ӧ�ó���
*/
int SumaSecure_WriteDataToUKey(HANDLE  hUkey,BYTE * pbyinData,WORD *pwLen)
{


}
/*
	1.	�ӿڶ���
		int SumaSTBSecure_ReadDataFromUKey(HANDLE hUkey,BYTE * pbyoutData,WORD *pwLen);
	2.	��������
		��USBKey�ж������ݡ�
	3.	����˵����
		���������
			hUkey��USBKey���
			pbyoutData���������ݵĻ�����
		���������
			pwLen��ʵ�ʶ��������ݵĳ���
		�� 	�أ�
			0��ʾ�ɹ���������ʾʧ�ܡ�
		4.	Ӧ�ó���

*/
int  SumaSecure_ReadDataFromUKey(HANDLE hUkey,BYTE * pbyoutData,WORD *pwLen)
{

}

/*=====================ͨѶ��غ���===============================*/
//�������ݵ�PDS������
// �����ɶ��˼
int SumaSTBSecure_SendDataToPDS(BYTE * pbySend,WORD wSendLen,BYTE * pbyRev,WORD *wpRevLen)
{

#define MAX_SEND_LEN 1460
	struct sockaddr* adr;
	struct sockaddr_in  so_ad;
	
	int socketindex;

	int err ;
	int ret;

	
	int ip ;/*Ҫ�������ݵ�IP��ַ*/
	int port = 8900;

	CH_iptoint("192.166.62.71",&ip);

	 socketindex = socket(AF_INET,SOCK_STREAM,IPPROTO_IP);
	
	memset(&so_ad,0,sizeof(struct sockaddr_in));

	if(socketindex == -1)
	{
		return -3;		
	}

	so_ad.sin_addr.s_addr = htonl(ip);
	so_ad.sin_port = htons(port);
	so_ad.sin_family = AF_INET;
	so_ad.sin_len    = sizeof(struct sockaddr_in);  

		/* ���� ������ */
	adr = (struct sockaddr *)&so_ad;

	err = connect(socketindex,adr,sizeof(struct sockaddr));

	if( err < 0)
	{
		return -3;
	}

	if(wSendLen <= MAX_SEND_LEN)
	{
		ret = send(socketindex,pbySend,wSendLen,0);
	}
	else
	{
		int pos=0;
		int i;
		int times = wSendLen/MAX_SEND_LEN;

			do_report(0,"SumaSTBSecure_SendDataToPDS Socket send len=%d tims=%d\n",wSendLen,times);

			for(i = 0;i < times;i++)
			{
				do_report(0,"pos= %d\n",pos);
				ret	= send(socketindex,(pbySend+pos),MAX_SEND_LEN,0);
				
				do_report(0,"send len %d\n",ret);
				
				if( ret  == MAX_SEND_LEN)/* ������������*/
				{
					pos += MAX_SEND_LEN;			
				}
				else 
				{
					return pos;
				}
				
			}

			if( (wSendLen%MAX_SEND_LEN)  !=  0)
			{

				int templen = wSendLen -times*MAX_SEND_LEN;
				ret = send(socketindex,(pbySend+pos),templen,0);
				if( ret  ==  templen)
				{
					pos += templen;
				}
				else
				{
					return pos;
				}
					
			}
			
		
	}

	ch_recvtimeout(socketindex, pbyRev, wpRevLen,0, 5);
	//recv(socketindex, pbyRev,wpRevLen, 0);

}

#endif


