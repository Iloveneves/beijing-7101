/*******************************************************************************
Copyright (c) 2007 by iPanel Technologies, Ltd.
All rights reserved. You are not allowed to copy or distribute
the code without permission.
*******************************************************************************/

#ifndef __IPANEL_PORTING_TASK_H______
#define __IPANEL_PORTING_TASK_H______

#include "ipanel_typedef.h"

/*
** ǿ�ҽ��鲻Ҫʹ��"EIS_"��ͷ����Щdefinitions!  --McKing
*/

#ifdef __cplusplus
extern "C" {
#endif

/* ipanel_porting_sem_wait()/ipanel_porting_queue_recv()�ĵȴ���ʽ: �������� �� ���õȴ� */
#define EIS_QUEUE_SEM_WAIT_RET_NOW  (0)
#define EIS_QUEUE_SEM_WAIT_FOREVER  (-1)

#define IPANEL_NO_WAIT              (0)
#define IPANEL_WAIT_FOREVER         (-1)

/*******************************************************************************
** Queue Message Structure used by the OSP.
** Note that it was decided to use a structure for the message type so as to
** provide bounds checking on the message being placed in the queue. An array
** was the obvious alternative to such a structure, but it was felt that the
** array would not clearly indicate to the user the the limitations on the
** message size to use.
*******************************************************************************/
typedef struct
{
    UINT32_T q1stWordOfMsg;                /* First word of a queue message.  */
    UINT32_T q2ndWordOfMsg;                /* Second word of a queue message. */
    UINT32_T q3rdWordOfMsg;                /* Third word of a queue message.  */
    UINT32_T q4thWordOfMsg;                /* Fourth word of a queue message. */
} IPANEL_QUEUE_MESSAGE;

/*
** constants for returning status of waiting semaphore(P operation)
*/
enum
{
	EIS_OS_QUEUE_SEM_STATUS_AVAILABLE=IPANEL_OK,    // queue_wait/semaphore_wait is available(not locked, etc)
    EIS_OS_QUEUE_SEM_STATUS_UNAVAILABLE=IPANEL_ERR  // queue_wait/semaphore_wait is unavailable(locked, etc)
};

enum
{
    IPANEL_TASK_WAIT_FIFO, //�Ƽ�ʹ�õ�ģʽ
    IPANEL_TASK_WAIT_PRIO
};
#define EIS_TASK_WAIT_FIFO	(0)
#define EIS_TASK_WAIT_PRIO	(1)

typedef  VOID (*IPANEL_TASK_PROC)(VOID *param);

/*!!!!!!!ע��!!!!!!!!*/
/*PSOS::::���func�ڳ������еĹ������˳���ϵͳ�Զ������˸�task handle; ���funcһֱ�����У���ô��Ҫ���ٸ�task handle. */
/*VxWorks::::func�����еĹ������˳�����task handleϵͳû������. */
/*******************************************************************************
����:
name��          ���������
Func:       ������ڵ�ַ:typedef VOID(*IPANEL_TASK_PROC)(VOID *);
Param:      �����б�(һ����ΪNULL)
stack_size: ջ��С(>0��Ч)
priority:   ���ȼ���...(ipanel���ȼ���0��31,31���,0���)
����ֵ:
handle( 0 ʧ��)
*******************************************************************************/
UINT32_T ipanel_porting_task_create(CONST CHAR_T *name, IPANEL_TASK_PROC func, VOID *param,
                                    INT32_T priority, UINT32_T stack_size);

/*******************************************************************************
����:
handle: task handle(��0,�Ҵ���,��Ч)
����ֵ:
0:success
-1:failed
*******************************************************************************/
INT32_T ipanel_porting_task_destroy(UINT32_T handle);

/*******************************************************************************
����:
       ������ʱ�䣬��λ����
����ֵ:����
*******************************************************************************/
VOID ipanel_porting_task_sleep(INT32_T ms);

/*******************************************************************************
����:
name: semaphore name(����ֻ��pSOS��sm_create()��Ҫ��name, ԭ����: char name[4])
initial_count:����ʼ������(���ڵ���0��Ч)this is the initial token count
        for the semaphore being created. This value will determine the maximum
        number of simultaneous accesses allowed to whatever resource is
        being protected by the semaphore.
wait_mode: this parameter determines how tasks will wait on a
        token from an 'empty' semaphore. There are two options for this
        parameter;
        EIS_TASK_WAIT_FIFO - the first task to start pending on the token, will
                    receive the token when is made available.�Ƚ���ȴ����е�������
                    ����ź���
        EIS_TASK_WAIT_PRIO - the highest priority task pending on the token,
                    will receive the token when it is made available.���ȼ��ߵ�����
                    �Ȼ���ź���
        �������е�RTOS��֧�������ֵȴ�ģʽ��win32��OS20��Ecos��UCOS��Linux��
        �������ã�PSOS��VxWorks�������á�
����ֵ:
Handle(0 ʧ��)
*******************************************************************************/
UINT32_T ipanel_porting_sem_create(CONST CHAR_T *name, INT32_T initial_count,
                                   UINT32_T wait_mode);

/*******************************************************************************
����:
handle  semaphore handle(��0,�Ҵ���,��Ч)
����ֵ:
0:success
-1:failed
*******************************************************************************/
INT32_T ipanel_porting_sem_destroy(UINT32_T handle);

/*******************************************************************************
����:
handle  semaphore handle(��0,�Ҵ���,��Ч)
wait_time  �ȴ�semaphore��ʱ��,  0 ��ʾ��������, -1 ��ʾһֱ�ȴ���
����ֵ:
EIS_OS_QUEUE_SEM_STATUS �źŵĵ�ǰ״̬
��EIS_OS_QUEUE_SEM_STATUS_AVAILABLEʱ,��ʾ�ȴ����ź�.
EIS_OS_QUEUE_SEM_STATUS_UNAVAILABLE:failed
*******************************************************************************/
INT32_T ipanel_porting_sem_wait(UINT32_T handle, INT32_T wait_time);

/*******************************************************************************
����:
handle  semaphore handle(��0,�Ҵ���,��Ч)
��handle�ͷ�һ���ź�
����ֵ:
0:success
-1:failed
*******************************************************************************/
INT32_T ipanel_porting_sem_release(UINT32_T handle);

/*******************************************************************************
����:
name:   queue name
len, this specifies the maximum number of messages that
    can reside in the Queue at any given moment. When the number of
    un-read messages in the queue reaches this value, then any
    subsequent attempts to place a message in this queue will fail.
wait_mode, this parameter determines how tasks waiting on a
    message from an empty queue will wait. There are two options
    for this parameter;
    EIS_TASK_WAIT_FIFO - the first task to start pending on the queue, will
                receive the message when it arrives.
                �Ƚ�����е������ȵõ���Ϣ
    EIS_TASK_WAIT_PRIO - the highest priority waiting task will receive the
                message when it arrives.
                ���ȼ��ߵ������ȵõ���Ϣ��
    ��������RTOS��֧������������ģʽ��win32��OS20��ECOS��UCOS��Linux�������ã�
    PSOS��VxWorks�������á�
����ֵ:
handle   0 ʧ��
*******************************************************************************/
UINT32_T ipanel_porting_queue_create(CONST CHAR_T *name, UINT32_T len,
                                     UINT32_T wait_mode);

/*******************************************************************************
����:
queue handle (��0,�Ҵ���,��Ч)
����ֵ:
0:success
-1:failed
*******************************************************************************/
INT32_T ipanel_porting_queue_destroy(UINT32_T handle);

/*******************************************************************************
����:
queue handle (��0,�Ҵ���,��Ч)
msg:Pointer to a IPANEL_QUEUE_MESSAGE structure containing the
    message to put in the queue.
����ֵ:
0:success
-1:failed
*******************************************************************************/
INT32_T ipanel_porting_queue_send(UINT32_T handle, IPANEL_QUEUE_MESSAGE *msg);

/*******************************************************************************
** Function: ipanel_porting_queue_recv(): This function is called to attempt to
** retrieve a message from the specified queue. If a message is present, then
** the RTOS will copy the message into the place holder specified in the
** input parameter list. If no message is present, then depending on the input
** parameters, the calling task will either wait for a timeout, or return
** immediately. In either case, the return code will indicate whether or not
** a message was received. If no message is present, and the task has chosen
** to wait forever, then the calling task will block until a message is posted
** to this queue.
** NOTE: The millSecsToWait period is limited by the interrupt frequency of the
** hardware timer in the system. For that reason, if it is greater than zero,
** it is rounded to the lower period, for which OSP_TIMER_TICK_INT_PERIOD is
** an integral.
**
** Inputs:  handle: the id of the queue from which the message is to be read.
**          msg:     a pointer to a message structure - 16 bytes in length.
**          wait_time: �ȴ�ʱ�䣬��λΪ���롣Ϊ0ʱ��ʾ�������أ�Ϊ-1��ʾ���õȴ���
**
** Outputs: NONE.
**
** Returns:
**  EIS_OS_QUEUE_SEM_STATUS function status
**  EIS_OS_QUEUE_SEM_STATUS_AVAILABLE:success
**  EIS_OS_QUEUE_SEM_STATUS_UNAVAILABLE:failed
*******************************************************************************/
INT32_T ipanel_porting_queue_recv(UINT32_T handle, IPANEL_QUEUE_MESSAGE *msg,
                                  INT32_T wait_time);

#ifdef __cplusplus
}
#endif

#endif // __IPANEL_PORTING_TASK_H______
