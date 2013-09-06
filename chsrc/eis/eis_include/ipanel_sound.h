/******************************************************************************/
/*    Copyright (c) 2005 iPanel Technologies, Ltd.                     */
/*    All rights reserved. You are not allowed to copy or distribute          */
/*    the code without permission.                                            */
/*    This is the demo implenment of the SOUND Porting APIs needed by iPanel  */
/*    MiddleWare.                                                             */
/*    Maybe you should modify it accorrding to Platform.                      */
/*                                                                            */
/*    $author huzh 2007/11/22                                                 */
/******************************************************************************/

#ifndef _IPANEL_MIDDLEWARE_PORTING_SOUND_API_FUNCTOTYPE_H_
#define _IPANEL_MIDDLEWARE_PORTING_SOUND_API_FUNCTOTYPE_H_

#include "ipanel_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

#if 0 // move to ipanel_typedef.h
typedef enum
{
    IPANEL_XMEM_PCM = 1,
    IPANEL_XMEM_MP3,
    IPANEL_XMEM_TS,
    IPANEL_XMEM_ES
} IPANEL_XMEM_PAYLOAD_TYPE_e;

typedef struct
{
    IPANEL_XMEM_PAYLOAD_TYPE_e destype;  /* 1����ʾPCM���������������ʽ������������Ͷ��� */
    UINT32_T samplerate;                 /* ����Ƶ�� HZ */
    UINT16_T channelnum;                 /* ͨ��������1��������2˫���� */
    UINT16_T bitspersample;              /* �������ȣ�bpp */
    UINT16_T bsigned;                    /* �з��Ż����޷���, 1�з��ţ�0�޷��� */
    UINT16_T bmsbf;                      /* �Ƿ��λ��ǰ��most significant bit first����1����λ��ǰ��0��λ��ǰ */
    UINT32_T samples;                    /* �������� */
} IPANEL_PCMDES, *pIPANEL_PCMDES;

// �������ݿ��ͷź���ָ��
typedef VOID (*IPANEL_AUDIO_FREE_NOTIFY)(VOID *pblk);

typedef struct
{
    VOID *pdes;                          /* pbuf�е������������� */
    IPANEL_AUDIO_FREE_NOTIFY pfree;     /* IPANEL_XMEMBLK���ݿ��ͷź���ָ�� */
    UINT32_T *pbuf;                      /* ���ݻ�������ַ */
    UINT32_T len;                        /* ���������� */
} IPANEL_XMEMBLK, *pIPANEL_XMEMBLK;

typedef enum
{
    IPANEL_DEV_USE_SHARED,               /* �������û�����ʹ���豸 */
    IPANEL_DEV_USE_EXCUSIVE              /* ��ռʹ���豸 */
} IPANEL_DEV_USE_MODE;
#endif

typedef IPANEL_XMEM_FREE IPANEL_AUDIO_FREE_NOTIFY;

typedef enum
{
    IPANEL_AUDIO_DATA_CONSUMED,          /* ĳ�����ݿ��е������Ѿ�������� */
    IPANEL_AUDIO_DATA_LACK               /* ���е����ݴ�����ϣ��������ϻ���µ����ݾͻᵼ������ֹͣ */
} IPANEL_AUDIO_MIXER_EVENT;

typedef enum
{
    IPANEL_MIXER_SET_VOLUME   	= 1,
    IPANEL_MIXER_CLEAR_BUFFER 	= 2,
    IPANEL_MIXER_PAUSE 			= 3,
    IPANEL_MIXER_RESUME 		= 4
} IPANEL_MIXER_IOCTL_e;

typedef VOID (*IPANEL_AUDIO_MIXER_NOTIFY)(UINT32_T hmixer, IPANEL_AUDIO_MIXER_EVENT event, UINT32_T *param);

UINT32_T ipanel_porting_audio_mixer_open(IPANEL_DEV_USE_MODE mode, IPANEL_AUDIO_MIXER_NOTIFY func);
INT32_T ipanel_porting_audio_mixer_close(UINT32_T handle);
IPANEL_XMEMBLK *ipanel_porting_audio_mixer_memblk_get(UINT32_T handle, UINT32_T size);
INT32_T ipanel_porting_audio_mixer_memblk_send(UINT32_T handle, IPANEL_XMEMBLK *pcmblk);
INT32_T ipanel_porting_audio_mixer_ioctl(UINT32_T handle, IPANEL_MIXER_IOCTL_e op, VOID *arg);

#ifdef __cplusplus
}
#endif

#endif//_IPANEL_MIDDLEWARE_PORTING_SOUND_API_FUNCTOTYPE_H_
