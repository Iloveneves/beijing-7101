/*******************************************************************************
* �ļ����� : CHMID_TCPIP_api.c											
* �������� : ʵ��HMID_TCPIPģ���ṩ���ϲ�Ӧ�õ��õĹ��ܽӿں�����
*
* ����:flex
* �޸���ʷ:
* ����					����						����
* 2009/3/30				flex							create
*
********************************************************************************/

/*---------------------------------type--------------------------------*/
#ifndef __CHDRV_NET_H__
#define __CHDRV_NET_H__
#include <stdio.h>
#include "stdlib.h"

#include "stapp_os.h"

/*****************************************************************************
* The following types are used in the SMSC Network stack
* u8_t is an unsigned 8 bit integer
* s8_t is a signed 8 bit integer
* u16_t is an unsigned 16 bit integer
* s16_t is a signed 16 bit integer
* u32_t is an unsigned 32 bit integer
* s32_t is a signed 32 bit integer
* mem_ptr_t is an unsigned integer who bit width is the same as a pointer (void *)
* NOTE:
*   mem_size_t is not currently used and may be deprecated in the future.
*****************************************************************************/
typedef unsigned   char    u8_t;
typedef signed     char    s8_t;
typedef unsigned   short   u16_t;
typedef signed     short   s16_t;
typedef unsigned   int    u32_t;
typedef signed     int    s32_t;

typedef u32_t mem_ptr_t;
typedef u16_t mem_size_t;

#ifndef boolean 
#define  boolean unsigned   char 
#endif

#ifndef true 
#define true 1
#endif
#ifndef false 
#define false 0
#endif

#ifdef NULL
#undef NULL
#endif
#define NULL 0UL

#ifdef CHDRV_NET_TIMEOUT_INFINITY
#undef CHDRV_NET_TIMEOUT_INFINITY
#endif
#define CHDRV_NET_TIMEOUT_INFINITY 0xFFFFFFFFUL

#ifdef CHDRV_NET_TIMEOUT_IMMEDIATE
#undef CHDRV_NET_TIMEOUT_IMMEDIATE
#endif
#define CHDRV_NET_TIMEOUT_IMMEDIATE 0x0UL

/*test typedef*/
/*--------------------------------start-----------------------------*/
typedef semaphore_t * CH_RTOS_SemaphoreHandler;
typedef task_t * CH_RTOS_TaskHadler;
typedef message_queue_t * CH_RTOS_MessageQueue_t;
/*--------------------------------end-----------------------------*/

typedef  CH_RTOS_SemaphoreHandler CHDRV_NET_SEMAPHORE;
typedef CH_RTOS_TaskHadler CHDRV_NET_THREAD;
typedef CH_RTOS_MessageQueue_t CHDRV_NET_MESSAGEQUEUE;
typedef void (*CHDRV_NET_InterruptHandler)(void *rvp_Arg);
typedef u32_t   CHDRV_NET_Handle_t;
typedef  void  (*CHDRV_NET_CallbackFun_t)(void *rvp_Arg);

typedef enum
{
	AUTO_DHCP,
	MANUAL
}IP_GET_MODEL;

typedef enum
{
	CM_PORT_MODEL,/*cable modem*/
	RJ45_PORT_MODEL/*RJ45 */
}DHCP_PORT_MODEL;

#define IP_STRING_LENGTH 4
#define MAC_ADDRESS_LENGTH 6
#define DHCP_OPTION_60_LEN 	30
#define HOST_NAME_LEN		20
typedef struct tagNetConfig
{
	u8_t macaddress[MAC_ADDRESS_LENGTH];			/*MAC Address*/
	u8_t ipaddress[IP_STRING_LENGTH];					/*IP Address*/
	u8_t netmask[IP_STRING_LENGTH];					/*Netmask*/
	u8_t gateway[IP_STRING_LENGTH];					/*Gateway Address*/
	u8_t dns[IP_STRING_LENGTH];						/*DNS Address*/
	IP_GET_MODEL ipmodel;							/*Get IP mode*/
	DHCP_PORT_MODEL dhcpport;						/*Get IP port,RJ43 OR CM*/
	s8_t Opt60ServerName[DHCP_OPTION_60_LEN];		/*Option60 server name*/
	s8_t Opt60ClientName[DHCP_OPTION_60_LEN];		/*Option60 client name*/
	s8_t hostname[HOST_NAME_LEN];					/*Local host name*/
	u8_t vod[IP_STRING_LENGTH];						/*VOD server address*/
	u8_t http[IP_STRING_LENGTH];						/*Reserved*/
}CHDRV_NET_Config_t;

/*Return result*/
typedef enum
{
	CHDRV_NET_OK = 0,					/*	�����ɹ�*/
	CHDRV_NET_FAIL = -1,				/*	����ʧ��*/
	CHDRV_NET_NOT_INIT = -2,			/*	ģ��û�г�ʼ��*/
	CHDRV_NET_ALREADY_INIT = -3,		/*   ģ���Ѿ���ʼ��*/
	CHDRV_NET_TIMEOUT = -4,			/*	��ʱ*/
	CHDRV_NET_INVALID_PARAM = -5	/*	��������*/
}CHDRV_NET_RESULT_e;

/*PHY mode*/
typedef enum 
{
	CHDRV_NET_PHY_MODE_10MHD,	/*10M��˫��	*/
	CHDRV_NET_PHY_MODE_100MHD,/*100M��˫��*/	
	CHDRV_NET_PHY_MODE_10MFD,	/*10Mȫ˫��	*/
	CHDRV_NET_PHY_MODE_100MFD,	/*100Mȫ˫��*/
	CHDRV_NET_PHY_MODE_AUTO	/*����Ӧ	*/
}CHDRV_NET_PHY_MODE_e;

/*Version*/
typedef struct
{
	s32_t i_InterfaceMainVer;/*Ϊ�ӿ��ĵ������汾��*/
	s32_t i_InterfaceSubVer;	/*Ϊ�ӿ��ĵ��Ĵΰ汾��*/
	s32_t i_ModuleMainVer;	/*Ϊģ��ʵ�ֵ����汾*/
	s32_t i_ModuleSubVer;	/*Ϊģ��ʵ�ֵĴΰ汾*/
}CHDRV_NET_Version_t;

typedef osclock_t CHDRV_NET_CLOCK_t;

/*-------------------------------DEFINE START-------------------------------------*/
/*netif define*/
#define NET_INTERFACE_NUM	1
#define NET_INTERFACE_NAME_LEN	20

/*print contrl*/
#define CHDRV_NET_TRACE_ENABLE		1
#define CHDRV_NET_NOTICE_ENABLE		1
#define CHDRV_NET_WARNING_ENABLE		1
#define CHDRV_NET_ASSERT_ENABLE		1
#define CHDRV_NET_ERROR_ENABLE		1
/*debug contrl*/
#define CHDRV_NET_PLATFORM_DEBUG	1
#define CHDRV_NET_DRIVER_DEBUG		1

#if (CHDRV_NET_TRACE_ENABLE)
#define CHDRV_NET_TRACE(condition,message)	do{	\
	if(condition){	\
		CHDRV_NET_Print("CHDRV_NET_TRACE:");	\
		CHDRV_NET_Print message ;		\
		CHDRV_NET_Print("\n");	\
	}	\
}while(0)
#else
#define CHDRV_NET_TRACE(condition,message) {}
#endif

#if (CHDRV_NET_NOTICE_ENABLE)
#define CHDRV_NET_NOTICE(condition,message)	do{	\
	if(condition){	\
		CHDRV_NET_Print("CHDRV_NET_NOTICE:");	\
		CHDRV_NET_Print message;		\
		CHDRV_NET_Print("\n");	\
	}	\
}while(0)
#else
#define CHDRV_NET_NOTICE(condition,message)	{}
#endif

#if (CHDRV_NET_WARNING_ENABLE)
#define CHDRV_NET_WARNING(condition,message)	do{	\
	if(condition){	\
		CHDRV_NET_Print("CHDRV_NET_WARNING:");	\
		CHDRV_NET_Print message;		\
		CHDRV_NET_Print("\n");	\
	}	\
}while(0)
#else
#define CHDRV_NET_WARNING(condition,message)	{}
#endif

#if (CHDRV_NET_ERROR_ENABLE)
#define CHDRV_NET_ERROR(condition,message)	do{	\
	if(condition){	\
		CHDRV_NET_Print("CHDRV_NET_ERROR:");	\
		CHDRV_NET_Print message;		\
		CHDRV_NET_Print("\n file=%s,line=%d\n",__FILE__,__LINE__);\
	}	\
}while(0)
#else
#define CHDRV_NET_ERROR(condition,message)	{}
#endif

#if (CHDRV_NET_ASSERT_ENABLE)
#define CHDRV_NET_ASSERT(condition)	do{	\
	if(!(condition)){							\
		CHDRV_NET_Print("CHDRV_NET:");	\
		CHDRV_NET_Print("ASSERT FAILURE at file=%s,line=%d\n",__FILE__,__LINE__);	\
	}	\
}while(0)
#else
#define CHDRV_NET_ASSERT(condition)	{}
#endif
/*-------------------------------DEFINE END-------------------------------------*/
/*-------------------------------OS----------------------------------*/

/*--------------------------------------------------------------------------*/
/* �������� : CHDRV_NET_ThreadCreate								*/
/* �������� : ����һ������							*/
/* ��ڲ��� : 											*/
/* *rpf_function,�̺߳���ָ��			*/
/* *rpv_Arg,�̺߳���ָ�����				*/
/*  rui_StackSize,�̶߳�ջֵ						*/
/*  rsi_Priority	,�߳����ȼ�				*/
/*  rsc_Name,�߳���			*/
/*  ruc_Flage,	��־��				*/
/* ���ڲ��� : ��											*/
/* ����ֵ: �ɹ���������ָ��,ʧ�ܷ���NULL					*/
/* ע��:								*/
/*--------------------------------------------------------------------------*/
CHDRV_NET_THREAD CHDRV_NET_ThreadCreate(void (*rpf_function) (void *rpv_Arg),void *rpv_Arg, u32_t rui_StackSize,s32_t rsi_Priority,s8_t *rsc_Name,u8_t ruc_Flags);
/*--------------------------------------------------------------------------*/
/* �������� : CHDRV_NET_ThreadResume								*/
/* �������� : �ָ�һ������Ϊ����״̬							*/
/* ��ڲ��� : 											*/
/* *rh_Task,������			*/
/* ���ڲ��� : ��											*/
/*����ֵ˵�����ɹ�����CHDRV_NET_OK*/
/* ע��:	��CHDRV_NET_ThreadSuspend֮�����							*/
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e CHDRV_NET_ThreadResume(CHDRV_NET_THREAD	rh_Task);
/*--------------------------------------------------------------------------*/
/* �������� : CHDRV_NET_ThreadSuspend								*/
/* �������� : ����һ������							*/
/* ��ڲ��� : 											*/
/* *rh_Task,������			*/
/* ���ڲ��� : ��											*/
/*����ֵ˵�����ɹ�����CHDRV_NET_OK*/
/* ע��:								*/
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e  CHDRV_NET_ThreadSuspend(CHDRV_NET_THREAD rh_Task);
/*--------------------------------------------------------------------------*/
/* �������� : CHDRV_NET_ThreadSetPriority								*/
/* �������� : ����һ����������ȼ�							*/
/* ��ڲ��� : 											*/
/* *rh_Task,������			*/
/* ri_Priority,���ȼ�				*/
/* ���ڲ��� : ��											*/
/*����ֵ˵�����ɹ�����CHDRV_NET_OK*/
/* ע��:								*/
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e  CHDRV_NET_ThreadSetPriority(CHDRV_NET_THREAD	rh_Task,s32_t ri_Priority);
/*--------------------------------------------------------------------------*/
/* �������� : CHDRV_NET_ThreadDelay								*/
/* �������� : ��ʱ����							*/
/* ��ڲ��� : 											*/
/* ri_DelayTimeMS,ʱ��(ms)			*/
/* ���ڲ��� : ��											*/
/* ����ֵ: ��				*/
/* ע��:								*/
/*--------------------------------------------------------------------------*/
extern unsigned int ST_GetClocksPerSecond( void );
#define CHDRV_NET_ThreadDelay(ri_DelayTimeMS) task_delay(ST_GetClocksPerSecond()/1000*ri_DelayTimeMS)
/*--------------------------------------------------------------------------*/
/* �������� : CHDRV_NET_ThreadLock								*/
/* �������� : �������						*/
/* ��ڲ��� : ��											*/
/* ���ڲ��� : ��											*/
/*����ֵ˵�����ɹ�����CHDRV_NET_OK*/
/* ע��:	������CHDRV_NET_ThreadUnLock����ʹ��				*/
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e  CHDRV_NET_ThreadLock (void);
/*--------------------------------------------------------------------------*/
/* �������� : CHDRV_NET_ThreadUnLock								*/
/* �������� : �������						*/
/* ��ڲ��� : ��											*/
/* ���ڲ��� : ��											*/
/*����ֵ˵�����ɹ�����CHDRV_NET_OK*/
/* ע��:	������CHDRV_NET_ThreadLock����ʹ��				*/
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e CHDRV_NET_ThreadUnLock (void);
/*--------------------------------------------------------------------------*/
/* �������� : CHDRV_NET_SemaphoreCreate								*/
/* �������� : ����һ���ź���					*/
/* ��ڲ��� : 											*/
/* ri_Count,�ź�����ʼֵ			*/
/* ���ڲ��� : */
/* rph_Semaphore,�ź���ָ��*/
/*����ֵ˵�����ɹ�����CHDRV_NET_OK*/
/* ע��:								*/
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e CHDRV_NET_SemaphoreCreate(s32_t 	ri_Count,CHDRV_NET_SEMAPHORE *rph_Semaphore);
/*--------------------------------------------------------------------------*/
/* �������� : CHDRV_NET_SemaphoreWait								*/
/* �������� : �ź����ȴ�							*/
/* ��ڲ��� : 											*/
/* rh_Semaphore,�ź������			*/
/* ri_TimeOut,��ʱʱ��(ms),-1Ϊ���õȴ�			*/
/* ���ڲ��� : ��											*/
/*����ֵ˵�����ɹ�����CHDRV_NET_OK*/
/* ע��:								*/
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e CHDRV_NET_SemaphoreWait(CHDRV_NET_SEMAPHORE rh_Semaphore,s32_t ri_TimeOut);
/*--------------------------------------------------------------------------*/
/* �������� : CHDRV_NET_SemaphoreSignal								*/
/* �������� : �ź����ͷ�								*/
/* ��ڲ��� : 											*/
/* rh_Semaphore,�ź������			*/
/* ���ڲ��� : ��											*/
/*����ֵ˵�����ɹ�����CHDRV_NET_OK*/
/* ע��:								*/
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e  CHDRV_NET_SemaphoreSignal(CHDRV_NET_SEMAPHORE	 rh_Semaphore);
/*--------------------------------------------------------------------------*/
/* �������� : CHDRV_NET_SemaphoreFree								*/
/* �������� : �ź�������							*/
/* ��ڲ��� : 											*/
/* rh_Semaphore,�ź������			*/
/* ���ڲ��� : ��											*/
/*����ֵ˵�����ɹ�����CHDRV_NET_OK			*/
/* ע��:								*/
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e CHDRV_NET_SemaphoreFree(CHDRV_NET_SEMAPHORE rh_Semaphore);
/*--------------------------------------------------------------------------*/
/* �������� : CHDRV_NET_CreateMessageQueue								*/
/* �������� : ������Ϣ����						*/
/* ��ڲ��� : 											*/
/* ui_MsgLen,��ϢԪ�س���			*/
/* ui_QueueLen,���г���			*/
/* ���ڲ��� : ��											*/
/*����ֵ˵�����ɹ�������Ϣ���о��,���򷵻�NULL			*/
/* ע��:								*/
/*--------------------------------------------------------------------------*/
CHDRV_NET_MESSAGEQUEUE CHDRV_NET_CreateMessageQueue(u32_t ui_MsgLen,u32_t ui_QueueLen);
/*--------------------------------------------------------------------------*/
/* �������� : CHDRV_NET_ClaimMessageTimeout								*/
/* �������� : ����Ϣ����������һ����ϢԪ��						*/
/* ��ڲ��� : 											*/
/* MsgQueue,��Ϣ���о��		*/
/* ui_TimeMs,���볬ʱMs		*/
/* ���ڲ��� : ��											*/
/*����ֵ˵�����ɹ�����һ��������һ����Ϣ��ָ��,���򷵻�NULL			*/
/* ע��:								*/
/*--------------------------------------------------------------------------*/
void * CHDRV_NET_ClaimMessageTimeout(CHDRV_NET_MESSAGEQUEUE MsgQueue,u32_t ui_TimeMs);
/*--------------------------------------------------------------------------*/
/* �������� : CHDRV_NET_ClaimMessageTimeout								*/
/* �������� : ������Ϣ������һ����ϢԪ��						*/
/* ��ڲ��� : 											*/
/* MsgQueue,��Ϣ���о��		*/
/* up_Msg,��ϢԪ�ؾ��		*/
/* ���ڲ��� : ��											*/
/*����ֵ˵������CHDRV_NET_RESULT_e����				*/
/* ע��:								*/
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e CHDRV_NET_MessageRelease(CHDRV_NET_MESSAGEQUEUE MsgQueue,void * up_Msg);
/*--------------------------------------------------------------------------*/
/* �������� : CHDRV_NET_ClaimMessageTimeout								*/
/* �������� : ������Ϣ������һ����ϢԪ��						*/
/* ��ڲ��� : 											*/
/* MsgQueue,��Ϣ���о��		*/
/* up_Msg,��ϢԪ�ؾ��		*/
/* ���ڲ��� : ��											*/
/*����ֵ˵������CHDRV_NET_RESULT_e����				*/
/* ע��:								*/
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e CHDRV_NET_MessageSend(CHDRV_NET_MESSAGEQUEUE MsgQueue,void * up_Msg);
/*--------------------------------------------------------------------------*/
/* �������� : CHDRV_NET_ClaimMessageTimeout								*/
/* �������� : ������Ϣ						*/
/* ��ڲ��� : 											*/
/* MsgQueue,��Ϣ���о��		*/
/* ui_TimeMs,���ճ�ʱms	*/
/* ���ڲ��� : ��											*/
/*����ֵ˵�����ɹ�����һ��������һ����Ϣ��ָ��,���򷵻�NULL			*/
/* ע��:								*/
/*--------------------------------------------------------------------------*/
void * CHDRV_NET_MessageReceive(CHDRV_NET_MESSAGEQUEUE MsgQueue,u32_t ui_TimeMs);

/*--------------------------------------------------------------------------*/
/* �������� : CHDRV_NET_MemAlloc								*/
/* �������� : �����ڴ�							*/
/* ��ڲ��� : 											*/
/* rui_Size,�ڴ泤��		*/
/* ���ڲ��� : ��											*/
/*����ֵ˵�����ɹ������ڴ�ָ�룬���򷵻�NULL			*/
/* ע��:								*/
/*--------------------------------------------------------------------------*/
void *CHDRV_NET_MemAlloc(u32_t rui_Size);
/*--------------------------------------------------------------------------*/
/* �������� : CHDRV_NET_MemFree								*/
/* �������� : �ͷ��ڴ�						*/
/* ��ڲ��� : 											*/
/* rp_MemHandle,�ڴ�ָ��			*/
/* ���ڲ��� : ��											*/
/*����ֵ˵������			*/
/* ע��:								*/
/*--------------------------------------------------------------------------*/
void CHDRV_NET_MemFree(void *rp_MemHandle);

/*-------------------------------PLATFORM---------------------------------------*/

/*--------------------------------------------------------------------------*/
/* �������� : CHDRV_NET_Print								*/
/* �������� : ��ӡ����						*/
/* ��ڲ��� : 											*/
/* rsc_ContentStr,���ӡ���ַ���			*/
/* ���ڲ��� : ��											*/
/*����ֵ˵������		*/
/* ע��:								*/
/*--------------------------------------------------------------------------*/
void CHDRV_NET_Print(const char *rsc_ContentStr,...);
/*--------------------------------------------------------------------------*/
/* �������� : CHDRV_NET_Chksum								*/
/* �������� : checksum���㺯��						*/
/* ��ڲ��� : 											*/
/* rvp_Data,����ָ��			*/
/* rvp_Data,���ݳ���(�ֽ���)			*/
/* ���ڲ��� : ��											*/
/*����ֵ˵����������		*/
/* ע��:								*/
/*--------------------------------------------------------------------------*/
u16_t CHDRV_NET_Chksum (void *rvp_Data, s32_t rsi_Len);
/*--------------------------------------------------------------------------*/
/* �������� : CHDRV_NET_LoadNetConfig								*/
/* �������� : ��STB FLASH�ж�ȡ����������Ϣ						*/
/* ��ڲ��� : ��										*/
/* ���ڲ��� : */
/* Config,������Ϣ�ṹ��*/
/*����ֵ˵�����ɹ�����CHDRV_NET_OK			*/
/* ע��:								*/
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e CHDRV_NET_LoadNetConfig(CHDRV_NET_Config_t * rph_NetConfig);
/*--------------------------------------------------------------------------*/
/* �������� : CHDRV_NET_LoadNetConfig								*/
/* �������� : ��������������Ϣ��������						*/
/* ��ڲ��� : ��										*/
/* ���ڲ��� : */
/* Config,������Ϣ�ṹ��*/
/*����ֵ˵�����ɹ�����CHDRV_NET_OK			*/
/* ע��:								*/
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e CHDRV_NET_SaveNetConfig(CHDRV_NET_Config_t * rph_NetConfig);
/*--------------------------------------------------------------------------*/
/* �������� : CHDRV_NET_TimeNow								*/
/* �������� : �õ���ǰϵͳʱ��						*/
/* ��ڲ��� : ��										*/
/* ���ڲ��� : ��									*/
/*����ֵ˵����ϵͳ��ǰʱ��(tick)			*/
/* ע��:								*/
/*--------------------------------------------------------------------------*/
#define CHDRV_NET_TimeNow() 			time_now()
/*--------------------------------------------------------------------------*/
/* �������� : CHDRV_NET_TimePlus								*/
/* �������� : ϵͳʱ�Ӻͼ���						*/
/* ��ڲ��� : 	*/
/* t1,ʱ��1*/
/* t2,ʱ��2*/
/* ���ڲ��� : ��									*/
/*����ֵ˵���������(t1+t2)		*/
/* ע��:								*/
/*--------------------------------------------------------------------------*/
#define CHDRV_NET_TimePlus(t1,t2) 		time_plus(t1,t2)
/*--------------------------------------------------------------------------*/
/* �������� : CHDRV_NET_TimeAfter								*/
/* �������� : ϵͳʱ�ӱȽ�						*/
/* ��ڲ��� : ��	*/
/* t1,ʱ��1*/
/* t2,ʱ��2*/
/* ���ڲ��� : ��									*/
/*����ֵ˵����(t1>t2)?1:0			*/
/* ע��:								*/
/*--------------------------------------------------------------------------*/
#define CHDRV_NET_TimeAfter(t1,t2) 		time_after(t1,t2)
/*--------------------------------------------------------------------------*/
/* �������� : CHDRV_NET_TimeMinus								*/
/* �������� : ϵͳʱ�Ӳ����				*/
/* ��ڲ��� : ��	*/
/* t1,ʱ��1*/
/* t2,ʱ��2*/
/* ���ڲ��� : ��									*/
/*����ֵ˵���������(t1-t2)		*/
/* ע��:								*/
/*--------------------------------------------------------------------------*/
#define CHDRV_NET_TimeMinus(t1,t2) 		time_minus(t1,t2)
/*--------------------------------------------------------------------------*/
/* �������� : CHDRV_NET_TicksOfSecond								*/
/* �������� : ����1����ϵͳʱ�ӵ�tick��						*/
/* ��ڲ��� : ��	*/
/* ���ڲ��� : ��									*/
/*����ֵ˵����ϵͳ1���ӵ�ʱ��(tick)��			*/
/* ע��:								*/
/*--------------------------------------------------------------------------*/
#define CHDRV_NET_TicksOfSecond() 		ST_GetClocksPerSecond()
/*--------------------------------------------------------------------------*/
/* �������� : CHDRV_NET_InterruptInstall								*/
/* �������� : �����ϵͳע�������жϺ���					*/
/* ��ڲ��� : ��	*/
/* ���ڲ��� : int_func,�жϺ���ָ��,arg,�жϺ�������									*/
/*����ֵ˵������CHDRV_NET_RESULT_e����			*/
/* ע��:								*/
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e CHDRV_NET_InterruptInstall(CHDRV_NET_InterruptHandler * int_func,void * arg);
/*--------------------------------------------------------------------------*/
/* �������� : CHDRV_NET_InterruptUninstall								*/
/* �������� : ע�������жϺ���					*/
/* ��ڲ��� : ��	*/
/* ���ڲ��� : ��									*/
/*����ֵ˵������CHDRV_NET_RESULT_e����			*/
/* ע��:								*/
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e CHDRV_NET_InterruptUninstall(void);

/*-------------------------------DRIVER---------------------------------------*/
/*--------------------------------------------------------------------------*/
/* �������� : CHDRV_NET_Init								*/
/* �������� : �����豸��ʼ��								*/
/* ��ڲ��� : 											*/
/* rpfun_CallBack,�ص�����ָ�룬��ʱ���ã����Ժ���չʹ��	*/
/* rph_NetDevice,�豸���									*/
/* ruc_MacAddr ,MAC��ַ									*/
/* ���ڲ��� : ��											*/
/* ����ֵ: �ɹ�����CHDRV_NET_OK,����ʧ��					*/
/* ע��:	�˺���һ�㲻�ظ�����							*/
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e CHDRV_NET_Init(void);/**/
/*--------------------------------------------------------------------------*/
/*������:CHDRV_TCPIP_Handle_t *CHDRV_TCPIP_OpenEthenet ( )    */
/*�����˺Ϳ���ʱ��:flex 2009-04-14                        */
/*������������:��һ����̫��������                               */
/*���������	*/
/*ri_NetifIndex:������������ */
/*�������:                                                     */
/*rph_NetDevice:������������*/
/*����ֵ˵����CHDRV_NET_OK��ʾ�����ɹ�,������ʾ����ʧ��*/   
/*����ע������:                                                   */
/*����˵��:                                                       */  
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e CHDRV_NET_Open(s32_t ri_NetifIndex, CHDRV_NET_Handle_t * rph_NetDevice);
/*--------------------------------------------------------------------------*/
/*������:CHDRV_NET_Close */
/*�����˺Ϳ���ʱ��:flex 2009-04-14                        */
/*������������:�ر�һ����̫��������                               */
/*���������*/
/*rph_NetDevice:�����豸���		*/
/*�������: ��                                                    */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                      */
/*���õ���Ҫ�����б�                                            */
/*����ֵ˵����CHDRV_NET_OK��ʾ�����ɹ�                   */   
/*����ע������:                                                   */
/*����˵��:                                                       */  
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e CHDRV_NET_Close( CHDRV_NET_Handle_t rph_NetDevice);
/*--------------------------------------------------------------------------*/
/* �������� : CHDRV_NET_Recv									*/
/* �������� : ��ȡ��ǰ��Ҫ�����յ����ݰ�����*/
/* ��ڲ��� :						 */
/*  rh_NetDevice,�豸���				*/
/* rpuc_Buffer,���ݻ���ָ��			*/
/* rpi_Length,���ݻ��峤��				*/
/* ���ڲ��� :										 */
/*  rpi_Length,�յ������ݳ���					*/
/* ����ֵ:	�ɹ�����CHDRV_NET_OK*/
/* ע��:			*/
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e  CHDRV_NET_Recv(CHDRV_NET_Handle_t rh_NetDevice,u8_t *rpuc_Buffer,u16_t *rpi_Length);
/*--------------------------------------------------------------------------*/
/* �������� : CHDRV_NET_Send											*/
/* �������� : ����һ�����ݰ�*/
/* ��ڲ��� : 									*/
/* rh_NetDevice,�豸���					*/
/* rpuc_Data,����ָ��				*/
/* rpi_Length,���ݳ���				*/
/* ���ڲ��� : ��																*/
/* ����ֵ:	�ɹ�����CHDRV_NET_OK				*/
/* ע��:					*/
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e  CHDRV_NET_Send(CHDRV_NET_Handle_t rh_NetDevice,u8_t *rpuc_Data,u16_t *rpi_Length);
/*--------------------------------------------------------------------------*/
/*������:CHDRV_NET_GetVersion( )                     */
/*�����˺Ϳ���ʱ��:zengxianggen 2007-08-10                        */
/*������������:�õ�CHDRV_NET�汾��*/
/*���������  ��*/
/*�������:     rpstru_Ver:�汾��Ϣ*/                                              
/*����ֵ˵�����ɹ�����CHDRV_NET_OK*/
/*����ע������:                                                   */
/*����˵��:                                                       */  
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e CHDRV_NET_GetVersion(CHDRV_NET_Version_t *rpstru_Ver);
/*--------------------------------------------------------------------------*/
/*������:CHDRV_NET_Reset*/
/*�����˺Ϳ���ʱ��:zengxianggen     2006-06-01                    */
/*������������:  ��λethenet оƬ                             */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                       */
/*����:	*/
/* rh_NetDevice,�豸���*/
/*����ֵ˵�����ɹ�����CHDRV_NET_OK*/
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e CHDRV_NET_Reset(CHDRV_NET_Handle_t rh_NetDevice);
/*--------------------------------------------------------------------------*/
/* �������� : CHDRV_NET_GetPacketLength									*/
/* �������� : ��ȡ��ǰ��Ҫ�����յ����ݰ�����*/
/* ��ڲ��� : ��						*/
/* ���ڲ��� : ��																*/
/* ����ֵ:	����0����				*/
/* ע��:			*/
/*--------------------------------------------------------------------------*/
u16_t CHDRV_NET_GetPacketLength (CHDRV_NET_Handle_t rph_NetDevice);
/*--------------------------------------------------------------------------*/
/*������:void CHDRV_NET_SetMacAddr(void)*/
/*�����˺Ϳ���ʱ��:flex     2009-04-13                   */
/*������������: ���������豸MAC��ַ                            */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                       */
/*����:	*/
/* rh_NetDevice,�豸���*/
/* rpuc_MacAddr,MAC��ַ*/
/*����ֵ˵�����ɹ�����CHDRV_NET_OK*/
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e CHDRV_NET_SetMacAddr(CHDRV_NET_Handle_t rh_NetDevice,u8_t *rpuc_MacAddr);
/*--------------------------------------------------------------------------*/
/*������:CHDRV_NET_GetMacAddr( )                     */
/*�����˺Ϳ���ʱ��:zengxianggen 2007-08-10                        */
/*������������:�õ�оƬ�������õ�MAC��ַ*/
/*���������  rh_NetDevice,�豸���*/
/*�������:     rpuc_MacAddr,MAC��ַ*/                                              
/*����ֵ˵�����ɹ�����CHDRV_NET_OK*/
/*����ע������:                                                   */
/*����˵��:                                                       */  
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e CHDRV_NET_GetMacAddr(CHDRV_NET_Handle_t rh_NetDevice,u8_t *rpuc_MacAddr);
/*--------------------------------------------------------------------------*/
/*������:void CHDRV_NET_SetPHYMode(void)*/
/*�����˺Ϳ���ʱ��:flex     2009-04-13                   */
/*������������:  ������������ģʽ                          */
/*����:		*/
/* rh_NetDevice,�豸���*/
/* renm_PHYMode,����ģʽ*/
/*����ֵ˵�����ɹ�����CHDRV_NET_OK*/
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e CHDRV_NET_SetPHYMode(CHDRV_NET_Handle_t rh_NetDevice,CHDRV_NET_PHY_MODE_e	renm_PHYMode);
/*--------------------------------------------------------------------------*/
/*������:void CHDRV_NET_SetMultiCast(void)*/
/*�����˺Ϳ���ʱ��:flex     2009-04-13                   */
/*������������:  ����Ϊ�ಥģʽ                          */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                       */
/*����:	*/
/* rh_NetDevice,�豸���*/
/*����ֵ˵�����ɹ�����CHDRV_NET_OK*/
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e CHDRV_NET_SetMultiCast(CHDRV_NET_Handle_t	rh_NetDevice);
/*--------------------------------------------------------------------------*/
/*������:void CHDRV_NET_GetLinkStatus*/
/*�����˺Ϳ���ʱ��:flex     2009-04-13                   */
/*������������: �õ�link״̬                         */
/*����:	*/
/* rh_NetDevice,�豸���*/
/* ���:*/
/* rpuc_LinkStatus,link״̬*/
/*����ֵ˵�����ɹ�����CHDRV_NET_OK*/
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e CHDRV_NET_GetLinkStatus(CHDRV_NET_Handle_t	rh_NetDevice,u8_t*rpuc_LinkStatus);
/*--------------------------------------------------------------------------*/
/*������:void CHDRV_NET_Term(void)*/
/*�����˺Ϳ���ʱ��:flex     2009-04-13                   */
/*������������:  �ر�NETģ��                             */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                       */
/*����:��								*/
/*����ֵ˵�����ɹ�����CHDRV_NET_OK*/
/*--------------------------------------------------------------------------*/
CHDRV_NET_RESULT_e CHDRV_NET_Term (void);
/*--------------------------------------------------------------------------*/
/*������:void CHDRV_NET_DebugInfo(void)*/
/*�����˺Ϳ���ʱ��:flex     2009-04-13                   */
/*������������:  ��ӡ�����еĶ�״̬��Ϣ����������      */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                       */
/*����:��								*/
/*����ֵ˵������*/
/*--------------------------------------------------------------------------*/
void CHDRV_NET_DebugInfo(void);

#endif/*__CHDRV_NET_H__*/


