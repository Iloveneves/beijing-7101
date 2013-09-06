/*=====================================================

File Name:
	CH_TUNER.H

Purpose:


Author:
	ZhouXuCheng

History:
	Date				Author			Actor
------------------------------------------------------
	2007-7-12		ZhouXuCheng		Create

======================================================*/



#ifndef	CH_TUNER_H_
#define	CH_TUNER_H_
/******************************Include files******************************/
#include "stddefs.h"
#include "..\dbase\vdbase.h"

/******************************Macro define******************************/


/*************************globle variable extern*************************/


/***************************Function externs***************************/


/**************************************************************
Fuction Name:
	CHDVBNewTuneReq

Author:
	ZhouXucheng

Purpose:
	Set the freqency and symbol

Input parameter:
	NULL
	
Output parameter:
	NULL
	
Return Status:
	NULL
**************************************************************/
void	CHDVBNewTuneReq ( TRANSPONDER_INFO_STRUCT *pstTransponderInfo );


/**************************************************************
Fuction Name:
	CH_TUNER_SetParameters

Purpose:


Using the globle and struct:

Input parameter:
	ri_FrequencyKHz:	Frequency in K Hz
	ri_SymbolRateKHz:	SymbolRate in K Hz
	ri_QamMode:		0 8 16 32 64 128 256
	ri_IQInversion:	0 or 1

Output parameter:
	NULL

Return Status:
	TRUE	FAILED
	FALSE	SUCCESFULL

Note:
	NULL

**************************************************************/
void CH_TUNER_SetParameters(int ri_FrequencyKHz, int ri_SymbolRateKHz, int ri_QamMode, int ri_IQInversion);



/**************************************************************
Fuction Name:
	CH_TUNER_IsLocked

Purpose:
	�жϸ�Ƶͷ�Ƿ�����

Using the globle and struct:

Input parameter:
	NULL

Output parameter:
	NULL

Return Status:
	TRUE	��Ƶͷ����״̬
	FALSE	��Ƶͷδ����״̬

Note:
	NULL

**************************************************************/
BOOL CH_TUNER_IsLocked(void);



/**************************************************************
Fuction Name:
	CH_TUNER_GetStrengthAndQuality

Purpose:
	�õ��ź�������ǿ�Ⱥ������ٷֱ���ʾ

Using the globle and struct:

Input parameter:
	*rpi_Strength	

Output parameter:
	����ֵ:  0-100 �ֱ����0% - 100%

Return Status:
	TRUE	FAILED
	FALSE	SUCCESFULL

Note:
	NULL

**************************************************************/
void CH_TUNER_GetStrengthAndQuality(int *rpi_Strength, int *rpi_Quality);



/**************************************************************
Fuction Name:
	CH_TUNER_GetSNR

Purpose:
	Get the tuner signal noise rate in the db value
	�õ��źŵ�����ȣ� ��λDB

Using the globle and struct:

Input parameter:
	NULL

Output parameter:
	NULL

Return Status:
	TRUE	FAILED
	FALSE	SUCCESFULL

Note:
	NULL

**************************************************************/
U32 CH_TUNER_GetSNR(void);




/**************************************************************
Fuction Name:
	CH_TUNER_GetRFLevel

Purpose:
	�õ��������ź�ǿ�ȣ��Ѿ������0-100֮���ֵ

Using the globle and struct:

Input parameter:
	NULL

Output parameter:
	NULL

Return Status:
	TRUE	FAILED
	FALSE	SUCCESFULL

Note:
	NULL

**************************************************************/
U32 CH_TUNER_GetRFLevel(void);



/**************************************************************
Fuction Name:
	CH_TUNER_GetBerRateString

Purpose:
	�õ������ʵ��ַ�������ʽ��xx E -4 �ṩ

Using the globle and struct:

Input parameter:
	NULL

Output parameter:
	NULL

Return Status:
	TRUE	FAILED
	FALSE	SUCCESFULL

Note:
	NULL

**************************************************************/
void CH_TUNER_GetBerRateString(char *rpuc_BerString);



/**************************************************************
Fuction Name:
	CH_TUNER_GetBerRate

Purpose:
	�õ��źŵ�������
	1 ���� 1�ĸ�4�η�
	10000 ����1

Using the globle and struct:

Input parameter:
	NULL

Output parameter:
	NULL

Return Status:
	TRUE	FAILED
	FALSE	SUCCESFULL

Note:
	NULL

**************************************************************/
float CH_TUNER_GetBerRate(void);


/**************************************************************
Fuction Name:
	CH_TUNER_Setup

Purpose:
	��ʼ����Ƶͷ���뱣֤�˺����еĲ����Ƿ���ȷ
	����:��ѯ��Ӳ������ʦ���õ�RF�ľ����ͺţ�Ȼ��
		���ͺ���ȷ���͸�CH_DVBTunerInit����

Using the globle and struct:
	gST_PTSema :	6964�Ŀ����źţ���Ҫ���ڿ��������Ƶ�״̬
	��ȷ���˱����ĳ�ʼ���ڵ��ô˺���֮ǰ

Input parameter:
	NULL

Output parameter:
	NULL

Return Status:
	TRUE	FAILED
	FALSE	SUCCESFULL

Note:
	NULL

**************************************************************/
int  CH_TUNER_Setup(void);


#endif


