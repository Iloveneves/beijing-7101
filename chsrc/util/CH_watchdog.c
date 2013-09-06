/*******************�ļ�˵��ע��************************************/
/*��˾��Ȩ˵������Ȩ��2006���鳤������Ƽ����޹�˾���С�           */
/*�ļ�����CH_watchdog.c                                              */
/*�汾�ţ�V1.0                                                     */
/*���ߣ�  �����                                                   */
/*�������ڣ�2006-08-08                                             */
/*ģ�鹦��:������Ź���λ                      */
/*��Ҫ����������:                                                  */
/*                                                                 */
/*�޸���ʷ���޸�ʱ��    ����      �޸Ĳ���       ԭ��              */

/*******************************************************************/

/**************** #include ����*************************************/
#include "appltype.h"
#include "..\main\initterm.h"
#include "stddefs.h"
#include "stcommon.h"
#include "string.h"
#include "stsys.h"	
#include "..\report\report.h"

void CH_EnableWatchDog(void);
/*******************************************************************/
/**************** #define ����**************************************/

#define SYstemBaseAddr                 0x20f00000
#define WATCHDOG_CFG0_OFFSET  0x130
#define WATCHDOG_CFG1_OFFSET  0x134
#define WATCHDOG_RESET_OFFSET 0x140
#define REGISTER_LOCK_OFFSET      0x300
 /*[31:5]reserved,[4]WD_COUNT_EN:watchdog counter enable [3:0] the value and WATCHDOG_CFG0 value combine to maximum 1048576 seconds*/


typedef volatile unsigned int system_device;




/*******************************************************************/

/***************  ȫ�ֱ������岿�� *********************************/
semaphore_t *pWatchAlamSema;



typedef void (*APP_SetEntry)(void);
/******************************************************************/
/*������:void CH_LongJmp(unsigned long int rAdress)               */
/*�����˺Ϳ���ʱ��:zengxianggen 2007-05-15                        */
/*������������:��ת��ָ������λ��                                 */
/*����ԭ��˵��:                                                   */
/*�����������                                                    */
/*�������: ��                                                    */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                      */
/*���õ���Ҫ�����б�                                            */
/*����ֵ˵������                               */   
/*����ע������:                                                   */
/*����˵��:                                                       */    
void CH_LongJmp(unsigned long int rAdress)
{
#if 0
APP_SetEntry AppJmp;
AppJmp=rAdress;
AppJmp();
#else/*ͨ���Ƿ�����ʵ�ֿ��Ź���������*/
int *pi_Error=NULL;
*pi_Error=0xFFFF;
#endif
}

void CH_WatchSemaInit(void)
{
  	pWatchAlamSema=semaphore_create_fifo( 1);	
}
/*******************************************************************/
/************************����˵��***********************************/
/*������:void CH_SetWatchDog(void)            */
/*�����˺Ϳ���ʱ��:zengxianggen 2006-08-08                         */
/*������������:���ÿ��Ź�����                                      */
/*����ԭ��˵��:                                                    */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                       */
/*���õ���Ҫ�����б�                                             */
/*����ֵ˵������                                                   */
void CH_SetWatchDog(void)
{
	semaphore_wait(pWatchAlamSema);


	*(volatile unsigned long*) (SYstemBaseAddr+REGISTER_LOCK_OFFSET)=0x0F;
	*(volatile unsigned long*) (SYstemBaseAddr+REGISTER_LOCK_OFFSET)=0xF0;
	  
	*(volatile unsigned long*) (SYstemBaseAddr+WATCHDOG_CFG0_OFFSET) = 0x0f/*15��*/;
	*(volatile unsigned long*) (SYstemBaseAddr+WATCHDOG_CFG1_OFFSET) = 0x00;
	
	CH_EnableWatchDog();/*�򿪿��Ź�����*/
	semaphore_signal(pWatchAlamSema);
	return ;

}

/*������:void CH_EnableWatchDog(void)            */
/*�����˺Ϳ���ʱ��:zengxianggen 2006-08-08                         */
/*������������:�򿪿��Ź�����                                        */
/*����ԭ��˵��:                                                    */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                       */
/*���õ���Ҫ�����б�                                             */
/*����ֵ˵������                                                   */
void CH_EnableWatchDog(void)
{
      volatile unsigned long ResetSetting;
	*(volatile unsigned long*) (SYstemBaseAddr+WATCHDOG_CFG1_OFFSET)= 0x10;
#if 0	
	/*��ȡ��ǰ��RESET����*/
	ResetSetting=*(volatile unsigned long*) (SYstemBaseAddr+WATCHDOG_RESET_OFFSET);
	/*���ÿ��Ź�����*/
	ResetSetting=ResetSetting|0x01;
#endif	
	*(volatile unsigned long*) (SYstemBaseAddr+WATCHDOG_RESET_OFFSET)= 0x181;
	return ;

}


void CH_ReadWatchDog(void)
{
	#if 1
	volatile unsigned long data;
	data = *(volatile unsigned long*) (SYstemBaseAddr+WATCHDOG_CFG0_OFFSET);
	do_report(0,"data0=%x \n",data);
	data = *(volatile unsigned long*) (SYstemBaseAddr+WATCHDOG_CFG1_OFFSET);
	do_report(0,"data1=%x \n",data);
	data = *(volatile unsigned long*) (SYstemBaseAddr+WATCHDOG_RESET_OFFSET);
	do_report(0,"data2=%x \n",data);
	
	return;
	#endif
}


/*������:static void WatchDogProcess(void *pvParam)        */
/*�����˺Ϳ���ʱ��:zengxianggen 2006-08-08                         */
/*������������:���Ź���ؽ���                                   */
/*����ԭ��˵��:                                                    */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                       */
/*���õ���Ҫ�����б�                                             */
/*����ֵ˵������                                                   */
static void WatchDogProcess(void *pvParam)
{
	
	
	while(true)
	{
		CH_SetWatchDog();
		task_delay(ST_GetClocksPerSecond());
		CH_ReadWatchDog();  
	}
}
/*������:BOOL CH_WatchDogInit(void)                               */
/*�����˺Ϳ���ʱ��:zengxianggen 2006-08-08                         */
/*������������:���Ź���ؽ���                                      */
/*����ԭ��˵��:                                                    */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                       */
/*���õ���Ҫ�����б�                                             */
/*����ֵ˵������                                                   */
BOOL CH_WatchDogInit(void)
{
	task_t *ptidWatchDogTask;
	if ((ptidWatchDogTask = Task_Create(WatchDogProcess,NULL,512 ,5,
		"WatchDogTask",0))==NULL)
    {
		
		STTBX_Print(("Failed to start WatchDog task\n"));
		return TRUE;
    }
	else 
	{
		STTBX_Print(("creat WatchDog task successfully \n"));
		return FALSE;
	}

}


/*������:void CH_Restart(void)                               */
/*�����˺Ϳ���ʱ��:zengxianggen 2006-08-08                         */
/*������������:��������                                      */
/*����ԭ��˵��:                                                    */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                       */
/*���õ���Ҫ�����б�                                             */
/*����ֵ˵������                                                   */

void CH_Restart(void)   
{   
#if 0

     task_lock();
	interrupt_lock();
	semaphore_wait(pWatchAlamSema);

     
	*(volatile unsigned long*) (SYstemBaseAddr+REGISTER_LOCK_OFFSET)=0x0F;
	*(volatile unsigned long*) (SYstemBaseAddr+REGISTER_LOCK_OFFSET)=0xF0;

	*(volatile unsigned long*) (SYstemBaseAddr+WATCHDOG_CFG0_OFFSET) = 0x02/*2��*/;
	*(volatile unsigned long*) (SYstemBaseAddr+WATCHDOG_CFG1_OFFSET) = 0x00;
	CH_EnableWatchDog();/*�򿪿��Ź�����*/
	
	semaphore_signal(pWatchAlamSema);

	task_delay(10000);
	
	interrupt_unlock();
	task_unlock();
#else
   
   /* kernel_timeslice(OS21_FALSE);
    task_priority_set(NULL, MAX_USER_PRIORITY);
    task_lock();
    interrupt_lock();*/
    while(true)
       CH_LongJmp(0xA0000000);
  /*  interrupt_unlock();
    task_unlock();*/
#endif

}
/*++++++++++++++++++++++++++++++++++++++*/





