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

#ifndef _IPANEL_MIDDLEWARE_PORTING_NVRAM_API_FUNCTOTYPE_H_
#define _IPANEL_MIDDLEWARE_PORTING_NVRAM_API_FUNCTOTYPE_H_

#include "ipanel_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    IPANEL_NVRAM_DATA_BASIC,		/* ����FFS, ������Ҫ���������仯������(��, Ƶ���������) */
    IPANEL_NVRAM_DATA_SKIN,			/* Skinר�� */
    IPANEL_NVRAM_DATA_THIRD_PART,	/* ���������� */
    IPANEL_NVRAM_DATA_QUICK,		/* ����д���NVRAM */
    IPANEL_NVRAM_DATA_APPMGR,   	/* ��������Ӧ�ó��� */
    IPANEL_NVRAM_DATA_USERDEF,    	/* �����м�����ص��û��Զ�������*/
    IPANEL_NVRAM_DATA_AUX,			/* ����FFS, ������Զ�ʧ�Ҿ���д������(��, cookie) */
	IPANEL_NVRAM_DATA_BOOT,			/* BOOT���� */
	IPANEL_NVRAM_DATA_LOADER,		/* LOADER���� */
	IPANEL_NVRAM_DATA_ROOTFFS,		/* ROOTFFS */
	IPANEL_NVRAM_DATA_SYSTEM,		/* �ں˻����ϵͳ */
	IPANEL_NVRAM_DATA_APPSOFT,		/* APPSOFT */
    IPANEL_NVRAM_DATA_UNKNOWN
} IPANEL_NVRAM_DATA_TYPE_e;

typedef enum
{
    IPANEL_NVRAM_FAILED = -1,
    IPANEL_NVRAM_BURNING = 0,
    IPANEL_NVRAM_SUCCESS,
    IPANEL_NVRAM_UNKNOWN
} IPANEL_NVRAM_STATUS_e;

typedef enum
{
    IPANEL_NVRAM_BURN_DELAY = 0, /* traditional mode: burn by the FLASH process */
    IPANEL_NVRAM_BURN_NOW		 /* burn into FLASH immediately */
} IPANEL_NVRAM_BURN_MODE_e;

/* NVRAM information, address and size, usually is FLASH memory */
INT32_T ipanel_porting_nvram_info(BYTE_T **addr, INT32_T *numberofsections,
                                  INT32_T *sect_size, INT32_T flag);

/* read NVRAM data */
INT32_T ipanel_porting_nvram_read(UINT32_T flash_addr, BYTE_T *buff_addr, 
                                  INT32_T len);

/* write data to NVRAM */
INT32_T ipanel_porting_nvram_burn(UINT32_T flash_addr, CONST CHAR_T *buf_addr,
                                  INT32_T len, IPANEL_NVRAM_BURN_MODE_e mode);

/* erase NVRAM section(usually at least 64K) */
INT32_T ipanel_porting_nvram_erase(UINT32_T flash_addr, INT32_T len);

/* return the status of the NVRAM block: IPANEL_NVRAM_SUCCESS(1) means finished. */
INT32_T ipanel_porting_nvram_status(UINT32_T flash_addr, INT32_T len);


/*
** EEPROM APIs =================================================================
*/
/* EEPROM information, address and size */
INT32_T ipanel_porting_eeprom_info(BYTE_T **addr, INT32_T *size);
/* read EEPROM data */
INT32_T ipanel_porting_eeprom_read(UINT32_T eeprom_addr, BYTE_T *buf_addr, INT32_T len);
/* write data to EEPROM */
INT32_T ipanel_porting_eeprom_burn(UINT32_T eeprom_addr, CONST BYTE_T *buf_addr, INT32_T len);

#ifdef __cplusplus
}
#endif

#endif//_IPANEL_MIDDLEWARE_PORTING_NVRAM_API_FUNCTOTYPE_H_
