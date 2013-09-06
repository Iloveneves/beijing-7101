/*
  * ==========================================================================================
  * Copyright By CHANGHONG network
  * file: chmid_msg.h
  * des: ������Ϣ�Ľṹ����غ���
  * -------DATE---------WORK--------NAME---------
  *             081110              ����		������
  * ==========================================================================================
  */

#ifndef __CHMID_MSG_H__
#define  __CHMID_MSG_H__

/* ������Ϣ������ȼ����� */
#define CHMID_MSG_PRIORITY_NUMBER	16

/* ������Ϣ�м��Ĵ������� */
typedef  enum
{
	CHMID_MSG_OK			= 0,		 	/* ��ȷ */
	CHMID_MSG_NO_MEM,				/* ���ڴ� */
	CHMID_MSG_PARAM,					/* ��������ȷ */
	CHMID_MSG_REPEAT,					/* �ظ���Ϣ */
	CHMID_MSG_UNKNOW_ERROR			/* �������� */
}chmid_msg_error_e;

/* ������Ϣ������ */
typedef enum
{
}chmid_msg_type_e;

/* ����һ����Ϣ�Ľṹ */
typedef struct chmid_msg_node_s
{
	chmid_msg_type_e			me_MsgType;		/* ��Ϣ���� 				*/
	void						*mp_MsgParam;		/* ��Ϣ����ָ�� 		*/
	struct chmid_msg_node_s	*mp_NextMsg;		/* ��һ����Ϣ��ָ�� */
}chmid_msg_node_t;

/*  ����һ����Ϣ���еĽṹ */
typedef struct
{
	int					mi_MsgCount;		/* �ö�����Ϣ�ṹ 		*/
	chmid_msg_node_t		*mp_QueueHead;	/* ����ָ�� 					*/
	chmid_msg_node_t		*mp_QueueTail;		/* ����βָ�� 				*/
}chmid_msg_queue_t;

/* �����ƽ̨��bool���� */
typedef enum
{
	CHMID_MSG_FALSE = 0,
	CHMID_MSG_TRUE
}chmid_msg_bool_e;

/* ����msg�м����Ϣ����ȫ������ */
static chmid_msg_queue_t 	gt_msg_mid_queue[CHMID_MSG_PRIORITY_NUMBER];
static ST_Partition_t		*gp_MsgPartition 	= NULL;
static semaphore_t			*gp_MsgSemaphore 	= NULL;

/*
  * Func: CHMID_MSG_Init
  * Desc: ��ʼ����Ϣ�м��
  * In:	���÷���ָ��
  * Out:	n/a
  * Ret:	CHMID_MSG_PARAM	-> ������ڴ����ָ�벻��
  		CHMID_MSG_NO_MEM	-> ���������ź���ʧ��
  		CHMID_MSG_OK		-> ��ʼ���ɹ�
  */
chmid_msg_error_e CHMID_MSG_Init ( void* rp_Partition )
{
	int i;

	/* �жϲ����Ƿ���ȷ */
	if ( NULL == rp_Partition )
	{
		return CHMID_MSG_PARAM;
	}

	/* �����������ź��� */
	gp_MsgSemaphore = semaphore_create_fifo ( 1 );

	if ( NULL == gp_MsgSemaphore )
	{
		return CHMID_MSG_NO_MEM;
	}

	gp_MsgPartition = ( ST_Partition_t* )rp_Partition;

	/* ѭ����ʼ����Ϣ�������� */
	for ( i = 0; i < CHMID_MSG_PRIORITY_NUMBER; i ++ )
	{
		gt_msg_mid_queue[i].mi_MsgCount 	= 0;
		gt_msg_mid_queue[i].mp_QueueHead	= NULL;
		gt_msg_mid_queue[i].mp_QueueTail	= NULL;
	}

	/* ��ȷ���� */
	return CHMID_MSG_OK;
}

/*
  * Func: CHMID_MSG_Put
  * Desc: ����Ϣ�м�㷢����Ϣ
  * In:	re_MsgType			-> ��Ϣ����
  		rc_MsgPriority			-> ��Ϣ���ȼ�
  							     0~15
  							     0 �����
  							     15�����
  		rp_MsgParam			-> ��Ϣ����ָ��
  		re_CheckRepeat		-> �Ƿ����ظ���Ϣ
  							     ����������Ϣ��ò�Ҫ����ظ�
  * Out:	n/a
  * Ret:	CHMID_MSG_REPEAT	-> �������Ϣ�Ѿ�����Ϣ��������
  		CHMID_MSG_NO_MEM	-> ���������ź���ʧ��
  		CHMID_MSG_OK		-> ��ʼ���ɹ�
  */
chmid_msg_error_e CHMID_MSG_Send ( chmid_msg_type_e re_MsgType, char rc_MsgPriority, void* rp_MsgParam, chmid_msg_bool_e re_CheckRepeat )
{
	int 					i_MsgQueueIndex;	/* ��Ϣ������� */
	int					i_Index;
	chmid_msg_node_t		*pt_MsgNode;

	semaphore_wait ( gp_MsgSemaphore );

	/* ������ȼ� */
	if ( rc_MsgPriority > CHMID_MSG_PRIORITY_NUMBER - 1 )
	{
		rc_MsgPriority = CHMID_MSG_PRIORITY_NUMBER - 1;
	}

	if (  rc_MsgPriority < 0 )
	{
		rc_MsgPriority = 0;
	}

	/* �õ���Ϣ���е����� */
	i_MsgQueueIndex = CHMID_MSG_PRIORITY_NUMBER - 1 - rc_MsgPriority;

	if ( CHMID_MSG_TRUE == re_CheckRepeat )
	{
		pt_MsgNode 	= gt_msg_mid_queue[i_MsgQueueIndex].mp_QueueHead;
		i_Index		= gt_msg_mid_queue[i_MsgQueueIndex].mi_MsgCount;

		/* �ж��Ƿ��ظ� */
		while ( i_Index > 0 && NULL != pt_MsgNode )
		{
			if ( NULL != pt_MsgNode && pt_MsgNode->me_MsgType == re_MsgType )
			{
				semaphore_signal ( gp_MsgSemaphore );
				return CHMID_MSG_REPEAT;
			}
		}
	}
	
	/* ��������Ӹ���Ϣ */
	pt_MsgNode = memory_allocate ( gp_MsgPartition, sizeof ( chmid_msg_node_t ) );

	if ( NULL == pt_MsgNode )
	{
		semaphore_signal ( gp_MsgSemaphore );
		return CHMID_MSG_NO_MEM;		
	}

	pt_MsgNode->me_MsgType 		= re_MsgType;
	pt_MsgNode->mp_MsgParam	= rp_MsgParam;
	pt_MsgNode->mp_NextMsg		= NULL;
	if ( NULL == gt_msg_mid_queue[i_MsgQueueIndex].mp_QueueHead )
	{
		gt_msg_mid_queue[i_MsgQueueIndex].mp_QueueHead = pt_MsgNode;
	}

	if ( NULL == gt_msg_mid_queue[i_MsgQueueIndex].mp_QueueTail )
	{
		gt_msg_mid_queue[i_MsgQueueIndex].mp_QueueTail = pt_MsgNode;
	}
	else
	{
		gt_msg_mid_queue[i_MsgQueueIndex].mp_QueueTail->mp_NextMsg = pt_MsgNode;
		gt_msg_mid_queue[i_MsgQueueIndex].mp_QueueTail = pt_MsgNode;
	}

	semaphore_signal ( gp_MsgSemaphore );
	return CHMID_MSG_OK;
}

#endif

/*--eof--------------------------------------------------------------------------------------------------------------*/

