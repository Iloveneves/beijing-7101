/******************************************************************************/
/*    Copyright (c) 2008 iPanel Technologies, Ltd.                     */
/*    All rights reserved. You are not allowed to copy or distribute          */
/*    the code without permission.                                            */
/*    This is interface definition for joypad device                          */
/*                                                                            */
/*    $author tujz 2008/10/31                                           */
/******************************************************************************/

#ifndef _IPANEL_MIDDLEWARE_PORTING_JOYPAD_API_FUNCTOTYPE_H_
#define _IPANEL_MIDDLEWARE_PORTING_JOYPAD_API_FUNCTOTYPE_H_

#include "ipanel_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    IPANEL_JOYPAD_GET_DEV_NUM 	= 1,
    IPANEL_JOYPAD_GET_DEV_INFO 	= 2,
    IPANEL_JOYPAD_SEND_MSG 		= 3
} IPANEL_JOYPAD_IOCTL_e;

typedef struct 
{
    INT32_T id; 		//�豸��ʶ
    CHAR_T  name[256];	//�豸�����ƣ����û�л����޷���֪�ɷ����ַ���"NULL"
    INT32_T stateless;	//�Ƿ�����״̬�豸��������ֵ0������һ�ɸ�ֵΪΪ����ֵ(�������ɻ�֪�������
}IPANEL_JOYPAD_DEV_INFO;

typedef struct
{
    INT32_T id; 	//�豸��ID
    CHAR_T 	*pmsg; 	//��Ϣ����
    INT32_T len; 	//��Ϣ����
}IPANEL_JOYPAD_MSG;

INT32_T ipanel_porting_joypad_ioctl(IPANEL_JOYPAD_IOCTL_e cmd, VOID *arg);

#ifdef __cplusplus
}
#endif

#endif//_IPANEL_MIDDLEWARE_PORTING_JOYPAD_API_FUNCTOTYPE_H_

