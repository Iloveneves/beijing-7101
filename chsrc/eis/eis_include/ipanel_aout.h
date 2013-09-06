/******************************************************************************/
/*    Copyright (c) 2005 iPanel Technologies, Ltd.                     */
/*    All rights reserved. You are not allowed to copy or distribute          */
/*    the code without permission.                                            */
/*    This is the demo implenment of the Porting APIs needed by iPanel        */
/*    MiddleWare.                                                             */
/*    Maybe you should modify it accorrding to Platform.                      */
/*                                                                            */
/*    $author huzh 2007/11/19                                                 */
/******************************************************************************/

#ifndef _IPANEL_MIDDLEWARE_PORTING_AUDIO_OUTPUT_API_FUNCTOTYPE_H_
#define _IPANEL_MIDDLEWARE_PORTING_AUDIO_OUTPUT_API_FUNCTOTYPE_H_

#include "ipanel_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    IPANEL_AOUT_DEVICE_ANALOG_STERO = 0x01, // ģ�����������
    IPANEL_AOUT_DEVICE_ANALOG_MUTI  = 0x02, // ģ����������
    IPANEL_AOUT_DEVICE_SPDIF        = 0x04, // S/PDIF���
    IPANEL_AOUT_DEVICE_HDMI         = 0x08, // HDMI���
    IPANEL_AOUT_DEVICE_I2S          = 0x10, // I2S���
    IPANEL_AOUT_DEVICE_ALL          = 0xff  // ���ж˿�
} IPANEL_AOUT_DEVICE_e;

typedef enum
{
    IPANEL_AOUT_SET_OUTPUT   = 1,
    IPANEL_AOUT_SET_VOLUME   = 2,
    IPANEL_AOUT_SET_BALANCE  = 3,
	IPANEL_AOUT_GET_VOLUME	 = 4
} IPANEL_AOUT_IOCTL_e;

/* audio output */
/*����Ƶ�������Ԫ*/
UINT32_T ipanel_porting_audio_output_open(VOID);

/*�ر�ָ������Ƶ�����Ԫ*/
INT32_T ipanel_porting_audio_output_close(UINT32_T handle);

/*������������ý���һ�������������������úͻ�ȡ��������豸�Ĳ���������*/
INT32_T ipanel_porting_audio_output_ioctl(UINT32_T handle, IPANEL_AOUT_IOCTL_e op, VOID *arg);

#ifdef __cplusplus
}
#endif

#endif // _IPANEL_MIDDLEWARE_PORTING_AUDIO_OUTPUT_API_FUNCTOTYPE_H_
