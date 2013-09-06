/*
  * ===================================================================================
  * CopyRight By CHANGHONG NET L.T.D.
  * �ļ�: 	eis_api_media.c
  * ����: 	ʵ��ý�������صĽӿ�
  * ����:	 ��ս�ң�������
  * ʱ��:	2008-10-24
  * ===================================================================================
  */

#include "eis_api_define.h"
#include "eis_api_globe.h"
#include "eis_api_debug.h"

#include "..\dbase\vdbase.h"
#include "channelbase.h"

#include "eis_include\ipanel_adec.h"
#include "eis_include\ipanel_aout.h"
#include "eis_include\ipanel_avm.h"
#include "eis_include\ipanel_media.h"
#include "eis_include\ipanel_mic.h"
#include "eis_include\ipanel_sound.h"
#include "eis_api_media.h"
#include "eis_api_osd.h"
#include "..\main\initterm.h"
#include "stddefs.h"
#include "stcommon.h"
#include "string.h"
#include "..\report\report.h"
#include "..\dbase\vdbase.h"
extern U16 vPid ,aPid,pcrPid;

typedef enum
{
	BRIGHTNESSGAIN, 
	CONTRASTGAIN,
	TINTANGLE,
	SATURATIONGAIN    
} PSI_ColorProcessing_t;

/*����spdif���ģʽ sqzow20100730*/
enum
{
	SPDIF_OUT_AC3,
	SPDIF_OUT_PCM
};
static int gi_SpdifOutMode = SPDIF_OUT_PCM;
message_queue_t  *gp_EisIFramePlay_Message = NULL;    
typedef struct eis_IFrame_s
{
	BOOL		IFrame_play_only;	
	CH6_VideoControl_Type VideoControl;
	BOOL AudioControl;
	CH6_AVControl_Type ControlType;	
}eis_IFrame_t;

#ifdef MP3_DATA	
/*MP3����״̬����*/
typedef enum
{
	MP3_STATUS_UNKOWN_E = 0,/*����״̬δ֪*/
	MP3_STATUS_PLAY_E,/*���ڲ���*/
	MP3_STATUS_PAUSE_E,/*��ͣ*/
	MP3_STATUS_STOP_E,/*ֹͣ*/
	MP3_STATUS_OVER_E,/*���Ÿոս���*/
	MP3_STATUS_FREE_E/*����*/
}ch_MP3status_e;

typedef void (*IPANEL_PLAYER_EVENT_NOTIFY)(UINT32_T player, INT32_T event, void *param);

#endif

typedef void (*player_event_notify)(UINT32_T player, INT32_T event, void *param);

void EIS_ShowIFrame(void);

int MP3_Open =  0; /* MP3�豸�Ƿ��Ѿ���1 Ϊ�Ѿ���*/

#ifdef MP3_DATA
ch_MP3status_e  MP3_play_state=MP3_STATUS_UNKOWN_E;
#endif

#ifdef IPANEL_AAC_FILE_PLAY

typedef enum
{
	AAC_STATUS_UNKOWN_E = 0,/*����״̬δ֪*/
	AAC_STATUS_PLAY_E,/*���ڲ���*/
	AAC_STATUS_PAUSE_E,/*��ͣ*/
	AAC_STATUS_STOP_E,/*ֹͣ*/
	AAC_STATUS_OVER_E,/*���Ÿոս���*/
	AAC_STATUS_FREE_E/*����*/
}ch_AACstatus_e;
ch_AACstatus_e  AAC_play_state=AAC_STATUS_UNKOWN_E;
int AAC_Open =  0; /* AAC�豸�Ƿ��Ѿ���1 Ϊ�Ѿ���*/
int AAC_handel=1001;

typedef void (*IPANEL_PLAYER_EVENT_NOTIFY)(UINT32_T player, INT32_T event, void *param);
IPANEL_AUDIO_MIXER_NOTIFY AAC_CallBackFunc;
#endif
//#define AV_SYN_NOPCR

STAUD_StreamContent_t gst_AudioType = STAUD_STREAM_CONTENT_MPEG1;
U8                                  gc_AudioMode   = 1;/*0=STAUD_STEREO_DUAL_MONO,1=STAUD_STEREO_DUAL_LEFT,2=STAUD_STEREO_DUAL_RIGHT,3=STAUD_STEREO_STEREO*/
U8                                  gc_VideoType     = MPEG2_VIDEO_STREAM;

static U8 * eis_IFrame_buf = NULL;/* ���ڱ��� I_FRAME����*/
static int 	  eis_IFrame_len;           /* ���ڱ��� I_FRAME���� ����*/
static clock_t eis_IFrame_time = 0;
static unsigned int  eis_VDECstart_time = 0;
boolean    gb_DisplayOtherIFram = true;

static       clock_t eis_AUDIOstart_time = 0;

static        int   gi_FirstAudioControl = true;
static        IPANEL_VDEC_IOCTL_e enum_LastVideoControl=IPANEL_VDEC_UNDEFINED;
static        IPANEL_ADEC_IOCTL_e enum_LastAudioControl=IPANEL_ADEC_UNDEFINED;
static        IPANEL_VDEC_IOCTL_e eis_actVideoControl;
static        IPANEL_ADEC_CHANNEL_OUT_MODE_e last_audio_mode=IPANEL_AUDIO_MODE_UNDEFINED;
static 	char gi_IFrameShow = false;/*i֡��ʾ״̬ sqzow20100723*/

//#define __EIS_API_MEDIA_DEBUG__


boolean EIS_ResumeAudioPlay(void);
U8 EIS_GetVIdeoType( void );

extern BOOLEAN		bAudioMuteState ; 
IPANEL_AUDIO_MIXER_NOTIFY MP3_CallBackFunc;
int MP3_handel=1000;
pIPANEL_XMEMBLK ipanel_pcmblk;

void CH_StopVidSlot(void)
{
	ST_ErrorCode_t  error;
	STVID_ClearParams_t clearParams;
	STPTI_SlotClearPid( VideoSlot );
	STPTI_SlotClearPid( PCRSlot );

	clearParams.ClearMode        = STVID_CLEAR_FREEING_DISPLAY_FRAME_BUFFER;
	clearParams.PatternAddress1_p = (void *)NULL;
	clearParams.PatternSize1      = 0;
	clearParams.PatternAddress2_p = (void *)NULL;
	clearParams.PatternSize2      = 0;

	STVID_Clear(VIDHandle[0], &clearParams);
	STVID_Clear(VIDHandle[1], &clearParams);
	task_delay((clock_t)(ST_GetClocksPerSecond()/10));

          
}
void CH_ClearVidBuf(void)
{
	ST_ErrorCode_t  error;
	STVID_ClearParams_t clearParams;
	clearParams.ClearMode        = STVID_CLEAR_FREEING_DISPLAY_FRAME_BUFFER;

	clearParams.PatternAddress1_p = (void *)NULL;
	clearParams.PatternSize1      = 0;
	clearParams.PatternAddress2_p = (void *)NULL;
	clearParams.PatternSize2      = 0;

	STVID_Clear(VIDHandle[0], &clearParams);
	task_delay((clock_t)(ST_GetClocksPerSecond()/10));

          
}

/*-------------------------------��������ע��ͷ--------------------------------------
    FuncName:CH_AudioDelayControl
    Purpose: ��̨���ӳ���ms�������������̨��״̬�л�ʱ�ı�������
    Reference: 
    in:
        bMute >> -1������ 1���� 0�رվ���
    out:
        none
    retval:
        success >> 
        fail >> 
   
notice : 
    sqZow 2010/07/30 10:50:10 
---------------------------------�����嶨��----------------------------------------*/
void CH_AudioDelayControl(int Mute)
{
#if 1 	/*sqzow20100730*/
	static U32 uiMuteTime = 0;
	static boolean bMuteStatus = false, bMuteControl = false;
	U32 uiTimeTemp;
	
	if(Mute == 1)
	{
		if(bMuteStatus == false)
		{
			CH6_MuteAudio();
			eis_report("\n�Ҿ�����");
			bMuteStatus = true;
		}
		bMuteControl = false;
	}
	else if(Mute == 0)
	{
		if(bMuteControl == false)
		{
			uiMuteTime = get_time_ms();
			bMuteControl = true;
		}
	}
	else
	{
		if(bMuteStatus == true
			&& bMuteControl == true)
		{
			uiTimeTemp = get_time_ms(); 
			if(uiTimeTemp < uiMuteTime
				|| uiTimeTemp - uiMuteTime > 1500)
			{
				if (bAudioMuteState == FALSE ) /*��̨���ӳٻָ���������ֹ���� sqzow20100706*/ 
				{
					CH6_UnmuteAudio();
					eis_report("\n�һָ�������");
				}
				bMuteStatus = false;
			}
		}
	}
#endif
}

void CH_StopAudSlot(void)
{
	STPTI_SlotClearPid( AudioSlot );
	STPTI_SlotClearPid( PCRSlot );
}
/*
����˵����
	��һ�����뵥Ԫ��
����˵����
	�����������
	�����������
�� �أ�
	����IPANEL_NULL���ɹ��������������
	����IPANEL_NULL��ʧ��
*/
UINT32_T ipanel_porting_adec_open(VOID)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_porting_adec_open " );
	#endif

	return IPANEL_OK;
}
/*
����˵����
	�ر�ָ���Ľ��뵥Ԫ��
����˵����
	���������
		decoder: Ҫ�رյĽ��뵥Ԫ�����
	�����������
	�� �أ�
		IPANEL_OK:�ɹ�;
		IPANEL_ERR:ʧ�ܡ�
*/
INT32_T ipanel_porting_adec_close(UINT32_T decoder)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_porting_adec_close " );
	#endif
	return IPANEL_OK;	
}

/*
����˵����
	��decoder ����һ�������������������úͻ�ȡdecoder �豸�Ĳ��������ԡ�
����˵����
	���������
		decoder: ���뵥Ԫ�����
		op �� ��������
		typedef enum
		{
			IPANEL_ADEC_SET_SOURCE =1,
			IPANEL_ADEC_START,
			IPANEL_ADEC_STOP,
			IPANEL_ADEC_PAUSE,
			IPANEL_ADEC_RESUME,
			IPANEL_ADEC_CLEAR,
			IPANEL_ADEC_SYNCHRONIZE,
			IPANEL_ADEC_SET_CHANNEL_MODE,
			IPANEL_ADEC_SET_MUTE,
			IPANEL_ADEC_SET_PASS_THROUGH,
			IPANEL_ADEC_SET_VOLUME,
			IPANEL_ADEC_GET_BUFFER_RATE
		} IPANEL_ADEC_IOCTL_e;
		arg �C �������������Ĳ�����������ö���ͻ�32 λ����ֵʱ��arg ��ǿ��ת��
		�ɶ�Ӧ�������͡�
		op, arg ȡֵ���淶�б�
	�����������
	�� �أ�
		IPANEL_OK:�ɹ�;
		IPANEL_ERR:ʧ��
*/
INT32_T ipanel_porting_adec_ioctl(UINT32_T decoder, IPANEL_ADEC_IOCTL_e op, VOID *arg)
{
	INT32_T ret = IPANEL_OK ; 
	UINT32_T oparg = (UINT32_T)arg;
	boolean AudioControl =false;

	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\ntime =%d, ++>eis ipanel_porting_adec_ioctl op = %d, arg = %d\n" ,get_time_ms(),op, ((int*)arg));
	#endif
	switch(op)
	{
		case IPANEL_ADEC_SET_SOURCE:  /* ָ��Audio decoder����������Դ */
			break;
			
		case IPANEL_ADEC_START:  /* ����ָ����decoder����ָ����������ݸ�ʽ*/
			eis_report("\n>>>>>>>>>IPANEL_ADEC_START  vPid=%d, aPid=%d, pcrPid=%d,gst_AudioType=%d,gc_AudioMode=%d",vPid, aPid, pcrPid,arg,gc_AudioMode);
			#if 0
			CH6_AVControl(VIDEO_DISPLAY, true, CONTROL_AUDIO);
			  #endif
		      if((IPANEL_ADEC_AUDIO_FORMAT_e)(arg) == IPANEL_AUDIO_CODEC_AC3)
			{
                       gst_AudioType = STAUD_STREAM_CONTENT_AC3;
			}
		      else 
			  if((IPANEL_ADEC_AUDIO_FORMAT_e)(arg) == IPANEL_AUDIO_CODEC_AAC)
		  	{
		  	gst_AudioType = STAUD_STREAM_CONTENT_MPEG_AAC;
		  	}
			  else
		      	{
                        gst_AudioType = STAUD_STREAM_CONTENT_MPEG1;
		      	}
#if 1 	/*sqzow20100730*/
			if(gi_SpdifOutMode == SPDIF_OUT_AC3)
			{
				if(gst_AudioType != STAUD_STREAM_CONTENT_AC3)
				{
					if(CH6_GetSPDIFDigtalOutput() != STAUD_DIGITAL_MODE_NONCOMPRESSED)
					{
						CH6_DisableSPDIFDigtalOutput();
					        CH6_EnableSPDIFDigtalOutput_UNCOMPRESSED();
					}
				}
				else
				{
					if(CH6_GetSPDIFDigtalOutput() != STAUD_DIGITAL_MODE_COMPRESSED)
					{
						CH6_DisableSPDIFDigtalOutput();
					        CH6_EnableSPDIFDigtalOutput_COMPRESSED();
					}
				}
			}
#endif
			  
			if(enum_LastVideoControl == IPANEL_VDEC_STOP)
			{
			 	CH_OnlyAudioControl(8191,aPid,true,gst_AudioType,gc_AudioMode);
			}
			else
			{
				CH_OnlyAudioControl(vPid,aPid,true,gst_AudioType,gc_AudioMode);
			}
			
			//CH_OnlyAudioControl(8191,aPid,true,gst_AudioType,gc_AudioMode);						
			eis_AUDIOstart_time=time_now();
			  gi_FirstAudioControl = true;
			  gi_IFrameShow = false;

			  enum_LastAudioControl=IPANEL_ADEC_START;
#if 1 	/*�ӳٻָ����� sqzow20100722*/	
			CH_AudioDelayControl(0);
#else
			if (bAudioMuteState == FALSE ) /*��̨��ָ���������ֹ���� sqzow20100706*/ 
			{
				CH6_UnmuteAudio();
				eis_report("\n�һָ�������");
			}
#endif
#ifdef AV_SYN_NOPCR			
		CH_ResetVID_AYSYNCallback(vPid, aPid);
	        CH_StartAVSynBaseline();
		CH_RegistBaseLINESYNEvent(0);
#endif			
			  
			break; 
			
		case IPANEL_ADEC_STOP:   /* ָֹͣ����decoder*/
			eis_report("\n>>>>>>>>>>>>>>>>>>>IPANEL_ADEC_STOP  ");
			CH_AudioDelayControl(1);
			CH6_AVControl(VIDEO_DISPLAY, false, CONTROL_AUDIO);
			gi_FirstAudioControl = false;
			enum_LastAudioControl=IPANEL_ADEC_STOP;
			CH_StopAudSlot();
			break;

		case IPANEL_ADEC_PAUSE: /* �ݶ���������decoder���뵥Ԫ�Ľ��롣*/
			eis_report("\n>>>>>>>>>>>>>>>>>>>IPANEL_ADEC_PAUSE  ");
			CH6_AVControl(VIDEO_DISPLAY, false, CONTROL_AUDIO);
			  gi_FirstAudioControl = false;
			   enum_LastAudioControl=IPANEL_ADEC_PAUSE;
			break;

		case IPANEL_ADEC_RESUME:  /* �ָ����ݶ���decoder���뵥Ԫ�Ľ��롣*/
			eis_report("\n>>>>>>>>>>>>>>>>>>>IPANEL_ADEC_RESUME  ");
			CH6_AVControl(VIDEO_DISPLAY, true, CONTROL_AUDIO);
			  gi_FirstAudioControl = true;
			  gi_IFrameShow = false;
			  enum_LastAudioControl=IPANEL_ADEC_RESUME;
			break;

		case IPANEL_ADEC_CLEAR:  /* ���Decoder���뵥Ԫ�ڲ�������������*/
			eis_report("\n>>>>>>>>>>>>>>>>>>>IPANEL_ADEC_CLEAR  ");
			enum_LastAudioControl=IPANEL_ADEC_CLEAR;
			break;

		case IPANEL_ADEC_SYNCHRONIZE:  /* ��ֹ����������Ƶͬ������*/
			break;
			
		case IPANEL_ADEC_SET_CHANNEL_MODE:  /* ����˫�������ģʽ��*/
			{
			IPANEL_ADEC_CHANNEL_OUT_MODE_e p_Parm;

			p_Parm = ( IPANEL_ADEC_CHANNEL_OUT_MODE_e)(arg);
			eis_report(" \n IPANEL_AUDIO_MODE=%d\n ",p_Parm);

				if(last_audio_mode!=p_Parm)
				{
					last_audio_mode=p_Parm;
			switch ( p_Parm )
			{
			case IPANEL_AUDIO_MODE_STEREO:				/* ������*/
						eis_report(" IPANEL_AUDIO_MODE_STEREO ");
				gc_AudioMode = 3;
				break;
			case IPANEL_AUDIO_MODE_LEFT_MONO:			/* ������*/
				gc_AudioMode = 2;
						eis_report(" IPANEL_AUDIO_MODE_LEFT_MONO ");
				break;
			case IPANEL_AUDIO_MODE_RIGHT_MONO:		/* ������*/
						eis_report(" IPANEL_AUDIO_MODE_RIGHT_MONO ");
				gc_AudioMode = 1;
				break;
			case IPANEL_AUDIO_MODE_MIX_MONO:			/* �����������*/
				gc_AudioMode = 0;
						eis_report(" IPANEL_AUDIO_MODE_MIX_MONO ");
				break;
			case IPANEL_AUDIO_MODE_STEREO_REVERSE:	/* ������������������ת*/
				gc_AudioMode = 3;
						eis_report(" IPANEL_AUDIO_MODE_STEREO_REVERSE ");
				break;
			}

				#if 1
					if(enum_LastAudioControl==IPANEL_ADEC_START)
					{
						eis_report("\n  set PANEL_ADEC_CHANNEL_OUT_MODE--gc_AudioMode=%d-----------",gc_AudioMode);
						if(enum_LastVideoControl == IPANEL_VDEC_STOP)
						{
						 	CH_OnlyAudioControl(8191,aPid,true,gst_AudioType,gc_AudioMode);
						}
						else
						{
							CH_OnlyAudioControl(vPid,aPid,true,gst_AudioType,gc_AudioMode);
						}
						gi_FirstAudioControl = true;
						eis_AUDIOstart_time=time_now();
					}
				#endif
			}
		}
			break;
			
		case IPANEL_ADEC_SET_MUTE:  /* ��ֹ�����������ܡ�*/
		eis_report("\n>>>>>>>>>>>>>>>>>>>IPANEL_ADEC_SET_MUTE  ");
		  {
				if( time_minus(time_now(),eis_AUDIOstart_time) < (ST_GetClocksPerSecond()))
				{
					CH_UnSubaudioNewfram();
					CH_SubaudioNewfram();
					CH_WaitAudioNewFram(500);
				}
				{
					if (bAudioMuteState == FALSE ) /*MUTE*/
					{
						CH6_MuteAudio();
						bAudioMuteState = TRUE ;
					}
					else
					{  
						//if( time_minus(time_now(),eis_AUDIOstart_time) > (ST_GetClocksPerSecond()))
						CH6_UnmuteAudio();
						bAudioMuteState = FALSE ;			
					}
				}
			}
			
			break;

		case IPANEL_ADEC_SET_PASS_THROUGH:
			if((int)(arg) == 1)/*AC3*/
				{
					gi_SpdifOutMode = SPDIF_OUT_AC3;
			          	CH6_DisableSPDIFDigtalOutput();
			              CH6_EnableSPDIFDigtalOutput_COMPRESSED();

				}
			else            /*PCM*/
				{
					gi_SpdifOutMode = SPDIF_OUT_PCM;
				       CH6_DisableSPDIFDigtalOutput();
				       CH6_EnableSPDIFDigtalOutput_UNCOMPRESSED();
				}
			break;		
			
		case IPANEL_ADEC_SET_VOLUME:
#ifdef __EIS_API_MEDIA_DEBUG__
		eis_report("\n ipanel_porting_adec_ioctl  IPANEL_ADEC_SET_VOLUME %d",(int)arg);
#endif

			break;		

		case IPANEL_ADEC_GET_BUFFER_RATE:
			break;
			
		default:
			break;
	}
	#ifdef __EIS_API_MEDIA_DEBUG__
   	//eis_report ( "\n++>eis ipanel_porting_adec_ioctl end  time=%d ",get_time_ms());
	#endif

	return IPANEL_OK ; 
}

/*
����˵����
	����Ƶ�������Ԫ��
����˵����
	�����������
	�����������
�� �أ�
	����IPANEL_NULL���ɹ�����Ƶ�������Ԫ�����
	����IPANEL_NULL��ʧ��
*/
UINT32_T ipanel_porting_audio_output_open(VOID)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_porting_audio_output_open " );
	#endif

	return IPANEL_OK;
}

/*
����˵����
	�ر�ָ������Ƶ�����Ԫ��
����˵����
	���������
	�����������
	�� �أ�
		IPANEL_OK:�ɹ�;
		IPANEL_ERR:ʧ�ܡ�
  */
INT32_T ipanel_porting_audio_output_close(UINT32_T handle)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_porting_audio_output_close " );
	#endif

	return IPANEL_OK;
}

/*
����˵����
	������������ý���һ�������������������úͻ�ȡ��������豸�Ĳ��������ԡ�
����˵����
	���������
		audio�� ����豸���
		op �� ��������
		typedef enum
		{
			IPANEL_AOUT_SET_OUTPUT =1,
			IPANEL_AOUT_SET_VOLUME,
			IPANEL_AOUT_SET_BALANCE,
		} IPANEL_AOUT_IOCTL_e;
		arg �C �������������Ĳ�����������ö���ͻ�32 λ����ֵʱ��arg ��ǿ��ת��
		�ɶ�Ӧ�������͡�
		op, arg ȡֵ����ֲ�ĵ��еı�
	�����������
	�� �أ�
		IPANEL_OK:�ɹ�;
		IPANEL_ERR:ʧ�ܡ�
*/
INT32_T ipanel_porting_audio_output_ioctl(UINT32_T handle, IPANEL_AOUT_IOCTL_e op, VOID *arg)
{
	INT32_T ret = IPANEL_OK ; 
	UINT32_T oparg = (UINT32_T)arg;
	int 		temp_volume  ;
	static int last_volume;

	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_porting_audio_output_ioctl op=%d" ,op);
	#endif

	switch(op)
	{
		case IPANEL_AOUT_SET_OUTPUT:  /* ָ����������豸*/
			break;
			
		case IPANEL_AOUT_SET_VOLUME:  /* ������Ƶ�������*/
			{
				boolean AudioControl =false;
				if((bAudioMuteState == FALSE)&&(last_volume == ((int)arg)))
					break;
				if(gi_FirstAudioControl && time_minus(time_now(),eis_AUDIOstart_time)< (ST_GetClocksPerSecond()))
				{
					//break;
					task_delay(ST_GetClocksPerSecond()/3);
				}
				last_volume = ((int)arg);
				temp_volume = ((int)arg);
				temp_volume=((temp_volume)+3);
				if(temp_volume>63)
					temp_volume=63;
				if(temp_volume<0)
					temp_volume=0;
				
				eis_report("\n ipanel_porting_audio_output_ioctl IPANEL_AOUT_SET_VOLUME ipanel volume %d,real volume %d",((int)arg),temp_volume);
				pstBoxInfo->GlobalVolume=temp_volume;
				
				if(gi_FirstAudioControl == true)
				{
					#if 0
					CH_UnSubaudioNewfram();
					CH_SubaudioNewfram();
					CH_WaitAudioNewFram(100);
					#endif
					AudioControl=true;
					gi_FirstAudioControl = false;
				}
				if (bAudioMuteState == TRUE ) /*MUTE*/
				{  
					STAUD_Mute(AUDHandle,FALSE, FALSE);
					mpeg_set_audio_volume_LR(temp_volume,temp_volume);
					CH_CloseMute();
					bAudioMuteState = FALSE ;			
				}
				else
				{
				      mpeg_set_audio_volume_LR(temp_volume,temp_volume);
				}
				if(AudioControl)
				{
					//CH_OnlyAudioControl(8191,aPid,true,gst_AudioType,gc_AudioMode);
				}
			}
			break;
			
		case IPANEL_AOUT_SET_BALANCE: /* ������Ƶ������������������*/
			break;

		default:
			break;
	}	

	return IPANEL_OK ; 
}

/*
����˵����
	��һ����Ƶ���뵥Ԫ��
����˵����
	�����������
	�����������
	�� �أ�
		����IPANEL_NULL���ɹ�����Ƶ���뵥Ԫ�����
		����IPANEL_NULL��ʧ��
*/
UINT32_T ipanel_porting_vdec_open(VOID)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_porting_vdec_open " );
	#endif
	CH6_AVControl(VIDEO_DISPLAY, true, CONTROL_VIDEO);
	return IPANEL_OK;
}

/*
����˵����
	�ر�ָ���Ľ��뵥Ԫ��
����˵����
	���������
	decoder��Ҫ�رյĽ��뵥Ԫ���
	�����������
	�� �أ�
		IPANEL_OK:�ɹ�;
		IPANEL_ERR:ʧ�ܡ�
*/
INT32_T ipanel_porting_vdec_close(UINT32_T decoder)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_porting_vdec_close" );
	#endif
	CH6_AVControl(VIDEO_BLACK, false, CONTROL_VIDEO);
	return IPANEL_OK;	
}

/*
����˵����
	��decoder ����һ�������������������úͻ�ȡdecoder �豸�Ĳ��������ԡ�
����˵����
	���������
		decoder�� ���뵥Ԫ���
		op �� ��������
		typedef enum
		{
			IPANEL_VDEC_SET_SOURCE =1,
			IPANEL_VDEC_START,
			IPANEL_VDEC_STOP,
			IPANEL_VDEC_PAUSE,
			IPANEL_VDEC_RESUME,
			IPANEL_VDEC_CLEAR,
			IPANEL_VDEC_SYNCHRONIZE,
			IPANEL_VDEC_GET_BUFFER_RATE,
			IPANEL_VDEC_PLAY_I_FRAME,
			IPANEL_VDEC_STOP_I_FRAME��
			IPANEL_VDEC_SET_STREAM_TYPE,
		} IPANEL_VDEC_IOCTL_e;
		arg �C �������������Ĳ�����������ö���ͻ�32 λ����ֵʱ��arg ��ǿ��ת��
		�ɶ�Ӧ�������͡�
		op, arg ȡֵ����ֲ�ĵ��б�
	�����������
	�� �أ�
		IPANEL_OK:�ɹ�;
		IPANEL_ERR:ʧ�ܡ�
*/
boolean gb_h264play = true;

INT32_T ipanel_porting_vdec_ioctl(UINT32_T decoder,IPANEL_VDEC_IOCTL_e op, VOID *arg)
{
	INT32_T ret = IPANEL_OK ; 
	UINT32_T oparg = (UINT32_T)arg;
#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report("\n ipanel_porting_vdec_ioctl been called op=%d,time=%d, arg = %d\n",op,get_time_ms(), ((int*)arg));
#endif
  
	eis_actVideoControl = op;

	switch(op)
	{
		case IPANEL_VDEC_SET_SOURCE:
			break;
			
		case IPANEL_VDEC_START:
			eis_report("\n VideoType=%d ",arg);
			switch((int)arg)
			{
				case IPANEL_VIDEO_STREAM_MPEG1:
					gc_VideoType=MPEG1_VIDEO_STREAM;
					break;
				case IPANEL_VIDEO_STREAM_MPEG2:
					gc_VideoType=MPEG2_VIDEO_STREAM;
					break;
				case IPANEL_VIDEO_STREAM_H264:
					gc_VideoType=H264_VIDEO_STREAM;
					break;
					
				default:
					gc_VideoType=MPEG1_VIDEO_STREAM;
					break;
			}
			eis_VDECstart_time=get_time_ms();
			CH_SetVideoAspectRatioConversion(CH_GetSTVideoARConversionFromCH(pstBoxInfo->VideoARConversion));/*20070813 add*/

			//aPid = 1211;
			
			eis_report("\n IPANEL_VDEC_START  vPid=%d, aPid=%d, pcrPid=%d,gst_AudioType=%d,gc_AudioMode=%d",vPid, aPid, pcrPid,arg,gc_AudioMode);

			 if((enum_LastVideoControl==IPANEL_VDEC_CLEAR)||(enum_LastVideoControl==IPANEL_VDEC_PLAY_I_FRAME))
		 	{
		 		CH6_AVControl(VIDEO_BLACK, false, CONTROL_VIDEO);
		 	}
#if 0
			if(gc_VideoType == H264_VIDEO_STREAM && true == gb_h264play)
			{
				gb_h264play = false;
				
				do_report(0,"\n ��һ�ε��ò���H264\n");
				
				//ipanel_porting_graphics_set_alpha(40);

				CH_OnlyVideoControl_xx(vPid,true,pcrPid,H264_VIDEO_STREAM);
				CH6_AVControl(VIDEO_DISPLAY, true, CONTROL_VIDEO);
			
			}

#else
			if(gc_VideoType == H264_VIDEO_STREAM && true == gb_h264play)
			{
				gb_h264play = false;

				do_report(0,"\n ��һ�ε��ò���H264\n");

				//ipanel_porting_graphics_set_alpha(40);
				CH6_AVControl(VIDEO_BLACK, false, CONTROL_VIDEO);
				CH_OnlyVideoControl(vPid,true,pcrPid,gc_VideoType);
			}
#endif	
			else
			{
				do_report(0,"\n ����֮ǰ���ŵĽӿ�\n");
			
				CH_OnlyVideoControl(vPid,true,pcrPid,gc_VideoType);
			}

			enum_LastVideoControl = IPANEL_VDEC_START;

#ifdef AV_SYN_NOPCR			
			CH_ResetVID_AYSYNCallback(vPid, aPid);
			CH_StartAVSynBaseline();
			CH_RegistBaseLINESYNEvent(0);
#endif			
				break; 

		case IPANEL_VDEC_STOP: 
			eis_VDECstart_time=get_time_ms();
			#if 1
			{
				 CH_StopVidSlot();
				if((gc_VideoType==MPEG1_VIDEO_STREAM)||(gc_VideoType==MPEG2_VIDEO_STREAM)||(gc_VideoType==H264_VIDEO_STREAM))
				{
				 	CH_VID_SetMode();
				}
				if(arg == 0)
				{
					CH6_AVControl(VIDEO_FREEZE, false, CONTROL_VIDEO);
					eis_report("\n test print   IPANEL_VDEC_STOP VIDEO_FREEZE");
				}
				else
				{
					eis_report("\n test print    IPANEL_VDEC_STOP VIDEO_BLACK");
					CH6_AVControl(VIDEO_BLACK, false, CONTROL_VIDEO);
				}
			}
			
			#endif
			enum_LastVideoControl = IPANEL_VDEC_STOP;

			break; 

		case IPANEL_VDEC_PAUSE:
			CH6_AVControl(VIDEO_FREEZE, false, CONTROL_VIDEO);
			enum_LastVideoControl = IPANEL_VDEC_PAUSE;
			break;

		case IPANEL_VDEC_RESUME:
			CH6_AVControl(VIDEO_DISPLAY, true, CONTROL_VIDEO);
			enum_LastVideoControl = IPANEL_VDEC_RESUME;
			break;

		case IPANEL_VDEC_CLEAR:
			#if 1		
			if((NULL!=eis_IFrame_buf)&&(eis_IFrame_len>0))
			{
			       if((enum_LastVideoControl == IPANEL_VDEC_RESUME || enum_LastVideoControl == IPANEL_VDEC_START))
				{
#ifdef __EIS_API_MEDIA_DEBUG__
					eis_report("\n test print    IPANEL_VDEC_CLEAR VIDEO_SHOWPIC_AUTORECOVERY");
#endif
					if(EIS_GetVIdeoType() != H264_VIDEO_STREAM ||false == CH_TUNER_IsLocked() )
					{
						CH6_AVControl(VIDEO_SHOWPIC_AUTORECOVERY,false,CONTROL_VIDEO);
					}
				
				}
				else
				{
#ifdef __EIS_API_MEDIA_DEBUG__
					eis_report("\n test print    IPANEL_VDEC_CLEAR VIDEO_SHOWPIC");
#endif
					CH6_AVControl(VIDEO_SHOWPIC,false,CONTROL_VIDEO);
				}
				   gb_DisplayOtherIFram = false;
			}			
			#endif
			break;

		case IPANEL_VDEC_SYNCHRONIZE:
			break;
		
		case IPANEL_VDEC_GET_BUFFER_RATE:
			break;
		case IPANEL_VDEC_PLAY_I_FRAME:
			
			{
				IPANEL_IOCTL_DATA *p_Param;
				  boolean b_IframChange  = true;
                            if(arg != NULL)
                            {
						p_Param = (IPANEL_IOCTL_DATA *)arg;
						if(p_Param->data == NULL || p_Param->len == 0)
						{
                                             break;
						}
	                                  semaphore_wait(gp_EisSema);		
						/*�Ⱥ��ϴα���IFrame���ݱȽ������ͬ����ʾ*/			  
						if((NULL != eis_IFrame_buf))
						{
							b_IframChange=memcmp(eis_IFrame_buf,p_Param->data,p_Param->len) ;
							if(b_IframChange== 0 && (time_minus(time_now(),eis_IFrame_time) < (ST_GetClocksPerSecond()/10)))
							{
								semaphore_signal(gp_EisSema);
								#ifdef __EIS_API_MEDIA_DEBUG__
								eis_report("\n test print    IPANEL_VDEC_PLAY_I_FRAME ���ϴ�I֡��ͬ����ʾ");
								#endif
								break;
							}
						}
						
						/*����IFrame����*/
						if(NULL == eis_IFrame_buf)
						{
							eis_IFrame_buf = (U8*)ipanel_porting_malloc(0x80000);
						}
						
						if((NULL != eis_IFrame_buf)&&(b_IframChange!=0)&&(p_Param->len<0x80000))
						{
							memcpy(eis_IFrame_buf,p_Param->data,p_Param->len);
							eis_IFrame_len=p_Param->len;
							gb_DisplayOtherIFram = true;
						}
						
						semaphore_signal(gp_EisSema);	
						 if(enum_LastVideoControl == IPANEL_VDEC_RESUME || enum_LastVideoControl == IPANEL_VDEC_START)
							{
							#ifdef __EIS_API_MEDIA_DEBUG__
							eis_report("\n test print    IPANEL_VDEC_PLAY_I_FRAME ���ڲ��Ų�����ʾ");
							#endif
						}
						 else
							{
							CH6_AVControl(VIDEO_SHOWPIC,false,CONTROL_VIDEO);
							 gb_DisplayOtherIFram = false;
							eis_IFrame_time = time_now();
							#ifdef __EIS_API_MEDIA_DEBUG__
							eis_report("\n test print    IPANEL_VDEC_PLAY_I_FRAME ��ʾI֡IFRAME len=%d",eis_IFrame_len);
							#endif
						 }
					
						
                            }
				        
			}	
			 BootSETHDVideo();
			break;

		case IPANEL_VDEC_STOP_I_FRAME:
			if(enum_LastVideoControl == IPANEL_VDEC_RESUME || enum_LastVideoControl == IPANEL_VDEC_START)
			{
			#ifdef __EIS_API_MEDIA_DEBUG__
			eis_report("\n test print    IPANEL_VDEC_STOP_I_FRAME ���ڲ��Ų�����ʾֻ�����ڴ�");
			#endif
			}
			else
			{
			CH6_AVControl(VIDEO_BLACK, true, CONTROL_VIDEO);
			 gb_DisplayOtherIFram = true;
			 #ifdef __EIS_API_MEDIA_DEBUG__
			 eis_report("\n test print    IPANEL_VDEC_STOP_I_FRAME ����I֡����");
			 #endif
			}
			 semaphore_wait(gp_EisSema);
			memset(eis_IFrame_buf,0,eis_IFrame_len);
			semaphore_signal(gp_EisSema);	
			
			break;

		default:
			break;
	}
	#ifdef __EIS_API_MEDIA_DEBUG__
   	eis_report ( "\n++>eis ipanel_porting_vdec_ioctl end  time=%d ",get_time_ms());
	#endif
	return ret ; 
}

/*
����˵����
	��һ����ʾ��Ԫ��
����˵����
	�����������
	�����������
	�� �أ�
		����IPANEL_NULL���ɹ�����ʾ��Ԫ�����
		����IPANEL_NULL��ʧ��
*/
UINT32_T ipanel_porting_display_open (VOID)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_porting_display_open " );
	#endif

	return IPANEL_OK;	
}

/*
����˵����
	�ر�һ����ʾ����Ԫ��
����˵����
	�����������
	�����������
	�� �أ�
		����IPANEL_NULL���ɹ�����ʾ����Ԫ�����
		����IPANEL_NULL��ʧ��
*/
INT32_T ipanel_porting_display_close(UINT32_T display)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_porting_display_close " );
	#endif

	return IPANEL_OK;	
}

/*
����˵����
	��ָ������ʾ����Ԫ�ϵĴ���һ�����ڡ�
����˵����
	���������
		display�� һ����ʾ��Ԫ������
		type�� ���ڵ����ͣ�ֻ֧��0����ʾ��Ƶ���ڡ�
	�����������
	�� �أ�
		����IPANEL_NULL���ɹ������ھ����
		����IPANEL_NULL��ʧ��
*/
UINT32_T ipanel_porting_display_open_window(UINT32_T display, INT32_T type)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_porting_display_open_window " );
	#endif

	  return 1;	
}

/*
����˵����
	�ر�ָ���Ĵ��ڡ�
����˵����
	���������
		display: ��ʾ��Ԫ��������
		window�� Ҫ�رյĴ��ھ����
	�����������
	�� �أ�
		IPANEL_OK:�ɹ�;
		IPANEL_ERR:ʧ�ܡ�
*/
INT32_T ipanel_porting_display_close_window(UINT32_T display,UINT32_T window)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_porting_display_close_window " );
	#endif
	CH6_AVControl(VIDEO_BLACK, false, CONTROL_VIDEOANDAUDIO);
	return IPANEL_OK;	
}

/*
����˵����
	����ʾ�豸����һ�������������������úͻ�ȡ��ʾ�豸�Ĳ��������ԡ�
����˵����
	���������
		display����ʾ����Ԫ���
		op �� ��������
		typedef enum
		{
			IPANEL_DIS_SELECT_DEV =1,
			IPANEL_DIS_ENABLE_DEV,
			IPANEL_DIS_SET_MODE,
			IPANEL_DIS_SET_VISABLE,
			IPANEL_DIS_SET_ASPECT_RATIO,
			IPANEL_DIS_SET_WIN_LOCATION,
			IPANEL_DIS_SET_WIN_TRANSPARENT,
			IPANEL_DIS_SET_CONTRAST,
			IPANEL_DIS_SET_HUE,
			IPANEL_DIS_SET_BRIGHTNESS,
			IPANEL_DIS_SET_SATURATION,
			IPANEL_DIS_SET_SHARPNESS,
		} IPANEL_DIS_IOCTL_e;
		arg �C �������������Ĳ�����������ö���ͻ�32 λ����ֵʱ��arg ��ǿ��ת��
		�ɶ�Ӧ�������͡�
	�����������
	�� �أ�
		IPANEL_OK:�ɹ�;
		IPANEL_ERR:ʧ�ܡ�
*/
INT32_T ipanel_porting_display_ioctl(UINT32_T display, IPANEL_DIS_IOCTL_e op, VOID *arg)
{
	INT32_T ret = IPANEL_OK ; 
	UINT32_T  oparg = (UINT32_T)arg;
	int temp;

	eis_report("\n ipanel_porting_display_ioctl been called,op is %d",op);
	#ifdef __EIS_API_MEDIA_DEBUG__
	#endif
		
	switch(op)
	{		
		case IPANEL_DIS_SELECT_DEV:   /* ������Ƶ����豸*/
			break ; 
			
		case IPANEL_DIS_ENABLE_DEV :  /* �����Ƿ�����Ӧ�Ľӿ��豸������ź�*/
			break ; 
			
	case IPANEL_DIS_SET_MODE:    /* ������Ƶ���ת��ģʽ*/
		
		  
			break ; 
			
		case IPANEL_DIS_SET_VISABLE:  /* ������Ƶ���ڵĿɼ���*/
			break ; 
			
		case IPANEL_DIS_SET_ASPECT_RATIO: /* ������Ƶ���ݺ�Ⱥ���ʾ�豸�ݺ�� */
			  if((int)(arg) == IPANEL_DIS_AR_FULL_SCREEN)
			  	{
				  	if(CONVERSION_FULLSCREEN != pstBoxInfo->VideoARConversion)
				  	{
					   pstBoxInfo->VideoARConversion = CONVERSION_FULLSCREEN;
					   
                       		   CH_NVMUpdateByAddress(idNvmBoxInfoId,&pstBoxInfo->VideoOutputType,40);
				  	}
			     }
			  else                            
			  	{
                                  if(CONVERSION_LETTETBOX != pstBoxInfo->VideoARConversion)
				  	{
					   pstBoxInfo->VideoARConversion = CONVERSION_LETTETBOX;
					  
                       		   CH_NVMUpdateByAddress(idNvmBoxInfoId,&pstBoxInfo->VideoOutputType,40);
				  	}
			  	}	
			break ; 
			
		case IPANEL_DIS_SET_WIN_LOCATION:  /* ������Ƶ���ڵ� */
			#if 1
			{
				IPANEL_RECT *rect  = 	(IPANEL_RECT *)oparg;
				int x,y,w,h;

				x = (rect->x+EIS_REGION_STARTX+2)/2*2;
				y = (rect->y+EIS_REGION_STARTY)/2*2;
				w = rect->w/2*2;
				h = rect->h/2*2;
				eis_report("\n ipanel_porting_display_ioctl %d  %d %d %d",x,y,w,h);

				if( x < 0 )
					x = 0;
				if( y < 0 )
					y = 0;
				
				if(w ==0||h==0)
					CH6_SetVideoWindowSize(false, x, y, w, h);
				else
					CH6_SetVideoWindowSize(true, x, y, w, h);
	
			}
			#endif
			break ; 
			
		case IPANEL_DIS_SET_WIN_TRANSPARENT :  /* ����͸����*/
			break ; 
			
		case IPANEL_DIS_SET_CONTRAST:  /* ����ͼ��Աȶ�*/
			if(pstBoxInfo->abiTVContrast != (int)(arg))
			{
			eis_report ( "\n set CONTRASTGAIN>>>>>>>>>>>>>>>arg=%d ",(int)(arg) );
			 pstBoxInfo->abiTVContrast=(int)(arg);	
			temp=Con_ConvertHD(pstBoxInfo->abiTVContrast);
			ColorProcessingTest(CONTRASTGAIN, temp);
			CH_NVMUpdate( idNvmBoxInfoId);
			}
			break ; 

			
		case IPANEL_DIS_SET_HUE:   /* ����ͼ��ɫ��*/
			if(pstBoxInfo->abiTVSaturation != (int)(arg))
			{
			eis_report ( "\n set SATURATIONGAIN>>>>>>>>>>>>>>>arg=%d ",(int)(arg) );
			 pstBoxInfo->abiTVSaturation=(int)(arg);
			temp=Chr_ConvertHD(pstBoxInfo->abiTVSaturation);
			 ColorProcessingTest(SATURATIONGAIN,temp);
			 CH_NVMUpdate( idNvmBoxInfoId);
			}
			break ; 
			
		case IPANEL_DIS_SET_BRIGHTNESS:  /* ����ͼ������ */
			if(pstBoxInfo->abiTVBright!=(int)(arg))
			{
			eis_report ( "\n set BRIGHTNESSGAIN>>>>>>>>>>>>>>>arg=%d ",(int)(arg) );
			pstBoxInfo->abiTVBright=(int)(arg);
			temp=Lum_ConvertHD(pstBoxInfo->abiTVBright);
			ColorProcessingTest(BRIGHTNESSGAIN,temp);
			CH_NVMUpdate( idNvmBoxInfoId);
			}
			break ; 
			
		case IPANEL_DIS_SET_SATURATION:  /* ����ͼ�񱥺Ͷ�*/
			break ; 
			
		case IPANEL_DIS_SET_SHARPNESS:  /* ����ͼ����� */
			break ; 
              case IPANEL_DIS_SET_HD_RES:/*��������ֱ���*/
			 {
			 	int VideoMode=(int)arg;
				switch(VideoMode)
				{
					case IPANEL_DIS_HD_RES_1080I:
					  	{
						  	if(MODE_1080I50 != pstBoxInfo->HDVideoTimingMode)
						  	{
							   pstBoxInfo->HDVideoTimingMode = MODE_1080I50;
							   if(CH_SetVideoOut(pstBoxInfo->HDVideoOutputType,pstBoxInfo->HDVideoTimingMode,pstBoxInfo->VideoOutputAspectRatio,pstBoxInfo->VideoARConversion))
							   {
								   STTBX_Print(("\nYxlInfo:CH_SetVideoOut() isn't successful"));						
							   }
							   CH_RstoreALLOSD();
		                       		   CH_NVMUpdateByAddress(idNvmBoxInfoId,&pstBoxInfo->VideoOutputType,40);
						  	}
					     }
						break;
					case IPANEL_DIS_HD_RES_720P:
					  	{
		                                  if(MODE_720P50 != pstBoxInfo->HDVideoTimingMode)
						  	{
							   pstBoxInfo->HDVideoTimingMode = MODE_720P50;
							   if(CH_SetVideoOut(pstBoxInfo->HDVideoOutputType,pstBoxInfo->HDVideoTimingMode,pstBoxInfo->VideoOutputAspectRatio,pstBoxInfo->VideoARConversion))
							   {
								   STTBX_Print(("\nYxlInfo:CH_SetVideoOut() isn't successful"));						
							   }
							    CH_RstoreALLOSD();
		                       		   CH_NVMUpdateByAddress(idNvmBoxInfoId,&pstBoxInfo->VideoOutputType,40);
						  	}
					  	}
						break;
					case IPANEL_DIS_HD_RES_576P:
					  	{
		                                  if(MODE_576P50 != pstBoxInfo->HDVideoTimingMode)
						  	{
							   pstBoxInfo->HDVideoTimingMode = MODE_576P50;
							   if(CH_SetVideoOut(pstBoxInfo->HDVideoOutputType,pstBoxInfo->HDVideoTimingMode,pstBoxInfo->VideoOutputAspectRatio,pstBoxInfo->VideoARConversion))
							   {
								   STTBX_Print(("\nYxlInfo:CH_SetVideoOut() isn't successful"));						
							   }
							    CH_RstoreALLOSD();
		                       		   CH_NVMUpdateByAddress(idNvmBoxInfoId,&pstBoxInfo->VideoOutputType,40);
						  	}
					  	}
						break;
					default:
						break;
				}
              	}
			break;
		default:
			break;
	}

	return ret ; 
}

/*
����˵����
	��һ��PCM �����豸ʵ����Ϊ�˽����Դ��ͻ���⣬��Լ�����ٶ�ֻ����һ��Ӳ��
	�豸���������
	���Ѵ���ʵ�����Զ�ռ��ʽ��IPANEL_DEV_USE_EXCUSIVE����ʱ�����������д򿪲�
	�������ɹ�
	����ǰ��ʵ�������Թ���ʽ��IPANEL_DEV_USE_SHARED����ʱ�������ٴ򿪹���ʵ
	���Ͷ�ռʵ����������ж��ǹ���ʵ���������ȳ���ʽ���ţ������ʵ���Ƕ�ռ��ʽ�򿪣�
	����ǰ���й���ʵ�����������������������
����˵����
	���������
		mode�� ʹ���豸�ķ�ʽ��
		typedef enum
		{
		IPANEL_DEV_USE_SHARED, /�������û�����ʹ���豸/
		IPANEL_DEV_USE_EXCUSIVE, /��ռʹ���豸/
		}IPANEL_DEV_USE_MODE;
		func�� �û�ʵ�ֵ��¼�֪ͨ����ָ�룬�û�Ҳ���Բ�����ָ�룬��ʱfunc ����
		��IPANEL_NULL��0����
		VOID (*IPANEL_AUDIO_MIXER_NOTIFY)(UINT32_T hmixer,
		IPANEL_AUDIO_MIXER_ENENT event, UINT32_T *param)
		AUDIO_DATA_CONSUMED��param ָ����������ݿ�����������
		���ݿ���������ڵײ��ͷŵ�����param Ϊ��ָ�롣
		AUDIO_DATA_LACK��param Ϊ��ָ�롣
	�������: ��
	����ֵ:��
	�����������
	�� �أ�
		����IPANEL_NULL���ɹ���PCM �����豸��Ԫ�����
		����IPANEL_NULL��ʧ��
*/

UINT32_T ipanel_porting_audio_mixer_open(IPANEL_DEV_USE_MODE mode, IPANEL_AUDIO_MIXER_NOTIFY func)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_porting_audio_mixer_open " );
	#endif
	
#ifdef MP3_DATA	
	return IPANEL_NULL;
#else
	if(MP3_Open ==  0)	
	{
		extern ST_Partition_t *SystemPartition;
		MP3_Open =1;
		CH6_AVControl(VIDEO_DISPLAY, false, CONTROL_AUDIO);/*�ر���Ƶ*/
		CH_AudioUnLink();
		CH_MP3_PlayerInitlize(SystemPartition,4,6);
		CH_MP3_PCMDecodeInit();
        //�������״̬���־���
        if (bAudioMuteState == FALSE)
        {
		    CH6_UnmuteAudio();/*wz add MP3��������*/
        }
	}
	MP3_CallBackFunc = func;
	return MP3_handel;
#endif
}

/*
����˵����
	�ر�ͨ��ipanel_porting_audio_mixer_open �򿪵�PCM �����豸ʵ����ͬʱҪ�ͷſ���
	���ڵĴ��������ݿ顣
����˵����
	���������
		handle�� PCM �����豸ʵ�������
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_porting_audio_mixer_close(UINT32_T handle)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_porting_audio_mixer_close " );
	#endif
#ifdef MP3_DATA	
	return IPANEL_ERR;
#else
	if(MP3_Open)
	{
		CH_MP3_ExitPlayer();
		CH_AudioUnLink();
		CH_AudioLink();
		EIS_ResumeAudioPlay();
		CH6_AVControl(VIDEO_DISPLAY,TRUE,CONTROL_AUDIO); 
		MP3_Open =0;
		MP3_CallBackFunc=NULL;
	}
	return IPANEL_OK;
#endif
}

/*
����˵����
	��PCM �����豸��ȡ�ڴ��
����˵����
	���������
		handle��PCM �����豸ʵ�����
		size�� ��Ҫ���ڴ���С��
	�����������
	�� �أ�
		!=IPANEL_NULL:����IPANEL_XMEMBLK �ṹ���ײ������ڴ�ָ����������
		���з��أ�
		==IPANEL_NULL:ʧ��
*/
pIPANEL_XMEMBLK ipanel_porting_audio_mixer_memblk_get(UINT32_T handle,UINT32_T size)
{
	static IPANEL_XMEMBLK  temp;
	extern void CH_MP3_MemoryDeallocate(void* pvMemory);
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_porting_audio_mixer_memblk_get " );
	#endif
	
#ifdef MP3_DATA	
	return IPANEL_NULL;
#else
	
	temp.pbuf=(UINT32_T  *)CH_MP3_MemoryAllocate(size);
	if(temp.pbuf!=IPANEL_NULL)
	{
		temp.pfree=CH_MP3_MemoryDeallocate;
		temp.len=size;
		return &temp;
	}
	else
	{
		return IPANEL_NULL;
	}
#endif
}

/*
����˵����
	��PCM �����豸ʵ�����������Ƶ���ݣ��ú��������������ء�IPANEL_XMEMBLK
	������ʵ�ʵ����ݡ�����ʵ�ʲ��ŵĽ���/�߳�����������ݡ�
����˵����
	�������:
		handle �CPCM �����豸ʵ�����
		pcmblk - IPANEL_XMEMBLK �ṹ���ǰҳ��
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_porting_audio_mixer_memblk_send(UINT32_T handle,pIPANEL_XMEMBLK pcmblk)
{
	pIPANEL_PCMDES pcm_des = NULL;
	#ifdef __EIS_API_MEDIA_DEBUG__
	//eis_report ( "\n++>eis ipanel_porting_audio_mixer_memblk_send " );
	#endif
#ifdef MP3_DATA	
	return IPANEL_ERR;
#else
	
	if(NULL != pcmblk)
	{
		if(pcmblk->pdes)
		{
			pcm_des = (pIPANEL_PCMDES)pcmblk->pdes;
			CH_SetPCMPara(pcm_des->samplerate,pcm_des->bitspersample,pcm_des->channelnum);
		}
		ipanel_pcmblk=pcmblk;
		if((pcmblk->pbuf) && (pcmblk->len>0))
		{
			if(CH_IPANEL_MP3Start(pcmblk->pbuf, pcmblk->len))
			{
				return IPANEL_OK;
			}
			else
			{
				return IPANEL_ERR;
			}
		}
	}
	return IPANEL_OK;
#endif
}

/*
����˵����
	��PCM �����豸ʵ���������һ�������������������úͻ�ȡPCM �����豸ʵ����
	���Ĳ��������ԡ�
����˵����
	���������
		handle �CPCM �����豸ʵ�����
		op �� ��������
		typedef enum
		{
			IPANEL_MIXER_SET_VOLUME =1,
			IPANEL_MIXER_CLEAR_BUFFER
		} IPANEL_MIXER_IOCTL_e;
		arg �C �������������Ĳ�����������ö���ͻ�32 λ����ֵʱ��arg ��ǿ��ת���ɶ�
		Ӧ�������͡�
		op ��arg ��ȡֵ��ϵ���±�
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ��
*/
INT32_T ipanel_porting_audio_mixer_ioctl(UINT32_T handle, IPANEL_MIXER_IOCTL_e op, VOID *arg)
{
	int temp_volume;
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_porting_audio_mixer_ioctl  " );
	#endif
	switch(op)
	{
		case IPANEL_MIXER_SET_VOLUME:
			eis_report ( " IPANEL_MIXER_SET_VOLUME  " );
				temp_volume = ((int)arg);
				temp_volume=((temp_volume)+3);
				if(temp_volume>63)
					temp_volume=63;
				if(temp_volume<0)
					temp_volume=0;
				 mpeg_set_audio_volume_LR(temp_volume,temp_volume);
			break;
		case IPANEL_MIXER_CLEAR_BUFFER:
			eis_report ( " IPANEL_MIXER_CLEAR_BUFFER  " );
			
			ipanel_mediaplayer_close(0); 
			
			
			//ipanel_mediaplayer_stop(0);
			//CH_CleanBuf();
			break;
		case IPANEL_MIXER_PAUSE:
			eis_report ( " IPANEL_MIXER_PAUSE  " );
			CH_MP3_Pause();
			break;
		case IPANEL_MIXER_RESUME:
			eis_report ( " IPANEL_MIXER_RESUME  " );
			CH_MP3_Resume();
			break;
		default:
			break;
	
	}
	return IPANEL_OK;
}

void CH_IPANEL_MP3CallBack(void)
{
#ifdef IPANEL_AAC_FILE_PLAY
		if(AAC_CallBackFunc)
		{
			AAC_CallBackFunc(AAC_handel,IPANEL_AUDIO_DATA_CONSUMED,(UINT32_T*)(ipanel_pcmblk));
			do_report(0,"\n   ������һ��");
		}
	else
#endif
	if(MP3_CallBackFunc)
	{
		MP3_CallBackFunc(MP3_handel,IPANEL_AUDIO_DATA_CONSUMED,(UINT32_T*)(ipanel_pcmblk));
	}
}

/*
����˵����
	��һ�������ɼ��豸ʵ����
����˵����
���������
	Func �� �û�ʵ�ֵ��¼�֪ͨ����ָ�룬�û�Ҳ���Բ�����ָ�룬��ʱfunc ����
	��IPANEL_NULL��0����
	typedef void (*IPANEL_MIC_NOTIFY)( UINT32_T handle, INT32_T event, VOID *param)
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
UINT32_T ipanel_porting_mic_open(IPANEL_MIC_NOTIFY func)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_porting_mic_open δ���" );
	#endif

	return IPANEL_OK;
}

/*
����˵����
	�ر�ͨ��ipanel_porting_mic_open �򿪵������ɼ��豸ʵ����ͬʱҪ�ͷſ��ܴ��ڵ�
	���������ݿ顣
����˵����
	���������
		handle �C�����ɼ��豸ʵ�����
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_porting_mic_close(UINT32_T handle)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_porting_mic_close δ���" );
	#endif

	return IPANEL_OK;
}

/*
����˵����
	�������ɼ��豸ʵ������һ�������������������úͻ�ȡ�����ɼ��豸ʵ���Ĳ�������
	�ԡ�
����˵����
	���������
		handle �C�����ɼ��豸ʵ�����
		op �� ��������
		typedef enum
		{
			IPANEL_MIC_START=1,
			IPANEL_MIC_STOP,
			IPANEL_MIC_CLEAR_BUFFER,
			IPANEL_MIC_SET_PARAM
		} IPANEL_MIC_IOCTL_e;
		arg �C �������������Ĳ�����������ö���ͻ�32 λ����ֵʱ��arg ��ǿ��ת���ɶ�
		Ӧ�������͡�
		op ��arg ��ȡֵ��ϵ���±�
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_porting_mic_ioctl(UINT32_T handle, IPANEL_MIC_IOCTL_e op, VOID *arg)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_porting_mic_ioctl δ���" );
	#endif

	return IPANEL_OK;
}

/*
����˵����
	��ȡ�����������ݡ�
����˵����
	���������
		handle �C�����ɼ��豸ʵ�����
		buf �� ָ�����ݿ��ָ��
		len �� buf �ĳ���
	���������
		buf����ȡ�Ĳ�������
	�� �أ�
		>=0: ����ִ�гɹ�,���ض�ȡ�����ݳ���;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_porting_mic_read(UINT32_T handle,BYTE_T *buf, UINT32_T len)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_porting_mic_read δ���" );
	#endif

	return IPANEL_OK;
}

/*
* ����˵����
	����һ������Ƶservice��srv �д��ݵ���һ��service �������Ϣ�����service
	��CA ���ŵģ�ʵ����Ӧ�ý�CA ��ص���Ϣ���ݸ�CA ģ�顣
����˵����
	���������
		srv����Ŀ��Ϣ�ṹ��ָ�룻
		typedef struct
		{
			UINT16_T service_id;
			BYTE_T serviename[128];
			UINT16_T video_pid;
			UINT16_T audio_pid;
			UINT16_T pcr_pid;
			// ͬ�������, PMT common loop ���ж��CA_descriptor 
			UINT16_T pmt_pid;
			/ PMT PID(һЩCA Ҫ��PMT ��PID ��ʼ, ECM/EMM ��PID ����!) /
			UINT16_T ecmPids[IPANEL_CA_MAX_NUM];
			UINT16_T ecmCaSysIDs[IPANEL_CA_MAX_NUM];
			UINT16_T audioEcmPids[IPANEL_CA_MAX_NUM];
			UINT16_T audioCaSysIDs[IPANEL_CA_MAX_NUM];
			UINT16_T videoEcmPids[IPANEL_CA_MAX_NUM];
			UINT16_T videoCaSysIDs[IPANEL_CA_MAX_NUM];
		} IPANEL_SERVICE_INFO;
	#define IPANEL_CA_MAX_NUM 8
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
  */
INT32_T ipanel_avm_play_srv(IPANEL_SERVICE_INFO *srv)
{
#if 0
	TRANSPONDER_INFO_STRUCT 	t_XpdrInfo;
eis_report("\n ipanel_avm_play_srv freq=%d, symbolrate=%d,video_pid=%d, audio_pid=%d, pcr_pid=%d\n ",
	srv->stansport.transport.dvbc.tuningfreqhz,srv->stansport.transport.dvbc.symbolrate,
	srv->video_pid,srv->audio_pid,srv->pcr_pid);
#if 1
	eis_report ( "\n++>eis ipanel_avm_play_srv,video_pid=%x,audio_pid=%x,pcr_pid=%x" ,srv->video_pid,srv->audio_pid,srv->pcr_pid);
	t_XpdrInfo.iTransponderFreq 	= srv->stansport.transport.dvbc.tuningfreqhz / 10;
	t_XpdrInfo.iSymbolRate			= srv->stansport.transport.dvbc.symbolrate / 10;
	t_XpdrInfo.ucQAMMode			= 3/*smodulation*/;
	//CHDVBNewTune0Req(&t_XpdrInfo);

	CH_UpdatesCurTransponderId ( -1 );
#endif
	CH6_NewChannel(srv->video_pid,srv->audio_pid,srv->pcr_pid,true,true,STAUD_STREAM_CONTENT_MPEG1,MPEG1_VIDEO_STREAM);
	//CH6_NewChannel(0x204,0x308,srv->pcr_pid,true,true,STAUD_STREAM_CONTENT_MPEG1,MPEG2_VIDEO_STREAM);

	CH6_AVControl ( VIDEO_DISPLAY, true, CONTROL_VIDEOANDAUDIO );

	/*  ע��˴������CA */
#endif
	return IPANEL_OK;
}

/*
����˵����
	ֹͣ��ǰservice �Ĳ��š�
����˵����
	���������
		��
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
  */
INT32_T ipanel_avm_stop_srv(VOID)
{
	CH6_NewChannel ( 0x1fff, 0x1ffe, 0x1ffd, true, false, STAUD_STREAM_CONTENT_MPEG1, MPEG1_VIDEO_STREAM );
	CH6_AVControl ( VIDEO_BLACK, false, CONTROL_VIDEOANDAUDIO );

	return IPANEL_OK;
}

/*
����˵����
	��AV manager ģ�����ص��豸���в������ơ�
����˵����
	���������
		op����������
		typedef enum
		{
			IPANEL_AVM_SET_CHANNEL_MODE,
			IPANEL_AVM_SET_MUTE,
			IPANEL_AVM_SET_PASS_THROUGH,
			IPANEL_AVM_SET_VOLUME,
			IPANEL_AVM_SELECT_DEV,
			IPANEL_AVM_ENABLE_DEV,
			IPANEL_AVM_SET_TVMODE,
			IPANEL_AVM_SET_VISABLE,
			IPANEL_AVM_SET_ASPECT_RATIO,
			IPANEL_AVM_SET_WIN_LOCATION,
			IPANEL_AVM_SET_WIN_TRANSPARENT,
			IPANEL_AVM_SET_CONTRAST,
			IPANEL_AVM_SET_HUE,
			IPANEL_AVM_SET_BRIGHTNESS,
			IPANEL_AVM_SET_SATURATION,
			IPANEL_AVM_SET_SHARPNESS,
			IPANEL_AVM_PLAY_I_FRAME��
			IPANEL_AVM_STOP_I_FRAME��
			IPANEL_AVM_SET_STREAM_TYPE��
			IPANEL_AVM_START_AUDIO,
			IPANEL_AVM_STOP_AUDIO,
		} IPANEL_AVM_IOCTL_e;
		arg �C �������������Ĳ�����������ö���ͻ�32 λ����ֵʱ��arg ��ǿ��ת��
		�ɶ�Ӧ�������͡�
		op, arg ȡֵ���±�
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_avm_ioctl(IPANEL_AVM_IOCTL_e op,VOID *arg)
{
	eis_report("\n ipanel_avm_ioctl op=%d",op);
	switch ( op )
	{
	case IPANEL_AVM_SET_CHANNEL_MODE:
		{
			IPANEL_ADEC_CHANNEL_OUT_MODE_e p_Parm;

			p_Parm = ( IPANEL_ADEC_CHANNEL_OUT_MODE_e)(arg);
			switch ( p_Parm )
			{
			case IPANEL_AUDIO_MODE_STEREO:				/* ������*/
				eis_report("\n IPANEL_AUDIO_MODE_STEREO ");
				gc_AudioMode = 3;
				CH_SetAudioModeOnly(gc_AudioMode);
				break;
			case IPANEL_AUDIO_MODE_LEFT_MONO:			/* ������*/
				gc_AudioMode = 2;
				eis_report("\n IPANEL_AUDIO_MODE_LEFT_MONO ");
				CH_SetAudioModeOnly(gc_AudioMode);
				break;
			case IPANEL_AUDIO_MODE_RIGHT_MONO:		/* ������*/
				eis_report("\n IPANEL_AUDIO_MODE_RIGHT_MONO ");
					gc_AudioMode = 1;
				CH_SetAudioModeOnly(gc_AudioMode);
				break;
			case IPANEL_AUDIO_MODE_MIX_MONO:			/* �����������*/
					gc_AudioMode = 0;
					eis_report("\n IPANEL_AUDIO_MODE_MIX_MONO ");
			      CH_SetAudioModeOnly(gc_AudioMode);
				break;
			case IPANEL_AUDIO_MODE_STEREO_REVERSE:	/* ������������������ת*/
				gc_AudioMode = 3;
				eis_report("\n IPANEL_AUDIO_MODE_STEREO_REVERSE ");
				CH_SetAudioModeOnly(gc_AudioMode);
				break;
			}
		}
		break;
	case IPANEL_AVM_SET_MUTE:
		{
			IPANEL_SWITCH_e p_Param;

			p_Param = (int)(( int  *)(arg));
			switch ( p_Param )
			{
				case IPANEL_DISABLE:
					CH6_UnmuteAudio ();
					break;
				case IPANEL_ENABLE:
					CH6_MuteAudio ();
					break;
			}
		}
		break;
	case IPANEL_AVM_SET_PASS_THROUGH:
		{
		#ifdef __EIS_API_MEDIA_DEBUG__
			eis_report ( "\n++>eis IPANEL_AVM_SET_PASS_THROUGH δʵ��" );
		#endif
			break;
		}
	case IPANEL_AVM_SET_VOLUME:
		{
			int 	p_Param;

			p_Param = (int)(arg);
				//CH6_MuteAudio();
			mpeg_set_audio_volume_LR( (p_Param)  / 3+ 31, (p_Param)  / 3 + 31 );
				//CH6_UnmuteAudio();

			//main_set_audio_volume_LR ( (*p_Param)  / 3+ 31, (*p_Param)  / 3 + 31 );
		}
		break;
	case IPANEL_AVM_SELECT_DEV:
		{
			IPANEL_VDIS_VIDEO_OUTPUT_e p_Param;

			p_Param =(IPANEL_VDIS_VIDEO_OUTPUT_e) arg;
			switch ( p_Param )
			{
			case IPANEL_VIDEO_OUTPUT_CVBS:
			case IPANEL_VIDEO_OUTPUT_SVIDEO:
			case IPANEL_VIDEO_OUTPUT_RGB:
			case IPANEL_VIDEO_OUTPUT_YPBPR:
			case IPANEL_VIDEO_OUTPUT_HDMI:
				#ifdef __EIS_API_MEDIA_DEBUG__
				eis_report ( "\n++>eis IPANEL_AVM_SELECT_DEV δʵ��" );
				#endif
				break;
			}
		}
		break;
	case IPANEL_AVM_ENABLE_DEV:
		{
			#ifdef __EIS_API_MEDIA_DEBUG__
			eis_report ( "\n++>eis IPANEL_AVM_ENABLE_DEV δʵ��" );
			#endif
		}
		break;
	case IPANEL_AVM_SET_TVMODE:
		{
			#ifdef __EIS_API_MEDIA_DEBUG__
			eis_report ( "\n++>eis IPANEL_AVM_SET_TVMODE ֻ�ڱ���������" );
			#endif
		}
		break;
	case IPANEL_AVM_SET_VISABLE:
		{
			IPANEL_SWITCH_e p_Param;

			p_Param = ( IPANEL_SWITCH_e  )arg;

			switch ( p_Param )
			{
			case IPANEL_ENABLE:
				CH6_AVControl ( VIDEO_BLACK, FALSE, CONTROL_VIDEOANDAUDIO );
				break;
			case IPANEL_DISABLE:
				CH6_AVControl ( VIDEO_DISPLAY, true, CONTROL_VIDEOANDAUDIO );
				break;
			}
		}
		break;
	case IPANEL_AVM_SET_ASPECT_RATIO:
		{
			#ifdef __EIS_API_MEDIA_DEBUG__
			eis_report ( "\n++>eis IPANEL_AVM_SET_ASPECT_RATIO δʵ��" );
			#endif
		}
		break;
	case IPANEL_AVM_SET_WIN_LOCATION:
		{
			IPANEL_RECT *p_Param;

			p_Param = (IPANEL_RECT *)(arg);

			eis_report( "\n++>eis video_location,x=%d,y=%d,w=%d,h=%d",p_Param->x, p_Param->y, p_Param->w, p_Param->h );
			#ifdef eis_if_debug
			#endif

			if ( ( p_Param->x==0 && p_Param->y==0 && p_Param->w==0 && p_Param->h==0 )
				||(p_Param->x==0 && p_Param->y==0 && p_Param->w==720 && p_Param->h==576 ) )
			{
				CH6_SetVideoWindowSize( false, 0, 0, 720, 576 );
			 }
			else
			{
				//eis_set_video_window(x,y,w,h);
				if(((p_Param->x+EIS_SCREEN_STARTX)>=10)&&((p_Param->y+EIS_SCREEN_STARTY)>=30))
					CH6_SetVideoWindowSize( TRUE, p_Param->x+30-4, p_Param->y+10, p_Param->w, p_Param->h );
				else
					CH6_SetVideoWindowSize( TRUE, p_Param->x+40-4, p_Param->y+40, p_Param->w, p_Param->h );					
			}
		}
		break;
	case IPANEL_AVM_SET_WIN_TRANSPARENT:
		{
			#ifdef __EIS_API_MEDIA_DEBUG__
			eis_report ( "\n++>eis IPANEL_AVM_SET_WIN_TRANSPARENT δʵ��" );
			#endif
		}
		break;
	case IPANEL_AVM_SET_CONTRAST:
		{
		}
		break;
	case IPANEL_AVM_SET_BRIGHTNESS:
		{
			#ifdef __EIS_API_MEDIA_DEBUG__
			eis_report ( "\n++>eis IPANEL_AVM_SET_BRIGHTNESS δʵ��" );
			#endif			
		}
		break;
	case IPANEL_AVM_SET_SATURATION:
		{
			#ifdef __EIS_API_MEDIA_DEBUG__
			eis_report ( "\n++>eis IPANEL_AVM_SET_SATURATION δʵ��" );
			#endif			
		}
		break;
	case IPANEL_AVM_SET_SHARPNESS:
		{
			#ifdef __EIS_API_MEDIA_DEBUG__
			eis_report ( "\n++>eis IPANEL_AVM_SET_SHARPNESS δʵ��" );
			#endif
		}
		break;
	case IPANEL_AVM_PLAY_I_FRAME:
		break;
	case IPANEL_AVM_STOP_I_FRAME:
		CH6_CloseMepg ();
		CH6_AVControl ( VIDEO_BLACK, false, CONTROL_VIDEOANDAUDIO );
		break;
	case IPANEL_AVM_SET_STREAM_TYPE:
		{
			#ifdef __EIS_API_MEDIA_DEBUG__
			eis_report ( "\n++>eis IPANEL_AVM_SET_STREAM_TYPE δʵ��" );
			#endif
		}
		break;
	case IPANEL_AVM_START_AUDIO:
		{
			CH6_UnmuteAudio ();
		}
		break;
	case IPANEL_AVM_STOP_AUDIO:
		{
			CH6_MuteAudio ();
		}
		break;
	}
}

/*
����˵����
	��һ��������ʵ����
����˵����
	���������
		des�� ��������ʼ�����������庬������塣
		cbk�� �򿪲�����ʱע���һ���ص�������
		typedef void (*player_event_notify)(UINT32_T player, INT32_T event, void
		*param)
		player: ������ʵ�����
		event�� ������
		param�� event ��������ָ�롣������
	�����������
	�� �أ�
		���� IPANEL_NULL�����ش򿪵ľ����ý��handle��
		���� IPANEL_NULL��ʧ�ܡ�
*/
#ifdef IPANEL_AAC_FILE_PLAY
static boolean AAC_play=false;
void CH_set_AAC_play_state(boolean AAC_play_state)
{
	AAC_play=AAC_play_state;
}
boolean CH_get_AAC_play_state(void )
{
	return AAC_play;
}

UINT32_T ipanel_mediaplayer_open(const CHAR_T *des, player_event_notify cbk)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_mediaplayer_open " );
	#endif
	
	if(/*des==NULL ||  sqzow20100602*/cbk==NULL)
		return IPANEL_NULL;
	CH_set_AAC_play_state(true);
	if(AAC_Open ==  0)	
	{
		extern ST_Partition_t *SystemPartition;
		AAC_Open =1;
		CH6_AVControl(VIDEO_DISPLAY, false, CONTROL_AUDIO);/*�ر���Ƶ*/
		CH_AudioUnLink();
		CH_MP3_PlayerInitlize(SystemPartition,4,6);
		CH_CleanBuf();
		AAC_play_state=AAC_STATUS_FREE_E ;
	}
	else
	{
		CH_CleanBuf();
		CH_MP3_ExitPlayer();
		CH_MP3_PlayerInitlize(SystemPartition,4,6);
	}
	AAC_CallBackFunc = cbk;
	return AAC_handel;
}

/*
����˵����
	ֹͣ���ڲ��ŵġ���ͣ�ġ�ֹͣ�ġ�����ġ����˵�ý��Դ�������ͷŸ�ý��Դ
	��Ϣ���������Ϣ���ú����Ƿ������ġ�
����˵����
	���������
		player: һ�����������
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ��
*/
INT32_T ipanel_mediaplayer_close(UINT32_T player)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_mediaplayer_close " );
	#endif
	if(AAC_Open)
	{
		CH_MP3_ExitPlayer();
		CH_AudioUnLink();
		CH_AudioLink();
		CH6_AVControl(VIDEO_DISPLAY,TRUE,CONTROL_AUDIO); 
		AAC_Open =0;
		AAC_CallBackFunc=NULL;
		AAC_play_state=AAC_STATUS_UNKOWN_E ;
		CH_set_AAC_play_state(false);

	}
	return IPANEL_OK;
}

/*
����˵����
	����ʼλ�ò�����mrl ָ��������Ƶ����
����˵����
	���������
		player�����������
		mrl��ָ��ý��λ�úͻ�ȡ��ʽ���磺
		�鲥:igmp://238.22.22.22:1000;
		�㲥��rtsp://host/path/;
		�㲥��dvb://f=384M,s=6875,M=qam64��serviceid = 8��
		des�����ų�ʼ�����������庬�������
		˵������Ϊ����û������setup ��play ���̣��ɿ���ʹ��play ������չ֧
		�֡�
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
/*int gs_test_content = STAUD_STREAM_CONTENT_AIFF; sqzow20100604*/
INT32_T ipanel_mediaplayer_play(UINT32_T player,const BYTE_T *mrl,const BYTE_T *des)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_mediaplayer_play " );
	#endif
#if 0	
	if(((AAC_play_state == AAC_STATUS_STOP_E)||(AAC_play_state == AAC_STATUS_PAUSE_E) )&&(AAC_Open))
	{
		CH_CleanBuf();
		CH_MP3_Resume();
	}
	AAC_play_state = AAC_STATUS_PLAY_E;
#else
	CH_set_AAC_play_state(true);
	if(AAC_Open ==  0)	
	{
		extern ST_Partition_t *SystemPartition;
		AAC_Open =1;
		CH6_AVControl(VIDEO_DISPLAY, false, CONTROL_AUDIO);/*�ر���Ƶ*/
		CH_AudioUnLink();
		CH_MP3_PlayerInitlize(SystemPartition,4,6);
		CH_CleanBuf();
		AAC_play_state=AAC_STATUS_FREE_E ;
	}
	else
	{
		CH_CleanBuf();
		CH_MP3_ExitPlayer();
		CH_MP3_PlayerInitlize(SystemPartition,4,6);
	}

	//��ʼ����������
	#ifdef IPANEL_AAC_FILE_PLAY
	if(CH_get_AAC_play_state())
	{
		int steam_content, sample_rate, i;
		unsigned char buf[24], *pstr;
		const char *pRateString = "sample_rate=", *pTypeString = "mediatype=";
		
#if 0
		steam_content = gs_test_content;
		gs_test_content = gs_test_content >> 1;
		if(gs_test_content == 1)
		{
			gs_test_content = STAUD_STREAM_CONTENT_AIFF;
			sttbx_Print("\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
		}
#else
		/*����Ĭ��ֵ*/
		steam_content = STAUD_STREAM_CONTENT_MPEG_AAC;
		sample_rate = 48000;
		eis_report("\nipanel_mediaplayer_play des =[%s]", des);
		/*��ȡ���Ų���*/
		if(des)
		{
			/*��ȡ������*/
			pstr = strstr(des, pRateString);
			if(pstr)
			{
				pstr += strlen(pRateString);
				sscanf((char*)pstr, "%d", &sample_rate);
				if(sample_rate == 0)
				{
					sample_rate = 48000;
				}
			}
			/*��ȡ����*/
			pstr = strstr(des, pTypeString);
			if(pstr)
			{
				pstr += strlen(pTypeString);
				for(i = 0; pstr[i] != '&' && pstr[i]; i++)
				{
					buf[i] = pstr[i];
				}
				buf[i] = 0;
			}
			else
			{
				buf[0] = 0;
			}
			/*��������*/
			if(strcmp(buf, "aac+LC") == 0)
			{
				steam_content = STAUD_STREAM_CONTENT_MPEG_AAC;/*ok*/
			}
			else if(strcmp(buf, "aac_raw") == 0)
			{
				steam_content = STAUD_STREAM_CONTENT_RAW_AAC;/*ok*/
			}
			else if(strcmp(buf, "aac_HE") == 0)
			{
				steam_content = STAUD_STREAM_CONTENT_MP4_FILE;/*ok*/
			}
			/*else if(strcmp(des, ".aac_LTP") == 0)
			{
				steam_content = STAUD_STREAM_CONTENT_RAW_AAC;
			} sqzow20100604*/
		}
#endif		
		AAC_Comp_Start(steam_content, sample_rate);
	}
	else
	#endif
    	CH_MP3_PCMDecodeInit();

	AAC_play_state=AAC_STATUS_PLAY_E;
	return AAC_handel;

#endif
	return IPANEL_OK;
}

/*
����˵����
	�����ڲ��ŵġ���ͣ�ġ����˵ġ�ǰ������Ƶֹͣ���ţ��������״̬��
����˵����
	���������
		player�����������
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_mediaplayer_stop(UINT32_T player)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_mediaplayer_stop " );
	#endif
	if((AAC_Open))
	{
		CH_AAC_Pause();
		AAC_play_state = AAC_STATUS_STOP_E;
	}
	return IPANEL_OK;
}

/*
����˵����
	��ʱֹͣ���ڲ��ŵ���Ƶ����Ҫ����ס��Ƶ��״̬(����ʱ�䡢λ�ú�ģʽ��)��
����˵��:
	���������
		player: ���������
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_mediaplayer_pause(UINT32_T player)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_mediaplayer_pause " );
	#endif
	if((AAC_play_state== AAC_STATUS_PLAY_E)&&(AAC_Open))
	{
		CH_AAC_Pause();
		AAC_play_state = AAC_STATUS_PAUSE_E;
	}
	return IPANEL_OK;
}

/*
����˵����
	�ָ���ʱֹͣ���ŵ���Ƶ���ú����Ƿ������ġ�
����˵����
	���������
		player�����������
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_mediaplayer_resume(UINT32_T player)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_mediaplayer_resume " );
	#endif
	if((AAC_play_state== AAC_STATUS_PAUSE_E)&&(AAC_Open))
	{
		CH_MP3_Resume();
		AAC_play_state = AAC_STATUS_PLAY_E;
	}
	return IPANEL_OK;
}

/*
����˵����
	�Բ�ͬ���ٶȿ��ٲ��ŵ���Ƶ�� 2 ��ʾ2 ���٣�5 ��ʾ5 ���٣�����ֵ��Ϊ��Ч��
	ֱ�ӷ���-1���ú����Ƿ������ġ�
����˵����
���������
	player�����������
	rate�� ���ٲ��ŵı�����2��5 �ȡ�
�����������
�� �أ�
	IPANEL_OK: ����ִ�гɹ�;
	IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_mediaplayer_forward(UINT32_T player, INT32_T rate)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_mediaplayer_forward " );
	#endif

	return IPANEL_OK;
}

/*
����˵����
	�Բ�ͬ���ٶ����ٲ��ŵ���Ƶ�� 2 ��ʾ1/2 ���٣�5 ��ʾ1/5 ���٣�ֵΪ������
	��Ч������ֵ��Ϊ��Ч��ֱ�ӷ���-1���ú����Ƿ������ġ�
����˵����
	���������
		player�����������
		rate�����ٲ��ŵı�����2��5 �ȡ�
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_mediaplayer_slow(UINT32_T player, INT32_T rate)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_mediaplayer_slow " );
	#endif

	return IPANEL_OK;
}

/*
����˵����
	�Բ�ͬ���ٶȻ��˲��ŵ���Ƶ�� 2 ��ʾ2 ���٣�5 ��ʾ5 ���٣�����ֵ��Ϊ��Ч��
	ֱ�ӷ���-1���ú����Ƿ������ġ�
����˵����
���������
	player�����������
	rate��������Ƶ�ı�����2��5 �ȡ�
�����������
�� �أ�
	IPANEL_OK: ����ִ�гɹ�;
	IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_mediaplayer_rewind(UINT32_T player,INT32_T rate)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_mediaplayer_slow δ���" );
	#endif

	return IPANEL_OK;
}

/*
����˵����
	��ת��һ��ָ����ʱ��㿪ʼ�������š�
����˵����
	���������
		player�����������
		pos��ʱ�䣬��������Ϊ��λ��offset �����ʱ�䣨�磺2007 ��10 ��18 ��
		11��00��00����
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ��
*/
INT32_T ipanel_mediaplayer_seek(UINT32_T player,CHAR_T *pos)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_mediaplayer_slow δ���" );
	#endif

	return IPANEL_OK;
}

/*
����˵����
	���úͻ�ȡ�������ĸ��ֲ�����
	duration(Get)��position��Set/Get����repeat��change service(MTS),����ѡ��
	����ѡ��, status(play,pause,ff,fr),rate��adec��vdec �Ŀ��ơ�display �Ŀ��Ƶȡ�
	����������ö�����ͣ�
	typedef enum
	{
		IPANEL_MEDIA_GET_DURATION,
		IPANEL_MEDIA_GET_STATUS,
		IPANEL_MEDIA_GET_RATE,
		IPANEL_MEDIA_GET_POSITION,
		IPANEL_MEDIA_SET_POSITION,
		IPANEL_MEDIA_SET_REPEAT,
		IPANEL_MEDIA_SET_SERVICE,
		IPANEL_MEDIA_SET_AUDIO,
		IPANEL_MEDIA_SET_SOUND_CHANNEL,
		// adec �Ŀ���
		// vdec �Ŀ���
		// display �Ŀ��Ƶȡ�
	} IPANEL_MEDIA_PLAYER_IOCTL_e;
	�ⲿ�ֽӿ�ʹ�õ������ͻ��У�
	typedef enum _MEDIA_STATU_TYPE
	{
		running �� ��,
		stop,
		pause,
		rewind,
		reverse,
		forward,
		slow
	} MEDIA_STATU_TYPE
	typedef struct _MEDIA_STATUS
	{
		INT32_T statu;
		INT32_T param;
	} MEDIA_STATUS;
����˵����
	���������
		player�����������
		op�� ��������
		param����ͬ�������������Ĳ���
		op �Ͷ�Ӧ��param ȡֵ���±�
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_mediaplayer_ioctl(UINT32_T player,INT32_T op,UINT32_T *param)
{
	pIPANEL_XMEMBLK pcmblk;
	pIPANEL_PCMDES pcm_des = NULL;
	boolean push_data_sussce;
	#ifdef __EIS_API_MEDIA_DEBUG__
	/*eis_report ( "\n++>eis ipanel_mediaplayer_ioctl  op=%d",op ); sqzow20100603*/
	#endif
	switch(op)
	{
		case 7:
			pcmblk=(pIPANEL_XMEMBLK)param;
			if(NULL != pcmblk)
			{
				if((pcmblk->pbuf) && (pcmblk->len>0))
				{
					/*eis_report ( "\n  ipanel_mediaplayer_ioctl pcmblk->pbuf=%x,pcmblk->len=%d", pcmblk->pbuf,pcmblk->len); sqzow20100603*/
					push_data_sussce=CH_IPANEL_AACStart(pcmblk->pbuf, pcmblk->len);
				}
				
				if(push_data_sussce)
				{
					return IPANEL_OK;
				}
				else
				{
					/*eis_report("\n ipanel_mediaplayer_ioctl failed "); sqzow20100611*/
					return IPANEL_ERR;
				}
			}
			else
			{
				return IPANEL_ERR;
			}
			
			break;
		default:
			break;
	}
	return IPANEL_OK;
}

/*
����˵����
	��ʼ��¼��Ŀ��һ���豸��
����˵����
	���������
		player�����������
		mrl��ָ��ý��λ�úͻ�ȡ��ʽ���磺
		�鲥:igmp://238.22.22.22:1000;
		�㲥��rtsp://host/path/;
		�㲥��dvb://f=384M,s=6875,M=qam64��serviceid = 8��
		���¼�Ƶ�ǰplayer �����ڲ��ŵĽ�Ŀ��mrl ����Ϊ�ա�
		device��¼���豸��ʼ�����������庬�������
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_mediaplayer_start_record(UINT32_T player, const BYTE_T *mrl, const BYTE_T *device)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_mediaplayer_start_record δ���" );
	#endif

	return IPANEL_OK;
}

/*
����˵����
	ֹͣ¼�ƽ�Ŀ��
����˵����
	���������
		player�����������
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_mediaplayer_stop_record(UINT32_T player)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_mediaplayer_start_record δ���" );
	#endif

	return IPANEL_OK;
}

/*
����˵����
	����ǰ����λ����Ϊ�ϵ㣬����¼��ʱ��״̬����ȥ���������飬Ȼ������ת��
	�������������ţ��ò���ֻ�����������ŵ�״̬�²ſ�����Ч�����ú����Ƿ������ġ�
ע�⣺��tujz��
	��ǩ����ʵ����Ҫ�о��ǿͻ���ʵ�ֵĻ��Ƿ�������ʵ�ֵ�
	���������ǩ��Ҫ��Ӧ�ӿڷ��ص���ǩλ�ã��ӿڴ�����
����˵����
	�����������
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_mediaplayer_break(VOID)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_mediaplayer_break δ���" );
	#endif

	return IPANEL_OK;
}

#else
#ifdef MP3_DATA	
UINT32_T ipanel_mediaplayer_open(CONST CHAR_T *des, IPANEL_PLAYER_EVENT_NOTIFY cbk)
#else
UINT32_T ipanel_mediaplayer_open(const CHAR_T *des, player_event_notify cbk)
#endif
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_mediaplayer_open " );
	#endif
	
#ifdef MP3_DATA	
	if(des==NULL || cbk==NULL)
		return IPANEL_NULL;
	if(MP3_Open ==  0)	
	{
		extern ST_Partition_t *SystemPartition;
		MP3_Open =1;
		CH6_AVControl(VIDEO_DISPLAY, false, CONTROL_AUDIO);/*�ر���Ƶ*/
		CH_AudioUnLink();
		CH_MP3_PlayerInitlize(SystemPartition,4,6);
		MP3_play_state=MP3_STATUS_FREE_E ;
	}
	else
	{
		CH_CleanBuf();
		CH_MP3_ExitPlayer();
		CH_MP3_PlayerInitlize(SystemPartition,4,6);
	}
	
	MP3_CallBackFunc = cbk;
	return MP3_handel;
#endif

	return IPANEL_OK;
}

/*
����˵����
	ֹͣ���ڲ��ŵġ���ͣ�ġ�ֹͣ�ġ�����ġ����˵�ý��Դ�������ͷŸ�ý��Դ
	��Ϣ���������Ϣ���ú����Ƿ������ġ�
����˵����
	���������
		player: һ�����������
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ��
*/
INT32_T ipanel_mediaplayer_close(UINT32_T player)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_mediaplayer_close " );
	#endif
#ifdef MP3_DATA	
	if(MP3_Open)
	{
		if(MP3_play_state ==  MP3_STATUS_PLAY_E)
		{
			CH_DecodesentlastData();
		}
	
		CH_MP3_ExitPlayer();
		CH_AudioUnLink();
		CH_AudioLink();
		CH6_AVControl(VIDEO_DISPLAY,TRUE,CONTROL_AUDIO); 
		MP3_Open =0;
		MP3_CallBackFunc=NULL;
		MP3_play_state=MP3_STATUS_UNKOWN_E ;
	}
	return IPANEL_OK;
#endif

	return IPANEL_OK;
}

/*
����˵����
	����ʼλ�ò�����mrl ָ��������Ƶ����
����˵����
	���������
		player�����������
		mrl��ָ��ý��λ�úͻ�ȡ��ʽ���磺
		�鲥:igmp://238.22.22.22:1000;
		�㲥��rtsp://host/path/;
		�㲥��dvb://f=384M,s=6875,M=qam64��serviceid = 8��
		des�����ų�ʼ�����������庬�������
		˵������Ϊ����û������setup ��play ���̣��ɿ���ʹ��play ������չ֧
		�֡�
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_mediaplayer_play(UINT32_T player,const BYTE_T *mrl,const BYTE_T *des)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_mediaplayer_play " );
	#endif
	#ifdef MP3_DATA
	if(((MP3_play_state == MP3_STATUS_STOP_E)||(MP3_play_state == MP3_STATUS_PAUSE_E) )&&(MP3_Open))
	{
		CH_MP3_Resume();
	}
	MP3_play_state = MP3_STATUS_PLAY_E;
	#endif
	return IPANEL_OK;
}

/*
����˵����
	�����ڲ��ŵġ���ͣ�ġ����˵ġ�ǰ������Ƶֹͣ���ţ��������״̬��
����˵����
	���������
		player�����������
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_mediaplayer_stop(UINT32_T player)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_mediaplayer_stop " );
	#endif
	#ifdef MP3_DATA
	if((MP3_Open))
	{
		CH_MP3_Pause();
		MP3_play_state = MP3_STATUS_STOP_E;
	}
	#endif
	return IPANEL_OK;
}

/*
����˵����
	��ʱֹͣ���ڲ��ŵ���Ƶ����Ҫ����ס��Ƶ��״̬(����ʱ�䡢λ�ú�ģʽ��)��
����˵��:
	���������
		player: ���������
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_mediaplayer_pause(UINT32_T player)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_mediaplayer_pause " );
	#endif
	#ifdef MP3_DATA
	if((MP3_play_state== MP3_STATUS_PLAY_E)&&(MP3_Open))
	{
		CH_MP3_Pause();
		MP3_play_state = MP3_STATUS_PAUSE_E;
	}
	#endif
	return IPANEL_OK;
}

/*
����˵����
	�ָ���ʱֹͣ���ŵ���Ƶ���ú����Ƿ������ġ�
����˵����
	���������
		player�����������
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_mediaplayer_resume(UINT32_T player)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_mediaplayer_resume " );
	#endif
	#ifdef MP3_DATA
	if((MP3_play_state== MP3_STATUS_PAUSE_E)&&(MP3_Open))
	{
		CH_MP3_Resume();
		MP3_play_state = MP3_STATUS_PLAY_E;
	}
	#endif
	return IPANEL_OK;
}

/*
����˵����
	�Բ�ͬ���ٶȿ��ٲ��ŵ���Ƶ�� 2 ��ʾ2 ���٣�5 ��ʾ5 ���٣�����ֵ��Ϊ��Ч��
	ֱ�ӷ���-1���ú����Ƿ������ġ�
����˵����
���������
	player�����������
	rate�� ���ٲ��ŵı�����2��5 �ȡ�
�����������
�� �أ�
	IPANEL_OK: ����ִ�гɹ�;
	IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_mediaplayer_forward(UINT32_T player, INT32_T rate)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_mediaplayer_forward " );
	#endif

	return IPANEL_OK;
}

/*
����˵����
	�Բ�ͬ���ٶ����ٲ��ŵ���Ƶ�� 2 ��ʾ1/2 ���٣�5 ��ʾ1/5 ���٣�ֵΪ������
	��Ч������ֵ��Ϊ��Ч��ֱ�ӷ���-1���ú����Ƿ������ġ�
����˵����
	���������
		player�����������
		rate�����ٲ��ŵı�����2��5 �ȡ�
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_mediaplayer_slow(UINT32_T player, INT32_T rate)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_mediaplayer_slow " );
	#endif

	return IPANEL_OK;
}

/*
����˵����
	�Բ�ͬ���ٶȻ��˲��ŵ���Ƶ�� 2 ��ʾ2 ���٣�5 ��ʾ5 ���٣�����ֵ��Ϊ��Ч��
	ֱ�ӷ���-1���ú����Ƿ������ġ�
����˵����
���������
	player�����������
	rate��������Ƶ�ı�����2��5 �ȡ�
�����������
�� �أ�
	IPANEL_OK: ����ִ�гɹ�;
	IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_mediaplayer_rewind(UINT32_T player,INT32_T rate)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_mediaplayer_slow δ���" );
	#endif

	return IPANEL_OK;
}

/*
����˵����
	��ת��һ��ָ����ʱ��㿪ʼ�������š�
����˵����
	���������
		player�����������
		pos��ʱ�䣬��������Ϊ��λ��offset �����ʱ�䣨�磺2007 ��10 ��18 ��
		11��00��00����
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ��
*/
INT32_T ipanel_mediaplayer_seek(UINT32_T player,CHAR_T *pos)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_mediaplayer_slow δ���" );
	#endif

	return IPANEL_OK;
}

/*
����˵����
	���úͻ�ȡ�������ĸ��ֲ�����
	duration(Get)��position��Set/Get����repeat��change service(MTS),����ѡ��
	����ѡ��, status(play,pause,ff,fr),rate��adec��vdec �Ŀ��ơ�display �Ŀ��Ƶȡ�
	����������ö�����ͣ�
	typedef enum
	{
		IPANEL_MEDIA_GET_DURATION,
		IPANEL_MEDIA_GET_STATUS,
		IPANEL_MEDIA_GET_RATE,
		IPANEL_MEDIA_GET_POSITION,
		IPANEL_MEDIA_SET_POSITION,
		IPANEL_MEDIA_SET_REPEAT,
		IPANEL_MEDIA_SET_SERVICE,
		IPANEL_MEDIA_SET_AUDIO,
		IPANEL_MEDIA_SET_SOUND_CHANNEL,
		// adec �Ŀ���
		// vdec �Ŀ���
		// display �Ŀ��Ƶȡ�
	} IPANEL_MEDIA_PLAYER_IOCTL_e;
	�ⲿ�ֽӿ�ʹ�õ������ͻ��У�
	typedef enum _MEDIA_STATU_TYPE
	{
		running �� ��,
		stop,
		pause,
		rewind,
		reverse,
		forward,
		slow
	} MEDIA_STATU_TYPE
	typedef struct _MEDIA_STATUS
	{
		INT32_T statu;
		INT32_T param;
	} MEDIA_STATUS;
����˵����
	���������
		player�����������
		op�� ��������
		param����ͬ�������������Ĳ���
		op �Ͷ�Ӧ��param ȡֵ���±�
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_mediaplayer_ioctl(UINT32_T player,INT32_T op,UINT32_T *param)
{
	pIPANEL_XMEMBLK pcmblk;
	pIPANEL_PCMDES pcm_des = NULL;
	
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_mediaplayer_ioctl  op=%d",op );
	#endif
	switch(op)
	{
		case 7:
			#ifdef MP3_DATA	

			pcmblk=(pIPANEL_XMEMBLK)param;
			if(NULL != pcmblk)
			{
				#if 0
				if(pcmblk->pdes)
				{
					pcm_des = (pIPANEL_PCMDES)pcmblk->pdes;
					CH_SetPCMPara(pcm_des->samplerate,pcm_des->bitspersample,pcm_des->channelnum);
				}
				#endif
				if((pcmblk->pbuf) && (pcmblk->len>0))
				{
					eis_report ( "\n  ipanel_mediaplayer_ioctl pcmblk->pbuf=%x,pcmblk->len=%d", pcmblk->pbuf,pcmblk->len);
					CH_IPANEL_MP3Start(pcmblk->pbuf, pcmblk->len);
				}
				ipanel_pcmblk=pcmblk;
			}
			return IPANEL_OK;
			
			#endif
			break;
		default:
			break;
	}
	return IPANEL_OK;
}

/*
����˵����
	��ʼ��¼��Ŀ��һ���豸��
����˵����
	���������
		player�����������
		mrl��ָ��ý��λ�úͻ�ȡ��ʽ���磺
		�鲥:igmp://238.22.22.22:1000;
		�㲥��rtsp://host/path/;
		�㲥��dvb://f=384M,s=6875,M=qam64��serviceid = 8��
		���¼�Ƶ�ǰplayer �����ڲ��ŵĽ�Ŀ��mrl ����Ϊ�ա�
		device��¼���豸��ʼ�����������庬�������
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_mediaplayer_start_record(UINT32_T player, const BYTE_T *mrl, const BYTE_T *device)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_mediaplayer_start_record δ���" );
	#endif

	return IPANEL_OK;
}

/*
����˵����
	ֹͣ¼�ƽ�Ŀ��
����˵����
	���������
		player�����������
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_mediaplayer_stop_record(UINT32_T player)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_mediaplayer_start_record δ���" );
	#endif

	return IPANEL_OK;
}

/*
����˵����
	����ǰ����λ����Ϊ�ϵ㣬����¼��ʱ��״̬����ȥ���������飬Ȼ������ת��
	�������������ţ��ò���ֻ�����������ŵ�״̬�²ſ�����Ч�����ú����Ƿ������ġ�
ע�⣺��tujz��
	��ǩ����ʵ����Ҫ�о��ǿͻ���ʵ�ֵĻ��Ƿ�������ʵ�ֵ�
	���������ǩ��Ҫ��Ӧ�ӿڷ��ص���ǩλ�ã��ӿڴ�����
����˵����
	�����������
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_mediaplayer_break(VOID)
{
	#ifdef __EIS_API_MEDIA_DEBUG__
	eis_report ( "\n++>eis ipanel_mediaplayer_break δ���" );
	#endif

	return IPANEL_OK;
}
#endif
/*
����˵����
��ʾI FAME����	
ע�⣺

����˵����
	�����������
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
*/

void EIS_ShowIFrame(void)
{
	boolean errno=true; 
	int loop=8;
	
#if 1 	/*�ظ��ಥ�ż��� �����ʱ��ʾʧ�ܵ����� sqzow20100708*/

	if(eis_IFrame_buf == NULL)
	{
		return;	
	}
	semaphore_wait(gp_EisSema);

	while(loop -- > 0)
	{
		eis_report("\n ShowIFrame %d\n", 8 - loop);
		
		CH_DisplayIFrameFromMemory ( (U8 *)eis_IFrame_buf,eis_IFrame_len, 8, 4);
		errno = CH_GetBackIFramData();
		if(errno == 0)
		{
			gi_IFrameShow = true;
			break;
		}
	}
	semaphore_signal(gp_EisSema);	
	
#else
	semaphore_wait(gp_EisSema);
	eis_report("\n ShowIFrame");
	if(eis_IFrame_buf != NULL)
	{
	   CH_DisplayIFrameFromMemory ( (U8 *)eis_IFrame_buf,eis_IFrame_len, 8, 4);
	 }
	semaphore_signal(gp_EisSema);	
	CH_GetBackIFramData();
#endif

}

boolean EIS_GetOtherIFramStaus(void)
{
    boolean b_status;
    semaphore_wait(gp_EisSema);	
    b_status = gb_DisplayOtherIFram;
    semaphore_signal(gp_EisSema);
    return b_status;
}
 
boolean EIS_GetLastVideoControlStaus(void)
{
	unsigned int time_now=get_time_ms();
	if((enum_LastVideoControl == IPANEL_VDEC_RESUME || enum_LastVideoControl == IPANEL_VDEC_START)&&((time_now-eis_VDECstart_time) > 3000))
	{
		//eis_report("\n time_now=%d eis_VDECstart_time=%d",time_now,eis_VDECstart_time);
		return true;
	}
	else
		return false;
}

U8 EIS_GetVIdeoType( void )
{
	return gc_VideoType;
}

void EIS_ResumeVideoPlay( void )
{
	task_delay(ST_GetClocksPerSecond()/10);
	CH6_AVControl(VIDEO_BLACK, false, CONTROL_VIDEO);
	CH_OnlyVideoControl(vPid,true,pcrPid,gc_VideoType);
}
boolean EIS_ResumeAudioPlay(void)
{
	if(enum_LastAudioControl == IPANEL_ADEC_RESUME|| enum_LastAudioControl == IPANEL_ADEC_START)
	{
		CH_OnlyAudioControl(8191,aPid,true,gst_AudioType,gc_AudioMode);
		return true;
	}
}
/*-------------------------------��������ע��ͷ--------------------------------------
    FuncName:EIS_GetIFrameStatus
    Purpose: ���I֡��������ʾ״̬
    Reference: 
    in:
        void >> 
    out:
        none
    retval:
        success >> 
        fail >> 
   
notice : 
    sqZow 2010/07/23 20:05:53 
---------------------------------�����嶨��----------------------------------------*/
char EIS_GetIFrameStatus(void)
{
	return gi_IFrameShow;
}
INT32_T ipanel_porting_media_close(VOID)
{
	return 0;
}

#if 0
void TestVid(void)
{
					      CH_OnlyVideoControl(vPid,true,pcrPid,gc_VideoType);

}

boolean play_over=false;
void  test_callbak(UINT32_T hmixer, IPANEL_AUDIO_MIXER_EVENT event, UINT32_T *param)
{
	play_over=true;
}
void test_pcm_player(void)
{
#if 1
	FILE *fp = NULL;
	int handel;
	unsigned char * pcm_buf=NULL;
	int size,act_size;
	int block_size=128*1024;
	IPANEL_XMEMBLK player_block;
	int file_size=600*1024/*730112*/;
			CH6_AVControl(VIDEO_DISPLAY, false, CONTROL_AUDIO);/*�ر���Ƶ*/
		CH_AudioUnLink();
		CH_MP3_PlayerInitlize(SystemPartition,4,6);

	CH_MP3_NewMp3("G://mp3_err/8000-16-mono-8.mp3",8000);
	Task_Delay(ST_GetClocksPerSecond() * 20);  
			CH_MP3_ExitPlayer();
		CH_AudioUnLink();
		CH_AudioLink();
		CH6_AVControl(VIDEO_DISPLAY,TRUE,CONTROL_AUDIO); 

		return ;
		
       //fp = fopen("D://billie_jean.pcm","rb");
       fp = fopen("G://mp3_err/44100-16-mono-32.mp3","rb");
	   
	if(fp == NULL)
		return ;
	pcm_buf=ipanel_porting_malloc(file_size+1024);
	if(NULL==pcm_buf)
	{
		do_report(0,"\n  �ڴ治��");
		return ;
	}
	size = fread((void *)pcm_buf, 1,file_size, fp);
	if(size>0)
	{
		act_size=0;
		player_block.pbuf=pcm_buf;
		player_block.len=block_size;
		handel=ipanel_porting_audio_mixer_open(0,test_callbak);
		ipanel_porting_audio_mixer_memblk_send(handel,&player_block);
		act_size+=block_size;
		play_over=false;
		do_report(0,"\n   ��ʼ����");
		while((act_size+block_size)<size)
		{
			if(play_over)
			{
				player_block.pbuf=(UINT32_T *)((int)pcm_buf+act_size);
				player_block.len=block_size;
				ipanel_porting_audio_mixer_memblk_send(handel,&player_block);
				act_size+=block_size;
				play_over=false;
				do_report(0,"\n   ������һ��");
			}
			Task_Delay(ST_GetClocksPerSecond() / 20);  
		}
	}
	else
	{
		do_report(0,"\n   ���ļ�ʧ��");
	}
	
	fclose(fp);
	ipanel_porting_free(pcm_buf);
	#endif
}
#ifdef  IPANEL_AAC_FILE_PLAY
static CHAR_T * AAC_buff=NULL;
static unsigned int  ACT_len=0;
unsigned int file_size=(3901440);
void test_aac_player(void)
{
	FILE *fp = NULL;
	unsigned int size;
	do_report(0,"\n ���ſ�ʼ");
	CH6_AVControl(VIDEO_DISPLAY, false, CONTROL_AUDIO);/*�ر���Ƶ*/
	CH_AudioUnLink();
	if(AAC_buff==NULL)
	{
		AAC_buff=(CHAR_T *)memory_allocate ( CHSysPartition,file_size);
		if(AAC_buff==NULL)
			return ; 
		#if 0
		memcpy((U8 *)AAC_buff,0xa1000000,0x400000);
		#else
		   fp = fopen("D:\\TDdownload\\Music\\i'm gonna getcha good.mp3","rb");
			if(fp == NULL)
				return ;
		size = fread((void *)AAC_buff, 1,file_size, fp);
		
		if(size<=0)
		{
			fclose(fp);
			return ;
		}

		fclose(fp);
		#endif
	}
	do_report(0, "\n��������OK");
	ACT_len=0;
	AAC_Comp_Start(STAUD_STREAM_CONTENT_MP3, 44100);
	while(ACT_len<size)
	{
	Task_Delay(ST_GetClocksPerSecond() );  
	}
	Task_Delay(ST_GetClocksPerSecond() *5);  
	Comp_Stop();
	CH_AudioUnLink();
	CH_AudioLink();
	CH6_AVControl(VIDEO_DISPLAY,TRUE,CONTROL_AUDIO); 
	do_report(0,"\n ���Ž���");
	
	return  ; 
}


void CH_GetAACData(U16* uST_GetBuffer,
										   U32* uST_Length )
{
	int	Buf_Length = 128*1024, i_Loop;
	if(ACT_len>=file_size)
		return;
	
	if(ACT_len+Buf_Length<file_size)
	{
		memcpy(uST_GetBuffer,AAC_buff+ACT_len,Buf_Length);
		*uST_Length = Buf_Length;	
		ACT_len+=Buf_Length;
		do_report(0,"\n �Ѿ����ų���ACT_len=%d",ACT_len);
	}
	else
	{
		memcpy(uST_GetBuffer,
		AAC_buff+ACT_len,
		file_size-ACT_len);
		*uST_Length = file_size-ACT_len;
		ACT_len=file_size;
	}
#if 0/*�ŵ�ע��ʱ�任 sqzow20100607*/
	for ( i_Loop = 0 ; i_Loop < (*uST_Length >> 1); i_Loop++ )
	{
		__asm__("swap.b %1, %0" : "=r" (uST_GetBuffer[i_Loop]): 
				"r" (uST_GetBuffer[i_Loop]));
	}
#endif

}
#endif
#endif
/*--eof---------------------------------------------------------------------------------------------------*/

