/*
  * ===================================================================================
  * CopyRight By CHANGHONG NET L.T.D.
  * �ļ�: 	eis_api_info.c
  * ����: 	ʵ���豸��صĽӿ�
  * ����:	 ��ս�ң�������
  * ʱ��:	2008-10-22
  * ===================================================================================
  */

#include "eis_api_define.h"
#include "eis_api_globe.h"
#include "eis_api_debug.h"
#include "eis_api_device.h"
#include "..\dbase\vdbase.h"

#include "eis_include\ipanel_fpanel.h"
#include "eis_include\ipanel_tuner.h"
#include "eis_include\ipanel_upgrade.h"
#include "eis_include\Ipanel_nvram.h"

#ifdef __EIS_API_DEBUG_ENABLE__
#define __EIS_API_DEVICE_DEBUG__
#endif

typedef enum {
    IPANEL_EVENT_TIME     = 0,
    IPANEL_EVENT_TUNER = 1,
    IPANEL_EVENT_UNKNOWN
} IPANEL_CHECK_EVENT_e;

static semaphore_t *gpsema_eis_time_play	= NULL;	/* ����time��ʾ���� */
static IPANEL_CHECK_EVENT_e ipanel_event=IPANEL_EVENT_UNKNOWN;
static boolean  recheck_locktuner=false;
static clock_t eis_locktuner_time = 0;
INT32_T locktuner_id=0;
static boolean display_time=false;
/*
����˵����
	������ָ����Ƶ�㡣�����ɹ�ʱ�����ͳɹ��¼���iPanel MiddleWare������ʧ��
	ʱ��������ʧ���¼���iPanel MiddleWare���ú�����Ҫ�������أ��Ƿ������ġ���
	����Ƶ��Ҫ��ʱ��ģ�һ�������ǰ���Ƶ��������һ��ר�ŵ��̣߳�������߳��н�
	����Ƶ�����ײ������Ƶ��ͨ����Ϣ������Ƶ�������id �š�
	����ӿڿ��ܻ��������ã�����ж�ε��ã�ִֻ�����һ�Σ��м�ĵ��ö�����
	�������ж����ڽ��е���Ƶ�������������һ�ε��ò�������Ƶ��
	�������������У����tuner ʧ�������뷢��ʧ����Ϣ��iPanel MiddleWare��
����˵����
	���������
		tunerid:Ҫ������tuner ��ţ���0 ��ʼ��
		frequency: Ƶ�ʣ���λ��Hz ��ʮ��������ֵ��
		symbol_rate: �����ʣ���λ��sym/s ��ʮ��������ֵ��
		modulation: ���Ʒ�ʽ��
		typedef enum
		{
			IPANEL_MODULATION_QAM16 = 1,
			IPANEL_MODULATION_QAM32,
			IPANEL_MODULATION_QAM64,
			IPANEL_MODULATION_QAM128,
			IPANEL_MODULATION_QAM256
		} IPANEL_MODULATION_MODE_e��
		id��һ����־��,����������iPanel MiddleWare �����¼�ʱ�Ĳ�����
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
		�¼�˵����
		����Ƶ�ɹ���ʧ��ʱ��Ҫͨ��ipanel_proc ��������Ϣ����iPanel MiddleWare
		�С�
			��Ƶ�ɹ���Ϣ��
				event[0] = IPANEL_EVENT_TYPE_DVB,
				event[1] = IPANEL_DVB_TUNE_SUCCESS
				event[2] = id
			��Ƶʧ����Ϣ��
				event[0] = IPANEL_EVENT_TYPE_DVB,
				event[1] = IPANEL_DVB_TUNE_FAILED
				event[2] = id
*/
INT32_T ipanel_porting_tuner_lock_delivery(INT32_T tunerid, INT32_T frequency, INT32_T symbol_rate, INT32_T modulation, INT32_T id)
{
	TRANSPONDER_INFO_STRUCT 	t_XpdrInfo;
	U8							uLoop;	
	unsigned int 					event[3];
	static boolean first_tuner_lock=false;
	if ( tunerid > 1 )
	{
		eis_report ( "\n ipanel_porting_tuner_lock_delivery tunerid  ����!" );
		return IPANEL_ERR;
	}

	eis_report ( "\n++>time=%d, eis ipanel_porting_tuner_lock_delivery freq=%d, symbol=%d,modulation=%d", ipanel_porting_time_ms(),frequency, symbol_rate,modulation );

	/* ������������Ϊ�����ظ���ͬһ��Ƶ��*/
	if(first_tuner_lock==true)
	{
		if((t_XpdrInfo.iTransponderFreq==frequency / 10)&&(t_XpdrInfo.iSymbolRate == symbol_rate / 10)&&(t_XpdrInfo.ucQAMMode== modulation))
		{
			if ( ch_Tuner_EIS_StillLocked() )
			{
				locktuner_id=id;
				eis_report("\n send msg IPANEL_DVB_TUNE_SUCCESS ,time=%d",get_time_ms());
				eis_api_msg_send ( IPANEL_EVENT_TYPE_DVB, IPANEL_DVB_TUNE_SUCCESS,locktuner_id );
				return IPANEL_OK;
			}
		}
	}

	t_XpdrInfo.iTransponderFreq 	= frequency / 10;
	t_XpdrInfo.iSymbolRate			= symbol_rate / 10;
	t_XpdrInfo.ucQAMMode			= modulation;
	t_XpdrInfo.DatabaseType=CH_DVBC;

	event[0] = IPANEL_EVENT_TYPE_DVB;
	CH6_SendNewTuneReq(0,&t_XpdrInfo);	
	#if 1
	if(first_tuner_lock==false)
	{
		first_tuner_lock=true;
		eis_report("\n send msg IPANEL_DVB_TUNE_SUCCESS ,time=%d",get_time_ms());
		eis_api_msg_send ( IPANEL_EVENT_TYPE_DVB, IPANEL_DVB_TUNE_SUCCESS,id );
	}
	else
	{
	ipanel_event=IPANEL_EVENT_TUNER;
	eis_locktuner_time = time_now();
	locktuner_id=id;
	eis_report("\n  ipanel_porting_tuner_lock_delivery ,time=%d",get_time_ms());
	semaphore_signal(gpsema_eis_time_play);
	}
	return IPANEL_OK;
	#else
	uLoop = 0;
	while( uLoop <= 2)
	{
		task_delay( ST_GetClocksPerSecondLow() /3);
		if ( TRUE== CH_TUNER_IsLocked() )
		{
			eis_api_msg_send ( IPANEL_EVENT_TYPE_DVB, IPANEL_DVB_TUNE_SUCCESS, id );
			eis_report ( "\n++>eis ipanel_porting_tuner_lock_delivery return time=%d", ipanel_porting_time_ms() );
			return IPANEL_OK;
		}

		uLoop ++;
	}
	
	eis_api_msg_send ( IPANEL_EVENT_TYPE_DVB, IPANEL_DVB_TUNE_FAILED, id );
	return IPANEL_OK;
	#endif

}

/*
����˵����
	�õ���ǰƵ���ź������� �����ķ�Χ��0 �� 100�� 0 ���źţ�100 ��ǿ�źš�
����˵����
	���������
		tunerid:Ҫ��ѯ��tuner ��š�
	�����������
	�� �أ�
		IPANEL_ERR: ����ִ�д���
		>=0:��ǰƵ���ź�����ֵ��0~100����
  */
INT32_T ipanel_porting_tuner_get_signal_quality(INT32_T tunerid)
{
	int i_Strength, i_Quality;

	if ( tunerid > 1 )
	{
		eis_report ( "\n ipanel_porting_tuner_get_signal_quality tunerid  ����!" );
		return IPANEL_ERR;
	}
	eis_report ( "\n ipanel_porting_tuner_get_signal_quality " );
	ch_Tuner_MID_GetStrengthAndQuality(&i_Strength, &i_Quality);
	eis_report("\n >>>tuner_print ipanel_porting_tuner_get_signal_quality i_Quality=%d",i_Quality);
	if(i_Quality<0) i_Quality=0;
		
	//i_Quality=i_Quality%100;
	i_Quality=i_Quality%101;
	return i_Quality;
}

/*
����˵����
	�õ���ǰƵ���ź�ǿ�ȡ�ǿ�ȵķ�Χ��0 �� 100��0 ���źţ�100 ��ǿ�źš�
����˵����
	���������
		tunerid:Ҫ��ѯ��tuner ��š�
	�����������
	�� �أ�
		IPANEL_ERR:ʧ�ܡ�
		>=0:��ǰƵ���ź�ǿ��ֵ��0~100����
*/
INT32_T ipanel_porting_tuner_get_signal_strength(INT32_T tunerid)
{
	int i_Strength, i_Quality;

	if ( tunerid > 1 )
	{
		eis_report ( "\n ipanel_porting_tuner_get_signal_strength tunerid  ����!" );
		return IPANEL_ERR;
	}
	eis_report ( "\n ipanel_porting_tuner_get_signal_strength " );

	ch_Tuner_MID_GetStrengthAndQuality(&i_Strength, &i_Quality);
		eis_report("\n >>>tuner_print ipanel_porting_tuner_get_signal_strength i_Strength=%d",i_Strength);
	if(i_Strength<0) i_Strength=0;
		
	i_Strength=i_Strength%100;

	return i_Strength;
}

/*
����˵����
	�õ���ǰƵ���ź������ʡ�
����˵����
	���������
		tunerid:Ҫ��ѯ��tuner ��š�
	�����������
	�� �أ�
		IPANEL_ERR:ʧ�ܡ�
		>=0:��ǰƵ���ź������ʡ�

	���Ǹ���ʽ����ֵ���涨���£�
	��ʮ��λ��ʾ�����ʵ��������֣�ֻȡ4 λ����������16 λ��ʾָ�����֡�
	���ֵΪ{4�� -5} �ͱ�ʾ������Ϊ4.00E-5;
	���ֵΪ{462, -7} �ͱ�ʾ������Ϊ4.62E-5;
	���ֵΪ{0, 0} �ͱ�ʾ������Ϊ0.00E+0;
  */
INT32_T ipanel_porting_tuner_get_signal_ber(INT32_T tunerid)
{
	short hi16, low16;
	U32	 u_Return;
	U32  ui_BerRate;

	if ( tunerid > 1 )
	{
		eis_report ( "\n ipanel_porting_tuner_get_signal_ber tunerid  ����!" );
		return IPANEL_ERR;
	}	
		eis_report ( "\n ipanel_porting_tuner_get_signal_ber " );

	ui_BerRate = ch_Tuner_MID_GetBerRate();
	hi16 	= ui_BerRate;
	low16	= -1;

	u_Return = ( hi16 << 16 ) | low16;
		eis_report("\n >>>tuner_print ipanel_porting_tuner_get_signal_ber u_Return=%d",u_Return);

	return u_Return;
}

/*
����˵����
	��ѯtuner �Ƿ�������״̬��
����˵����
	���������
		tunerid��
	�����������
	�� �أ�
		IPANEL_TUNER_LOST��û������Ƶ�㣻
		IPANEL_TUNER_LOCKED���ɹ�����Ƶ�㣻
		IPANEL_ERR: ����ִ�д���
*/
INT32_T ipanel_porting_tuner_get_status(INT32_T tunerid)
{
	if ( tunerid > 1 )
	{
		eis_report ( "\n ipanel_porting_tuner_get_status tunerid  ����!" );
		return IPANEL_ERR;
	}	
	eis_report ( "\n ipanel_porting_tuner_get_status " );
	if( TRUE == CH_TUNER_IsLocked() )
	{
		return IPANEL_TUNER_LOCKED;
	}
	else
	{
		return IPANEL_TUNER_LOST;
	}
}

/*
����˵����
	��ѯtuner �Ƿ�������״̬��
����˵����
	���������
		tunerid��
		op �� ��������
		typedef enum
		{
			IPANEL_TUNER_GET_QUALITY =1,
			IPANEL_TUNER_GET_STRENGTH,
			IPANEL_TUNER_GET_BER,
			IPANEL_TUNER_GET_LEVEL,
			IPANEL_TUNER_GET_SNR
		} IPANEL_TUNER _IOCTL_e;
		arg �C �������������Ĳ�����������ö���ͻ�32 λ����ֵʱ��arg ��ǿ��ת���ɶ�
		Ӧ�������͡�
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_porting_tuner_ioctl(INT32_T tunerid, IPANEL_TUNER_IOCTL_e op, VOID *arg)
{
	INT32_T t_Return, *p_Return;

	if ( tunerid > 1 )
	{
		eis_report ( "\n ipanel_porting_tuner_ioctl tunerid  ����!" );
		return IPANEL_ERR;
	}
	eis_report ( "\n ipanel_porting_tuner_ioctl op=%d " ,op);

	switch ( op )
	{
	case IPANEL_TUNER_GET_QUALITY:
		t_Return = ipanel_porting_tuner_get_signal_quality ( tunerid );
		break;
	case IPANEL_TUNER_GET_STRENGTH:
		t_Return = ipanel_porting_tuner_get_signal_strength ( tunerid );
		break;
	case IPANEL_TUNER_GET_BER:
		t_Return = ipanel_porting_tuner_get_signal_ber ( tunerid );
		break;
	default:
		return IPANEL_ERR;
	}

	p_Return 	= (INT32_T*)arg;
	*p_Return 	= t_Return;

	return IPANEL_OK;
}

/*
����˵����
	����STB ��ǰ��������LED ��ָʾ�Ƶ���ʾ����Ҫ��������ʾʱ�䣬������С��
	Ƶ���ţ�������С��������״̬֮�����Ϣ��
����˵����
	���������
		cmd ����������
		typedef enum
		{
			IPANEL_FRONT_PANEL_SHOW_TEXT=1,
			IPANEL_FRONT_PANEL_SHOW_TIME,
			IPANEL_FRONT_PANEL_SET_INDICATOR
		} IPANEL_FRONT_PANEL_IOCTL_e;
		arg �C �������������Ĳ�����������ö���ͻ�32 λ����ֵʱ��arg ��ǿ��ת��
		�ɶ�Ӧ�������͡�
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_porting_front_panel_ioctl(IPANEL_FRONT_PANEL_IOCTL_e cmd, VOID *arg)
{
	extern Semaphore_t * gp_SemVFD;
	IPANEL_FRONT_PANEL_INDICATOR *Arg=NULL;
	
	switch ( cmd )
	{
	case IPANEL_FRONT_PANEL_SHOW_TEXT:
		{
			U8 cBuff[5];
			if(arg!=NULL)
			{
			memset ( cBuff, 0, 5 );
                     //sprintf(cBuff,"%s-%s-%s-%s",arg);
                     eis_report("the txt===%s",arg);
			 eis_report("the txt===%x",arg);
			 semaphore_wait(gp_SemVFD);
			//CH_VFD_ClearAll();
			CH_CellControl(5,1);
			CH_CellControl(6,1);
                     CH_VFD_ShowChar(0,8,"         ");
			 memcpy(cBuff,arg,4);
			CH_LEDDis4ByteStr(arg);
			semaphore_signal(gp_SemVFD);
			display_time=false;
			}
			return IPANEL_OK;
		}
	case IPANEL_FRONT_PANEL_SHOW_TIME:
		{
			char ipanel_time[6],*end_ptr;
			int year,month,day,hour,minute,second;
			if(NULL==arg)
				return IPANEL_ERR;
			//eis_report("\n ipanel time=%s",(*(char *)arg));
		       //CH_VFD_ClearAll();
		       #if 0
			memset(ipanel_time,0,6);
		       memcpy(ipanel_time,(char *)arg+2,4);
			year=strtod(ipanel_time,&end_ptr);
			memset(ipanel_time,0,6);
		       memcpy(ipanel_time,(char *)arg+2+4,2);
			month=strtod(ipanel_time,&end_ptr);
			memset(ipanel_time,0,6);
		       memcpy(ipanel_time,(char *)arg+2+4+2,2);
			day=strtod(ipanel_time,&end_ptr);
			memset(ipanel_time,0,6);
		       memcpy(ipanel_time,(char *)arg+2+4+1,2);
			hour=strtod(ipanel_time,&end_ptr);
			memset(ipanel_time,0,6);
		       memcpy(ipanel_time,(char *)arg+2+4+1+2,2);
			minute=strtod(ipanel_time,&end_ptr);
			memset(ipanel_time,0,6);
		       memcpy(ipanel_time,(char *)arg+2+4+1+2+2,2);
			second=strtod(ipanel_time,&end_ptr);
			eis_report("\n ipanel time=%d:%d:%d ",hour,minute,second);
			SetCurrentTime(hour,minute,second);
			#endif
			semaphore_wait(gp_SemVFD);
		       CH_CellControl(5,1);
			CH_CellControl(6,1);
                     CH_VFD_ShowChar(0,8,"         ");
			semaphore_signal(gp_SemVFD);
                     //CH_DisplayCurrentTimeLed();
			display_time=true;
			return IPANEL_OK;
		}
	case IPANEL_FRONT_PANEL_SET_INDICATOR:
		{
			Arg=arg;
			semaphore_wait(gp_SemVFD);
			switch(Arg->mask)
				{
				     case 1:
					 	#if 1
					 	if(Arg->value==0)
					 		{
					 		    CH_CellControl(20,1);
					 		}
						else
							{
							    CH_CellControl(20,0);
							}
						#endif
					break;
				     case 2:
					 	if(Arg->value==0)
					 		{
					 		    CH_CellControl(20,1);
					 		}
						else
							{
							    CH_CellControl(20,0);
							}
					break;
				     case 3:
					 	break;/* TUNER0�ĵ��ɵײ���ƣ��м��������*/
					 	if(Arg->value==0)
					 		{
					 		    CH_CellControl(18,1);
					 		}
						else
							{
							    CH_CellControl(18,0);
							}
					break;
						
				}
			semaphore_signal(gp_SemVFD);
		}
		break;
	default:
		eis_report ( "\n ipanel_porting_front_panel_ioctl  default" );
		return IPANEL_OK;
	}
}

static void Eis_time_play_Monitor(void *ptr)
{
	boolean last_tuner_lock=false;
	int loop;
	int DisplayTime=0;
	clock_t time;
	while(1)
	{
		time=time_plus(time_now(),ST_GetClocksPerSecond()/2);
		if(semaphore_wait_timeout(gpsema_eis_time_play,&time)!=0)
		{
			if((display_time)&&(pstBoxInfo->abiBoxPoweredState==1))
			{
				CH_LED_DisplayStandby(); /* ��Ҫ��ʾʱ�����ð����˸һ�� */
			}
		}
		else
		if(ipanel_event==IPANEL_EVENT_TUNER)
		{
			loop=0;
		while(1)
		{
				if(ipanel_event!=IPANEL_EVENT_TUNER)
					break;
				
				if ( ch_Tuner_EIS_StillLocked() )
			{
					eis_report("\n send msg IPANEL_DVB_TUNE_SUCCESS ,time=%d",get_time_ms());
					eis_api_msg_send ( IPANEL_EVENT_TYPE_DVB, IPANEL_DVB_TUNE_SUCCESS,locktuner_id );
					last_tuner_lock=true;
					ipanel_event=IPANEL_EVENT_UNKNOWN;
				break;
			}
				if(IPANEL_NVRAM_BURNING!=ipanel_porting_nvram_status(0,0))
				loop++;
				if(loop>16)
				{
					eis_report("\n send msg IPANEL_DVB_TUNE_FAILED,time=%d",get_time_ms());
					eis_api_msg_send ( IPANEL_EVENT_TYPE_DVB, IPANEL_DVB_TUNE_FAILED,locktuner_id );
					last_tuner_lock=false;
					ipanel_event=IPANEL_EVENT_UNKNOWN;
					break;
				}
				
				task_delay( ST_GetClocksPerSecond()/10);
			}
		}
		
	}
}
void EIS_time_paly_init( void )
{
	gpsema_eis_time_play = semaphore_create_fifo_timeout(0);	
	if ( ( Task_Create( Eis_time_play_Monitor, NULL,
					  1024*10, 7,
					  "Eis_time_play_Monitor", 0 ) ) == NULL )
	{
		eis_report ( "\n Eis_time_play_Monitor creat failed\n");
	}
	
}

boolean EIS_get_tuner_state()
{
	if(ipanel_event!=IPANEL_EVENT_TUNER)
		return true;
	else
		return false;
}
#if 0
/*
����˵����
	�������յ���������������������������ģ�顣
����˵����
	���������
		des��������������ַ
		len����������������
	�����������
	�� �أ�
		-1 �C �������ݴ�����Ǳ��豸�����������
		0 - �汾��ȣ���������;
		1 - ���°汾���ֶ�����;
		2 - ���°汾��ǿ��������
*/
INT32_T ipanel_upgrade_check(CHAR_T *des,UINT32_T len)
{
#ifdef __EIS_API_DEVICE_DEBUG__
	eis_report ( "\n++>eis ipanel_upgrade_check�ú���δ֧��" );
#endif
	return -1;	
}

/*
����˵����
	֪ͨ����ִ��ģ��ִ������������
����˵����
	���������
		des��������������ַ
		len����������������
	�����������
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_upgrade_start(CHAR_T *des, UINT32_T len)
{
#ifdef __EIS_API_DEVICE_DEBUG__
	eis_report ( "\n++>eis ipanel_upgrade_start�ú���δ֧��" );
#endif
	return IPANEL_ERR;	
}
#endif
#if 0
INT32_T ipanel_porting_get_frequency(int on_id,int ts_id,int service_id)
{
	INT32_T freq=0;
	SHORT  sTransIndex;				/* Ƶ������ */
	SHORT sProgIndex;   
		eis_report ( "\nipanel_porting_get_frequency ts_id=%d, service_id=%d", ts_id,ts_id,service_id);
	if((ts_id==0xffff)&&(service_id!=0xffff))
	{
		sProgIndex = pstBoxInfo->iHeadServiceIndex;
		while(sProgIndex != INVALID_LINK )
		{	
			if(pastProgramFlashInfoTable[sProgIndex]->stProgNo.unShort.sLo16 ==service_id)
			{   
				break;
			}
			sProgIndex = pastProgramFlashInfoTable[sProgIndex]->sNextProgIndex;
		}
		if(sProgIndex!=INVALID_LINK)
		{
			freq=pastTransponderFlashInfoTable[pastProgramFlashInfoTable[sProgIndex]->stProgNo.unShort.sHi16].iTransponderFreq*10;
		}
		
	}
	else
	if((ts_id!=0xffff)&&(service_id==0xffff))
	{
		sTransIndex = pstBoxInfo->iHeadXpdrIndex;

		while( sTransIndex != INVALID_LINK )
		{
			if( pastTransponderFlashInfoTable[sTransIndex].stTransponderNo.unShort.sLo16 == ts_id )
				break;

			sTransIndex = pastTransponderFlashInfoTable[sTransIndex].cNextTransponderIndex;
		}

		if( sTransIndex != INVALID_LINK )
		{
			freq=pastTransponderFlashInfoTable[sTransIndex].iTransponderFreq*10;
		}
		
	}
	else
	if((ts_id!=0xffff)&&(service_id!=0xffff))
	{
		sProgIndex = pstBoxInfo->iHeadServiceIndex;
		while(sProgIndex != INVALID_LINK )
		{	
			if(pastProgramFlashInfoTable[sProgIndex]->stProgNo.unShort.sLo16 ==service_id)
			{   
				break;
			}
			sProgIndex = pastProgramFlashInfoTable[sProgIndex]->sNextProgIndex;
		}
		if(sProgIndex!=INVALID_LINK)
		{
			freq=pastTransponderFlashInfoTable[pastProgramFlashInfoTable[sProgIndex]->stProgNo.unShort.sHi16].iTransponderFreq*10;
		}
		
	}
	return freq;
}
#endif
/*--eof----------------------------------------------------------------------------------------------------------*/

