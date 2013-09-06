/*
  * ===================================================================================
  * CopyRight By CHANGHONG NET L.T.D.
  * �ļ�: 	eis_api_msg.h
  * ����: 	������Ϣ��صĽӿ�
  * ����:	 ��ս�ң�������
  * ʱ��:	2008-11-06
  * ===================================================================================
  */

#ifndef __EIS_API_MSG__
#define __EIS_API_MSG__

/*
  * Type: eis_msg_t
  * Desc: irdeto ��Ϣ����
  */
typedef struct
{
	U32		mu32_MsgType;
	U32		mu32_Param1;
	U32		mu32_Param2;
}eis_msg_t;

#define EIS_MSG_SIZE	sizeof ( eis_msg_t )		/* ������Ϣ��С */
#define EIS_MSG_COUNT	50						/* �����Ϣ���� */

/*
  * Func: eis_api_msg_init
  * Desc: eis��Ϣ���г�ʼ��
  * In:	n/a
  * Out:	n/a
  * Ret:	IPANEL_OK	-> �ɹ�
  		IPANEL_ERR	-> ���ɹ�
  */
extern int eis_api_msg_init ( void );

/*
  * Func: eis_api_msg_reset
  * Desc: eis��Ϣ���г�ʼ��
  * In:	n/a
  * Out:	n/a
  * Ret:	IPANEL_OK	-> �ɹ�
  		IPANEL_ERR	-> ���ɹ�
  */
extern void eis_api_msg_reset ( void );

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
extern int eis_api_msg_send ( U32 ru32_MsgType, U32 ri_Param1, U32 ri_Param2 );

/*
  * Func: eis_api_msg_send
  * Desc: ���������н�����Ϣ
  * Out:	rp_MsgType	-> ��������Ϣ����
  		rp_Param1	-> �����Ĳ���1
  		rp_Param2	-> �����Ĳ���2
  * Ret:	IPANEL_OK	-> ����Ϣ
  		IPANEL_ERR	-> ����Ϣ�����
  */
extern int eis_api_msg_get ( U32 *rp_MsgType, U32 *rp_Param1, U32 *rp_Param2 );

#endif

/*--eof----------------------------------------------------------------------------------------------------*/

