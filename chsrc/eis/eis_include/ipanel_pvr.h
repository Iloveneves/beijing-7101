/******************************************************************************/
/*    Copyright (c) 2009 iPanel Technologies, Ltd.							  */
/*    All rights reserved. You are not allowed to copy or distribute          */
/*    the code without permission.                                            */
/*    This is the demo implenment of the Porting APIs needed by iPanel        */
/*    MiddleWare.                                                             */
/*    Maybe you should modify it accorrding to Platform.                      */
/*                                                                            */
/*    $author huanghm 2009/03/19											  */
/******************************************************************************/

#ifndef _IPANEL_MIDDLEWARE_PORTING_PVR_API_FUNCTOTYPE_H_
#define _IPANEL_MIDDLEWARE_PORTING_PVR_API_FUNCTOTYPE_H_

#include "ipanel_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* pvr file options */
typedef enum
{
	IPANEL_PVR_FILE_TIME = 0,
	IPANEL_PVR_FILE_SIZE
} IPANEL_PVR_FILE_e;

/* pvr ioctl options */
typedef enum
{
	IPANEL_PVR_SET_SERVICE_ID_PARAM, //����IPANEL_PVRSERVICE_T��Ϣ
	IPANEL_PVR_SET_RECORD_DURATION,
	IPANEL_PVR_SET_RECORD_TIME,
	IPANEL_PVR_GET_RECORD_TIME
} IPANEL_PVR_IOCTL_e;

#define PVR_MAX_NUM_CA 8

typedef struct tagPVR_STREAM_PARAM
{
	BYTE_T stream_type; 
	UINT16_T pid;
	UINT16_T component_tag;
	UINT32_T numEcmPIDs;							
	UINT16_T ecmPID[PVR_MAX_NUM_CA];
	UINT16_T CASystemId[PVR_MAX_NUM_CA];
	
}PVR_STREAM_PARAM;

typedef struct tagPVR_SERVICE_PARAM
{
	UINT32_T service_id; 
	UINT16_T pcr_pid;
	
	UINT32_T num_stream; //pid��Ŀ
	PVR_STREAM_PARAM *list_stream;
}PVR_SERVICE_PARAM;

/* �ص�����ԭ�� */
typedef VOID (*IPANEL_PVR_NOTIFY_FUNC)(UINT32_T fd, INT32_T event, void *param);

/* ���ûص����� */
INT32_T ipanel_pvr_set_notify(IPANEL_PVR_NOTIFY_FUNC func);

/* ����һ��PVRͨ��, �����豸���(IPANEL_NULL��ʾʧ��) */
UINT32_T ipanel_pvr_create_channel(IPANEL_ACC_TRANSPORT *param, UINT16_T service_id);

/* ����һ��PVRͨ�� */
INT32_T ipanel_pvr_destroy_channel(UINT32_T fd);

/* ����������� */
INT32_T ipanel_pvr_ioctl(UINT32_T fd, IPANEL_PVR_IOCTL_e op, VOID *arg);


/* ��ʼ¼�񣬷���IPANEL_OK��ʾ�ɹ�������Ϊ������ 
    filename [input]: ¼������ļ���������·����
	duration [input]: ¼��ʱ�䳤��(��λ��)������0xFFFFFFFF(-1)��ʾһֱ¼��ֱ��STOP��
				��������ֵ��ʾ�������ʱ��͸��ǹ�ʱ���ݣ�ʱ��Ҫ��
*/
INT32_T ipanel_pvr_start_record(UINT32_T fd, CHAR_T *filename);

/* ֹͣ¼�񲢹ر�¼���ļ�������IPANEL_OK��ʾ�ɹ�������Ϊ������ */
INT32_T ipanel_pvr_stop_record(UINT32_T fd);

/* ���¼������ */
INT32_T ipanel_pvr_clean_record(UINT32_T fd);

/* ȡ�õ�һ��¼���ļ��������ع��˾����IPANEL_NULL��ʾʧ�� */
UINT32_T ipanel_pvr_get_first_file(CONST CHAR_T *match, CHAR_T *filename, INT32_T length);

/* �ͷŹ��˾�� */
INT32_T ipanel_pvr_release_file(UINT32_T filter);

/* ȡ��¼���ļ���Ϣ����IPANEL_OK��ʾ�ɹ�������Ϊ������ */
INT32_T ipanel_pvr_get_file_info(CONST CHAR_T *filename, IPANEL_PVR_FILE_e type, VOID *arg);

INT32_T ipanel_pvr_init(void);
INT32_T ipanel_pvr_exit(void);

#ifdef __cplusplus
}
#endif

#endif//_IPANEL_MIDDLEWARE_PORTING_PVR_API_FUNCTOTYPE_H_
