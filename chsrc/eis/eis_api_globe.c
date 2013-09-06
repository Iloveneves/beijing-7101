/*
  * ===================================================================================
  * CopyRight By CHANGHONG NET L.T.D.
  * �ļ�: 	eis_api_globe.c
  * ����: 	����ȫ�ֱ���
  * ����:	 ��ս�ң�������
  * ʱ��:	2008-10-22
  * ===================================================================================
  */
#include "eis_api_define.h"
#include "eis_api_globe.h"
#include "eis_api_msg.h"

clock_t 	geis_reboot_timeout 	= 0;		/* ����ʱ�� */
VOID *	pEisHandle 				= NULL;	/* ipanel�������� */
boolean 	log_out_state 	= false;		/* �˳���־ */

semaphore_t *gp_EisSema = NULL; 

static boolean eis_dhcp_set =false;/*  ���ڼ�¼IPANLE ģ������DPCH״̬true:set DHCP , false:DHCP end */
void eis_set_dhcp_state( boolean dhcp_state)
{
	eis_dhcp_set = dhcp_state;
}
boolean eis_get_dhcp_state( void )
{
	return eis_dhcp_set;
}

/*  ��ʼ���õ� */
void CH_Ipanel_demux_Init ( void )
{
      CH_Eispartition_init_VIDLMI();
       gp_EisSema = semaphore_create_fifo(1);
	ipanel_porting_task_init (); /*��ʼ��TASK��ز���*/
	Eis_Init (); /*���������ݽ���*/
	eis_api_msg_init (); /*��ʼ��IPANEL��Ϣ����*/
	ipanel_porting_init_nvm(); /*����FLASH��������*/
	EIS_time_paly_init();/*��������ʱʱ����˸����*/
	EIS_Msg_process_init();/*������Ϣ��ؽ���*/
}

/* ÿ�ν����õ� */
int eis_api_enter_init ( void )
{
	int i_Return;
#ifdef USE_EIS_2D 
	ipanel_2d_init();
#endif
	eis_api_stop_ca ();		/* ֹͣCA�Ľ��� */
	/*eis_api_msg_reset ();      ��ռ��20101029 ȥ��*/
	eis_init_region ();
	eis_clear_all ();
	eis_timeinit ();
	CH_ipanel_network_init();
	Eis_DMX_Init();

	return 0;
}

/* �˳�ʱ�õ� */
void eis_api_out_term ( void )
{
	CH_ipanel_network_exit();
	ipanel_porting_term_nvm ( );
	eis_del_region ();
	eis_clear_all ();
	eis_api_msg_reset ();
	Eis_DMX_term();
}


/*--eof-----------------------------------------------------------------------------------------------------*/

