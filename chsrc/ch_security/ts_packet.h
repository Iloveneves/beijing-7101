#ifndef __TSPACKET_H__
#define __TSPACKET_H__
#include "appltype.h"

#define SUMA_STB_GETTS_OK			0   /*�ɹ�*/
#define SUMA_STB_GETTS_FAILED		1   /*ʧ��*/
#define SUMA_STB_GETTS_LENERROR	2   /*���ȴ���*/
#define SUMA_STB_GETTS_GETTING     3   /*���ڻ�ȡ��*/


#define SECURITY_TSPACKET_LENGTH          188
#define CH_SECURITY_MAX_TSCAPTURE_NUM 10


typedef enum
{
SUMA_STB_GETTS_STOP = 0,         /*ֹͣ��ȡTS��*/
SUMA_STB_GETTS_START            /*��ʼ��ȡTS��*/
}tsm_porting_tsnotify_s;



typedef struct  TsCapturePara
{
  int i_frequency;      /*Ƶ��*/
  int i_symbol_rate;    /*������*/
  int i_modulation;     /*QAM*/
  int i_audiopid;        /*��Ƶpid*/
} CH_Security_TsCapturePara_t; 


typedef struct
{
	int  i_xptrnumber;
	CH_Security_TsCapturePara_t stru_CaptureParalist[CH_SECURITY_MAX_TSCAPTURE_NUM];
}CH_Security_tspacket_t;


#endif
