/*
  * ===================================================================================
  * CopyRight By CHANGHONG NET L.T.D.
  * �ļ�: 	eis_api_msg.c
  * ����: 	ʵ����Ϣ��صĽӿ�
  * ����:	��ս�ң�������
  * ʱ��:	2008-11-06
  * ===================================================================================
  */
#include "eis_api_define.h"
#include "eis_api_globe.h"
#include "eis_api_debug.h"
#include "eis_api_msg.h"

static message_queue_t	*gp_eis_msg_queue = NULL;

/*
  * Func: eis_api_msg_init
  * Desc: eis��Ϣ���г�ʼ��
  * In:	n/a
  * Out:	n/a
  * Ret:	IPANEL_OK	-> �ɹ�
  		IPANEL_ERR	-> ���ɹ�
  */
int eis_api_msg_init ( void )
{
	gp_eis_msg_queue = message_create_queue_timeout ( EIS_MSG_SIZE, EIS_MSG_COUNT );
	if ( NULL == gp_eis_msg_queue )
	{
		eis_report ( "\n++>eis fatal msg_queue create fail!!!!" );
		return IPANEL_ERR;
	}

	return IPANEL_OK;
}

/*
  * Func: eis_api_msg_reset
  * Desc: eis��Ϣ���г�ʼ��
  * In:	n/a
  * Out:	n/a
  * Ret:	IPANEL_OK	-> �ɹ�
  		IPANEL_ERR	-> ���ɹ�
  */
void eis_api_msg_reset ( void )
{
	eis_msg_t *pt_msg;

	if ( NULL == gp_eis_msg_queue )
		return;

	do
	{
		pt_msg = message_receive_timeout ( gp_eis_msg_queue, TIMEOUT_IMMEDIATE );

		if ( NULL != pt_msg )
		{
			message_release ( gp_eis_msg_queue, pt_msg );
		}
	}while ( pt_msg );

	return;
}

/*
  * Func: eis_api_msg_send
  * Desc: �������̷�����Ϣ
  * In:	ru32_MsgType	-> ��Ϣ����
  		ri_Param1	-> ��Ϣ����1
  		ri_Param2	-> ��Ϣ����2
  * Out:	n/a
  * Ret:	IPANEL_OK	-> �ɹ�
  		IPANEL_ERR	-> ���ɹ�
  */
int eis_api_msg_send ( U32 ru32_MsgType, U32 ri_Param1, U32 ri_Param2 )
{
	clock_t 		time_out;
	eis_msg_t 	*p_Msg = NULL;

	if ( NULL == gp_eis_msg_queue )
	{
		return IPANEL_ERR;
	}

	time_out 	= time_plus ( time_now(), ST_GetClocksPerSecondLow() * 10 );

	p_Msg	= (eis_msg_t*)message_claim_timeout ( gp_eis_msg_queue, &time_out );

	if ( NULL == p_Msg )
	{
		eis_report ( "\n++>eis msg allocate fail!!!!" );
		return IPANEL_ERR;
	}

	p_Msg->mu32_MsgType 	= ru32_MsgType;
	p_Msg->mu32_Param1		= ri_Param1;
	p_Msg->mu32_Param2		= ri_Param2;

	message_send ( gp_eis_msg_queue, p_Msg );

	return IPANEL_OK;
}

/*
  * Func: eis_api_msg_send
  * Desc: ���������н�����Ϣ
  * Out:	rp_MsgType	-> ��������Ϣ����
  		rp_Param1	-> �����Ĳ���1
  		rp_Param2	-> �����Ĳ���2
  * Ret:	IPANEL_OK	-> ����Ϣ
  		IPANEL_ERR	-> ����Ϣ�����
  */
int eis_api_msg_get ( U32 *rp_MsgType, U32 *rp_Param1, U32 *rp_Param2 )
{
	eis_msg_t *pt_msg;

	if ( NULL == gp_eis_msg_queue )
	{
		return IPANEL_ERR;
	}

	pt_msg = message_receive_timeout ( gp_eis_msg_queue, TIMEOUT_IMMEDIATE );

	if ( NULL == pt_msg )
	{
		return IPANEL_ERR;
	}

	*rp_MsgType 	= pt_msg->mu32_MsgType;
	*rp_Param1		= pt_msg->mu32_Param1;
	*rp_Param2		= pt_msg->mu32_Param2;
	message_release ( gp_eis_msg_queue, pt_msg );
	return IPANEL_OK;
}

/*--eof-----------------------------------------------------------------------------------------------------*/

