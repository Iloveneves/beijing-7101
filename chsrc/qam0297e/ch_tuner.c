/*=====================================================
File Name:
	CH_TUNER.C

Purpose:


Author:
	ZhouXuCheng

History:
	Date				Author			Actor
------------------------------------------------------
	2007-7-12		ZhouXuCheng		Create

======================================================*/



/******************************Include files******************************/
#include "ch_tuner.h"
#include "ch_tuner_mid.h"

/******************************Macro define******************************/

/******************************struct define******************************/


/*************************globle variable define*************************/



/***************************Function defines***************************/






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
void	CHDVBNewTuneReq ( TRANSPONDER_INFO_STRUCT *pstTransponderInfo )
{
	int iFrequencyKHz;
	int iSymbolRateKHz;
	int iIQInversion;
	int iQamMode;

	iFrequencyKHz = pstTransponderInfo->iTransponderFreq;
	iSymbolRateKHz = pstTransponderInfo->iSymbolRate;
	iQamMode = pstTransponderInfo->ucQAMMode;
	iIQInversion = pstTransponderInfo->abiIQInversion;

	ch_Tuner_MID_SetParameter(iFrequencyKHz, iSymbolRateKHz, iQamMode, iIQInversion);
		
}

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
void CH_TUNER_SetParameters(int ri_FrequencyKHz, int ri_SymbolRateKHz, int ri_QamMode, int ri_IQInversion)
{
	ch_Tuner_MID_SetParameter(ri_FrequencyKHz, ri_SymbolRateKHz, ri_QamMode, ri_IQInversion);
}


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
BOOL CH_TUNER_IsLocked(void)
{
	return ch_Tuner_MID_StillLocked();
}


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
void CH_TUNER_GetStrengthAndQuality(int *rpi_Strength, int *rpi_Quality)
{
	ch_Tuner_MID_GetStrengthAndQuality(rpi_Strength, rpi_Quality);
}


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
U32 CH_TUNER_GetSNR(void)
{
	return ch_Tuner_MID_GetSNR();
}



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
U32 CH_TUNER_GetRFLevel(void)
{
	return ch_Tuner_MID_GetRFLevel();

}


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
void CH_TUNER_GetBerRateString(char *rpuc_BerString)
{	
	U32  ui_BerRate;
	
	ui_BerRate = ch_Tuner_MID_GetBerRate();
	sprintf(rpuc_BerString, "%d E -4", ui_BerRate);
}


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
float CH_TUNER_GetBerRate(void)
{
	U32  ui_BerRate;
	
	ui_BerRate = ch_Tuner_MID_GetBerRate();

	return (float)(ui_BerRate/10000);
}

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
int  CH_TUNER_Setup(void)
{
	return ch_DVBTunerInit(/*TUNER_PLL_DCT70700*/TUNER_PLL_CD11XX);
}

/*����Ϊ�˼���ԭ����API����*/

U8 CH_GetNoiseEstimator(void)
{
  /* FE_297e_SignalInfo_t TestSignalInfo;
      FE_297e_GetSignalInfo(myHandle,	&TestSignalInfo);*/
      int SignalQuality;
      int SignalStrength;
  
      CH_TUNER_GetStrengthAndQuality(&SignalQuality,&SignalStrength);
      return SignalQuality;

}
 
U8 CH_GetPowerEstimator(void)
{
   /*FE_297e_GetSignalInfo(myHandle,FE_297e_SignalInfo_t	*pInfo)*/
      int SignalQuality;
      int SignalStrength;
  
      CH_TUNER_GetStrengthAndQuality(&SignalQuality,&SignalStrength);
      return SignalStrength;
}




boolean CH_IsLocked(void)
{
  return CH_TUNER_IsLocked();
}

void CH_GetSignalinfo(int *SignalQuality, int *SignalStrength)
{
   CH_TUNER_GetStrengthAndQuality(SignalQuality,SignalStrength);
}

boolean SRTNR_IsLocked()
{
	return CH_IsLocked();
}


