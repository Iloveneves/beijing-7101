/*******************ͷ�ļ�˵��ע��**********************************/
/*��˾��Ȩ˵������Ȩ��2009���鳤������Ƽ����޹�˾���С�           */
/*�ļ�����MGCA_Ipanel.h                                            */
/*�汾�ţ�V1.0                                                     */
/*���ߣ�  �����                                                   */
/*�������ڣ�2009-06-24                                             */
/*ͷ�ļ����ݸ�Ҫ����������/�����ȣ���                              */
/*                                                                 */
/*�޸���ʷ���޸�ʱ��    ����      �޸Ĳ���       ԭ��              */
/*                                                                 */
/*******************************************************************/
/*****************�������벿��**************************************/
#ifndef CH_MGCA_IPANEL
#define CH_MGCA_IPANEL
/*******************************************************************/
/**************** #include ����*************************************/
#include "stddef.h"
#include "stlite.h"
#include "ipanel_typedef.h"
#include "ca_dmx_api.h"
#include "ca_emm_api.h"
#include "ca_srv_api.h"
#include "stb_ca_app.h"
#include "ca_pmt_api.h"
#include "ipanel_typedef.h"

/*******************************************************************/
/**************** #define ����**************************************/



/**************** #typedef �������� ********************************/


typedef struct
{
  	int original_network_id;
	int transport_stream_id;
	int service_id;
	int video_pid;
	int audio_pid;
	int pcr_pid;
}CH_Service_Info_t;

typedef enum
{   
  CH_CA2IPANEL_EXTRACT_CARD,
  CH_CA2IPANEL_BAD_CARD,
  CH_CA2IPANEL_UNKNOW_CARD,
  CH_CA2IPANEL_OK_CARD,
  CH_CA2IPANEL_Have_Right,
  CH_CA2IPANEL_NO_Right,
  CH_CA2IPANEL_EXPIRED,
  CH_CA2IPANEL_BLACKOUT,
  CH_CA2IPANEL_RESET_PIN,
  CH_CA2IPANEL_NEW_MAIL,
  CH_CA2IPANEL_PAIR_OK,
  CH_CA2IPANEL_PAIR_FAIL,
}CH_CA2IPANEL_EVT_t;



/***************  �ⲿ������������ *********************************/



/***************  �ⲿ������������ *********************************/

extern boolean  CHCA_CardSendMess2Usif (CH_CA2IPANEL_EVT_t  r_evt,U8 *Data,int ri_len);
extern void CH_MGCA_IpanelNotify(UINT32 ulEventId, UINT32 ulResult, UINT32 ulDataLen, void* pData);
extern void CH_MGCA_IpanelNotify(UINT32 ulEventId, UINT32 ulResult, UINT32 ulDataLen, void* pData);
extern void CH_MGCA_Ipanel_UpdateCardStatus(CH_CA2IPANEL_EVT_t r_evt,U8 *Data,int ri_len);





/*******************************************************************/
#endif

