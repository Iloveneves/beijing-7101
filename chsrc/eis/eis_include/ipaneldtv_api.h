/******************************************************************************/
/*    Copyright (c) 2005 iPanel Technologies, Ltd.                            */
/*    All rights reserved. You are not allowed to copy or distribute          */
/*    the code without permission.                                            */
/*    This is the demo implenment of the Porting APIs needed by iPanel        */
/*    MiddleWare.                                                             */
/*    Maybe you should modify it accorrding to Platform.                      */
/*                                                                            */
/*    $author Zouxianyun 2005/04/28                                           */
/******************************************************************************/

#ifndef _IPANEL_MIDDLEWARE_API_H_
#define _IPANEL_MIDDLEWARE_API_H_

#include "ipanel_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * create iPanel application. you must input a memory size (>=5M).
 * and it will return browser handle, null is fail
 */
VOID *ipanel_create(UINT32_T mem_size);

/* process events */
INT32_T ipanel_proc(VOID *handle, UINT32_T msg, UINT32_T p1, UINT32_T p2);

/* destroy iPanel middleware */
INT32_T ipanel_destroy(VOID *handle);

INT32_T ipanel_open_uri(VOID *handle, CHAR_T *url);

INT32_T ipanel_open_overlay(VOID *handle, CHAR_T *url, UINT32_T x,UINT32_T y,UINT32_T w,UINT32_T h);

UINT32_T ipanel_get_utc_time();//���ش�1970�꿪ʼ����ǰ������
INT32_T ipanel_set_dns_server(INT32_T no, CHAR_T *ipaddr);
INT32_T ipanel_get_dns_server(INT32_T no, CHAR_T *ipaddr, INT32_T len);

/* register protocol */
typedef INT32_T (*IPANEL_PROTOCOL_EXT_CBK)(CONST CHAR_T *protocolurl);
INT32_T ipanel_register_protocol(CONST CHAR_T *protocol, IPANEL_PROTOCOL_EXT_CBK func);

typedef enum {
	IPANEL_OPEN_OC_HOMEPAGE = 1,	/* ָ��serviceid������ָ��,�ⲿAPP��ipanel�����serviceid��oc��ҳ */
	IPANEL_AUDIO_CHANNEL	= 2,	/* arg���ַ���ָ�룬�ַ��ֱ�Ϊ��"STEREO"--������, "LEFT_MONO"--������, "RIGHT_MONO"--������, "MIX_MONO"--����������ϣ�"STEREO_REVERSE"--������������������ת */
	IPANEL_AUDIO_MUTE	 	= 3,	/* argΪ�ַ���ָ�룬�ַ����ֱ�Ϊ��"UNMUTE"--�Ǿ�����"MUTE"--���� */
	IPANEL_AUDIO_VOLUME		= 4,	/* arg��int����ָ�� */
	IPANEL_PUSHVOD_START				= 5,	/* argΪָ����pushvodƵ���Ƶ������ָ�� */
	IPANEL_CHANGE_TS_BYOTHERAPP			= 6 	/* arg������*/ 
} IPANEL_IOCTL_e;
/*
 * ��iPanel MiddleWare��ǰʵ������һ�������������������úͻ�ȡiPanel MiddleWare��ǰʵ���Ĳ��������ԡ�  
 */
INT32_T ipanel_ioctl(VOID *handle, IPANEL_IOCTL_e op, VOID *arg);

/* �ýӿڸ�dvb3.0�ü���ʹ�ã����Դӵײ��ȡƵ�㣬�������dvb3.0�ü���ײ㲻��ʵ�֣���������ʵ���� */
int ipanel_porting_get_frequency(int on_id, int ts_id, int service_id);

#ifdef __cplusplus
}
#endif

#endif//_IPANEL_MIDDLEWARE_API_H_
