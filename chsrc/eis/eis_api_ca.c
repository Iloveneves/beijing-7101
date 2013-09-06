/*
  * ===================================================================================
  * CopyRight By CHANGHONG NET L.T.D.
  * �ļ�: 	eis_api_ca.c
  * ����: 	ʵ��CA ��صĽӿ�
  * ����:	 ��ս�ң�������
  * ʱ��:	2008-10-23
  * ===================================================================================
  */

#include "eis_api_define.h"
#include "eis_api_globe.h"
#include "eis_api_debug.h"
#include "eis_api_ca.h"

#include "eis_include\ipanel_smc.h"

#include "appltype.h"
#include "..\dbase\vdbase.h"

#ifdef __EIS_API_DEBUG_ENABLE__
#define __EIS_API_CA_DEBUG__
#endif

/*
����˵����
	����һ��������ͨ����������ָ���趨����ģʽ��
����˵����
	���������
		encryptmode������ģʽ��ȡֵ���£�
		typedef enum
		{
			IPANEL_DSM_TYPE_PES,
			IPANEL_DSM_TYPE_TS,
			IPANEL_DSM_TYPE_UNKNOWN
		} IPANEL_ENCRYPT_MODE_e;
	���������
		��
	�� �أ�
		���� IPANEL_NULL�� �ɹ��� ���ط���Ľ������ľ��
		����IPANEL_NULL�� ʧ�ܡ�
*/
UINT32_T ipanel_porting_descrambler_allocate(IPANEL_ENCRYPT_MODE_e encryptmode)
{
	#ifdef __EIS_API_OSD_DEBUG__
	eis_report ( "\n++>eis ipanel_porting_descrambler_allocate δ���" );
	#endif

	return IPANEL_NULL;
}

/*
����˵����
	�ͷ�һ��ͨ��ipanel_porting_descrambler_allocate ����Ľ�����ͨ����
����˵����
	���������
		handle�����������
	���������
	��
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_porting_descrambler_free (UINT32_T handle)
{
	#ifdef __EIS_API_OSD_DEBUG__
	eis_report ( "\n++>eis ipanel_porting_descrambler_free δ���" );
	#endif

	return IPANEL_OK;
}

/*
����˵����
	���ý�����ͨ��PID��
����˵����
	���������
		handle�����������
		pid����Ҫ���ŵ�����PID
	���������
		��
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ��
*/
INT32_T ipanel_porting_descrambler_set_pid(UINT32_T handle, UINT16_T pid)
{
	#ifdef __EIS_API_OSD_DEBUG__
	eis_report ( "\n++>eis ipanel_porting_descrambler_set_pid δ���" );
	#endif

	return IPANEL_OK;
}

/*
����˵����
	����������֡�
����˵����
	���������
		handle�����������
		key�������ֵ�ֵ
		len�������ֳ���
	���������
		��
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_porting_descrambler_set_oddkey(UINT32_T handle,BYTE_T *key, INT32_T len)
{
	#ifdef __EIS_API_OSD_DEBUG__
	eis_report ( "\n++>eis ipanel_porting_descrambler_set_oddkey δ���" );
	#endif

	return 0;
}

/*
����˵����
	����ż�����֡�
����˵����
	���������
		handle�����������
		key�������ֵ�ֵ
		len�������ֳ���
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_porting_descrambler_set_evenkey(UINT32_T handle, BYTE_T *key, INT32_T len)
{
	#ifdef __EIS_API_OSD_DEBUG__
	eis_report ( "\n++>eis ipanel_porting_descrambler_set_evenkey δ���" );
	#endif

	return IPANEL_OK;
}

/*
����˵����
	����Ҫ���������ܿ���ע�����ܿ��ص�������ʵ���߿����ڴ˺�������ɶ���
	Ӧ���ĳ�ʼ����������ʹ�ܿ���Ӳ���ӿڣ��Կ���Active �����ȡ�����ʱ̽�⵽��
	�����п�ʱ������Կ�����ϵ縴λ������
	���лص������õ������Ͷ���Ϊ��
	typedef INT32_T (*IPANEL_SC_STATUS_NOTIFY)(INT32_T cardno,
	IPANEL_SMARTCARD_STATUS_e status, BYTE_T *msgATR, IPANEL_SMARTCARD_STANDARD_e
	standard)
	���У�cardno - ���ܿ���ţ�
	status - ���ܿ�״̬��
	msgATR - ATR Ӧ���ַ�����
	standard �C ��ʹ�õı�׼
	���ܿ���״̬�����ܿ��ı�׼ö�ٶ������£�
	typedef enum
	{
		IPANEL_CARD_IN,
		IPANEL_CARD_OUT,
		IPANEL_CARD_ERROR
	} IPANEL_SMARTCARD_STATUS_e;
	typedef enum
	{
		IPANEL_SMARTCARD_STANDARD_ISO7816,
		IPANEL_SMARTCARD_STANDARD_NDS,
		IPANEL_SMARTCARD_STANDARD_EMV96,
		IPANEL_SMARTCARD_STANDARD_EMV2000,
		IPANEL_SMARTCARD_STANDARD_ECHOCHAR_T,
		IPANEL_SMARTCARD_STANDARD_UNDEFINE
	} IPANEL_SMARTCARD_STANDARD_e;
����˵����
	���������
		cardno�����ܿ����
		func�����ܿ�״̬�仯֪ͨ�ص������ĵ�ַ
	�����������
	�� �أ�
		�C >=0 �ɹ���
		�C 0���ӿڴ򿪲����ɹ���������п�����λʧ��
		�C 1���ӿڴ򿪲����ɹ���������޿�
		�C 2���ӿڴ򿪲����ɹ���������п�����λ�ɹ���ʹ��T0 Э��ͨѶ
		�C 3���ӿڴ򿪲����ɹ���������п�����λ�ɹ���ʹ��T1 Э��ͨѶ
		�C 4���ӿڴ򿪲����ɹ���������п�����λ�ɹ���ʹ��T14 Э��ͨѶ
		�C <0 ʧ�ܡ�
*/
INT32_T ipanel_porting_smartcard_open(INT32_T cardno, IPANEL_SC_STATUS_NOTIFY func)
{
	#ifdef __EIS_API_OSD_DEBUG__
	eis_report ( "\n++>eis ipanel_porting_smartcard_open δ���" );
	#endif

	return -1;
}

/*
����˵����
	�ر����ڲ��������ܿ��ӿڡ�ʵ���߿����ڴ˺�������ɶ���Ӧ�����ͷŲ�����
	��ֹͣ����Ӳ���ӿڣ��Կ���DeActive �����ȡ�
����˵����
	���������
		cardno�����ܿ����
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_porting_smartcard_close(INT32_T cardno)
{
	#ifdef __EIS_API_OSD_DEBUG__
	eis_report ( "\n++>eis ipanel_porting_smartcard_open δ���" );
	#endif

	return IPANEL_OK;
}

/*
����˵����
	�����ܿ�ִ�и�λ������
����˵����
	���������
		cardno�����ܿ����
	���������
		msgATR�� ����һ��ATR ���������߱�������㹻�Ŀռ䱣��ATR��Ҳ������
		�յģ��ײ㲻����ATR ����
	�� �أ�
		>=0 �ɹ���
		0����λ�ɹ���ʹ��T0 Э��ͨѶ
		1����λ�ɹ���ʹ��T1 Э��ͨѶ
		2����λ�ɹ���ʹ��T14 Э��ͨѶ
		С��0 ʧ�ܡ�
		-1 �޿�
		-2 ����ʶ��
		-3 ͨѶ����
*/
INT32_T ipanel_porting_smartcard_reset(INT32_T cardno, BYTE_T *msgATR)
{
	#ifdef __EIS_API_OSD_DEBUG__
	eis_report ( "\n++>eis ipanel_porting_smartcard_open δ���" );
	#endif

	return -1;
}

/*
����˵����
	�����ܿ�ͨѶ������������Ҫ���ݲ�ͬ��Э�����;�����δ���reqdata �е���
	�ݡ�
	T0 Э�飺
	д���ݣ�
	reqdata = header + data to write
	�����ݣ�
	reqdata = header
	repdata = data from smartcard
	T14 Э�飺
	д���ݣ�
	reqdata = data to write
	�����ݣ�
	repdata = data from smartcard
����˵����
	���������
		cardno�����ܿ����
		reqdata�������ֶ�
		reqlen�������ֶγ���
		repdata�������ֶ�
		replen�������ֶγ���
		statusword�����汾�β������ܿ����ص�״̬�ֵĵ�ַ
		*statusword �� SW1 << 8 | SW2
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		< 0 ��ʧ�ܡ�
		-1 �޿�
		-2 ����ʶ��
		-3 ͨѶ����
*/
INT32_T ipanel_porting_smartcard_transfer_data(INT32_T cardno,const BYTE_T *reqdata, INT32_T reqlen, BYTE_T*repdata, INT32_T *replen, UINT16_T *statusword)
{
	#ifdef __EIS_API_OSD_DEBUG__
	eis_report ( "\n++>eis ipanel_porting_smartcard_transfer_data δ���" );
	#endif

	return IPANEL_OK;
}

/*
  * Func: eis_api_stop_ca
  * Desc: ֹͣCA����
  */
void eis_api_stop_ca ( void )
{

 #ifdef NITCDT
	usifNotifyToNitandCdt(STOP_NIT_BUILDING);/*����NIT��Ϣ*/
	usifNotifyToNitandCdt(STOP_CDT_BUILDING);/*����CDT��Ϣ*/
#ifdef ADD_SDT_MONITOR
	usifNotifyToNitandCdt(STOP_SDT_BUILDING);
#endif
#endif
/*20071116 wlq modify */
#ifdef NDS_CA 
  CH_PMT_PutMessageToCAQue(-1,STOP_DATA_BASE_BUILDING);
	CH_CAT_PutMessageToCAQue(STOP_DATA_BASE_BUILDING);
#endif
	
	
#if 0/*20071116 wlq modify */
	CH_EPG_StopALLEIT();
	task_delay(3000);
	/**CH_EPG_FreeAllEITSource();***20060222 shb add handle free eit source******/
	CH_EIT_ClearAppMem();
#endif
}


/*--eof------------------------------------------------------------------------------------------*/

