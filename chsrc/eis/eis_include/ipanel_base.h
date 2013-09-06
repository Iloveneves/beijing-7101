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

#ifndef _IPANEL_MIDDLEWARE_PORTING_BASE_API_FUNCTOTYPE_H_
#define _IPANEL_MIDDLEWARE_PORTING_BASE_API_FUNCTOTYPE_H_

#include "ipanel_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* alloc memory function */
VOID *ipanel_porting_malloc(INT32_T memsize);

/* free memory function */
VOID ipanel_porting_free(VOID *ptr);

/* log output function */
INT32_T ipanel_porting_dprintf(CONST CHAR_T *fmt, ...);

/* iPanel middleware runtime base time, and unit is millisecond (1/1000 second) */
UINT32_T ipanel_porting_time_ms(VOID);

typedef enum
{
    IPANEL_SYS_INFO_NULL 				= 0,	/* ��Чȡֵ */
    IPANEL_LOADER_NAME 	 				= 1,  	/* ��ǰϵͳ���س�������� */
    IPANEL_LOADER_VERSION				= 2, 	/* ��ǰϵͳ���س���İ汾 */
    IPANEL_LOADER_PROVIDER				= 3,  	/* ��ǰϵͳ���س�����ṩ�� */
    IPANEL_LOADER_SIZE					= 4,  	/* ��ǰϵͳ���س���Ĵ�С */
    IPANEL_DRIVER_NAME					= 5,  	/* ��ǰ������������� */
    IPANEL_DRIVER_VERSION				= 6,  	/* ��ǰ��������İ汾 */
    IPANEL_DRIVER_PROVIDER				= 7, 	/* ��ǰ����������ṩ�� */
    IPANEL_DRIVER_SIZE					= 8,   	/* ��ǰ��������Ĵ�С */
    IPANEL_HARDWARE_SERIAL				= 9, 	/* Ӳ�����к� */
    IPANEL_HARDWARE_PROVIDER			= 10, 	/* Ӳ���ṩ�� */
    IPANEL_PRODUCT_DESC					= 11,  	/* ��Ʒ���� */
    IPANEL_PRODUCT_SERIAL				= 12,  	/* ��Ʒ���к� */
    IPANEL_PRODUCT_MAC_ADDR				= 13, 	/* ����MAC��ַ */
    IPANEL_PRODUCT_FLASH				= 14,  	/* ��Ʒ�����С */
    IPANEL_PRODUCT_RAM					= 15, 	/* ��Ʒ�ڴ��С */
    IPANEL_SMART_CARD_ID				= 16, 	/* ���ܿ�ID */
    IPANEL_PRODUCT_CA_NAME				= 17,  	/* ��ǰϵͳ��CA������ */
    IPANEL_PRODUCT_CA_VERSION			= 18,  	/* ��ǰϵͳ��CA�İ汾 */
    IPANEL_PRODUCT_CA_PROVIDER			= 19,  	/* ��ǰϵͳ��CA���ṩ�� */
    IPANEL_PRODUCT_CA_SIZE				= 20,  	/* ��ǰϵͳ��CA�Ĵ�С */
    IPANEL_PORTING_VERSION				= 21, 	/* ������Ŀ��Ҫporting�汾�� */
    IPANEL_SOFTWARE_VERSION				= 22,  	/* ������Ŀ��Ҫ��������İ汾�� */
    IPANEL_JFT_CARD_INFO				= 23,  	/* �θ�ͨˢ����Ϣ */
    IPANEL_JFT_CARD_STATUS				= 24, 	/* �θ�ͨˢ��״̬ */
    IPANEL_HARDWARE_PRODUCTIONBATCH		= 25, 	/* Ӳ����������*/  //Ϊ������Ŀ��ӵ��ر�꣬�����ʵ��
    IPANEL_SIHUA_REGION_ID 				= 26, 	/* û��������region id�������ʹ�õ�Ĭ��ֵ*/
    IPANEL_OC_SERVICE_ID				= 27,  	/* ��ʼOC��ʱ��Ӧ��service_id,DVB2.0��������֮������*/
    IPANEL_GD_TABLE_ID					= 28,	/* ���˽�б�table_id */
    IPANEL_GD_PID						= 29,	/* ���˽�б�pid */
    IPANEL_BOOT_TYPE					= 30,	/* ϵͳ��������:"warm":������,"cold":������ ,string ����,len<=8*/
    IPANEL_BOOT_STRING					= 31, 	/* ϵͳ����ʱ,��Ӧ��URL,string ����,len<=256*/
	IPANEL_PRODUCT_STARTUP_MODE			= 32,  	/* ��ȡiPanel����ģʽ --������ʵ�ֺ�ɾ��, porting guide��δ�����ö��ֵ*/
	IPANEL_PRODUCT_INI_FILENAME			= 33,  	/* ��ȡ�ļ����������ļ����ļ��� --������ʵ�ֺ�ɾ�� porting guide��δ�����ö��ֵ*/
    IPANEL_GET_TIME_ZONE				= 34,	/* ��ȡ�豸���ڵص�ʱ��,����Ϊ��λ��ʱ��*/
    IPANEL_GET_UTC_TIME					= 35,	/* ��ȡ��ǰʱ������Ƶĸ�������ʱ��,��ʼʱ��Ϊ1970��1��1��0��0��0��*/
	IPANEL_GET_START_FREQUENCY			= 36,	/* ��ȡ����Ƶ�㣬 DVB2.0������������Ҫ��*/
	IPANEL_HARDWARE_VERSION 			= 37,	/* ��ȡӲ���汾��*/
	IPANEL_LOADER_SYSIDENTIFICATION		= 38,	/* ��ȡloader system identification*/
	IPANEL_LOADER_KEYVERSION   			= 39,	/* ��ȡloader key version*/
	IPANEL_LOADER_SIGNATUREVERSION  	= 40,	/* ��ȡloader signature version*/
	IPANEL_LOADER_CSSN     				= 41,	/* ��ȡloader cssn*/
	IPANEL_LOADER_DOWNLOADID   			= 42,	/* ��ȡloader download Id*/
	IPANEL_MANUFACTURER_ID    			= 43,	/* ��ȡmanufacturer Id*/
	IPANEL_IRD_VARIANT     				= 44,	/* ��ȡIRD variant*/
	IPANEL_STB_PROVIDER_ID				= 45,	/* ��ȡSTB�ṩ��ID*/
	IPANEL_STB_MODEL_ID					= 46,	/* ��ȡSTB�ͺ�ID*/
	IPANEL_PRODUCT_CA_SYSTEM_ID			= 47,	/* ��ȡ����CA�ṩ��ID*/
	IPANEL_STB_GROUP_ID					= 48,	/* ��ȡ������group Id*/
	IPANEL_BOOT_VERSION					= 49,	/* BOOT�汾��*/
	IPANEL_ROOT_FFS_VERSION				= 50,	/* ROOT_FFS�汾��*/
	IPANEL_OS_VERSION					= 51,	/* ����ϵͳ�汾��*/
	IPANEL_SOFTWARE_RAM_SIZE			= 52,	/* ϵͳ����ʱ,����Լ�ȫ�ֱ�����ռ�þ�̬�ڴ��С*/
	IPANEL_DECODER_RAM_SIZE				= 53,	/* ϵͳ����ʱ,������ռ�ö�̬�ڴ��С*/
	IPANEL_STANDBY_FLAG					= 54,	/* ϵͳ����ʱ����ȡ�Ƿ���Ҫ��������Ĵ�����־λ��1��ʾ�����������״̬��0��ʾ�����󲻽�����״̬��*/
	IPANEL_SEARCH_FLAG					= 55,	/* ϵͳ����ʱ����ȡ�Ƿ���Ҫ��̨�ı�־λ��1��ʾ������0��ʾ��������*/
    IPANEL_CM_STATUS                    = 56,   /*��ȡ����CM����״̬��0:��ʾ����������CM��1:��ʾ������CM����������δ���ӣ�2:��ʾ������CM���������������ӡ�*/
	IPANEL_UPGRADE_FLAG					= 106,	/*ϵͳ����ʱ����ȡǰһ�ιػ�ǰ�����Ƿ�ɹ��ı�־λ:1��ʾ�����ɹ���0��ʾδ����*/
	IPANEL_SYS_INFO_UNKNOWN
} IPANEL_STB_INFO_e;
INT32_T ipanel_porting_system_get_info(IPANEL_STB_INFO_e name, CHAR_T *buf, INT32_T len);
INT32_T ipanel_porting_system_set_info(IPANEL_STB_INFO_e name, CHAR_T *buf, INT32_T len);

typedef enum 
{
    IPANEL_RESOUCE_FONT = 1,
    IPANEL_RESOUCE_UI	= 2
} IPANEL_RESOURCE_TYPE_e;
INT32_T ipanel_porting_get_outside_dat_info(CHAR_T **address, INT32_T *size, 
            IPANEL_RESOURCE_TYPE_e type);

INT32_T ipanel_porting_system_reboot(INT32_T s);
INT32_T ipanel_porting_system_standby(INT32_T s);

#ifdef __cplusplus
}
#endif

#endif//_IPANEL_MIDDLEWARE_PORTING_BASE_API_FUNCTOTYPE_H_
