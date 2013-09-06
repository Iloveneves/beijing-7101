/*******************�ļ�˵��ע��************************************/
/*��˾��Ȩ˵������Ȩ��2007���鳤������Ƽ����޹�˾���С�           */
/*�ļ�����ch_antifuse.c                                            */
/*�汾�ţ�V1.0                                                     */
/*���ߣ�  �����                                                   */
/*�������ڣ�2007-07-20                                             */
/*ģ�鹦��:
  STi7109/710 ��ȫ��������                                         */
/*��Ҫ����������:                                                  */
/*                                                                 */
/*�޸���ʷ���޸�ʱ��    ����      �޸Ĳ���       ԭ��              */
/*          20070720    zxg       ��STi5107��ֲ��STi7101ƽ̨       */
/*******************************************************************/

/**************** #include ����*************************************/
#include <stdlib.h>
#include <stdio.h>
#include "stlite.h"
#include "stsectoolfuse.h"
#include "stsectoolnuid.h"
#include "ch_antifuse.h"
#include "..\ch_flash\ch_flashmid.h"
#include "..\ch_tkdma\sttkdma_m1.h"/* zq add 20090906*/
/******************************************************************/
/**************** #define ����*************************************/

#define PAIRING_ADDR      0xA003FC00         /*���KEY���ݴ�ŵ�ַ*/

/***************  ȫ�ֱ������岿�� ********************************/

static boolean gb_Securinited=false;
/* ���ܣ� �жϰ�ȫģ���Ƿ��ʼ��                                 */
/* ֵ�� [fase,true]                                            */
/* Ӧ�û�����1:     CH_Secure_Init��void���е���                 */
/* ע�������                                                  */


/***************************�����嶨��*****************************/ 
/******************************************************************/
/*������:BOOL CH_Secure_Init( void )                              */
/*�����˺Ϳ���ʱ��:zengxianggen 2007-07-20                        */
/*������������:��ʼ��STi7101/7109��ȫģ���ʼ��                   */
/*����ԭ��˵��:                                                   */
/*�����������                                                    */
/*�������: ��                                                    */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                      */
/*���õ���Ҫ�����б�                                            */
/*����ֵ˵����TRUE, ʧ��,FALSE,�ɹ�                               */   
/*����ע������:                                                   */
/*����˵��:                                                       */      
BOOL CH_Secure_Init( void )      
{
	boolean b_result;
	if(gb_Securinited == false)
	{
     b_result = CH_Antifuse_Init();
     b_result += CH_NUID_Init();
	 gb_Securinited = true;
	}
	return b_result;
}
/* zq add 20090906*/
BOOL CH_Secure_Term(void)
{
	boolean b_result;
	if(gb_Securinited == true)
	{
		b_result = CH_Antifuse_Term();
                b_result += CH_NUID_Term();
		gb_Securinited = false;
	}
	return b_result;
}
/***********/
/******************************************************************/
/*������:BOOL CH_Antifuse_Init��void��                            */
/*�����˺Ϳ���ʱ��:zengxianggen 2007-07-20                        */
/*������������:��ʼ��STi7101/7109 fuseģ��                        */
/*����ԭ��˵��:                                                   */
/*�����������                                                    */
/*�������: ��                                                    */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                      */
/*���õ���Ҫ�����б�                                            */
/*����ֵ˵����TRUE, ʧ��,FALSE,�ɹ�                               */   
/*����ע������:                                                   */
/*����˵��:                                                       */     
BOOL CH_Antifuse_Init( void )
{
	ST_ErrorCode_t st_Error;
	st_Error = STSECTOOLFUSE_Init("SECTOOLFUSE", NULL, STSECTOOLFUSE_NOINT);
	if(st_Error == ST_NO_ERROR)
	{
       return false;
	}
	else
    	{
    		do_report(0,"\n<<<<<<<<<CH_Antifuse_Init  0x%x <<<<<<<<<\n",st_Error);
       		return true;
	}
}
/******************************************************************/
/*������:BOOL CH_Antifuse_WriteItem(STSECTOOLFUSE_ITEM_t st_item, U32 ui_value)*/
/*�����˺Ϳ���ʱ��:zengxianggen 2007-07-20                        */
/*������������:����д��Ӧ���ݵ�sti7101/7019 fuse                  */
/*����ԭ��˵��:                                                   */
/*���������STSECTOOLFUSE_ITEM_t st_item,��Ҫд�������������
            U32 ui_value,��Ҫд�������                           */
/*�������: ��                                                    */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                      */
/*���õ���Ҫ�����б�                                            */
/*����ֵ˵����TRUE, ʧ��,FALSE,�ɹ�                               */   
/*����ע������:                                                   */
/*����˵��:                                                       */     
BOOL CH_Antifuse_WriteItem(STSECTOOLFUSE_ITEM_t st_item, U32 ui_value)
{
	ST_ErrorCode_t st_Error;
	U32 ui_data;

	/* ��ʼ OTP ���� */
	st_Error = STSECTOOLFUSE_StartOTP();
	if(st_Error != ST_NO_ERROR)
	{
		do_report(0,"StartOTP error\n");
		return true;
	}
	/* ��ȡԭ�������� */
	st_Error = STSECTOOLFUSE_ReadItem(st_item, &ui_data);
    if(st_Error != ST_NO_ERROR)
	{
		do_report(0,"ReadItem error\n");
		return true;
	}
	/* ����д���� */
	st_Error = STSECTOOLFUSE_PermanentWriteEnable(st_item);
	 if(st_Error != ST_NO_ERROR)
	{
		do_report(0,"PermanentWriteEnable error\n");
		return true;
	}
	/* д���Ӧ������ */
	st_Error = STSECTOOLFUSE_WriteItem(st_item, ui_value);
    if(st_Error != ST_NO_ERROR)
	{
		do_report(0,"WriteItem error\n");
		return true;
	}
	return false;
}
/**********************************************************************/
/*������:boolean CH_EnableBootAuthenticaion( void )                   */
/*�����˺Ϳ���ʱ��:zengxianggen 2007-07-20                            */
/*������������:�򿪿�����ȫУ��״̬����                               */
/*����ԭ��˵��: 
crypt_sigchk_enable, 1 bit, read/write (0 to 1)                       */
/*�����������                                                        */
/*�������: ��                                                        */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                          */
/*���õ���Ҫ�����б�                                                */
/*����ֵ˵��:TRUE,����ʧ��,FALSE,�����ɹ�                             */   
/*����ע������:                                                       */
/*����˵��:                                                           */  
boolean CH_EnableBootAuthenticaion( void )
{
   /*����Ѿ��򿪸ù���,��ִ�в���*/
   if(CH_BootAuthenticaionStatus() == true)
   {
         return true;
   }

   return CH_Antifuse_WriteItem(STSECTOOLFUSE_CRYPT_SIGCHK_ENABLE_OTP_ITEM,0x01 );
}
/**********************************************************************/
/*������:boolean CH_EnableJTAGLOCK( void )                            */
/*�����˺Ϳ���ʱ��:zengxianggen 2007-07-20                            */
/*������������:��JTAG��������                                       */
/*����ԭ��˵��:                                                      
jtag_prot, 4 bits, read/write (0 to 1): 
Indicates JTAG protection state. 
00xx=JTAG not protected; 
01xx or 10xx=JTAG locked (password required); 
11xx=JTAG disabled. (Bits 0,1 not used)                               
/*�����������                                                        */
/*�������: ��                                                        */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                          */
/*���õ���Ҫ�����б�                                                */
/*����ֵ˵��:TRUE,����ʧ��,FALSE,�����ɹ�                             */   
/*����ע������:                                                       */
/*����˵��:                                                           */  
boolean CH_EnableJTAGLOCK( void )
{
    /*����Ѿ��򿪸ù���,��ִ�в���*/
   if(CH_JTAGLOCKStatus() == true)
   {
         return true;
   }

   return CH_Antifuse_WriteItem(STSECTOOLFUSE_JTAG_PROT_OTP_ITEM,0x08 );
}
/**********************************************************************/
/*������:boolean CH_EnableOTPBootCode( void )                         */
/*�����˺Ϳ���ʱ��:zengxianggen 2007-07-20                            */
/*������������:��BOOTCODE OPT����                                   */
/*����ԭ��˵��:                                                       */
/*�����������                                                        */
/*�������: ��                                                        */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                          */
/*���õ���Ҫ�����б�                                                */
/*����ֵ˵��:TRUE,����ʧ��,FALSE,�����ɹ�                             */   
/*����ע������:                                                       */
/*����˵��:                                                           */  
boolean CH_EnableOTPBootCode( void )
{
#if 0
	/*return 0	;*/
	if(CH_FLASH_GetFLashType()==FLASH_ST_M28||CH_FLASH_GetFLashType()==FLASH_ST_M29)/*ST FLASH*/
	{
		;
	}
	else
	{
		CH_LDR_7101SpansionOTPLock();
	}
#else
       CH_LDR_EnableFirst2BlockOTP();
#endif	
}

/**********************************************************************/
/*������:boolean CH_JTAGLOCKStatus( void )                            */
/*�����˺Ϳ���ʱ��:zengxianggen 2007-07-20                            */
/*������������:�õ�JTAG�Ƿ�����                                       */
/*����ԭ��˵��:                                                       */
/*�����������                                                        */
/*�������: ��                                                        */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                          */
/*���õ���Ҫ�����б�                                                */
/*����ֵ˵��:TRUE,JTAG����,FALSE,JTAGû������                         */   
/*����ע������:                                                       */
/*����˵��:                                                           */  
boolean CH_JTAGLOCKStatus( void )
{
	ST_ErrorCode_t st_Error;
	U32 ui_data;

	/* ��ʼ OTP ���� */
	st_Error = STSECTOOLFUSE_StartOTP();
	if(st_Error != ST_NO_ERROR)
	{
		do_report(0,"StartOTP error\n");
		return true;
	}

   	/* ��ȡ���� */
	st_Error = STSECTOOLFUSE_ReadItem(STSECTOOLFUSE_JTAG_PROT_OTP_ITEM, &ui_data);
    if(st_Error != ST_NO_ERROR)
	{
		do_report(0,"ReadItem error\n");
		return false;
	}
	if(((ui_data>>2)&0x03) == 0x00)
	{
       return false;
	}
	else
	{
       return true;
	}	
}

/**********************************************************************/
/*������:boolean CH_BootAuthenticaionStatus(void)                     */
/*�����˺Ϳ���ʱ��:zengxianggen 2007-07-20                            */
/*������������:�õ�������ȫУ��״̬                                   */
/*����ԭ��˵��:                                                       */
/*�����������                                                        */
/*�������: ��                                                        */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                          */
/*���õ���Ҫ�����б�                                                */
/*����ֵ˵��:TRUE,,������Ҫ��ȫУ��״̬,FALSE,��������Ҫ��ȫУ��״̬  */   
/*����ע������:                                                       */
/*����˵��:                                                           */                                             
boolean CH_BootAuthenticaionStatus(void)
{
	ST_ErrorCode_t st_Error;
	U32 ui_data;

	/* ��ʼ OTP ���� */
	st_Error = STSECTOOLFUSE_StartOTP();
	if(st_Error != ST_NO_ERROR)
	{
		do_report(0,"StartOTP error\n");
		return true;
	}
   	/* ��ȡ���� */
	st_Error = STSECTOOLFUSE_ReadItem(STSECTOOLFUSE_CRYPT_SIGCHK_ENABLE_OTP_ITEM, &ui_data);
    if(st_Error != ST_NO_ERROR)
	{
		do_report(0,"ReadItem error\n");
		return false;
	}
	
	if(ui_data == 0x00)
	{
		return false;
	}
	else
	{
		return true;
	}
}
/**********************************************************************/
/*������:boolean CH_OTPStatus( void )                                 */
/*�����˺Ϳ���ʱ��:zengxianggen 2007-07-20                            */
/*������������:�õ�BOOTCODE�����Ƿ�OTP                                */
/*����ԭ��˵��:                                                       */
/*�����������                                                        */
/*�������: ��                                                        */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                          */
/*���õ���Ҫ�����б�                                                */
/*����ֵ˵��:TRUE,BOOTCODE�Ѿ�OTP,FALSE,BOOTCODEû��OTP               */   
/*����ע������:                                                       */
/*����˵��:                                                           */   
boolean CH_OTPStatus( void )
{
#if 0
   /*return 0;*/
	 if(CH_FLASH_GetFLashType()==FLASH_ST_M28||CH_FLASH_GetFLashType()==FLASH_ST_M29)/*ST FLASH*/
	 {

	 }
     else/*SPANSION FLASH*/
	 {
		 return CH_LDR_7101SpansionOTPLockStatus();
	 }
#else
return CH_LDR_GetFirst2BlockOTPStatus();
#endif	 
}
/******************************************************************/
/*������:BOOL CH_NUID_Init��void��                                */
/*�����˺Ϳ���ʱ��:zengxianggen 2007-07-20                        */
/*������������:��ʼ��STi7101/7109 NUID                            */
/*����ԭ��˵��:                                                   */
/*�����������                                                    */
/*�������: ��                                                    */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                      */
/*���õ���Ҫ�����б�                                            */
/*����ֵ˵����TRUE, ʧ��,FALSE,�ɹ�                               */   
/*����ע������:                                                   */
/*����˵��:                                                       */ 
BOOL CH_NUID_Init( void )
{	
	ST_ErrorCode_t st_Error=ST_NO_ERROR;

	st_Error = STSECTOOLNUID_Init("NUID_Init",NULL,-1);
	if(st_Error == ST_NO_ERROR)
	{
		return false;
	}
	else
    {
    		do_report(0,"\n<<<<<<<<<CH_NUID_Init  0x%x <<<<<<<<<\n",st_Error);
		return true;
	}	
}

BOOL CH_NUID_Term(void)
{
    boolean b_result;

    b_result = STSECTOOLNUID_Term("NUID_Init",NULL);
    return b_result;
}
/******************************************************************/
/*������:boolean CH_GetNUID(U8* puc_NuIDKey,U32 *pui_CRC)            */
/*�����˺Ϳ���ʱ��:zengxianggen 2007-07-20                        */
/*������������:�õ�STi7101/7109 NUID  CRC��Ϣ��NUID Key           */
/*����ԭ��˵��:                                                   */
/*�����������                                                    */
/*�������: U8* puc_NuIDKey,NUID KEY,
            U32 *pui_CRC,CRC����ָ��                              */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                      */
/*���õ���Ҫ�����б�                                            */
/*����ֵ˵������                                                  */   
/*����ע������:                                                   */
/*����˵��:                                                       */ 
boolean CH_GetNUID(U8* puc_NuIDKey,U32 *pui_CRC)
{
    int i_STRes = 0;
    U8  *puc_Buffer = NULL;
	U8  *puc_AlineBuffer = NULL;
	U8  uc_NUIDBuf[16];
    U32 DevPubID;


	task_delay(1000);
	puc_Buffer = memory_allocate(NcachePartition,512);
	if(puc_Buffer != NULL)
	{
	  puc_AlineBuffer = (U8 *)((unsigned int)(puc_Buffer)/32*32+32);/*32 BYTE����*/
	  i_STRes = STSECTOOLNUID_GetId((U8 *)uc_NUIDBuf,pui_CRC,(U8* )puc_AlineBuffer);
	}
	else 
	  return false;

	task_delay(1000);

	DevPubID = CH_GetNUIDKey();
	//task_delay(ST_GetClocksPerSecond()/2);
#if 0	/*20070816 change*/
    puc_NuIDKey[12] = DevPubID&0xFF;
    puc_NuIDKey[13] = (DevPubID>>8)&0xFF;

	puc_NuIDKey[14] = (DevPubID>>16)&0xFF;
    puc_NuIDKey[15] = (DevPubID>>24)&0xFF;
#else

    puc_NuIDKey[15] = DevPubID&0xFF;
    puc_NuIDKey[14] = (DevPubID>>8)&0xFF;

	puc_NuIDKey[13] = (DevPubID>>16)&0xFF;
    puc_NuIDKey[12] = (DevPubID>>24)&0xFF;
#endif
	
	if(puc_Buffer != NULL)
	{
         	memory_deallocate(NcachePartition,puc_Buffer);
	}
	if(i_STRes != ST_NO_ERROR)
	{
		switch(i_STRes)
		{
		case ST_ERROR_BAD_PARAMETER:
			do_report(0,"STSECTOOLNUID_GetId ST_ERROR_BAD_PARAMETER\n");
			
			break;
		case STSECTOOLNUID_ERROR_OPTION_NOT_SUPPORTED:
			do_report(0,"STSECTOOLNUID_GetId STSECTOOLNUID_ERROR_OPTION_NOT_SUPPORTED\n");
			
			break;
		case STSECTOOLNUID_ERROR_COMMAND_FAILED:
			do_report(0,"STSECTOOLNUID_GetId STSECTOOLNUID_ERROR_COMMAND_FAILED\n");
			
			break;
        default:
			do_report(0,"STSECTOOLNUID_GetId UNKOW ERROR\n");
			break;
		}
		return false;
	}
     else
     	{
              return true;
     	}
	
}

ST_ErrorCode_t CH_GetNUIDFormTKDMA(U8* NUID)
{
	ST_ErrorCode_t  st_ErrorCode;
	STTKDMA_Key_t Key;
    
	st_ErrorCode = STTKDMA_ReadPublicID(&Key);

          memcpy(NUID,Key.key_data,16);

          return st_ErrorCode;
}

/******************************************************************/
/*������:U32 CH_GetNUIDKey( void )                                   */
/*�����˺Ϳ���ʱ��:zengxianggen 2007-07-20                        */
/*������������:�õ�STi7101/7109 unencrypted Nagra Unique ID.      */
/*����ԭ��˵��:                                                   */
/*�����������                                                    */
/*�������: ��                                                    */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                      */
/*���õ���Ҫ�����б�                                            */
/*����ֵ˵��������unencrypted Nagra Unique ID                     */   
/*����ע������:                                                   */
/*����˵��:��                                                     */ 
U32 CH_GetNUIDKey( void )
{
	return STSECTOOLNUID_GetNuid();
}

/*������:void CH_GetPairCASerial(U8 *CASerial)*/
/*�����˺Ϳ���ʱ��:zengxianggen 2007-01-18                       */
/*������������:�õ�CASerial��Ϣ                                  */
/*����ԭ��˵��:                                                  */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                     */
/*�����������                                                   */
/*�������: U8 *CASerial,CA���к�����                            */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                     */
/*���õ���Ҫ�����б�                                           */
/*����ֵ˵������                                                 */
/*����˵��:��                                                    */ 
void CH_GetPairCASerial(U8 *CASerial)
{
    memcpy(CASerial,(U8 *)(PAIRING_ADDR +0x04),4);
}


/*----------------------------------CRC32------------------------*/
unsigned long crc_table_ISO_3309[256];
/* Make the table for a fast CRC. */
void make_crc_table_ISO_3309(void)
{
	unsigned long c;
	int n, k;
	for (n = 0; n < 256; n++) {
		c = (unsigned long) n;
		for (k = 0; k < 8; k++) {
			if (c & 1) {
				c = 0xedb88320L ^ (c >> 1);
			} else {
				c = c >> 1;
			}
		}
		crc_table_ISO_3309[n] = c;
	}
}
unsigned long crc_ISO_3309( unsigned char *buf, int len)
{
	unsigned long c = 0 ^ 0xffffffffL;
	int n;
	make_crc_table_ISO_3309();
	for (n = 0; n < len; n++) {
		c = crc_table_ISO_3309[(c ^ buf[n]) & 0xff] ^ (c >> 8);
	}
	return c ^ 0xffffffffL;
}

/******************************************************************/
/*������:BOOL CH_Antifuse_Term��void��                            */
/*�����˺Ϳ���ʱ��:zengxianggen 2007-07-20                        */
/*������������:�ر�STi7101/7109 fuseģ��                        */
/*����ԭ��˵��:                                                   */
/*�����������                                                    */
/*�������: ��                                                    */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                      */
/*���õ���Ҫ�����б�                                            */
/*����ֵ˵����TRUE, ʧ��,FALSE,�ɹ�                               */   
/*����ע������:                                                   */
/*����˵��:                                                       */     
BOOL CH_Antifuse_Term( void )
{
	ST_ErrorCode_t st_Error;
	st_Error = STSECTOOLFUSE_Term("SECTOOLFUSE",NULL);
	if(st_Error == ST_NO_ERROR)
	{
       return false;
	}
	else
    {
       return true;
	}
}
