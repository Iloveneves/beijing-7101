/*******************�ļ�˵��ע��************************************/
/*��˾��Ȩ˵������Ȩ��2009���鳤������Ƽ����޹�˾���С�           */
/*�ļ�����MGCA_Ipanel.c                                            */
/*�汾�ţ�V1.0                                                     */
/*���ߣ�  �����                                                   */
/*�������ڣ�2009-06-23                                             */
/*ģ�鹦��:                                                        */
/*��Ҫ����������: ʵ��IPANEL3.0�м����MGCA֮ǰ���νӿ���          */
/*                                                                 */
/*�޸���ʷ���޸�ʱ��    ����      �޸Ĳ���       ԭ��              */
/*                                                                 */
/*******************************************************************/

/**************** #include ����*************************************/
#include "stddefs.h"  /* standard definitions           */
#include "stevt.h"

#include "stddefs.h"
#include "stcommon.h"
#include "appltype.h"

#include "ipanel_typedef.h"
#include "ca_dmx_api.h"
#include "ca_emm_api.h"
#include "ca_srv_api.h"
#include "stb_ca_app.h"
#include "ca_pmt_api.h"
#include "MGCA_Ipanel.h"
#include "..\chcabase\ChUtility.h"

static int               gi_pmt_filter_index = 1;                                     /*��ǰPMT FILTER����*/
static unsigned char     gc_pmt_version = 0xff;                                 /*��ǰPMT�汾��*/
static unsigned char     gc_pmt_recv_flag = 0;                                    /*��ǰPMT�Ƿ��Ѿ����յ���ʾ*/
static BOOL              gb_CAScrambleEnabled = 0;                              /*�Ƿ��Ѿ�����CA���Ŷ�����*/
static CH_Service_Info_t gch_CurServiceInfo;                                     /*��ǰ���Ž�Ŀ������Ϣ*/
static semaphore_t                    *gp_MGCA_Ipanel = NULL;
static STB_CA_CONFIG_S          g_STB_CA_ConfigInfo;                              /*CA����������Ϣ*/    
static STBCAAppCallback           g_STBCA_AppCallbackFuc = NULL;                            
STB_CA_STATUS_S                  g_STB_CA_Status;
STB_CA_OPERATOR_S              g_CA_OperatorList[16];                        /*CA ��Ϣ�б�*/
int                                           gi_OPINumber = 0;
STB_CA_ID_S                          g_CA_ID_S;                                          /*CA�汾��Ϣ*/                        
STB_CA_CARD_CONTENT_S      g_CA_CardContent;                              /*CA���ܿ�����*/
STB_CA_PID_LIST_S                g_CA_PIDList;
U16                                         g_PIDList[2];

/*������:void CH_MGCA_IPanel_Init(void)                                      */
/*�����˺Ϳ���ʱ��:                                                          */
/*������������:   ��ʼ��  MGCA_IPanelģ��������ݽṹ                        */
/*����ԭ��˵��:                                                              */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                                 */
/*���õ���Ҫ�����б�                                                       */
/*����ֵ˵����0���ɹ���-1��ʧ��                                              */
void CH_MGCA_IPanel_Init(void)
{
       gi_pmt_filter_index  = 1;
       gc_pmt_version       =  0xff;
       gc_pmt_recv_flag     = -1;
	gb_CAScrambleEnabled = 0;
       gch_CurServiceInfo.original_network_id = -1;
	gch_CurServiceInfo.transport_stream_id = -1;
	gch_CurServiceInfo.service_id          = -1;
	gch_CurServiceInfo.video_pid           = 8191;
	gch_CurServiceInfo.audio_pid           = 8191;
	gch_CurServiceInfo.pcr_pid             = 8191;
	g_STB_CA_Status.bCAStarted = 0;
       g_STB_CA_Status.bIsPaired  = 1;
       g_STB_CA_Status.enScStatus = STB_CA_CardNotFound;
	  
	memset(&g_CA_ID_S,0,sizeof(STB_CA_ID_S));
	memset(&g_CA_CardContent,0,sizeof(STB_CA_CARD_CONTENT_S));
	g_CA_PIDList.puwPID = g_PIDList;
	g_STBCA_AppCallbackFuc = NULL;
	gp_MGCA_Ipanel = semaphore_create_fifo(1);

}

/*������:void CH_MGCA_IPanel_Init(void)                                      */
/*�����˺Ϳ���ʱ��:                                                          */
/*������������:   ��ʼ��  MGCA_IPanelģ��������ݽṹ                        */
/*����ԭ��˵��:                                                              */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                                 */
/*���õ���Ҫ�����б�                                                       */
/*����ֵ˵����0���ɹ���-1��ʧ��                                              */
void CH_MGCA_Ipanel_GetService(CH_Service_Info_t *rp_ServiceInfo)
{
    if(rp_ServiceInfo == NULL)
	{
		return;
	}
	semaphore_wait(gp_MGCA_Ipanel);
	rp_ServiceInfo->original_network_id = gch_CurServiceInfo.original_network_id;
	rp_ServiceInfo->transport_stream_id = gch_CurServiceInfo.transport_stream_id;
	rp_ServiceInfo->service_id          = gch_CurServiceInfo.service_id         ;
	rp_ServiceInfo->video_pid           = gch_CurServiceInfo.video_pid          ;
	rp_ServiceInfo->audio_pid           = gch_CurServiceInfo.audio_pid          ;
	rp_ServiceInfo->pcr_pid             = gch_CurServiceInfo.pcr_pid            ;
	semaphore_signal(gp_MGCA_Ipanel);
}

static CH_CA2IPANEL_EVT_t ipanel_event_e = CH_CA2IPANEL_Have_Right;
#ifndef    NAGRA_PRODUCE_VERSION 	
static CH_CA2IPANEL_EVT_t last_ipanel_event_e = CH_CA2IPANEL_Have_Right;
#endif

/*������:INT32_T ipanel_cam_pmt_ctrl(IPANEL_CAM_PMT_CTRL_e act, BYTE_T *data, INT32_T len)          */
/*�����˺Ϳ���ʱ��:                                                          */
/*������������:                     */
/*����ԭ��˵��:                                                              */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                                 */
/*���õ���Ҫ�����б�                                                       */
/*����ֵ˵����0���ɹ���-1��ʧ��                                              */

INT32_T ipanel_cam_pmt_ctrl(IPANEL_CAM_PMT_CTRL_e act, BYTE_T *data, INT32_T len)
{
       unsigned short   us_servicetype;
       if(data == NULL)
       	{
                 return -1;
       	}
	if (/*act == IPANEL_CAM_SET_PMT &&*/ (data[0] == 0x02) && (len > 8)) /*PMT������Ч*/
	{
			
			gc_pmt_version = data[5] & 0x3E;
			
		
			   /*�����жϷ�������*/
		      ipanel_cam_get_curr_srv_type(&us_servicetype);
			if(us_servicetype == 0xF6)/*NVOD*/
			{
		            if(CHCA_AppPmtParseDes(data))
		      	      {
                                  CH_MGCA_Ipanel_UpdateCardStatus(CH_CA2IPANEL_Have_Right,NULL,0);
		      	     } 
			else
				{
                                   CH_MGCA_Ipanel_UpdateCardStatus(CH_CA2IPANEL_NO_Right,NULL,0);
				}
			}
			else		  
			{
				if (IPANEL_CAM_UPDATE_PMT == act)
				{
					ChSendMessage2PMT(gch_CurServiceInfo.service_id, gch_CurServiceInfo.transport_stream_id, START_PMT_NCTP);
				}
			            CHCA_InsertPMTData(gi_pmt_filter_index,data);/*֪ͨ����*/
			}

			
	}
	else
	{
		return -1;
	}
	return 0;
}
/*������:static INT32_T pmt_recv_notify(VOID* tag, BYTE_T* data, INT32_T len)*/
/*�����˺Ϳ���ʱ��:                                                          */
/*������������:   PMT����֪ͨ����                                            */
/*����ԭ��˵��:   the function must be non blocking                          */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                                 */
/*���õ���Ҫ�����б�                                                       */
/*����ֵ˵����0���ɹ���-1��ʧ��                                              */
static INT32_T pmt_recv_notify(VOID* tag, BYTE_T* data, INT32_T len)
{
       unsigned short   us_servicetype;
	if (tag && data && (tag == (void*)1000) && (data[0] == 0x02) && (len > 8)) /*PMT������Ч*/
	{
		if (gc_pmt_recv_flag == 0)
		{	
			gc_pmt_version = data[5] & 0x3E;
			
			gc_pmt_recv_flag = 1;
	              /*�����жϷ�������*/
		      ipanel_cam_get_curr_srv_type(&us_servicetype);
			if(us_servicetype == 0xF6)/*NVOD*/
			{
		            if(CHCA_AppPmtParseDes(data))
		      	      {
                                  CH_MGCA_Ipanel_UpdateCardStatus(CH_CA2IPANEL_Have_Right,NULL,0);
		      	     } 
			else
				{
                                   CH_MGCA_Ipanel_UpdateCardStatus(CH_CA2IPANEL_NO_Right,NULL,0);
				}
			}
			else		  
			{				
			      CHCA_InsertPMTData(gi_pmt_filter_index,data);/*֪ͨ����*/
			}

		}
		else if (gc_pmt_version != (data[5]&0x3E))       /*PMT�汾�仯*/
		{
			gc_pmt_version = data[5] & 0x3E;
		}
	}
	else
	{
		return -1;
	}
	return 0;
}
/*������:static INT32_T pmt_recv_notify(VOID* tag, BYTE_T* data, INT32_T len)*/
/*�����˺Ϳ���ʱ��:                                                          */
/*������������:  ����PMT������                                               */
/*����ԭ��˵��:                                                              */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                                 */
/*���õ���Ҫ�����б�                                                       */
/*����ֵ˵����                                                               */
static void add_pmt_filter(unsigned short srv_id, unsigned short pid)
{
	INT32_T nRet = -1;
	IPANEL_FILTER_INFO info[1] = {0};
	info->pid = pid;
	info->coef[0] = 0x02;
	info->coef[3] = (srv_id >> 8) & 0xff;
	info->coef[4] = srv_id & 0xff;
	info->mask[0] = 0xff;
	info->mask[3] = 0xff;
	info->mask[4] = 0xff;
	info->depth = 5;
	info->func = pmt_recv_notify;
	info->tag = (void*)1000;
	if (gi_pmt_filter_index == -1) /*û������PMT*/
	{
		gi_pmt_filter_index = ipanel_add_filter(info);
	}
	else if (gi_pmt_filter_index >= 0) /*֮ǰ�����PMT*/
	{
		nRet = ipanel_modify_filter(gi_pmt_filter_index, info);
	}
}
/*������:static void remove_pmt_filter(void)                                 */
/*�����˺Ϳ���ʱ��:                                                          */
/*������������:  �ͷ�PMT������                                               */
/*����ԭ��˵��:                                                              */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                                 */
/*���õ���Ҫ�����б�                                                       */
/*����ֵ˵����                                                               */
static void remove_pmt_filter(void)
{
	INT32_T nRet = -1;
	if (gi_pmt_filter_index >= 0) {
		nRet = ipanel_remove_filter(gi_pmt_filter_index);
		if (nRet >= 0)/*�ͷųɹ�*/
		{
			gi_pmt_filter_index = -1;
			gc_pmt_recv_flag    = 0;
		}
	}
}

#if 1
/*������:INT32_T ipanel_cam_srv_ctrl(IPANEL_CAM_ECM_CTRL_e act, IPANEL_CAM_SRV_INFO *psrv)*/
/*�����˺Ϳ���ʱ��:                                              */
/*������������:���ݺ����������ͣ�service��Ϣ��CA���/ɾ��/����   */
/*����ԭ��˵��:                                                  */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                     */
/*���õ���Ҫ�����б�                                           */
/*����ֵ˵����0���ɹ���-1��ʧ��                                  */
INT32_T ipanel_cam_srv_ctrl(IPANEL_CAM_ECM_CTRL_e act, IPANEL_CAM_SRV_INFO *psrv)
{
	if (act == IPANEL_CAM_ADD_SRV) /*��ӷ���*/
	{
		if (psrv) 
		{
			if(gb_CAScrambleEnabled == 1)/*��һ��������Ŀ���Ź���*/
			{
				gb_CAScrambleEnabled = 0;
				if(ChSendMessage2PMT(psrv->service_id,psrv->transport_stream_id,START_MGCA)==1)
				{
                    do_report(0,"\n[ipanel_cam_srv_ctrl==>] fail to send message the START_MGCA\n");
				}
			}
			else
			{
				if(gch_CurServiceInfo.transport_stream_id != psrv->transport_stream_id)
				{
					do_report(0,"NEW TSID = gi_LastPMT_TSID\n");
					if(ChSendMessage2PMT(psrv->service_id,psrv->transport_stream_id,START_PMT_CTP)==1)
					{
                        do_report(0,"\n[ipanel_cam_srv_ctrl==>] fail to send message the START_PMT_CTP\n");
					}
					
				}
				else
				{
					if(ChSendMessage2PMT(psrv->service_id,psrv->transport_stream_id,START_PMT_NCTP)==1)
					{
						do_report(0,"\n[ipanel_cam_srv_ctrl==>] fail to send message the START_PMT_NCTP\n");
					}
				}
			}
			#if 0
			/*ȷ���ͷ��ϴε�PMT FILTER*/
			remove_pmt_filter();
			
			/*��ʼ�µ�PMT����*/

			add_pmt_filter(psrv->service_id, psrv->pmt_pid);
                   #endif

            semaphore_wait(gp_MGCA_Ipanel);
            gch_CurServiceInfo.original_network_id = psrv->original_network_id;
	        gch_CurServiceInfo.transport_stream_id = psrv->transport_stream_id;
            gch_CurServiceInfo.service_id          = psrv->service_id;
	        gch_CurServiceInfo.video_pid           = psrv->video_pid;
	        gch_CurServiceInfo.audio_pid           = psrv->audio_pid;
	        gch_CurServiceInfo.pcr_pid             = psrv->pcr_pid;
            semaphore_signal(gp_MGCA_Ipanel);
		
		}
	}
	else if (act == IPANEL_CAM_DEL_SRV) /*ɾ������*/
	{
		//remove_pmt_filter();
	}
	else if(act == IPANEL_CAM_UPDATE_SRV)/*���·���*/
	{
		
	}
	return 0;
}
#endif
#if 0
/*������:INT32_T ipanel_cam_emm_ctrl(IPANEL_CAM_EMM_CTRL_e act, BYTE_T *data, INT32_T len)*/
/*�����˺Ϳ���ʱ��:                                                                       */
/*������������:���ݺ����������ͣ�CAT�����ݣ���������ֹͣEMM���ݽ���                       */
/*����ԭ��˵��:                                                                           */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                                              */
/*���õ���Ҫ�����б�                                                                    */
/*����ֵ˵����0���ɹ���-1��ʧ��                                                           */
INT32_T ipanel_cam_emm_ctrl(IPANEL_CAM_EMM_CTRL_e act, BYTE_T *data, INT32_T len)
{
     if(act == IPANEL_CAM_SET_EMM)        /*��ʼEMM���ݽ���*/
	 {

	 }
     else if(act == IPANEL_CAM_STOP_EMM)  /*ֹͣEMM���ݽ���*/
	 {

	 }
	 else if(act == IPANEL_CAM_UPDATE_EMM)/*����EMM���ݽ���*/
	 {

	 }
	 return 0;
}
#endif
/*������:INT32 STBInitCA(STB_CA_CONFIG_S  stConfig, STBCAAppCallback pfnNotify)           */
/*�����˺Ϳ���ʱ��:                                                                       */
/*������������: ����CAK                                                                   */
/*����ԭ��˵��:                                                                           */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                                              */
/*���õ���Ҫ�����б�                                                                    */
/*����ֵ˵����0���ɹ���-1��ʧ��                                                           */
INT32 STBInitCA(STB_CA_CONFIG_S  stConfig, STBCAAppCallback pfnNotify)
{
    memcpy(&g_STB_CA_ConfigInfo,&stConfig,sizeof(STB_CA_CONFIG_S));/*��ʱ����ʹ��*/
	/*��ʼ��ʼ��*/
	semaphore_wait(gp_MGCA_Ipanel);
    g_STBCA_AppCallbackFuc = pfnNotify;
	semaphore_signal(gp_MGCA_Ipanel);
	#ifdef  NAFR_KERNEL
	/*NAGRA FRANCE CA MODULE*/
	{
		extern  boolean  CH_CAModuleSetup(void);				
		if(CH_CAModuleSetup())
		{
			do_report(0,"Failed to initialise CH_CAModuleSetup\n");
			return -1; 
		}
	}	   
   #endif
    semaphore_wait(gp_MGCA_Ipanel);
	g_STB_CA_Status.bCAStarted = true;
	semaphore_signal(gp_MGCA_Ipanel);

	return 0;
}

/*������:INT32 STBGetCAStatus(STB_CA_STATUS_S* pstStatus)                                 */
/*�����˺Ϳ���ʱ��:                                                                       */
/*������������: CA״̬��ѯ                                                                */
/*����ԭ��˵��:                                                                           */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                                              */
/*���õ���Ҫ�����б�                                                                    */
/*����ֵ˵����0���ɹ���-1��ʧ��                                                           */
INT32 STBGetCAStatus(STB_CA_STATUS_S* pstStatus)
{
    if(pstStatus == NULL)
	{
       return -1;
	}
	else
	{
	   	semaphore_wait(gp_MGCA_Ipanel);
        memcpy(pstStatus,&g_STB_CA_Status,sizeof(STB_CA_STATUS_S));
	    semaphore_signal(gp_MGCA_Ipanel);
       return 0;
	}
}
/*������:INT32 STBSetSysPassword(CHAR* szOldPwd, CHAR* szNewPwd)                          */
/*�����˺Ϳ���ʱ��:                                                                       */
/*������������: ����ϵͳ����                                                              */
/*����ԭ��˵��:                                                                           */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                                              */
/*���õ���Ҫ�����б�                                                                    */
/*����ֵ˵����0���ɹ���-1��ʧ��                                                           */
INT32 STBSetSysPassword(CHAR* szOldPwd, CHAR* szNewPwd)
{
  return 0;
}
/*������:INT32 STBCheckSysPassword(CHAR* szPwd)                                           */
/*�����˺Ϳ���ʱ��:                                                                       */
/*������������: ���ϵͳ����ϵͳ����                                                      */
/*����ԭ��˵��:                                                                           */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                                              */
/*���õ���Ҫ�����б�                                                                    */
/*����ֵ˵����0���ɹ���-1��ʧ��                                                           */
INT32 STBCheckSysPassword(CHAR* szPwd)
{
  return 0;
}
/*������:INT32 STBGetOperatorInfo(STB_CA_OP_TYPE_E enType, UINT16 uwOpi)                  */
/*�����˺Ϳ���ʱ��:                                                                       */
/*������������: ��ȡ��Ӫ����Ϣ                                                            */
/*����ԭ��˵��:                                                                           */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                                              */
/*���õ���Ҫ�����б�                                                                    */
/*����ֵ˵����0���ɹ���-1��ʧ��                                                           */
INT32 STBGetOperatorInfo(STB_CA_OP_TYPE_E enType, UINT16 uwOpi)
{
	U32      OPINumber;                    /*��ǰOPI����*/
	if(enType == STB_CA_OP_GET_ONE || enType == STB_CA_OP_GET_FIRST)
            CHCA_IPanelGetOperatorInfo(g_CA_OperatorList,&gi_OPINumber);
	   
       if(gi_OPINumber <= 0)/*û�л�ȡ����Ϣ*/
	{
               CH_MGCA_IpanelNotify(STB_CA_EvOperator,STB_CA_CardNotFound,sizeof(STB_CA_OPERATOR_S),NULL);
	}
	else
	{
		if(enType == STB_CA_OP_GET_ONE)
		{
		#if 0
		for(OPINumber = 0;OPINumber<gi_OPINumber;OPINumber++)
			{
                          if(g_CA_OperatorList[OPINumber].uwOPI == uwOpi)
                          	{
                                break;
                          	}
			}
		#endif
		OPINumber = uwOpi;
		
		      if(OPINumber < gi_OPINumber)
			{
			  CH_MGCA_IpanelNotify(STB_CA_EvOperator,STB_CA_CardOk,sizeof(STB_CA_OPERATOR_S),&g_CA_OperatorList[OPINumber]);
			}
		else
			{
                        CH_MGCA_IpanelNotify(STB_CA_EvOperator,STB_CA_CardNotFound,sizeof(STB_CA_OPERATOR_S),NULL);
  
			}

		}
		else if(enType == STB_CA_OP_GET_FIRST)
		{
		        OPINumber = 0;
			 CH_MGCA_IpanelNotify(STB_CA_EvOperator,STB_CA_CardOk,sizeof(STB_CA_OPERATOR_S),&g_CA_OperatorList[OPINumber]);
		}
		else if(enType == STB_CA_OP_GET_NEXT)
		{
		#if 0
			for(OPINumber = 0;OPINumber<gi_OPINumber;OPINumber++)
			{
                          if(g_CA_OperatorList[OPINumber].uwOPI == uwOpi)
                          	{
                                break;
                          	}
			}
		#endif	
			OPINumber = uwOpi;
			 if(OPINumber < (gi_OPINumber-1))
			{
			  CH_MGCA_IpanelNotify(STB_CA_EvOperator,STB_CA_CardOk,sizeof(STB_CA_OPERATOR_S),&g_CA_OperatorList[OPINumber]);
			}
		else if(OPINumber == (gi_OPINumber-1))
			{
                        CH_MGCA_IpanelNotify(STB_CA_EvOperator,STB_CA_Idling,sizeof(STB_CA_OPERATOR_S),&g_CA_OperatorList[OPINumber]);

			}
		else
			{
                        CH_MGCA_IpanelNotify(STB_CA_EvOperator,STB_CA_CardNotFound,sizeof(STB_CA_OPERATOR_S),NULL);
  
			}
			
		}
	}
	return 0;
}
/*������:INT32 STBStartRatingCheck(CHAR* szPwd)                                           */
/*�����˺Ϳ���ʱ��:                                                                       */
/*������������: �������˼����                                                            */
/*����ԭ��˵��:                                                                           */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                                              */
/*���õ���Ҫ�����б�                                                                    */
/*����ֵ˵����0���ɹ���-1��ʧ��                                                           */
INT32 STBStartRatingCheck(CHAR* szPwd)
{
   return 0;
}
/*������INT32 STBStopRatingCheck(CHAR* szPwd)                                             */
/*�����˺Ϳ���ʱ��:                                                                       */
/*������������: ֹͣ���˼����                                                            */
/*����ԭ��˵��:                                                                           */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                                              */
/*���õ���Ҫ�����б�                                                                    */
/*����ֵ˵����0���ɹ���-1��ʧ��                                                           */
INT32 STBStopRatingCheck(CHAR* szPwd)
{
   return 0;
}
/*������INT32 STBGetRating(CHAR* szPwd)                                                   */
/*�����˺Ϳ���ʱ��:                                                                       */
/*������������: ��ȡ���˼�                                                                */
/*����ԭ��˵��:                                                                           */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                                              */
/*���õ���Ҫ�����б�                                                                    */
/*����ֵ˵����0���ɹ���-1��ʧ��                                                           */
INT32 STBGetRating(CHAR* szPwd)
{
   return 0;
}
/*������INT32 STBSetRating(UINT8 ucRating, CHAR* szPwd)                                   */
/*�����˺Ϳ���ʱ��:                                                                       */
/*������������: ��ȡ���˼�                                                                */
/*����ԭ��˵��:                                                                           */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                                              */
/*���õ���Ҫ�����б�                                                                    */
/*����ֵ˵����0���ɹ���-1��ʧ��                                                           */
INT32 STBSetRating(UINT8 ucRating, CHAR* szPwd)
{
   return 0;
}

/*������INT32 STBGetCAId(void)                                                            */
/*�����˺Ϳ���ʱ��:                                                                       */
/*������������: ��ȡCA�汾��Ϣ                                                            */
/*����ԭ��˵��:                                                                           */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                                              */
/*���õ���Ҫ�����б�                                                                    */
/*����ֵ˵����0���ɹ���-1��ʧ��                                                           */
INT32 STBGetCAId(void)
{

   if(CHCA_GetKernelRelease((char *)g_CA_ID_S.astVer[0].Release) == 0)
   	{
   	       memcpy(&g_CA_ID_S.astVer[0].Date,"25/06/09",sizeof("25/06/09"));
		memcpy(&g_CA_ID_S.astVer[0].Name,"NAGRA MG",sizeof("NAGRA MG"));  
              CH_MGCA_IpanelNotify(STB_CA_EvGetId,STB_CA_Ok,sizeof(STB_CA_ID_S),&g_CA_ID_S);
		return 0;

   	}
   else
   	{
   	      CH_MGCA_IpanelNotify(STB_CA_EvGetId,STB_CA_NotAvailable,sizeof(STB_CA_ID_S),NULL);
             return -1;
   	}
}

/*������INT32 STBGetCardContent(void)                                                     */
/*�����˺Ϳ���ʱ��:                                                                       */
/*������������: ��ȡ���ܿ�����                                                            */
/*����ԭ��˵��:                                                                           */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                                              */
/*���õ���Ҫ�����б�                                                                    */
/*����ֵ˵����0���ɹ���-1��ʧ��                                                           */
INT32 STBGetCardContent(void)
{
   STB_CA_CARD_APP_S  temp_ca_app;
        g_CA_CardContent.pstAppData = &temp_ca_app;
   if(CHCA_GetCardContent(g_CA_CardContent.ucSerialNumber ) == false)
   	{
   	  
              CH_MGCA_IpanelNotify(STB_CA_EvContent,STB_CA_Ok,sizeof(STB_CA_CARD_CONTENT_S),&g_CA_CardContent);
		return 0;

   	}
   else
   	{
   	      CH_MGCA_IpanelNotify(STB_CA_EvGetId,STB_CA_CardFailure,sizeof(STB_CA_CARD_CONTENT_S),NULL);
             return -1;
   	}
  
   return 0;
}
/*������INT32 STBGetFuncData(UINT16 uwOpi, UINT8 ucIndex)                                 */
/*�����˺Ϳ���ʱ��:                                                                       */
/*������������: ��ӹ�������                                                              */
/*����ԭ��˵��:                                                                           */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                                              */
/*���õ���Ҫ�����б�                                                                    */
/*����ֵ˵����0���ɹ���-1��ʧ��                                                           */
INT32 STBGetFuncData(UINT16 uwOpi, UINT8 ucIndex)
{
   return 0;
}

/*������INT32 STBCardUpdate(void);                                                        */
/*�����˺Ϳ���ʱ��:                                                                       */
/*������������:�������ܿ���Ϣ                                                             */
/*����ԭ��˵��:                                                                           */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                                              */
/*���õ���Ҫ�����б�                                                                    */
/*����ֵ˵����0���ɹ���-1��ʧ��                                                           */
INT32 STBCardUpdate(void)
{
   return 0;
}

/*������void CH_MGCA_Ipanel_UpdateCardStatus(CH_CA2IPANEL_EVT_t r_evt,U8 *Data)            */
/*�����˺Ϳ���ʱ��:                                                                       */
/*������������: ���¿���״̬u                                                          */
/*����ԭ��˵��:                                                                           */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                                              */
/*���õ���Ҫ�����б�                                                                    */
/*����ֵ˵����                                                 */

void CH_MGCA_Ipanel_UpdateCardStatus(CH_CA2IPANEL_EVT_t r_evt,U8 *Data,int ri_len)
	{

	 if(CH_GetSmartControlStatus()==false)/*��Ҫ������ģʽ*/
	 {
		semaphore_wait(gp_MGCA_Ipanel);
		ipanel_event_e = CH_CA2IPANEL_OK_CARD;
		g_STB_CA_Status.enScStatus = STB_CA_CardOk;
		CH_MGCA_IpanelNotify(STB_CA_EvReady,0,0,NULL);
		semaphore_signal(gp_MGCA_Ipanel);

		return;

	 }
	 
	do_report(0,"CH_MGCA_Ipanel_UpdateCardStatus r_evt =%d\n",r_evt);
	semaphore_wait(gp_MGCA_Ipanel);
	ipanel_event_e = r_evt;

	switch(r_evt)
	{
                case   CH_CA2IPANEL_EXTRACT_CARD:
			    g_STB_CA_Status.enScStatus = STB_CA_CardNotFound;
#ifdef SUMA_SECURITY
				CH_SKB_EvExtractAction();
#endif
			   CH_MGCA_IpanelNotify(STB_CA_EvExtract,0,0,NULL);	
			   break;
                case   CH_CA2IPANEL_BAD_CARD:
			   g_STB_CA_Status.enScStatus = STB_CA_CardFailure;
		          CH_MGCA_IpanelNotify(STB_CA_EvBadCard,0,0,NULL);
			  break;
                case   CH_CA2IPANEL_UNKNOW_CARD:
			   g_STB_CA_Status.enScStatus = STB_CA_CardFailure;
		          CH_MGCA_IpanelNotify(STB_CA_EvBadCard,0,0,NULL);
			  break;
                case   CH_CA2IPANEL_OK_CARD:
			   g_STB_CA_Status.enScStatus = STB_CA_CardOk;
			   CH_MGCA_IpanelNotify(STB_CA_EvReady,0,0,NULL);	
			  break;
                case   CH_CA2IPANEL_Have_Right:
			  g_CA_PIDList.puwPID[0] = gch_CurServiceInfo.video_pid;
		         g_CA_PIDList.puwPID[1] = gch_CurServiceInfo.audio_pid;
			  g_CA_PIDList.ucNumber = 2;
			  
			 CH_MGCA_IpanelNotify(STB_CA_EvAcknowledged,STB_CA_CMAck,sizeof(STB_CA_PID_LIST_S),&g_CA_PIDList);		
			  break;
                case   CH_CA2IPANEL_NO_Right:
			 g_CA_PIDList.puwPID[0] = gch_CurServiceInfo.video_pid;
		         g_CA_PIDList.puwPID[1] = gch_CurServiceInfo.audio_pid;
			  g_CA_PIDList.ucNumber = 2;
			  
			 CH_MGCA_IpanelNotify(STB_CA_EvRejected,STB_CA_CMNANoRights,sizeof(STB_CA_PID_LIST_S),&g_CA_PIDList);
			 break;
		case CH_CA2IPANEL_EXPIRED:
		         g_CA_PIDList.puwPID[0] = gch_CurServiceInfo.video_pid;
		         g_CA_PIDList.puwPID[1] = gch_CurServiceInfo.audio_pid;
			  g_CA_PIDList.ucNumber = 2;
			  CH_MGCA_IpanelNotify(STB_CA_EvRejected,STB_CA_CMNAExpired,sizeof(STB_CA_PID_LIST_S),&g_CA_PIDList);			
			 break;
                 case  CH_CA2IPANEL_BLACKOUT:
		         g_CA_PIDList.puwPID[0] = gch_CurServiceInfo.video_pid;
		         g_CA_PIDList.puwPID[1] = gch_CurServiceInfo.audio_pid;
			  g_CA_PIDList.ucNumber = 2;
			  CH_MGCA_IpanelNotify(STB_CA_EvRejected,STB_CA_CMNABlackout,sizeof(STB_CA_PID_LIST_S),&g_CA_PIDList);			
	 	
			  break;
                case   CH_CA2IPANEL_RESET_PIN:
			  CH_MGCA_IpanelNotify(STB_CA_EvResetPwd,0,ri_len,Data);			

			  break;
                case   CH_CA2IPANEL_NEW_MAIL:
			   CH_MGCA_IpanelNotify(STB_CA_EvNewMail,0,ri_len,Data);	
			 break;
                case   CH_CA2IPANEL_PAIR_OK:
			   CH_MGCA_IpanelNotify(STB_CA_EvPairing,STB_CA_Pair_Yes,0,NULL);	
			  break;
                case CH_CA2IPANEL_PAIR_FAIL:
			   CH_MGCA_IpanelNotify(STB_CA_EvPairing,STB_CA_Pair_No,0,NULL);	
			 break;

	}
      
	  
             semaphore_signal(gp_MGCA_Ipanel);
}


/*������VOID CH_MGCA_IpanelNotify(UINT32 ulEventId, UINT32 ulResult, UINT32 ulDataLen, void* pData)        */
/*�����˺Ϳ���ʱ��:                                                                       */
/*������������:֪ͨIPANEL CA������ݺ�״̬                                */
/*����ԭ��˵��:                                                                           */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                                              */
/*���õ���Ҫ�����б�                                                                    */
/*����ֵ˵����                                                 */
void CH_MGCA_IpanelNotify(UINT32 ulEventId, UINT32 ulResult, UINT32 ulDataLen, void* pData)
{
   if(g_STBCA_AppCallbackFuc != NULL)
   	{
           	      g_STBCA_AppCallbackFuc(ulEventId,ulResult,ulDataLen,pData);

   	}
}
/*������int STBCheckPaired(void)      */
/*�����˺Ϳ���ʱ��:                                                                       */
/*������������:                    */
/*����ԭ��˵��:                                                                           */
/*ʹ�õ�ȫ�ֱ�������ͽṹ��                                                              */
/*���õ���Ҫ�����б�                                                                    */
/*����ֵ˵����                                                 */
int STBCheckPaired(void)
{
    return CH_IPNAEL_MGCA();
}
