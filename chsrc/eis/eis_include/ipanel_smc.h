/******************************************************************************/
/*    Copyright (c) 2005 iPanel Technologies, Ltd.                     */
/*    All rights reserved. You are not allowed to copy or distribute          */
/*    the code without permission.                                            */
/*    This is the demo implenment of the Porting APIs needed by iPanel        */
/*    MiddleWare.                                                             */
/*    Maybe you should modify it accorrding to Platform.                      */
/*                                                                            */
/*    $author Zouxianyun 2005/04/28                                           */
/******************************************************************************/

#ifndef _IPANEL_MIDDLEWARE_PORTING_SMC_API_FUNCTOTYPE_H_
#define _IPANEL_MIDDLEWARE_PORTING_SMC_API_FUNCTOTYPE_H_

#include "ipanel_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* redefinitions for old usage */
#define EIS_CARD_IN		IPANEL_CARD_IN
#define EIS_CARD_OUT	IPANEL_CARD_OUT
#define EIS_CARD_ERROR	IPANEL_CARD_ERROR

/* smart card */
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

/*******************************************************************************
�����ܿ�ʱ�Ļص�����
���У�
    cardno -�����ܿ����
    status -�����ܿ�״̬
    msgATR -  ATRӦ���ַ���
    standard �C ��ʹ�õı�׼
*******************************************************************************/
typedef INT32_T (*IPANEL_SC_STATUS_NOTIFY)(INT32_T cardno, 
										 IPANEL_SMARTCARD_STATUS_e status, 
										 BYTE_T *msgATR, 
										 IPANEL_SMARTCARD_STANDARD_e standard);

/*******************************************************************************
�����ܿ�ִ�и�λ������
input:	cardno:���ܿ����
	    msgATR:���濨���ص�ATR����buffer��ַ��ʹ��ʱ�������ַ�Ƿ��ǿ�ָ�롣
output:	msgATR: ����ATR������Ϊ�ա�
return:	>=0 �ɹ�
					0,��λ�ɹ�,ʹ��T0Э��ͨѶ
					1,��λ�ɹ�,ʹ��T1Э��ͨѶ
					2,��λ�ɹ�,ʹ��T14Э��ͨѶ
				С��0 ʧ��
					-1,�޿�
					-2,����ʶ��
					-3,ͨѶ����
*******************************************************************************/
INT32_T ipanel_porting_smartcard_reset(INT32_T cardno, BYTE_T *msgATR);

/*******************************************************************************
����Ҫ���������ܿ���ע�����ܿ��ص�������ʵ���߿����ڴ˺�������ɶ���Ӧ����
��ʼ����������ʹ�ܿ���Ӳ���ӿڣ��Կ���Active�����ȡ�����ʱ̽�⵽������п�ʱ��
����Կ�����ϵ縴λ������
input:	cardno ���ܿ����
				sc_notify ���ܿ�״̬�仯֪ͨ�ص������ĵ�ַ��
output:	None
return:	>=0 �ɹ�
					0���ӿڴ򿪲����ɹ���������п�����λʧ��
					1���ӿڴ򿪲����ɹ���������޿�
					2���ӿڴ򿪲����ɹ���������п�����λ�ɹ���ʹ��T0Э��ͨѶ
					3���ӿڴ򿪲����ɹ���������п�����λ�ɹ���ʹ��T1Э��ͨѶ
					4���ӿڴ򿪲����ɹ���������п�����λ�ɹ���ʹ��T14Э��ͨѶ
				С��0 ʧ��
*******************************************************************************/
INT32_T ipanel_porting_smartcard_open(INT32_T cardno,
                                      IPANEL_SC_STATUS_NOTIFY func);

/*******************************************************************************
�ر����ڲ��������ܿ��ӿڡ�ʵ���߿����ڴ˺�������ɶ���Ӧ�����ͷŲ�������ֹͣ����
Ӳ���ӿڣ��Կ���DeActive�����ȡ�
input:	cardno ���ܿ����
output:	None
rerurn:	IPANEL_OK �ɹ�; IPANEL_ERR ʧ��
*******************************************************************************/
INT32_T ipanel_porting_smartcard_close(INT32_T cardno);

/*******************************************************************************
�����ܿ�ͨѶ������������Ҫ���ݲ�ͬ��Э�����;�����δ���reqdata�е�����
T0Э�飺
	д���ݣ�
	reqdata �� header + data to write
	�����ݣ�
	reqdata �� header
	repdata �� data from smartcard
T14Э�飺
	д���ݣ�
	reqdata �� data to write
	�����ݣ�
	repdata �� data from smartcard

input:	cardno:���ܿ����
				reqdata:�����ֶ�
				reqlen:�����ֶγ���
				repdata:�����ֶ�
				replen:�����ֶγ���
				StatusWord:���汾�β������ܿ����ص�״̬�ֵĵ�ַ
					*StatusWordS �� SW1 << 8 | SW2
output:	None
return:	IPANEL_OK �ɹ�
				С��0 ʧ��
					-1:�޿�
					-2:����ʶ��
					-3:ͨѶ����
*******************************************************************************/
INT32_T ipanel_porting_smartcard_transfer_data(INT32_T cardno,
                                               CONST BYTE_T *reqdata,
                                               INT32_T reqlen, 
											   BYTE_T *repdata, 
                                               INT32_T *replen, 
                                               UINT16_T *statusword);



#ifdef __cplusplus
}
#endif

#endif // _IPANEL_MIDDLEWARE_PORTING_SMC_API_FUNCTOTYPE_H_
