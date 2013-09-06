/*
*  Copyright (c) 2007,  ��������Ƽ��������ι�˾
*  All rights reserved.
*  
*  �ļ����ƣ�QAM0297.H
*  �ļ���ʶ��
*  ժ           Ҫ��  TUNER�ӿں�����������
*  
*  ��ǰ�汾��1.0
*  ��           �ߣ������  
*  ������ڣ�2007-06-15
*/




#ifndef _QAM0297_H_
#define _QAM0297_H_

#include "stddefs.h"

/**************************************************************
Fuction Name:
	ch_QAM0297E_SetParameter

Purpose:


Using the globle and struct:

Input parameter:
	ri_FrequencyKHz		Frequency in Khz
	ri_SymbolRateKHz		SymbolRate in KHz

Output parameter:
	NULL

Return Status:
	TRUE	FAILED
	FALSE	SUCCESFULL

Note:
	NULL

**************************************************************/
extern int  ch_QAM0297E_SetParameter (int ri_FrequencyKHz, int ri_SymbolRateKBds, int ri_QamMode, int ri_IQInversion);

/**************************************************************
Fuction Name:
	ch_TUNER_Qam0297_Init

Purpose:


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
extern BOOL ch_TUNER_Qam0297_Init(void);

#endif



