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

#ifndef _IPANEL_MIDDLEWARE_PORTING_DSM_API_FUNCTOTYPE_H_
#define _IPANEL_MIDDLEWARE_PORTING_DSM_API_FUNCTOTYPE_H_

#include "ipanel_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* descrambler */
/* encryption modes */
typedef enum
{
	IPANEL_DSM_TYPE_PES,
	IPANEL_DSM_TYPE_TS,
	IPANEL_DSM_TYPE_UNKNOWN
} IPANEL_ENCRY_MODE_e;

/*******************************************************************************
����һ��������ͨ����������ָ���趨����ģʽ��
input:	encryptmode������ģʽ����TS��PES
output:	None
return:	> 0 �ɹ��� ���ط���Ľ������ľ��; 0 ʧ�ܡ�
*******************************************************************************/
UINT32_T ipanel_porting_descrambler_allocate(IPANEL_ENCRY_MODE_e encryptmode);

/*******************************************************************************
�ͷ�һ��ͨ��ipanel_porting_descrambler_allocate����Ľ�����ͨ����
input:	handle:�������ľ��
output:	None
return:	IPANEL_OK �ɹ�, IPANEL_ERR ʧ��
*******************************************************************************/
INT32_T ipanel_porting_descrambler_free(UINT32_T handle);

/*******************************************************************************
���ý�����ͨ��PID
input:	handle:�������ľ��
				pid:��Ҫ���ŵ�����PID(AudioPid,VideoPid)
output:	None
return:	IPANEL_OK �ɹ�; IPANEL_ERR ʧ��
*******************************************************************************/
INT32_T ipanel_porting_descrambler_set_pid(UINT32_T handle, UINT16_T pid);

/*******************************************************************************
����������֡�
input:	handle:�������ľ��
				key:�����ֵ�ֵ
				keylen:�����ֳ���
output:	None
return:	IPANEL_OK �ɹ�; IPANEL_ERR ʧ��
*******************************************************************************/
INT32_T ipanel_porting_descrambler_set_oddkey(UINT32_T handle, BYTE_T *key, INT32_T keylen);

/*******************************************************************************
����ż�����֡�
input:	handle:�������ľ��
				key:�����ֵ�ֵ
				keylen:�����ֳ���
output:	None
return:	IPANEL_OK �ɹ�; IPANEL_ERR ʧ��
*******************************************************************************/
INT32_T ipanel_porting_descrambler_set_evenkey(UINT32_T handle, BYTE_T *key, INT32_T keylen);

#ifdef __cplusplus
}
#endif

#endif // _IPANEL_MIDDLEWARE_PORTING_DSM_API_FUNCTOTYPE_H_
