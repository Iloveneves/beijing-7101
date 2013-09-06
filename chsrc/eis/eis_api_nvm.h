/*
  * ===================================================================================
  * CopyRight By CHANGHONG NET L.T.D.
  * �ļ�: 	eis_api_nvm.h
  * ����: 	�������ô洢��صĽӿ�
  * ����:	 ��ս�ң�������
  * ʱ��:	2008-10-23
  * ===================================================================================
  */
#ifndef __EIS_API_NVM__
#define __EIS_API_NVM__

#define EIS_FLASH_BLOCK_SIZE		0x20000							/* 128K */
#define EIS_FLASH_BASE_ADDR		0xa0000000

#define EIS_FLASH_SKIN_SIZE               0x600000
#define EIS_FLASH_OFFSET_SKIN           0x1800000

#define EIS_FLASH_BASIC_SIZE		(EIS_FLASH_BLOCK_SIZE)/* BASIC	������FLASH�е�ƫ�Ƶ�ַ */
#define EIS_FLASH_APPMGR_SIZE	(EIS_FLASH_BLOCK_SIZE*8)/* APPMGR	������FLASH�е�ƫ�Ƶ�ַ */
#define EIS_FLASH_THIRDPART_SIZE	(EIS_FLASH_BLOCK_SIZE)/* ���������� */
#define EIS_FLASH_QUICK_SIZE	(EIS_FLASH_BLOCK_SIZE)/* ����д���NVRAM */

#define EIS_FLASH_OFFSET_BASIC		(0x1E00000)/* BASIC	������FLASH�е�ƫ�Ƶ�ַ *//*(0x1800000) WZ ���SKIN��ĵ���011211*/
#define EIS_FLASH_OFFSET_APPMGR	(EIS_FLASH_OFFSET_BASIC+EIS_FLASH_BASIC_SIZE)/* APPMGR	������FLASH�е�ƫ�Ƶ�ַ */
#define EIS_FLASH_OFFSET_THIRD_PART	(EIS_FLASH_OFFSET_APPMGR+EIS_FLASH_APPMGR_SIZE)/* ���������� */
#define EIS_FLASH_OFFSET_QUICK	(EIS_FLASH_OFFSET_THIRD_PART+EIS_FLASH_THIRDPART_SIZE)/* ����д���NVRAM */


#define EIS_FLASH_MAX_ADDR (EIS_FLASH_OFFSET_QUICK+EIS_FLASH_QUICK_SIZE)
typedef enum
{
	IPANEL_FLASH_STATUS_IDLE	= 0,		/* IDLE */
	IPANEL_FLASH_STATUS_ERASING,		/* ���ڲ��� */
	IPANEL_FLASH_STATUS_WRITING,	/* ����д�� */
	IPANEL_FLASH_STATUS_OK,			/* ������д��ɹ� */
	IPANEL_FLASH_STATUS_FAIL			/* ������д��ʧ�� */
}IPANEL_FLASH_STATUS;

#endif

/*--eof----------------------------------------------------------------------------------------------------*/

