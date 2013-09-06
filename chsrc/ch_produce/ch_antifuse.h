/*******************ͷ�ļ�˵��ע��**********************************/
/*��˾��Ȩ˵������Ȩ��2007���鳤������Ƽ����޹�˾���С�           */
/*�ļ�����ch_antifuse.h                                              */
/*�汾�ţ�V1.0                                                     */
/*���ߣ�  �����                                                   */
/*�������ڣ�2007-07-20                                             */
/*ͷ�ļ����ݸ�Ҫ����������/�����ȣ���                              */
/*                                                                 */
/*�޸���ʷ���޸�ʱ��    ����      �޸Ĳ���       ԭ��              */
/*                                                                 */
/*******************************************************************/
/*****************�������벿��**************************************/
#ifndef CH_ANTIFUSE
#define CH_ANTIFUSE
/*******************************************************************/
/**************** #include ����*************************************/
#include "stddef.h"
#include "stlite.h"
/*******************************************************************/
/**************** #define ����**************************************/



/*******************************************************************/
/**************** #typedef �������� ********************************/




/*******************************************************************/
/***************  �ⲿ������������ *********************************/
extern ST_Partition_t *NcachePartition ;
/*******************************************************************/
/***************  �ⲿ������������ *********************************/
extern BOOL    CH_Secure_Init( void )   ;
extern boolean CH_EnableBootAuthenticaion( void );
extern boolean CH_EnableJTAGLOCK( void );
extern boolean CH_EnableOTPBootCode( void );
extern boolean CH_BootAuthenticaionStatus( void );
extern boolean CH_JTAGLOCKStatus(void);
extern boolean CH_OTPStatus( void );
extern boolean CH_GetNUID(U8* puc_NuIDKey,U32 *pui_CRC);
extern U32 CH_GetNUIDKey( void );
extern void CH_GetPairCASerial(U8 *CASerial);
extern void make_crc_table_ISO_3309(void);
extern unsigned long crc_ISO_3309( unsigned char *buf, int len);
/*******************************************************************/
#endif

