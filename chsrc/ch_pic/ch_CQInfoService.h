/************************************************************************/
/* ��Ȩ���У��Ĵ���������Ƽ����޹�˾(2006)                             */
/* �ļ�����ch_CQInfoService.h                                           */
/* �汾�ţ� V1. 0														*/
/* ���ߣ�����															*/
/* �������ڣ�2006.7.13													*/
/* ͷ�ļ����ݸ�������������Ϣָ���ĵ�����������ݽṹ�ͽӿ�API			*/
/* �޸���ʷ:															*/
/* -- ʱ��           ����     �޸Ĳ��ּ�ԭ��							*/                     
/* 2006-7-28      ����    	1).CH_ENUM_COMMAND_TYPE ��������������
 *						    2).����CH_TIMEOUT_EXIST��CH_SCROLL_PARAMETERS��CH_SCROLL_TEXT�������ݽṹ
 *						    3).����CH_GetTimeOutExistCommadnAttribute ��CH_GetScrollTextCommadnAttribute����
 *						    4).����CH_GetScrollParametersAttribute��CH_GetTimeAttribute����
 *						    5).���� CH_GetTimeOutExist��CH_SetTimeOutExist����
 * 2006/07/20    ������     1).����   CH_GetKeywordAttributeStr���� 
 *                          2).����   CH_IsKeywordClose����
06 /08/01 ����ǿ 1. �Ľṹ��PCH_ACTION->ui_TextID Ϊ S8 ui_TextID[ID_NAME_MAX_LENGTH+1];
 				ԭΪU16 ui_TextID;
 			2. �Ľṹ��PCH_ACTION->Action_Z_Parameter.ui_TextID Ϊ S8 ui_TextID[ID_NAME_MAX_LENGTH+1];
 				ԭΪU16 ui_TextID;
 			3. ���ӽṹ��Ԫ��PCH_TEXT_COMMAND->Sct_Scollbar;
06 /09/19   ����  1) CH_ENUM_ACTION_TYPE �� ����ACTION_ZGADD,ACTION_ZGDEC,ACTION_ZLADD,ACTION_ZLDEC,ACTION_TVGUIDE
				  2) CH_ACTION �ṹ������ Action_ZG_ZL_Parameter,Action_TVGuide_Parameter 
				  3) CH_ENUM_COMMAND_TYPE ������e_COMMAND_FORCEACTION
				  4) ���ӽṹCH_FORCE_ACTION
*****************************ͷ�ļ�ע�ͽ���*****************************/  

#ifndef _CH_CQINFOSERVICE_H_
#define _CH_CQINFOSERVICE_H_

#include "common.h"
#include "os_plat.h"

#define ID_NAME_MAX_LENGTH 50/*ID���Ե���󳤶�*/
#define MAX_KEY_LENGTH 50 /*�ؼ�����󳤶�*/
#define CARD_ID_LENGTH	18/*���ܿ��ŵĳ���*/

#define z_DEFAULT_BKCOL_VUL 0xf1f2f3
/*ָ������*/
typedef enum _command_type_
{
	e_COMMAND_AV = 0, 
	e_COMMAND_REGION, 
	e_COMMAND_IMG,
	e_COMMAND_TEXT, 
	e_COMMAND_KEYACTION,
	e_COMMAND_TEXTLINK,
	e_COMMAND_TIMEOUTEXIST,
	e_COMMAND_SCROLLTEXT,
	e_COMMAND_FORCEACTION,	/*��һ��ʱ���ǿ������ACTIONָ��*/
	e_COMMAND_CARDID,
	e_COMMAND_STBID,
	e_COMMAND_STBIDTEXT,
	e_COMMAND_CARDIDTEXT,
	e_COMMAND_NUMINPUT, /* ��������ָ��*/
	e_COMMAND_COPYRIGHT,/*��ʾ�汾��Ϣ*/
	e_COMMAND_VCHANNEL,/*��ʱִ������*/
#ifdef NET_ENABLE	
	e_COMMAND_LINKRESOURCEINFO,/*��Դ����*/
#endif	
	e_COMMAND_EPG,/*��������ģ��*/
#ifdef NET_ENABLE	
	e_COMMAND_BLINK,/*���õ����������*/
#endif	
/*.....*/
	e_COMMAND_GROUPACTION,/*�����ָ�� */
	__END_COMMAND
}CH_ENUM_COMMAND_TYPE;

/*ת���ַ���*/
typedef struct _esc_char_ 
{
	S8* pc_EscChars;/*ת���*/
	S8* pc_RealChars;/*������ַ�*/
}CH_ESC_CHARS,*PCH_ESC_CHARS;

/*��ʾ������*/
typedef struct _ShowRect_ 
{
	CH_ENUM_COMMAND_TYPE enum_CommandType;/*ʹ�ø���ʾ�����ָ���ָ������*/
	S32 l_SX;/*����������ʼXλ��(���Ͻ�)*/
	S32 l_SY;/*����������ʼYλ��(���Ͻ�)*/
	S32 l_EX;/*�����������Xλ��(���½�)*/
	S32 l_EY;/*�����������Yλ��(���½�)*/
	S32 CX; /*���ĵ������, sqzow add 0812*/
	S32 CY;
	U32 ul_BKColor;/*24λ��ʾ��RGB��ɫֵ,��8λδ�ã���Ϊ0.
					����ɫ��ʾ��Ƶ�л���ʼǰ����ʾ�ı�����ɫ
					��ul_BKColor == 0xffffffffʱ��ʾû�иò���*/
	S32 l_Z;/*�������ʱ��Z����˳������Խ��Խ����*/
}CH_SHOWRECT,*PCH_SHOWRECT;

/* CH_ACTION �ṹ�� uc_AVType��ȡֵ��ָʾ����Ƶ������Ƶ*/
#define  AV_VIDEO  0			/*��Ƶ*/
#define  AV_AUDIO  1			/*��Ƶ*/

#define TYPE_AV	0
#define TYPE_DAV 	1

/* CH_ACTION �ṹ�� enum_Action ȡֵ */
typedef enum _action_type_
{
	ACTION_NONE = 0,		/*�޶���*/
	ACTION_AV,				/*�л�����Ƶ*/
	ACTION_VACTION,			/*�Ե�ǰ��Ƶ����������ǰΪ��Ƶ���ò�����Ч��*/
	ACTION_AACTION,			/*�Ե�ǰ��Ƶ����������ǰΪ��Ƶ���ò�����Ч��*/
	ACTION_AVACTION,			/*������Ƶ����ͳһ����  sqzow 0925*/
	ACTION_DAV,
	ACTION_RLINK,			/*��Ƶ���л���ǰҳ��*/
	ACTION_LINK,			/*�л���ǰҳ��*/
	ACTION_DLINK,	/*�л���ǰҳ��*/
	ACTION_CPAGEDOWN,		/*�·�ҳ*/
	ACTION_CPAGEUP,			/*�Ϸ�ҳ*/
	ACTION_Z,				/*����ָ��� Z ˳�� */
	ACTION_ZGADD,			/*����ĳһ��ָ��� Z ˳�򣬽�Z˳����ڵ�1 ������
	                                                  * ȫ��ָ���z ˳�����ӵ�2������ָ����ֵ*/
	ACTION_ZGDEC,			/*����ĳһ��ָ��� Z ˳�򣬽�Z˳����ڵ�1 ������
	                                                  * ȫ��ָ���z ˳����ټӵ�2������ָ����ֵ*/
	ACTION_ZLADD,			/*����ĳһ��ָ��� Z ˳�򣬽�Z˳��С�ڵ�1 ������
	                                                  * ȫ��ָ���z ˳�����ӵ�2������ָ����ֵ*/
	ACTION_ZLDEC,			/*����ĳһ��ָ��� Z ˳�򣬽�Z˳��С�ڵ�1 ������
	                                                  * ȫ��ָ���z ˳����ټӵ�2������ָ����ֵ*/
	ACTION_ZRADD,		/*��Z���ڲ���1С�ڲ���2�Ķ�����ϲ���3*/
	ACTION_ZRDEC,		/*��Z���ڲ���1С�ڲ���2�Ķ����ȥ����3*/
	ACTION_TVGUIDE,			/*��ת������ָ����Ƶ��*/
	ACTION_EXIT,			/*�˳�����ϢӦ��*/
	ACTION_CHANNEL_UP,/*Ƶ���Ӽ�*/
	ACTION_CHANNEL_DOWN,
	ACTION_VODAV,				
	ACTION_GA,		/*�����ָ��*/
	ACTION_VODLINK,
#ifdef NET_ENABLE	
	ACTION_HTTPGETLINK,
	ACTION_HTTPPOSTLINK,
	ACTION_BLINK,
#endif	
	ACTION_EPG
}CH_ENUM_ACTION_TYPE;

/*l_VActionParameter �� l_AActionParameter ��ȡֵ*/
#define AV_CLOSE		0	/*�رյ�ǰ�ĵ���AVָ��ŵ�����Ƶ����AVΪȫ���򱳾����ţ���ָ���
							  ��ȫ��״̬�����AVָ������Ƶ����ͬʱȫ��ָ���ĵ�����Z��������AV
							  ָ����������»��ơ����ĵ�����AVָ��ò�����Ч.*/
#define AV_OPEN			1	/*���ŵ�ǰ�ĵ��е�����Ƶ�����AVָ������Ƶ����ͬʱȫ��ָ���ĵ�����
							  Z��������AVָ����������»��ơ����ĵ�����AVָ��ò�����Ч.*/
#define AV_FULL			2	/*�ò���ȡֵֻ����Ƶ��ȫ�����ţ�ͬʱȫ��ָ���ĵ�����Z��������AVָ��
							  ���������»��ơ����ĵ�����AVָ�������Ƶû�в��ţ��ò�����Ч.*/
#define AV_BACK			3	/*�ò���ȡֵֻ����Ƶ����Ƶ�ָ���ԭ��С��ͬʱȫ��ָ���ĵ�����Z����
							  ����AVָ����������»��ơ����ĵ�����AVָ��,��������Ƶָ��,�ò�����Ч.*/

/*�����ͺͻ����ָ���Action�����Ͳ���*/
typedef struct _action_ 
{
	CH_ENUM_COMMAND_TYPE enum_CommandType;/*ʹ�ø�Action��ָ���ָ������*/
	CH_ENUM_ACTION_TYPE enum_Action; /* ��Ӧ�Ĵ���ʽ(Action) */
	union
	{
		struct Action_AV_Parameter 
		{
			U32 uc_AVType;
			U16 ui_NetworkID;
			U16 ui_TSID;
			U16 ui_ServiceID;
			U16 ui_ComponetID;/*�������Ƶ�����ӦComponet1����*/
			U16 ui_ComponetID2;
		}stru_AVParameter; /* Action ΪAV ʱ�Ĳ���*/

		struct Action_DAV_Parameter 
		{
			U32 uc_AVType;/*��16λ��ʾvodtype, ��16λ��ʾavtype.vodtype = 0: vod1,vod2; =3: vod3*/
			U32 ui_Freq;
			U16 ui_SymbolRate;
			U16 ui_PMTID;
			U32 ui_TimeOut;
			U16 ui_Componet1;/*�������ֵ������������ĸ�����ȷ�����Ե�����
							���Դ�ʱ��������ȷ,ֻ�������������   sqzow 061021*/
			U16 ui_Componet2;
			U16 ui_Componet3;
			U16 ui_Componet4;
		}stru_DAVParameter; /* Action ΪDAV ʱ�Ĳ���*/
		
		S32 l_AVActionParameter;/*Action Ϊ VAction �� AAction ʱ�Ĳ���*/
		
		struct Action_Rlink_Parameter 
		{
			U16 ui_NetworkID;
			U16 ui_TSID;
			U16 ui_ServiceID;
			U16 ui_ComponetID;
			U16 ui_Service_Page_ID;
		}stru_RLinkParameter; /* Action Ϊ RLink ʱ�Ĳ���*/

		struct Action_Dlink_Parameter 
		{
			U32 ui_Freq;
			U16 ui_SymbolRate;
			U16 ui_QAMType;
			U16 ui_ServiceID;/*��Ŀ��*/
			U16 ui_PID;/*���ݵ�PID*/
			U16 ui_Service_Page_ID;
			U32 ui_TimeOut;
		}stru_DLinkParameter; /* Action Ϊ DLink ʱ�Ĳ���*/

		struct Action_Link_Parameter/* Action Ϊ Link  �� EPGʱ�Ĳ��� */
		{
			U32 ui_Service_Page_ID;
			S8* HttpURL;/*��ui_Service_Page_ID��16λ��Ϊ0ʱ��Ч����ʾ��http������ibl�ļ�*/
		}stru_LinkParameter;
		
		S8 pc_TextID[ID_NAME_MAX_LENGTH];
		/* Action Ϊ CPageDown �� CPageUp ��GAʱ�Ĳ�����
			            ��IDΪTextID��Textָ��������з�ҳ�� ����ִ������Ϊ���ַ����������*/
							
		struct Action_Z_Parameter
		{
			S8 pc_ElementID[ID_NAME_MAX_LENGTH];
			S32 l_ZOrderInt;
		}stru_ZParameter;/*Action Ϊ Z ʱ�Ĳ���������IDΪElementIDָ���Z˳��ΪZOrderInt*/
						
		struct Action_ZG_ZL_Parameter
		{
			S32 l_ZOrderInt;
			S32 l_FIXInt;
		}stru_ZGZLParameter;/*Action ΪZGADD,ZGDEC,ZLADD,ZLDEC, ʱ�Ĳ�����*/

		struct Action_ZR_Parameter
		{
			S32 l_FIXInt;
			S32 l_ZOrderMinInt,l_ZOrderMaxInt ;
		}stru_ZRParameter;/*Action ΪZRDEC,ZRADD, ʱ�Ĳ�����*/


		struct Action_TVGuide_Parameter 
		{
			U16 ui_NetworkID;
			U16 ui_TSID;
			U16 ui_ServiceID;
		}stru_TVGuideParameter; /* Action Ϊ TVGuide ʱ�Ĳ���*/

#ifdef NET_ENABLE
		struct Action_Blink_Parameter /* Action Ϊ BLINK ʱ�Ĳ���*/
		{
			S8* BlinkURL;
		}stru_BlinkParameter;

		struct Action_HttpPostLink_Parameter
		{
			S8* pc_URL;
			S8* pc_IP;
			U16 ui_Port;
			S8* pc_Location;
			S8* pc_FileName;
			S8* pc_Params;
		}stru_HttpPostLinkParameter;

		struct Action_HttpGetLink_Parameter
		{
			S8* pc_URL;
			S8* pc_IP;
			U16 ui_Port;
			S8* pc_Location;
			S8* pc_FileName;
		}stru_HttpGetLinkParameter;
#endif		
	}uni_Data;
}CH_ACTION,*PCH_ACTION;

/*���� l_Init ��ȡֵ*/
#define  INIT_CLOSED					0	/*��ʼʱ������Ƶ�ǹرյ� */
#define  INIT_FULLSCREEN_PLAY			1	/*��ʼʱ����Ƶ��ȫ�����ţ�������Ƶ����Ч */
#define  INIT_AREA_PLAY					2	/*��ʼʱ����(sx,sy)(ex,ey)���򲥷���Ƶ��������Ƶ����Ч */
#define  INIT_NOT_CLOSESERIVECOMPONET	3	/*��ʼʱ�����رյ�ǰService	�е�����Ƶ Componet */

/*AV ָ��ṹ*/
typedef struct _AV_command_ 
{
	S8 pc_ID[ID_NAME_MAX_LENGTH+1];/*��ʾָ�����ĵ���Χ��Ψһ�ı��*/
	U16 ui_AVtype; /*������DAV����AV*/ 
	CH_SHOWRECT stru_ShowArea;/*��ʾ����ͱ���*/
	CH_ACTION stru_Action; /*Action�����Ͳ���*/
	S32 l_Init;/*��ʾ����Ƶ��ָ���ĵ�������ʾʱ��״̬*/
}CH_AV_COMMAND,*PCH_AV_COMMAND;

/*Region ָ��*/
typedef struct _region_command_ 
{
	S8 pc_ID[ID_NAME_MAX_LENGTH+1];/*��ʾָ�����ĵ���Χ��Ψһ�ı��*/
	CH_SHOWRECT stru_ShowArea;/*��ʾ����ͱ���*/
	U32 ui_BkImg;/*��ʾ�ı���ͼƬ��Դ������Э���е� Servive_Obj_Id,0��ʾû��BkImg���ԡ� */
	U32 ul_Transparent;/* ��ʾ��Region �İ�͸��ֵ */
}CH_REGION_COMMAND,*PCH_REGION_COMMAND;

/*Text ָ��*/
typedef struct _text_command_ 
{
	S8 pc_ID[ID_NAME_MAX_LENGTH+1];/*��ʾָ�����ĵ���Χ��Ψһ�ı��*/
	CH_SHOWRECT stru_ShowArea;/*��ʾ����ͱ���*/
	U32 ul_Color;/* 24λ��ʾ��RGB��ɫֵ,��8λδ�ã���Ϊ0.	����ɫ��ʾ�ı���ɫ*/
	CH_BOOL b_Nowarp;/* �� b_Nowarp == CH_TRUE ʱ��ex λ�ò��û���,ȱʡ�� CH_FALSE */
	CH_BOOL b_Scrollbar;/* �� b_Scrollbar == CH_TRUE ʱ���ı����� ey λ��ʱҪ��ʾ������,ȱʡ�� CH_FALSE */
	U32 Sct_Scollbar;/*��������������ݣ���b_Srollbar == ch_true ������*/
	S8* p_Content;/*��Ҫ��ʾ���ı���ʵ������,UTF-8��ʾ*/
}CH_TEXT_COMMAND,*PCH_TEXT_COMMAND;

typedef int (*chz_key_action_func)(PCH_ACTION pkey_action);

/* KeyAction ָ��*/
typedef struct _keyaction_command_ 
{
	S8 pc_ID[ID_NAME_MAX_LENGTH+1];/*��ʾָ�����ĵ���Χ��Ψһ�ı��*/
	U8 uc_Key;/* ��Ӧ�ļ�ֵ */
	CH_ACTION stru_Action; /*Action�����Ͳ���*/
	chz_key_action_func action_func;/*������*/
}CH_KEY_ACTION_COMMAND,*PCH_KEY_ACTION_COMMAND;

/* Img ָ��*/
typedef struct _img_command_ 
{
	S8 pc_ID[ID_NAME_MAX_LENGTH+1];/*��ʾָ�����ĵ���Χ��Ψһ�ı��*/
	CH_SHOWRECT stru_ShowArea;/*��ʾ����ͱ���*/
	U32 ui_SRC;/*��ʾ�ı���ͼƬ��Դ������Э���е� Servive_Obj_Id */
	U32 ui_FSRC;/*�����ͼƬ��Դ����*/
	CH_BOOL b_IsDefaultfocus;/*���Ϊ CH_TRUE ��ʾ��ָ�����ĵ���Ĭ�Ͻ���*/
	CH_ACTION stru_Action; /*Action�����Ͳ���*/
	chz_key_action_func action_func;/*������*/
}CH_IMG_COMMAND,*PCH_IMG_COMMAND;

/* TextLink ָ�� */
typedef struct _textlink_command_ 
{
	S8 pc_ID[ID_NAME_MAX_LENGTH+1];/*��ʾָ�����ĵ���Χ��Ψһ�ı��*/
	CH_SHOWRECT stru_ShowArea;/*��ʾ����ͱ���*/
	U32 ul_Color;/* 24λ��ʾ��RGB��ɫֵ,��8λδ�ã���Ϊ0.
					����ɫ��ʾ�ı���ɫ*/
	U32 ul_Fcolor;/*����ǰ����ɫ*/
	U32 ul_Fbkcolor;/*���㱳����ɫ*/
	CH_BOOL b_Nowarp;/* �� b_Nowarp == CH_TRUE ʱ��ex λ�ò��û���,ȱʡ�� CH_FALSE */
	CH_BOOL b_IsDefaultfocus;/*���Ϊ CH_TRUE ��ʾ��ָ�����ĵ���Ĭ�Ͻ���*/
 /*TXTLINK_SCROLL*/
	CH_BOOL b_FScroll;/*�Ƿ������ʾ���� */

	CH_ACTION stru_Action; /*Action�����Ͳ���*/

	S8* p_Content;/*��Ҫ��ʾ���ı���ʵ������,UTF-8��ʾ*/
	chz_key_action_func action_func;/*������*/
}CH_TEXTLINK_COMMAND,*PCH_TEXTLINK_COMMAND;

/*TimeOutExist ָ��*/
typedef struct _timeoutexist_command_
{
	S8 pc_ID[ID_NAME_MAX_LENGTH+1];/*��ʾָ�����ĵ���Χ��Ψһ�ı��*/
	U32 l_Time;/*��ʾ��ʱ�˳���ʱ�䶨��*/
}CH_TIMEOUT_EXIST,*PCH_TIMEOUT_EXIST;

/*������صĲ���*/
typedef struct _scroll_parameters_
{
	CH_ENUM_COMMAND_TYPE enum_CommandType;/*ʹ�ø�scroll_parameters��ָ���ָ������*/
	S32 l_ScrollType;/*������ʽ
						0:  �������
						1:  �������*/	
	U32 l_ScrollTime;/*���ι���ˢ��ʱ��*/
	S32 l_ScrollStep;/*���ι�������*/
}CH_SCROLL_PARAMETERS,*PCH_SCROLL_PARAMETERS;

/*ScrollText ָ��*/
typedef struct _scroll_text_
{
	S8 pc_ID[ID_NAME_MAX_LENGTH+1];/*��ʾָ�����ĵ���Χ��Ψһ�ı��*/
	CH_SHOWRECT stru_ShowArea;/*��ʾ����ͱ���*/
	U32 ul_Color;/* RGB��ɫֵ,	����ɫ��ʾ�ı���ɫ*/
	CH_BOOL b_Nowarp;/* �� b_Nowarp == CH_TRUE ʱ��ex λ�ò��û���,ȱʡ�� CH_FALSE */
	CH_SCROLL_PARAMETERS stru_ScrollParamters;
	S8* p_Content;/*��Ҫ��ʾ���ı���ʵ������,UTF-8��ʾ*/
}CH_SCROLL_TEXT,*PCH_SCROLL_TEXT;

/*ForceActionָ��*/
typedef struct _force_action_
{
	S8 pc_ID[ID_NAME_MAX_LENGTH+1];/*��ʾָ�����ĵ���Χ��Ψһ�ı��*/
	U32 l_Time;/*��ʾ��ʱ�˳���ʱ�䶨��*/
	CH_ACTION stru_Action; /*Action�����Ͳ���*/
	U32 i_CurrentTime;/*Ŀǰ�Ѿ���ȥ��ʱ��.*/
}CH_FORCE_ACTION,*PCH_FORCE_ACTION;

/*STBIDTextָ��*/
typedef struct _stbid_action
{
	S8 pc_ID[ID_NAME_MAX_LENGTH+1];/*��ʾָ�����ĵ���Χ��Ψһ�ı��*/
	U16 ui_Time, ui_CurrentTime;
	U16 ui_Type;
	S8 pc_CardID1[CARD_ID_LENGTH+1];
	S8 pc_CardID2[CARD_ID_LENGTH+1];
	S8 *pc_CardID;
	S8* p_Content;/*��Ҫ��ʾ���ı���ʵ������,UTF-8��ʾ*/
	chz_key_action_func action_func;/*������*/
	CH_ACTION stru_Action; /*Action�����Ͳ���*/
}CH_STBID_ACTION,*PCH_STBID_ACTION; /* Action Ϊ STBID ʱ�Ĳ���*/


/*CardIDTextָ��*/
typedef struct _cardid_action
{
	S8 pc_ID[ID_NAME_MAX_LENGTH+1];/*��ʾָ�����ĵ���Χ��Ψһ�ı��*/
	U32 ui_Time, ui_CurrentTime;
	U16 ui_Type;
	S8 pc_CardID1[CARD_ID_LENGTH+1];
	S8 pc_CardID2[CARD_ID_LENGTH+1];
	S8 *pc_CardID;
	S8* p_Content;/*��Ҫ��ʾ���ı���ʵ������,UTF-8��ʾ*/
	chz_key_action_func action_func;/*������*/
	CH_ACTION stru_Action; /*Action�����Ͳ���*/
}CH_CARDID_ACTION,*PCH_CARDID_ACTION; /* Action Ϊ CARDID ʱ�Ĳ���*/


/*STBIDTextָ��*/
typedef struct _stbidtext_
{
	S8 pc_ID[ID_NAME_MAX_LENGTH+1];/*��ʾָ�����ĵ���Χ��Ψһ�ı��*/
	CH_SHOWRECT stru_ShowArea;/*��ʾ����ͱ���*/
	U32 ul_Color;/* RGB��ɫֵ,	����ɫ��ʾ�ı���ɫ*/
	CH_BOOL b_Nowarp;/* �� b_Nowarp == CH_TRUE ʱ��ex λ�ò��û���,ȱʡ�� CH_FALSE */
	S8* p_Content;/*��Ҫ��ʾ���ı���ʵ������,UTF-8��ʾ*/
}CH_STBID_TEXT,*PCH_STBID_TEXT;

/*CardIDTextָ��*/
typedef struct _cardidtext_
{
	S8 pc_ID[ID_NAME_MAX_LENGTH+1];/*��ʾָ�����ĵ���Χ��Ψһ�ı��*/
	CH_SHOWRECT stru_ShowArea;/*��ʾ����ͱ���*/
	U32 ul_Color;/* RGB��ɫֵ,	����ɫ��ʾ�ı���ɫ*/
	CH_BOOL b_Nowarp;/* �� b_Nowarp == CH_TRUE ʱ��ex λ�ò��û���,ȱʡ�� CH_FALSE */
	S8* p_Content;/*��Ҫ��ʾ���ı���ʵ������,UTF-8��ʾ*/
}CH_CARDID_TEXT,*PCH_CARDID_TEXT;

/*COPYRIGHTָ��*/
typedef struct _copyright_
{
	CH_SHOWRECT stru_ShowArea;/*��ʾ����ͱ���*/
	U32 ul_Color;/* RGB��ɫֵ,	����ɫ��ʾ�ı���ɫ*/
	CH_BOOL b_Nowarp;/* �� b_Nowarp == CH_TRUE ʱ��ex λ�ò��û���,ȱʡ�� CH_FALSE */
}CH_COPYRIGHT,*PCH_COPYRIGHT;


/*NUMINPUTָ��*/
typedef struct _numInput_
{
	S8 pc_ID[ID_NAME_MAX_LENGTH+1];/*��ʾָ�����ĵ���Χ��Ψһ�ı��*/
	S8 str_Answer[20];
	int ui_type;/*0���������룬 1���ܿ�����(Ԥ��) 2������ʽ�� 3����*/
	int ui_length;
	CH_SHOWRECT stru_ShowArea;/*��ʾ����ͱ���*/
	U32 ul_Color;/* RGB��ɫֵ,	����ɫ��ʾ�ı���ɫ*/
	CH_BOOL b_Nowarp;/* �� b_Nowarp == CH_TRUE ʱ��ex λ�ò��û���,ȱʡ�� CH_FALSE */
	S8* p_Content;/*��Ҫ��ʾ���ı���ʵ������,UTF-8��ʾ*/
	chz_key_action_func action_func;/*������*/
	CH_ACTION stru_Action; /*Action�����Ͳ���*/
	CH_BOOL b_IsDefaultfocus;/*���Ϊ CH_TRUE ��ʾ��ָ�����ĵ���Ĭ�Ͻ���*/
}CH_NUM_INPUT,*PCH_NUM_INPUT;

/*VCHANNELָ��*/
typedef struct _timeraction_
{
	U16 ui_Timer;
	chz_key_action_func action_func;/*������*/
	CH_ACTION stru_Action; /*Action�����Ͳ���*/
}CH_TimerAction,*PCH_TimerAction; /*��ʱ����ָ��*/

/*GroupActionָ��*/
typedef struct _group_action_
{
	S8 pc_ID[ID_NAME_MAX_LENGTH+1];/*��ʾָ�����ĵ���Χ��Ψһ�ı��*/
	U32 l_Nums;/*ACTION��Ŀ*/
	S8* p_Content; /*���ݱհ�ָ�����ݡ� ������ɺ��ֵΪNULL*/
	CH_ACTION** ppActions; /*Action�����Ͳ���*/
}CH_GROUP_ACTION, *PCH_GROUP_ACTION;/*�����ָ��*/


#ifdef NET_ENABLE
/*��¼��ǰҳ�������ж����ID����*/
typedef struct _CommandPcIDRecord_
{
	S8* pc_ID;
	void* rp_Command;
	CH_ENUM_COMMAND_TYPE cmdType;
	unsigned long hash;
}CH_CmdPcIDRecord, *PCH_CmdPcIDRecord;

/*��¼��Ҫ��http������������url*/
typedef struct _HttpDataNeedResearch_
{
	char* url;
	char* dataType;
	unsigned long hash;
}CH_HttpDataNdSch_t, *PCH_HttpDataNdSch_t;
#endif

/*CMD��������*/
enum
{
/*����¼�¼*/
	INFO_ADDRECORD_E,
/*���ü�¼*/
	INFO_RESET_E,
/*ɾ�����м�¼*/
	INFO_DELETE_E,
/*��ѯ��¼������������*/
	INFO_SEARCH_E
};


/*����Ϊʵ�ֽ���ʹ�õĺ���*/
/**************************************************************************
** ����: �Ӹ������ַ����л�ȡָ�������
** ����: rp_Command,ָ��ṹָ�룬������ҪӦ��ת������Ӧ��ָ�����ݽṹ
**					ָ��ṹ����: CH_AV_COMMAND,CH_REGION_COMMAND,
**						CH_TEXT_COMMAND,CH_KEY_ACTION_COMMAND,CH_IMG_COMMAND,
**						CH_TEXTLINK_COMMAND,CH_TIMEOUT_EXIST,CH_SCROLL_TEXT
**       rp_Buffer: �����ַ���
**       rul_BufferLength: �����ַ�������
** ����: �ɹ����� CH_TURE, ʧ�ܷ��� CH_FALSE
**************************************************************************/
typedef CH_BOOL (*GET_KEY_ATTRIBUTE_FUNC)(void* rp_Command,S8 *rp_Buffer,U32 rul_BufferLength);

typedef struct _KeyMap_ 
{
	int enum_CommandTyep;	
	S8* pc_Key;/*�ؼ���*/
	GET_KEY_ATTRIBUTE_FUNC fn_GetAttribute;/*���ڸùؼ��֣���Ӧ�����Դ�����*/
}CH_KEY_MAP,*PCH_KEY_MAP;

/* ���������ַ���,���CH_AV_COMMANDָ�������
 * ����: rp_Command,CH_AV_COMMAND �ṹָ�� 
 * �����μ� GET_KEY_ATTRIBUTE_FUNC ˵�� */
CH_BOOL CH_GetAVCommadnAttribute(void* rp_Command,S8 *rp_Buffer,U32 rul_BufferLength);

/* ���������ַ���,��� CH_REGION_COMMAND ָ�������
 * ����: rp_Command,CH_REGION_COMMAND �ṹָ�� 
 * �����μ� GET_KEY_ATTRIBUTE_FUNC ˵�� */
CH_BOOL CH_GetRegionCommadnAttribute(void* rp_Command,S8 *rp_Buffer,U32 rul_BufferLength);

/* ���������ַ���,��� CH_TEXT_COMMAND ָ�������
 * ����: rp_Command,CH_TEXT_COMMAND �ṹָ�� 
 * �����μ� GET_KEY_ATTRIBUTE_FUNC ˵�� */
CH_BOOL CH_GetTextCommadnAttribute(void* rp_Command,S8 *rp_Buffer,U32 rul_BufferLength);

/* ���������ַ���,��� CH_KEY_ACTION_COMMAND ָ�������
 * ����: rp_Command,CH_KEY_ACTION_COMMAND �ṹָ�� 
 * �����μ� GET_KEY_ATTRIBUTE_FUNC ˵�� */
CH_BOOL CH_GetKeyActionCommadnAttribute(void* rp_Command,S8 *rp_Buffer,U32 rul_BufferLength);

/* ���������ַ���,��� CH_IMG_COMMAND ָ�������
 * ����: rp_Command,CH_IMG_COMMAND �ṹָ�� 
 * �����μ� GET_KEY_ATTRIBUTE_FUNC ˵�� */
CH_BOOL CH_GetImgCommadnAttribute(void* rp_Command,S8 *rp_Buffer,U32 rul_BufferLength);

/* ���������ַ���,��� CH_TEXTLINK_COMMAND ָ�������
 * ����: rp_Command,CH_TEXTLINK_COMMAND �ṹָ�� 
 * �����μ� GET_KEY_ATTRIBUTE_FUNC ˵�� */
CH_BOOL CH_GetTextlinkCommadnAttribute(void* rp_Command,S8 *rp_Buffer,U32 rul_BufferLength);

/* ���������ַ���,��� CH_TIMEOUT_EXIST ָ�������
 * ����: rp_Command,CH_TIMEOUT_EXIST �ṹָ�� 
 * �����μ� GET_KEY_ATTRIBUTE_FUNC ˵�� */
CH_BOOL CH_GetTimeOutExistCommadnAttribute(void* rp_Command,S8 *rp_Buffer,U32 rul_BufferLength);

/* ���������ַ���,��� CH_SCROLL_TEXT ָ�������
 * ����: rp_Command,CH_SCROLL_TEXT �ṹָ�� 
 * �����μ� GET_KEY_ATTRIBUTE_FUNC ˵�� */
CH_BOOL CH_GetScrollTextCommadnAttribute(void* rp_Command,S8 *rp_Buffer,U32 rul_BufferLength);

/* ���������ַ���,��� CH_FORCE_ACTION ָ�������
 * ����: rp_Command,CH_FORCE_ACTION �ṹָ�� 
 * �����μ� GET_KEY_ATTRIBUTE_FUNC ˵�� */
CH_BOOL CH_GetForceActionCommadnAttribute(void* rp_Command,S8 *rp_Buffer,U32 rul_BufferLength);
/**********************************************************************
** �������ַ����л��ID����
** ����: rpc_ID: �������,�洢������ID���ԡ��ѷ���ռ�,�����С�� ID_NAME_MAX_LENGTH+1,
**               �������������ID���ȴ���ID_NAME_MAX_LENGTH,���ȥ����Ĳ���
**       rp_Buffer: �����ַ���
**       rul_BufferLength: �����ַ�������
** ����: �ɹ����� CH_TURE, ʧ�ܷ��� CH_FALSE
**********************************************************************/
CH_BOOL CH_GetIDAttribute(S8* rpc_ID,S8 *rp_Buffer,U32 rul_BufferLength);

/**********************************************************************
** �������ַ����л��INIT����.
** ����: rp_Buffer: �����ַ���
**       rul_BufferLength: �����ַ�������
** ����ֵ: ���ؽ����õ���INIT,��������ַ�����û��INIT����,��Ӧ�÷���ȱʡֵ 2
**********************************************************************/
S32 CH_GetInitAttribute(S8 *rp_Buffer,U32 rul_BufferLength);

/**********************************************************************
** �������ַ����л��BkImg����.
** ����: rp_Buffer: �����ַ���
**       rul_BufferLength: �����ַ�������
** ����ֵ: ���ؽ����õ���BkImg����,��������ַ�����û��INIT����,��Ӧ�÷���ֵ0��
**         ��ʾû��BkImg���ԡ�
**********************************************************************/
U32 CH_GetBkImgAttribute(S8 *rp_Buffer,U32 rul_BufferLength);

/**********************************************************************
** �������ַ����л��Transparent����.
** ����: rp_Buffer: �����ַ���
**       rul_BufferLength: �����ַ�������
** ����ֵ: ���ؽ����õ���Transparent����,��������ַ�����û��Transparent����,
**         ��Ӧ�÷���ȱʡֵ 0
**********************************************************************/
U32 CH_GetTransparentAttribute(S8 *rp_Buffer,U32 rul_BufferLength);

/**********************************************************************
** �������ַ����л��Color����.
** ����: rp_Buffer: �����ַ���
**       rul_BufferLength: �����ַ�������
** ����ֵ: ���ؽ����õ���Color����
**********************************************************************/
U32 CH_GetColorAttribute(S8 *rp_Buffer,U32 rul_BufferLength);

/**********************************************************************
** �������ַ����л��Nowarp����.
** ����: rp_Buffer: �����ַ���
**       rul_BufferLength: �����ַ�������
** ����ֵ: ���ؽ����õ���Nowarp����,��������ַ�����û��Nowarp����,
**         ��Ӧ�÷���ȱʡֵCH_FALSE
**********************************************************************/
CH_BOOL CH_GetNowarpAttribute(S8 *rp_Buffer,U32 rul_BufferLength);

/**********************************************************************
** �������ַ����л��Scrollbar����.
** ����: rp_Buffer: �����ַ���
**       rul_BufferLength: �����ַ�������
** ����ֵ: ���ؽ����õ���Scrollbar����,��������ַ�����û��Scrollbar����,
**         ��Ӧ�÷���ȱʡֵCH_FALSE
**********************************************************************/
CH_BOOL CH_GetScrollbarAttribute(S8 *rp_Buffer,U32 rul_BufferLength);

/**********************************************************************
** �������ַ����л��Key(��ֵ)����.
** ����: rp_Buffer: �����ַ���
**       rul_BufferLength: �����ַ�������
** ����ֵ: ���ؽ����õ���Key����
**********************************************************************/
U8 CH_GetKeyAttribute(S8 *rp_Buffer,U32 rul_BufferLength);

/**********************************************************************
** �������ַ����л��Img��src����.
** ����: rp_Buffer: �����ַ���
**       rul_BufferLength: �����ַ�������
** ����ֵ: ���ؽ����õ���Img��src����,��������ַ�����û��src����,��Ӧ�÷���ֵ0��
**         ��ʾû��src���ԡ�
**********************************************************************/
U32 CH_GetImgSrcAttribute(S8 *rp_Buffer,U32 rul_BufferLength);

/**********************************************************************
** �������ַ����л��IsDefaultfocus����.
** ����: rp_Buffer: �����ַ���
**       rul_BufferLength: �����ַ�������
** ����ֵ: ���ؽ����õ���IsDefaultfocus����,��������ַ�����û��IsDefaultfocus����,
**         ��Ӧ�÷���ȱʡֵCH_FALSE
**********************************************************************/
CH_BOOL CH_GetIsDefaultfocusAttribute(S8 *rp_Buffer,U32 rul_BufferLength);

/**********************************************************************
** �������ַ����л�� CH_ACTION �������
** ����: rp_Action: �������,�洢������ CH_ACTION ��������ԡ��ѷ���ռ�.
**                  CH_ACTION �� enum_CommandTyp(CH_ENUM_COMMAND_TYPE)��Ա����
**                  �ú���������,�����ڵ��øú���ʱ�Ѿ�����.
**                  ����enum_CommandTyp == e_COMMAND_AV�ĵ���,����Ӧ������
**                  CH_ACTION�����ݳ�Աenum_Action(CH_ENUM_ACTION_TYPE)ΪACTION_AV
**       rp_Buffer: �����ַ���
**       rul_BufferLength: �����ַ�������
** ����: �ɹ����� CH_TURE, ʧ�ܷ��� CH_FALSE
**********************************************************************/
CH_BOOL CH_GetActionAttribute(PCH_ACTION rp_Action,	
						  S8 *rp_Buffer,
						  U32 rul_BufferLength);

/**********************************************************************
** �������ַ����л�� CH_SHOWRECT �������
** ����: rp_ShowRect: �������,�洢������ CH_SHOWRECT ������ԡ��ѷ���ռ�.
**                  CH_SHOWRECT ��enum_CommandTyp(CH_ENUM_COMMAND_TYPE)��Ա����
**                  �ú���������.
**       rp_Buffer: �����ַ���
**       rul_BufferLength: �����ַ�������
** ����: �ɹ����� CH_TURE, ʧ�ܷ��� CH_FALSE
**********************************************************************/
CH_BOOL CH_GetShowRectAttribute(PCH_SHOWRECT rp_ShowRect,	
						  S8 *rp_Buffer,
						  U32 rul_BufferLength);

/**********************************************************************
** �������ַ����л�� CH_SCROLL_PARAMETERS �������
** ����: rp_ScrollParameters: �������,�洢������ CH_SCROLL_PARAMETERS ������ԡ��ѷ���ռ�.
**                  CH_SCROLL_PARAMETERS ��enum_CommandTyp(CH_ENUM_COMMAND_TYPE)��Ա����
**                  �ú���������.
**       rp_Buffer: �����ַ���
**       rul_BufferLength: �����ַ�������
** ����: �ɹ����� CH_TURE, ʧ�ܷ��� CH_FALSE
**********************************************************************/
CH_BOOL CH_GetScrollParametersAttribute(PCH_SCROLL_PARAMETERS rp_ScrollParameters,	
						  S8 *rp_Buffer,
						  U32 rul_BufferLength);

/**********************************************************************
** �������ַ����л��time ����.
** ����: rp_Buffer: �����ַ���
**       rul_BufferLength: �����ַ�������
** ����ֵ: ���ؽ����õ���time ����,
**********************************************************************/
U32 CH_GetTimeAttribute(S8 *rp_Buffer,U32 rul_BufferLength);

/**********************************************************************
** ���ݹؼ���,�����Ӧ�����Դ�����
** ����: rpc_Key: �ؼ��֣���������ʱӦ�ò����ִ�Сд
** ����: �ɹ�,���ض�Ӧ�����Դ�������
**       ʧ��,����NULL
**********************************************************************/
GET_KEY_ATTRIBUTE_FUNC CH_GetAttributeHandleFunc(S8* rpc_Key);


/*����Ϊ�ⲿ���ýӿں���*/
/**********************************************************************
** ������Ϣ����
** ����: 
**      rui_PageId: ��Ҫ�������ĵ��� PageID
**		rpc_DataBuffer: �������Ļ���
**		rul_DataLength: ����ĳ���
** ����:
**     �ɹ����ط�0,ʧ�ܷ��� 0
***********************************************************************/
S32 CH_StartPageParse(U32 rui_PageId,S8* rpc_DataBuffer,U32 rul_DataLength);

/**********************************************************************
** ҳ�����,��Ҫ�Ǳ� CH_StartPageParse ����
** ����: 
**      rui_PageId: ��Ҫ�������ĵ��� PageID
**		rpc_DataBuffer: �������Ļ���
**		rul_DataLength: ����ĳ���
** ����:
**     �ɹ����ط�0,ʧ�ܷ��� 0
***********************************************************************/
S32 CH_ParsePage(U32 rui_PageId,S8* rpc_DataBuffer,U32 rul_DataLength);

/**********************************************************************
** ����ؼ���
** ����: 
**		rpp_InOutCurrentBuf: �������Ļ����ַ��ָ��,
**		rul_DataLength: ����ĳ���
** ����:
**     �ɹ����ط�0,rpp_InOutCurrentBuf�����ָ���Ƶ���һ��Ҫ�������ַ�λ��
**     ʧ�ܷ��� 0
***********************************************************************/
S32 CH_HandleKeyStart(S8** rpp_InOutCurrentBuf,U32 rul_CurrentBufLength);

/*��ʼ����Ϣ����ģ��,�������ʼ��ȫ�ֱ�����*/
S32 CH_InitInfoParser();

/*�˳���Ϣ����ģ��,�ͷ������Դ*/
S32 CH_DeleteInfoParser();

/*�õ��ĵ���TimeOutExistָ��,һ���ĵ�ҳֻ��һ��TimeOutExistָ��*/
PCH_TIMEOUT_EXIST CH_GetTimeOutExist();

/*�õ��ĵ���ForceActionָ�б�*/
PCH_GENERAL_ARRAY CH_GetForceActionArry();

/*�����ĵ���TimeOutExistָ��,һ���ĵ�ҳֻ��һ��TimeOutExistָ��*/
void CH_SetTimeOutExist(PCH_TIMEOUT_EXIST rp_TimeOutExist);

/*�õ��ĵ���AVָ��,һ���ĵ�ҳֻ��һ��AVָ��*/
PCH_AV_COMMAND CH_GetAVAction();

/*�����ĵ���AVָ��,һ���ĵ�ҳֻ��һ��AVָ��*/
void CH_SetAVAction(PCH_AV_COMMAND rp_AvAction);

/*�õ��ĵ���KeyAction����*/
PCH_GENERAL_ARRAY CH_GetKeyActionArray();

/*�õ��ĵ���XAction����*/
PCH_GENERAL_ARRAY CH_GetXActionArray();

/*�õ��ĵ���YAction����*/
PCH_GENERAL_ARRAY CH_GetYActionArray();

/*�õ��ĵ���ShowRect����*/
PCH_GENERAL_ARRAY CH_GetShowRectArray();

/*�õ��ĵ���TEXT����*/
PCH_GENERAL_ARRAY CH_GetTEXTArray();

PCH_GENERAL_ARRAY CH_GetTimerArray();

 PCH_GENERAL_ARRAY CH_GetGroupActionArray();


/*�õ���ǰ�ĵ���PageID*/
U16 CH_GetCurrentDocumentID();

/*���õ�ǰ�ĵ���PageID*/
void CH_SetCurrentDocumentID(U16 rui_PageID);

/*����Ԫ�ص�z ˳��.����ԭ����z ֵ*/
S32 CH_SetZCommand(PCH_SHOWRECT rp_ShowRect,S32 rl_Z);

/*******************************************************************
** ɾ����ǰ�ĵ�ҳ�������: ɾ��gsp_KeyAction,gsp_ShowRect�������е�ָ������.
** ���gsp_KeyAction,gsp_XAction,gsp_YAction,gsp_ShowRect,gsp_TEXT������
** Ϊ������(����ṹ��û�����,ֻ��������û��Ԫ�أ�Ԫ�ظ���Ϊ0�����黹����Ч)��
** ���� gsp_AVAction = NULL,gsui_CurrentPageID = 0;
** ����: ��
** ����: ��
/*******************************************************************/
void CH_DestroyCurrentIDPage();


/*******************************************************************
* ���ܣ� �������������rp_Buffer���Դ��вŲ���rp_SubString�ؼ��֣��ҵ��򽫽�
*        �����ص�rp_Result��.
*  ��������� rp_Buffuer;�����ַ���
*            rp_SubStr;�ؼ���
*            rul_BufferLen;�����ַ�������
*            rul_SubStrLen;�ؼ��ֳ���
* ���������
*            rp_Result;���عؼ��ֵ�����ֵ
* ����:  �ɹ�������rp_Resultd�ĳ���
*        ʧ�ܣ�����-1
/*******************************************************************/
U32 CH_GetKeywordAttributeStr(S8 *rp_Buffer,
                         S8 *rp_SubString,
						 S8 *rp_Result,
						 U32 rul_ResultLen,
						 U32 rul_BufferLength,
						 U32 rul_SubStrLen);


/**********************************************************************
** �ж��ǲ��Ǳհ�ָ��
** ����: 
**        sz_KeyWord: ���жϵ�ָ��;
** ����:
**     �ɹ�����true, ʧ�ܷ��� false;
***********************************************************************/
CH_BOOL CH_IsKeywordClose(S8 * sz_KeyWord);

/*******************************************************************************/
/*�������ܣ�
*           �Ƚ�.CH_ATION�е�uc_Key
*�����б�:
*		    pElement1 ��Ҫ�Ƚϵĵ�1��Ԫ�� 
*		    pElement2 ��Ҫ�Ƚϵĵ�2��Ԫ�� 
*		    pOtherArg ���Ƚ���Ҫ�������������
* ����ֵ:
* 	< 0     :     ��ʾ pElement1 С��  pElement2
*	0       :     ��ʾ pElement1 ����  pElement2
*	> 0     :     ��ʾ pElement1 ����  pElement2
****************************************�����嶨��*****************************/
S32 CH_KeyActionComp(void* pElement1,void* pElement2,void* pOtherArg);

/****************************����CH_ShowRectComp�Ķ���**************************
*�������ܣ�
*           �Ƚ�CH_SHOWRECT�е�l_Z;
*�����б�:
*		    pElement1 ��Ҫ�Ƚϵĵ�1��Ԫ�� 
*		    pElement2 ��Ҫ�Ƚϵĵ�2��Ԫ�� 
*		    pOtherArg ���Ƚ���Ҫ�������������
* ����ֵ:
* 	< 0     :     ��ʾ pElement1 С��  pElement2
*	0       :     ��ʾ pElement1 ����  pElement2
*	> 0     :     ��ʾ pElement1 ����  pElement2
****************************************�����嶨��*****************************/
S32 CH_ShowRectComp(void* pElement1,void* pElement2,void* pOtherArg);
/*************************����CH_StructActionComp�Ķ���**************************
*�������ܣ�
*           �Ƚ�CH_StructActionComp�е�xx;
*�����б�:
*		    pElement1 ��Ҫ�Ƚϵĵ�1��Ԫ�� 
*		    pElement2 ��Ҫ�Ƚϵĵ�2��Ԫ�� 
*		    pOtherArg ���Ƚ���Ҫ�������������
* ����ֵ:
* 	< 0     :     ��ʾ pElement1 С��  pElement2
*	0       :     ��ʾ pElement1 ����  pElement2
*	> 0     :     ��ʾ pElement1 ����  pElement2
****************************************�����嶨��*****************************/
S32 CH_StructActionComp(void* pElement1,void* pElement2,void* pOtherArg);

/*Ԫ�ص��ڴ��ͷź���*/
void CH_FreeArrayShowAreaElement(void* pElement, void *pOtherArg);


/**********************************************************************
** �������ַ����л��cardidtype ����.
** ����: rp_Buffer: �����ַ���
**       rul_BufferLength: �����ַ�������
** ����ֵ: ���ؽ����õ���time ����,
**********************************************************************/
U16 CH_GetTypeAttribute(S8 *rp_Buffer,U32 rul_BufferLength);

/* ���������ַ���,��� CH_CARDID_ACTION ָ�������
 * ����: rp_Command,CH_CARDID_ACTION �ṹָ�� 
 * �����μ� GET_KEY_ATTRIBUTE_FUNC ˵�� */
CH_BOOL CH_GetCardIDCommadnAttribute(void* rp_Command,S8 *rp_Buffer,U32 rul_BufferLength);


/* ���������ַ���,��� CH_STBID_ACTION ָ�������
 * ����: rp_Command,CH_STBID_ACTION �ṹָ�� 
 * �����μ� GET_KEY_ATTRIBUTE_FUNC ˵�� */
CH_BOOL CH_GetSTBIDCommadnAttribute(void* rp_Command,S8 *rp_Buffer,U32 rul_BufferLength);

/*���CARDIDTEXT �� STBIDTEXT������*/
CH_BOOL CH_GetCardIDTextCommadnAttribute(void* rp_Command,S8 *rp_Buffer,U32 rul_BufferLength);

/*���CARDIDTEXT �� STBIDTEXT������*/
CH_BOOL CH_GetSTBIDTextCommadnAttribute(void* rp_Command,S8 *rp_Buffer,U32 rul_BufferLength);

 PCH_CARDID_ACTION CH_GetCardID_Action();

 PCH_STBID_ACTION CH_GetSTBID_Action();
 
 CH_BOOL CH_GetNumInputAttribute(void* rp_NumInput,S8 *rp_Buffer,U32 rul_BufferLength);	
 
 CH_BOOL CH_GetCopyRightCommadnAttribute(void* rp_Command,S8 *rp_Buffer,U32 rul_BufferLength);

 CH_BOOL CH_GetVChannelCommadnAttribute(void* rp_Command,S8 *rp_Buffer,U32 rul_BufferLength);

     /* ���������ַ���,��� CH_GroupAction ָ�������
  * ����: rp_Command,CH_STBID_ACTION �ṹָ�� 
 * �����μ� GET_KEY_ATTRIBUTE_FUNC ˵�� */
 CH_BOOL CH_GetGroupActionAttribute(void* rp_Command,S8 *rp_Buffer,U32 rul_BufferLength);
 CH_BOOL CH_GetLinkResourceInfoAttribute(void* rp_Command,S8 *rp_Buffer,U32 rul_BufferLength);


#endif






