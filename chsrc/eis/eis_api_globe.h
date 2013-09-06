/*
  * ===================================================================================
  * CopyRight By CHANGHONG NET L.T.D.
  * �ļ�: 	eis_api_globe.h
  * ����: 	����ȫ�ֱ������ⲿ����
  * ����:	������
  * ʱ��:	2008-10-22
  * ===================================================================================
  */

#ifndef __EIS_API_GLOBE__
#define __EIS_API_GLOBE__

#include "stddefs.h"
#include "graphicbase.h"
#include "stflash.h"

typedef enum
{
	Payment,
	SMS,
	Edu,
	WebTV,
	RevTV,
	KaraOke,
	VOD,
	MAIL,
	Lottery,
	TVBank,
	Gameon,
	Stock,
}IPANEL_IP_URI_TYPE;

extern ST_Partition_t   *gp_appl2_partition;			/* ��׳��Ҫ�õ����ڴ���� */
extern ST_Partition_t   *appl_partition;				/* ��׳��Ҫ�õ����ڴ���� */
extern ST_Partition_t   *SystemPartition;				/* ϵͳ�ڴ���� */
extern ST_Partition_t   *CHSysPartition;				/* ��ͼ�ڴ���� */

extern clock_t 		geis_reboot_timeout;		/* �����ж� */
extern CH_ViewPort_t 	CH6VPOSD;

extern boolean 	log_out_state;					/* �˳���־ */

extern STFLASH_Handle_t FLASHHandle;
extern void* 			pEisHandle;					/* ipanel�������� */
extern semaphore_t *gp_EisSema ;
extern void  CH_FlashLock(void);
extern void  CH_FlashUnLock(void);

extern void CH_PUB_UnLockFlash(U32 Offset);
extern void CH_PUB_LockFlash(U32 Offset);

#endif
/*--eof-----------------------------------------------------------------------------------------------------*/

