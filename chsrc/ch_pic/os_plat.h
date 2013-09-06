/* ƽ̨��صĺ궨�壬ƽ̨�ı�����¶���� */
#ifndef _PLAT_CTRL_H_
#define _PLAT_CTRL_H_

#include "stdio.h"
#include "stvid.h"
#include "sttbx.h"
#include "graphconfig.h"

/*#define _BUILD_IN_WINDOWS_ ƽ̨����*/
#define gz_STATIC  /*��̬����*/


#define J_OSP_FreeMemory CH_FreeMem

#ifndef NULL
#undef  NULL
#define NULL 0
#endif

/*�޷�������,���ݲ�ͬ��ƽ̨Ӧ�������ı�*/
#ifndef DEFINED_U8
#define DEFINED_U8
#define U8 unsigned char
#endif

#ifndef DEFINED_U16
#define DEFINED_U16
#define U16 unsigned short
#endif

#ifndef DEFINED_U32
#define DEFINED_U32
#define U32 unsigned  long
#endif

#ifndef DEFINED_U64
#define DEFINED_U64
#define U64 CH_UNSIGNED_64
#endif

#ifndef UINT
#define UINT unsigned int/* �������ã���ò���,����U32�������������������뾯�� */
#endif

/*�������������ݲ�ͬ��ƽ̨Ӧ�������ı�*/
#ifndef DEFINED_S8
#define DEFINED_S8
#define S8 signed char
#endif

#ifndef DEFINED_S16
#define DEFINED_S16
#define S16 signed short
#endif

#ifndef DEFINED_S32
#define DEFINED_S32
#define S32 signed  long
#endif

#ifndef DEFINED_S64
#define DEFINED_S64
#define S64 CH_SIGNED_64
#endif

#if 0
#ifndef INT
#define INT int /* �������ã���ò���,����S32�������������������뾯�� */
#endif
#endif/*5107ƽ̨�Ѿ�������*/

#ifndef CH_FLOAT 
#define CH_FLOAT float
#endif

#ifndef CH_BOOL
#define CH_BOOL S8
#endif

#define CH_TRUE 1
#define CH_FALSE 0

#ifndef PU64
#define PU64 unsigned long int
#endif
/*��ʱ��״̬*/
typedef enum _TimerState_
{
	CH_TIMER_INIT	= 0,
	CH_TIMER_STOP	= 1,
	CH_TIMER_RUN	= 2,
	CH_TIMER_EXPIRE = 3
}CH_TIMER_STATE,*PCH_TIMER_STATE;

/*ƽ̨���OS ��ʼ������,�ɹ�����0��ʧ�ܷ���-1
   �������Ҫ��ʼ����ֱ�ӷ���0*/
S32 CH_InitPlatOS(void);

S32 CH_ReleasePlatOS(void);

/* ��ʱ���ص���������,�ɲ�ͬƽ̨�Ķ�ʱ���������øú���ʵ�ֶ�ʱ���û��ص� */
typedef void (*CH_TIMERFUNC) (struct _ChTimer_* pTimer);

/*��ʱ���ṹ*/

typedef enum
{
	CH_TIME_ONESHOT = 0, /*��ʱ��ֻ��Ӧһ��*/
	CH_TIME_PERIODIC = 1 /*��ʱ��ÿ��iDuration��Ӧһ��*/
}TIMER_TYPE;

typedef struct _ChTimer_ 
{
	void * pPlatTimer;		/* ָ��ƽ̨��صĶ�ʱ���ṹ,WINDOWS DDK����PKTIMER�ṹָ��*/
	U32 iDuration;			/* ����ʱ��,��λ������*/
	U32 iExpiration;		/* ��ʱ������ʱ��ʱ��,��λ������ */
	CH_TIMERFUNC TimerFunc;	/* ��ʱ������ʱ���õĺ���*/
	void* pArg;				/* ��ʱ���ص������Ĳ���*/
	CH_TIMER_STATE iState;	/* ��ʱ��״̬*/
	TIMER_TYPE eTimerType;	/* ��ʱ�����ͣ�0:TIME_ONESHOT*/
	void * pPlatArg;		/* ƽ̨ʵ�ֿ�����Ҫ�õ�����������*/
} CH_TIMER,*PCH_TIMER;

/*ƽ̨��صĺ�������:*/

/*��ʱ����ʱ�亯��:*/

/*************************************************************************
 * ��ʼ����ʱ���ṹ��ĳЩƽ̨�Ķ�ʱ��������Ҫ�ȳ�ʼ����ʱ��,�����������ǰ
 * pTimer Ӧ���Ѿ�������ڴ档
 ������
	void * pPlatArg;		 ƽ̨ʵ�ֿ�����Ҫ�õ�����������
	iDuration:				 ����ʱ��,��λ������
	TIMER_TYPE eTimerType;	 ��ʱ�����ͣ�0:TIME_ONESHOT
	CH_TIMERFUNC TimerFunc	 ��ʱ������ʱ���õĺ���
	void* pArg;				 ��ʱ���ص������Ĳ���
����:
	�ɹ����� ��0,ʧ�ܷ��� 0
 *************************************************************************/
S32 CH_InitializeTimer(PCH_TIMER pTimer,void * pPlatArg,U32 iDuration,
					   TIMER_TYPE eTimerType,CH_TIMERFUNC TimerFunc,void* pArg);

void CH_StartTimer(PCH_TIMER pTimer);	/*��ʱ����ʼ����*/

void CH_StopTimer(PCH_TIMER pTimer);	/*ֹͣ��ʱ������*/

void CH_DeleteTimer(PCH_TIMER pTimer);	/*ɾ����ʱ��,���ͷ������Դ*/

/* ��ʱ�����¼���.��������ΪpTimer->iDuration*/
#define CH_ResetTimer(pTimer) \
{\
	CH_StopTimer(pTimer);\
	CH_StartTimer(pTimer);\
}

CH_TIMER_STATE CH_GetTimerState(PCH_TIMER pTimer);/*�õ���ʱ��״̬*/

/*�õ�ϵͳʱ��(����)*/
U32 CH_GetSysTimeInSec(void);

/*�õ�ϵͳʱ��(������)*/
U32 CH_GetSysTimeInMillisec(void);

/*��ǰ�������� iMillisecond(����) */
void CH_Sleep(U32 iMillisecond);

/*�ڴ溯����*/

void *CH_AllocMem(U32 size);/*����size���ֽڵ��ڴ�*/

void *CH_ReAllocMem(void * ptr,U32 iSize);/*���·����ڴ�*/

void CH_FreeMemB(void *p);/*�ͷ��ڴ�*/


/*����dest����Ϊָ����cֵ,���ó���Ϊcount,����dest;(memset)
void *CH_MemSet(void *dest,S8 c,S32 count);*/
  void CH_Printf( char * Format_p, ...);
/*#define CH_Printf sttbx_Printͼ�β��ӡ���*/
#define CH_PutError CH_Printf
#define CH_MemSet memset

/*�ڴ濽������src��������Ϊcount���ڴ����ݵ�dest������dest*/
void *CH_MemCopy( void *dest, void *src, S32 count );

/*�ڴ��ƶ�����src��������Ϊcount���ڴ����ݵ�dest������dest
  *���src ��dest ���ظ��ĵط�������ȷ���ظ��ĵط�������
  * �ܱ���ȷ����*/
void *CH_MemMove( void *dest, void *src, S32 count );

/*Compare characters in two buffers.(����ͬ memcmp)
	Return Value    Relationship of First count Bytes of buf1 and buf2 
	< 0             buf1 less than buf2 
	0               buf1 identical to buf2 
	> 0             buf1 greater than buf2 
*/
S32 CH_MemCompare( void *buf1, void *buf2, S32 count );

void *CH_SpaceMemCopy( void *dest, void *src, S32 count ,S32 spaceFlag);

S32 CH_StrLen(S8 *pStr);/*���ַ�������*/

/* ��ʽ�����ݵ��ַ��� (sprintf ����),�����߱��뱣֤pOutBuff���幻��,����Խ��
   ����ԭ��:
S32 CH_SPrintf(S8 *pOutBuff, const S8* pInFormatStr, ...);  */
#define CH_SPrintf sprintf

/* ��ʽ���������׼����豸(printf)
   ����ԭ��:
S32 CH_Printf( const S8 *pInFormatStr ,... );
#define CH_Printf printf*/

/* ��һ���������и�ʽ����ȡ����(sscanf ����) 
   ����ԭ��:
S32 CH_SScanf(S8 *pInBuff, const S8* pInFormatStr, ...);*/
#define CH_SScanf sscanf

/*���������*/
/*������0~n֮����������,�����ظ�����ʾ��֧�ָù���*/
S32 CH_GetRandomInt(S32 n);

/*������0~1֮������ʵ���������ظ�����ʾ��֧�ָù���*/
float CH_GetRandom(void);


/*�߳�������ݽṹ*/

 /* �̻߳ص��������� */
#ifdef _BUILD_IN_WINDOWS_
#define CH_THREADFUNC void*
#else
typedef void (*CH_THREADFUNC)(U32 Param);
#endif
typedef struct  
{
	U32 ThreadHandle;			/* ���ݲ�ͬ��ƽ̨,��32λָ�����,��CH_CreateThread���.*/
	void * pPlatThread;			/*ָ��ƽ̨ʵ����ص����ݽṹ,*/
	CH_THREADFUNC ThreadFunc;	/* �̺߳���*/
	void *pArg;					/* �̺߳����Ĳ���*/
	S8 Status; /*�߳�״̬��0Ϊ���𣬷�0Ϊ������*/
}CH_THREAD,*PCH_THREAD;
#if 0
/* �������ܣ������̣߳����ִ�гɹ��������pThreadData��Handle�ֶΡ�
 * ����˵����pThreadData���߳����ݽṹָ�롣�ɵ����߸�������ڴ档
 * ����ֵ��ʧ�ܷ���NULL�����ɹ������ݲ�ͬ��ƽ̨����ֵ��32λָ�����,
 * ���ø�ֵ���pThreadData��Handle�ֶΡ�
 */
U32 CH_CreateThread(PCH_THREAD pThreadData);

/* �������ܣ��߳̿�ʼ���С�
 * ����˵����pThreadData���߳����ݽṹָ�룬��ch_CreateThead()������
 * ����ֵ��	
 */
void CH_StartThread(PCH_THREAD pThreadData);

/* �������ܣ��˳�,������ǰ�̣߳����߳�֮�����,������Ҫ�øú��������߳�.
 * ����˵����pThreadData���߳����ݽṹָ�룬pParam,ƽ̨ʵ����Ҫ�õ����κβ���ָ�롣
 */
S32 CH_TerminateThread(PCH_THREAD pThreadData,void * pParam);

/* �������ܣ������߳����С�
 * ����˵����pThreadData���߳����ݽṹָ�롣
 * ����ֵ��  
 */
void CH_ResumeThread(PCH_THREAD pThreadData);

/* �������ܣ������̡߳�
 * ����˵����pThreadData���߳����ݽṹָ��
 * ����ֵ����
 */
void CH_SuspendsThread(PCH_THREAD pThreadData);

/* �������ܣ��ر��߳̾����
 * ����˵����pThreadData���߳����ݽṹָ��
 * ����ֵ����
 */
void CH_CloseThread(PCH_THREAD pThreadData);
#endif

/* ����ͬ�����API */

/* ͬ���¼�������ݽṹ */

typedef enum
{
	CH_EVENT_SINAL = 0,
	CH_EVENT_NOSINAL = 1,
	CH_EVENT_TIMEOUT = 2,
	CH_EVENT_TIMEOUT_ERROR  = 3,
	CH_EVENT_ERROR = -1
}CH_EVENT_STATE;

#if 0
typedef struct  
{
	S32 iWaitNumber;
       CH_EVENT_STATE CH_EventSignal;
       S32 CH_TimeOut;
	semaphore_t EventHandle;/* ���ݲ�ͬ��ƽ̨,��32λָ�����,��CH_InitEvent���. */
	void * pPlatEvent;/* ָ��ƽ̨���ʵ�ֵ��¼����ݽṹ, */
} CH_EVENT,*PCH_EVENT;
#else
typedef semaphore_t CH_EVENT;
typedef semaphore_t* PCH_EVENT;
#endif

/* �������ܣ���ʼ���¼�,�����ɹ��������pEvent��Handle�ֶΡ�
 * ����˵����pEvent���¼����ݽṹָ��,�����߱��븺������ڴ�;pArg��ƽ̨ʵ�ֿ�����Ҫ�õ�������������
 * ����ֵ���ɹ�����pEventָ�룬ʧ�ܷ���NULL��
 */
PCH_EVENT CH_InitEvent(PCH_EVENT pEvent,void* pArg);

/* �������ܣ�����¼������ź�̬��
 * ����˵����pEvent���¼����ݽṹָ�롣
 * ����ֵ�� 
 */
void CH_ClearEvent(PCH_EVENT pEvent);

/* �������ܣ������¼����ź�̬��
 * ����˵����pEvent���¼����ݽṹָ��;pArg��ƽ̨ʵ�ֿ�����Ҫ�õ�������������
 * ����ֵ�� 
 */
void CH_SetEvent(PCH_EVENT pEvent);

/* �������ܣ������¼����ź�̬,Ȼ�����Ϸ��ص����ź�̬��
 * ����˵����pEvent���¼����ݽṹָ��
 * ����ֵ�� 
 */
void CH_PulseEvent(PCH_EVENT pEvent);

/* �������ܣ����¼����ź�״̬��
 * ����˵����pEvent���¼����ݽṹָ�롣
 * ����ֵ�� 
 */
CH_EVENT_STATE CH_ReadEventStatus(PCH_EVENT pEvent);

/* �������ܣ�ɾ���¼������ͷ������Դ��
 * ����˵����pEvent���¼����ݽṹָ�롣
 * ����ֵ�� 
 */
void CH_DelEvent(PCH_EVENT pEvent);

/* �������ܣ��������̣��ȴ��¼������ź�̬��
 * ����˵����pEvent���¼����ݽṹָ�롣
 *          iMilliseconds,�ȴ��ĺ�������-1�����޵ȴ���ֱ��pEvent���ź�̬��
 *          ���ƽ̨��֧��ʱ��ȴ����ò���Ӧ�� -1��
 * ����ֵ��  CH_EVENT_SINAL(0):�¼����ź�̬��CH_EVENT_TIMEOUT(1)��ʱ�䵽���أ�CH_TIMEOUT_ERROR��
 *			����iMilliseconds > 0,����ƽ̨��֧�֣���������Ϊ�¼����ź�̬,CH_EVENT_ERROR(-1):����
 */
S32 CH_WaitEvent(PCH_EVENT pEvent,S32 iMilliseconds);

/* ��������������ݽṹ*/
typedef struct  
{
	U32 MutexHandle;/* ���ݲ�ͬ��ƽ̨,��32λָ�����,��CH_InitMutex���. */
	void * pPlatMutex;/* ָ��ƽ̨���ʵ�ֵ��¼����ݽṹ, */
} CH_MUTEX,*PCH_MUTEX;

/* �������ܣ���ʼ���������,�����ɹ��������pMutex��Handle�ֶΡ�
 * ����˵����pMutex������������ݽṹָ��,�����߱��븺������ڴ�;
 *			 pArg��ƽ̨ʵ�ֿ�����Ҫ�õ�������������
 * ����ֵ���ɹ�����pMutexָ�룬ʧ�ܷ���NULL�� */
PCH_MUTEX CH_InitMutex(PCH_MUTEX pMutex, void* pArg);

/* �������ܣ��ͷŻ�����󣬻�������ź�̬��
 * ����˵����pMutex������������ݽṹָ�롣
 * ����ֵ�� 0 = ʧ�ܣ���0 = �ɹ���pMutex���ź�̬�� */
S32 CH_MutexUnLock(PCH_MUTEX pMutex);

/* �������ܣ����������״̬��
 * ����˵����pMutex������������ݽṹָ�롣
 * ����ֵ�� 0 = ���ź�̬��1 = �ź�̬��;-1 = ���� */
S32 CH_ReadMutexStatus(PCH_MUTEX pMutex);

/* �������ܣ�ɾ��������󣬲��ͷ������Դ��
 * ����˵����pMutex������������ݽṹָ�롣
 * ����ֵ��  */
void CH_DelMutex(PCH_MUTEX pMutex);

#define CH_MUTEX_SINAL 0
#define CH_MUTEX_TIMEOUT 1
#define CH_MUTEX_TIMEOUT_ERROR 2
#define CH_MUTEX_ERROR -1

/* �������ܣ��������̣��ȴ�����������ź�̬,����ȡ�û�������ӵ���ߡ�
 * ����˵����pMutex������������ݽṹָ�롣
 *          iMilliseconds,�ȴ��ĺ�������-1�����޵ȴ���ֱ��pMutex���ź�̬��
 *          ���ƽ̨��֧��ʱ��ȴ����ò���Ӧ�� -1��
 * ����ֵ��  CH_MUTEX_SINAL(0):�¼����ź�̬��CH_MUTEX_TIMEOUT(1)��ʱ�䵽���أ�CH_MUTEX_TIMEOUT_ERROR(3)��
 *			����iMilliseconds > 0,����ƽ̨��֧�֣���������Ϊ�¼����ź�̬,CH_MUTEX_ERROR(-1):����
 */
U32 CH_MutexLock(PCH_MUTEX pMutex,S32 iMilliseconds/*����*/ );

/* �ź�����������ݽṹ */
typedef struct  
{
	U32 SemaphoreHandle;/* ���ݲ�ͬ��ƽ̨,��32λָ�����,��CH_InitSemaphores���.*/
	U32 iMaxCount;		/*�����źŻ���������*/
	void * pPlatMutex;	/*ָ��ƽ̨���ʵ�ֵ��¼����ݽṹ,*/
} CH_SEMAPHORE,*PCH_SEMAPHORE ;


/* �������ܣ���ʼ���ź���,�����ɹ��������pSemaphores��Handle�ֶΡ�
 * ����˵����pSemaphores���ź������ݽṹָ��,�����߱��븺������ڴ�;pArg��ƽ̨ʵ�ֿ�����Ҫ�õ�������������
 * ����ֵ���ɹ�����pSemaphoresָ�룬ʧ�ܷ���NULL�� */
PCH_SEMAPHORE CH_InitSemaphores(PCH_SEMAPHORE pSemaphores,void* pArg);

/* �������ܣ����ź���״̬��
 * ����˵����pSemaphores���ź������ݽṹָ�롣
 * ����ֵ�� 0 = ���ź�̬����0 = �ź�̬�� */
S32 CH_ReadSemaphoresStatus(PCH_SEMAPHORE pSemaphores);

/* �������ܣ�ɾ���ź��������ͷ������Դ��
 * ����˵����pSemaphores���ź������ݽṹָ�롣
 * ����ֵ��  */
void CH_DelSemaphores (PCH_SEMAPHORE pSemaphores);

/* �������ܣ��������̣��ȴ��ź��������ź�̬��
 * ����˵����pSemaphores���ź������ݽṹָ�롣
 * ����ֵ��  */
void CH_WaitSemaphores (PCH_SEMAPHORE pSemaphores);

/* ���̼����ݴ���ṹ */
typedef enum
{
	/* ͨ�����ݴ������� */
	msgCmdGetData,
	msgCmdSetData,
	msgCmdExchangeData
}E_MSG_COMMAND;


typedef enum
{  
	eMsgStatusIgnore,            
	eMsgStatusConsumeNoDefault, 
	eMsgStatusConsumeDoDefault  
}E_MSG_STATUS;

typedef enum
{
	eMsgParser
}E_MSG_TYPE;


typedef struct 
{
	U32 iTagetID;/* ��Ϣ���͵�Ŀ�ĵأ�iTagetID �����ǽ���(�߳�)��������ھֱ���
	              * ������ָ����Ϣ����Ŀ��Ľṹָ�룩 */
	U32 iSourceID;/* ��Ϣ���͵ķ����߱�ʶ��iSourceID �����ǽ���(�߳�)��������ھֱ���
	               * ������ָ����Ϣ�����ߵĽṹָ�룩 */
	E_MSG_COMMAND eMsgCommond; /* ��Ϣ���� */
	E_MSG_TYPE eMsgType; /* ��Ϣ���� */
	/* ���ݴ���ṹ */
	void *pInputData; /* �������� */
	S32 iInputDataLength;
	void *pOutputBuff; /* �������,����Ϣ�����߷����ڴ� */
	S32 iOutputBuffLength;
	U32 iTime; /* ��Ϣ����ʱ��ʱ��(��λ������) */
}CH_MSG,*PCH_MSG;


/* return nonzero if success,return 0 if fail */
S32 CH_CopyFromUser(void * pPlat,S8* pDestBuf,S8* pSourceBuf,U32 iSize);

/* return nonzero if success,return 0 if fail */
S32 CH_CopyToUser(void * pPlat,S8* pDestBuf,S8* pSourceBuf,U32 iSize);

#ifndef CH_KERNEL_APPLICATION_DIVIDED

/*block current task and wait for pEvent ,while pEvent signed the function return.
  both  in_out_arg1 and in_out_arg2 are malloced space by caller.
  retrun nonzero if success,return 0 if fail*/
S32 CH_WaitMsgEvent(PCH_EVENT pEvent,U32 *in_out_arg1,U32 *in_out_arg2);
#endif

#endif
