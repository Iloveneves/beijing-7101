/*
STB2TSM.h
����Ҫ�������ģ��ʵ�ֵĽӿں���
*/

#ifndef SUMAVISION_STB_TO_SecTerminal_H
#define SUMAVISION_STB_TO_SecTerminal_H
#include "Appltype.h"

#ifndef BYTE 
#define BYTE unsigned char
#endif
#ifndef NULL
#define NULL 0
#endif
#ifndef false
#define false 0
#endif
#ifndef true
#define true 1
#endif
#ifndef WORD //16bit
#define WORD unsigned short
#endif
#ifndef HANDLE
#define HANDLE  long*
#endif


//�̺߳�����ַ����
typedef void (*pThreadFunc) (void *param);



#ifdef __cplusplus
extern "C" {
#endif

/*��ȡ�������ȫģ���flash�Ĵ�С�͵�ַ*/
int SumaSTBSecure_GetFlashAddr(int* plSize, int* plBaseAddr);

/*��flash*/
int SumaSTBSecure_ReadFlash(int lBeginAddr, int* plDataLen, BYTE* pData);
/*дflash*/
int SumaSTBSecure_WriteFlash(int lBeginAddr, int* plDataLen, const BYTE* pData);

/*��û����з���������ն˰�ȫģ�顱��ʹ�õ�OTP�Ļ���ַ�ʹ�С */
int SumaSTBSecure_GetOTPAddr(int* plSize, int* plBaseAddr);

/*��ȡ��ȫ���OTP���ݵ��ڴ�*/
int SumaSTBSecure_ReadOTP(int lBeginAddr, int* plDataLen, BYTE* pData);

/*��û����з���������ն˰�ȫģ�顱��ʹ�õ�EEPROM(�����������ݣ��Ǵ���)�Ļ���ַ�ʹ�С */
int SumaSTBSecure_GetEepromAddr(int* plSize, int* plBaseAddr);
/*��EEPROM���ڴ�*/
int SumaSTBSecure_ReadEeprom(int lBeginAddr, int* plDataLen, BYTE* pData);
/*д�ڴ浽EEPROM��*/
int SumaSTBSecure_WriteEeprom(int lBeginAddr, int* plDataLen, const BYTE* pData);


int SumaSTBSecure_SemaphoreInit( U32* pSemaphore ,U32 lInitCount);

/*�ź������źš�*/
int SumaSTBSecure_SemaphoreSignal( U32 lSemaphore);

/*�ȴ��ź���,�ȴ��ɹ�֮��,�ź���Ϊ���źš�*/
int  SumaSTBSecure_SemaphoreTimedWait(U32 lSemaphore,U32 lWaitCount);

/*��������*/
int SumaSTBSecure_CreateThread (const char * szName, pThreadFunc pTaskFun, int priority, void * param, int StackSize, U32*plThreadID);
/*���ٽ���*/
int  SumaSTBSecure_TerminateThread(U32 lThreadID);

/*===================== ��ȡ�����������Ϣ��غ��� ========================*/
/* ��ȡ������ID */
int SumaSTBSecure_GetSTBID(char* szSTBID, int* piLen);
/* ��ȡ������оƬID */
int SumaSTBSecure_GetChipID(char* szChipID, int* piLen);
/* ��ȡ�����е�ǰ���ܿ����� */
int SumaSTBSecure_GetCardID(char* szCardID, int* piLen);
/* ���ܣ�	��ȡ�����е�MAC��ַ */
int SumaSTBSecure_GetMacAddr(char* szMacAddr, int* piLen);

/*======================== ��ȡ��������ɵ��������� =====================*/

/*���ܣ���ȡ��������ʾʱ�䡢������cpu����������������һ���ڴ������*/
boolean SumaSTBSecure_GetSeeds(BYTE * pbyTime,BYTE * pbyTickCount,BYTE * pbyBuf,int nBufLen);

/*
���ܣ���ȡ�û�˽Կ��������
�����szUserPIN:	�û�˽Կ������������� 10 Bytes   
ʹ�ó�����

�����н��г�ʼ��ʱ����ȡ��ǰ���·�֤�飬ͬʱ�����·�����������Ƿ񵯳��˽�����ʾ�û�����������ڱ���˽Կ

*/
int SumaSTBSecure_GetUserPIN(char* szUserPIN);



/*===================== ����&��ʾ��Ϣ��غ��� =======================*/
/*���ܣ���ӡ������Ϣ��
Ҫ����������ֲʱ��ӡ������Ϣʹ�á���ֲ��Ϻ󣬸ú���ֱ�ӷ��ز����κδ���
������pszMsg:	������Ϣ���ݡ�*/
void SumaSTBSecure_AddDebugMsg(const char * szMsg, ... );

/*=====================UKeyʹ�õ���غ���===============================*/

HANDLE SumaSecure_OpenUKey(void);
int SumaSecure_WriteDataToUKey(HANDLE hUkey,BYTE * pbyinData,WORD *pwLen);
int  SumaSecure_ReadDataFromUKey(HANDLE hUkey,BYTE * pbyoutData,WORD *pwLen);


#ifdef __cplusplus
}
#endif

/*�������õ���FLASH��ַ*/
#define FLASH_SECURITY_ADDR   /*0xA11E0000 WZ ���SKIN��ĵ���011211*/ 0xA1F80000
#define MAX_SECURITY_FLASH_BLOCK 1


#define FLASH_SECURITYDATA_LEN  (128*1024)

#define FLASH_SECURITY_ADDR_SPARE /*0xA1200000 WZ ���SKIN��ĵ���011211*/ 0xA1FA0000


#define BIG_PINPIC_ADDR  /*0xA1220000 WZ ���SKIN��ĵ���011211*/ 0XA1100000
#define BIG_PINPIC_LEN 0x761ff

#define SMALL_PINPIC_ADDR  (BIG_PINPIC_ADDR+BIG_PINPIC_LEN+1)
#define SMALL_PINPIC_LEN  0X37e17

#define BIG_PINSTAR_ADDR  (SMALL_PINPIC_ADDR+SMALL_PINPIC_LEN+1)
#define BIG_PINSTAR_LEN	0X1B7

#endif
