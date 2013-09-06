/*
 * =====================
 * MODIFICATION	HISTORY:
 * =====================
 *
 * Date	       Modification	    remarks									
 * ----	       ------------		--------

 * 2005-02-     changed by yxl  �γ�һ��NORMAL Release�İ汾         
						        �����BEIJING_DBASE��ʾGeHua Release ����ΪNORMAL Release
				          
*/


#include "Stddefs.h"
#include "..\report\report.h"
#include "string.h"
#include "Appltype.h"
#include "..\main\initterm.h"
#include "graphicmid.h"
#include "..\key\keymap.h"
#include "..\key\key.h"
#include "..\dbase\vdbase.h"
#include "Audio.h"
#include "..\dbase\st_nvm.h"   
#include "channelbase.h"

void CH_OpenMute(void);
void CH_CloseMute(void);

extern semaphore_t *pst_AVControlSema;
 int MuteX;            /*������ʾX����*/
 int MuteY;            /*������ʾY����*/
#define MuteWidth	221    /*25*3        /*������ʾ���*/
#define MuteHeight	25       /*������ʾ�߶�*/
/*static wz 20071119 del*/U8  *MuteData=NULL;   /*������ʾ�ڴ�����*/

STAUD_Stereo_t  Audio_mode;
BOOLEAN		bAudioMuteState = FALSE;

static U16 MaxAttenuation;
static BOOL AudioCapability_Received=FALSE;


/* ��������:��������������˥��ֵ
 *����:
 		left_volume:����������ֵ
 		right_volumr:����������ֵ
 *����ֵ:��
*/
void mpeg_set_audio_volume_LR ( U16 left_volume, U16 right_volume )
{	
	STAUD_Attenuation_t Attenuation;	
	memset((char *)&Attenuation,0,sizeof(STAUD_Attenuation_t));
	

      left_volume = left_volume ;

	if( left_volume > 63 )
		left_volume = 63;
	
	if( right_volume > 63 )
		right_volume = 63;

	semaphore_wait(pst_AVControlSema);/*20070917 add*/

       STAUD_GetAttenuation(AUDHandle,&Attenuation);

	Attenuation.Left = 63-left_volume;       /* Convert to attenuation   */
	Attenuation.Right =63-right_volume;      
	
	
#if 1
	Attenuation.LeftSurround  =  63-left_volume;
	Attenuation.RightSurround = 63-left_volume;
	Attenuation.Center        =     63-left_volume;
	Attenuation.Subwoofer     =   63-left_volume;

	Attenuation.CsLeft=63-left_volume;
	Attenuation.CsRight=63-left_volume; 
	Attenuation.VcrLeft=63-left_volume;
	Attenuation.VcrRight=63-left_volume;

	
#endif
	STAUD_SetAttenuation( AUDHandle, Attenuation );
	semaphore_signal(pst_AVControlSema);/*20070917 add*/

	do_report(0,"Attenuation.Right=%d\n",Attenuation.Right);/**/


}



/*yxl  2005-02-28 add this function ,make it as a interface function between different TV station*/
void main_set_audio_volume_LR( U16 left_volume, U16 right_volume)
{

	mpeg_set_audio_volume_LR( left_volume, right_volume );
}
/*end yxl  2005-02-28 add this function,make it as a interface function between different TV station*/


/* ��������:������������������ֵΪ0
  * ����:��
  * ����ֵ:��
*/
void CH6_MuteAudio(void)
{
  ST_ErrorCode_t error;
 /*20060228 add for Mute*/
  CH_OpenMute();
  /**********************/
	 /*mpeg_set_audio_volume_LR(0, 0); */
	
	error =STAUD_Mute(AUDHandle,TRUE,TRUE);
	if ( error != ST_NO_ERROR )
		STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "STAUD_Mute=%s", GetErrorText(error) ));
}




/*��������:ȡ��������������������ֵΪԭֵ
 *����:��
 *����ֵ:��
*/
void CH6_UnmuteAudio(void)
{
       ST_ErrorCode_t error;
	error = STAUD_Mute(AUDHandle,FALSE, FALSE);
	if ( error != ST_NO_ERROR )
	STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "STAUD_Mute=%s", GetErrorText(error)));


       mpeg_set_audio_volume_LR(pstBoxInfo->GlobalVolume,pstBoxInfo->GlobalVolume);
	  /*20051012 add for UnMute*/
  CH_CloseMute();
  /**********************/

}




STPIO_Handle_t MuteHandle;
BOOL CH_MuteInit(void)
{
	STPIO_OpenParams_t OpenParams;
    ST_ErrorCode_t ErrCode;

    OpenParams.ReservedBits =PIO_BIT_1 ;
	OpenParams.BitConfigure[1] = STPIO_BIT_OUTPUT;    

    OpenParams.IntHandler = NULL;

    ErrCode = STPIO_Open(PIO_DeviceName[5], &OpenParams, &MuteHandle);
    if (ErrCode != ST_NO_ERROR)
    {
        STTBX_Print(("STPIO_Open ()=%s\n", GetErrorText(ErrCode)));
		return TRUE;
    }
	return FALSE;	 	
}

void CH_OpenMute(void)
{
#if 1
  STPIO_Set(MuteHandle, PIO_BIT_1);
    task_delay(10000);
 /*	 STPIO_Write(MuteHandle,0xFF);*/
#endif
}


void CH_CloseMute(void)
{
#if 1
	STPIO_Clear(MuteHandle, PIO_BIT_1);
	  task_delay(10000);
/*	STPIO_Write(MuteHandle,0x00);*/
#endif
}

#ifdef USE_IPANEL
STPIO_Handle_t ExigencyBroadcastHandle;
BOOL CH_ExigencyBroadInit(void)
{
	STPIO_OpenParams_t OpenParams;
    ST_ErrorCode_t ErrCode;

    OpenParams.ReservedBits =PIO_BIT_0 ;
	OpenParams.BitConfigure[0] = STPIO_BIT_OUTPUT;    

    OpenParams.IntHandler = NULL;

    ErrCode = STPIO_Open(PIO_DeviceName[5], &OpenParams, &ExigencyBroadcastHandle);
    if (ErrCode != ST_NO_ERROR)
    {
        STTBX_Print(("STPIO_Open ()=%s\n", GetErrorText(ErrCode)));
		return TRUE;
    }
	return FALSE;	 	
}

void CH_OpenExigencyBroad(void)
{
  STPIO_Set(ExigencyBroadcastHandle, PIO_BIT_0);
    task_delay(10000);
}


void CH_CloseExigencyBroad(void)
{
	STPIO_Clear(ExigencyBroadcastHandle, PIO_BIT_0);
	  task_delay(10000);
}
#endif

/*������:void CH_SetAudioModeOnly(char AudioMode)                        */
/*�����˺Ϳ���ʱ��:                                                          */
/*������������:  ������������ģʽ                      */
/*����ԭ��˵��:                                                              */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                                 */
/*���õ���Ҫ�����б�                                                       */
/*����ֵ˵����0���ɹ���-1��ʧ��                                              */

void CH_SetAudioModeOnly(char AudioMode)
{

	
	STAUD_Stereo_t StereoMode;
	ST_ErrorCode_t   st_Result;


	if(AudioMode==0)
	{
		StereoMode=STAUD_STEREO_DUAL_MONO;
	}
	else if(AudioMode==1)
	{
		StereoMode=STAUD_STEREO_DUAL_RIGHT;
	}
	else if(AudioMode==2)
	{
		StereoMode=STAUD_STEREO_DUAL_LEFT;
	}
	else 
	{
		StereoMode=STAUD_STEREO_STEREO;
	}
 
	st_Result=STAUD_SetStereoOutput(AUDHandle,StereoMode);
    
}

/*--eof------------------------------------------------------------------------*/


