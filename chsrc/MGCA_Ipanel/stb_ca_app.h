/******************************************************************************

  Copyright (C), 2001-2011, iPanel Co., Ltd.

 ******************************************************************************
  File Name     : stb_ca_app.h
  Version       : Initial Draft
  Author        : huzh
  Created       : 2009/6/1
  Last Modified :
  Description   : CA�ϲ�Ӧ�ö���ͷ�ļ����ṩ��iPanel�м��ʹ��
  Function List :
  History       :
    Modification: Created file

******************************************************************************/
#ifndef __STB_CA_APP_H__
#define __STB_CA_APP_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


/*******************************************************************************
*
*     ��������
*
*******************************************************************************/
/* canal CA������� */
#define STB_CA_MaxLib                     8

/* ���ܿ���Ʒ���볤�� */
#define STB_CA_CardProductCodeLength      2

/* ���ܿ����кų��� */
#define STB_CA_CardSerialNumberLength     6

/* λ���붨�� */
#define STB_CA_BitMask( n )               ( 1U << (n) )

/*******************************************************************************
*
*     ���Ͷ���
*
*******************************************************************************/
typedef char            INT8;  
typedef char            CHAR;
typedef unsigned char   UINT8; 
typedef signed short    INT16; 
typedef unsigned short  UINT16;
typedef signed long     INT32; 
typedef unsigned long   UINT32;
//typedef unsigned int    BOOL;

typedef void            VOID;


/*******************************************************************************
*
*     ���ݽṹ����
*
*******************************************************************************/
/* ��������״̬�� */
typedef enum tagSTB_CA_STATUS_CODE
{
   STB_CA_Ok,
   STB_CA_SysError,
   STB_CA_HWError,
   STB_CA_InvalidParam,
   STB_CA_NotFound,
   STB_CA_AlreadyExist,
   STB_CA_NotInitialized,
   STB_CA_Idling,
   STB_CA_Empty,
   STB_CA_Running,
   STB_CA_NotRunning,
   STB_CA_NoResource,
   STB_CA_NotReady,
   STB_CA_LockError,
   STB_CA_NotEntitled,
   STB_CA_NotAvailable,
   STB_CA_UNKnown
}STB_CA_STATUS_CODE_E;

/* ���ܿ�״̬�� */
typedef enum tagSTB_CA_CARD_REPORT
{
   STB_CA_CardOk,
   STB_CA_CardNotFound,
   STB_CA_CardBadParam,
   STB_CA_CardNoAnswer,
   STB_CA_CardFailure
}STB_CA_CARD_REPORT_E;

/* ���ܿ������� */
typedef enum tagSTB_CA_CARD_TYPE
{
	STB_CA_Unknown		= 0x00,
	STB_CA_Mediaguard	= 0x01,
	STB_CA_Multiple		= 0x02
} STB_CA_CARD_TYPE_E;

typedef enum tagSTB_CA_CARD_APPID
{
	STB_CA_IDUnknown,
	STB_CA_IDMediaguard
} STB_CA_CARD_APPID_E;

/* �¼�ID */
typedef enum tagSTB_CA_EVENTCODE
{
   STB_CA_EvSysError,
   STB_CA_EvNoResource,
   STB_CA_EvDumpText,
   STB_CA_EvMsgNotify,
   STB_CA_EvClose,
   STB_CA_EvHalted,
   STB_CA_EvExtract,
   STB_CA_EvBadCard,
   STB_CA_EvUnknown,
   STB_CA_EvReady,
   STB_CA_EvPassword,
   STB_CA_EvCmptStatus,
   STB_CA_EvRejected,
   STB_CA_EvAcknowledged,
   STB_CA_EvContent,
   STB_CA_EvAddFuncData,
   STB_CA_EvRating,
   STB_CA_EvUsrNotify,
   STB_CA_EvUpdate,
   STB_CA_EvOperator,
   STB_CA_EvGetId,
   STB_CA_EvNewMail,
   STB_CA_EvResetPwd,
   STB_CA_EvPairing
}STB_CA_EVENTCODE_E;

/* ������Լ������ */
typedef enum tagSTB_CA_EV_PAIRING_EXCODE
{
    STB_CA_Pair_Yes,    /* �����Ѿ�ƥ�� */
    STB_CA_Pair_No      /* ������ƥ�� */
}STB_CA_EV_PAIRING_EXCODE_E;

/* ����������ص�result code */
typedef enum tagSTB_CA_EV_PWD_EXCODE
{
   STB_CA_PwdNoCard,
   STB_CA_PwdHWFailure,
   STB_CA_PwdInvalid,
   STB_CA_PwdReset,
   STB_CA_PwdValid
}STB_CA_EV_PWD_EXCODE_E;

/* ����Ȩ��result code */
typedef enum tagSTB_CA_EV_ACK_EXCODE
{
   STB_CA_CMAck,
   STB_CA_CMOverdraft,
   STB_CA_CMPreview,
   STB_CA_CMPPV                   = STB_CA_BitMask(15)
}STB_CA_EV_ACK_EXCODE_E;

/* ����Ȩ��result code */
typedef enum tagSTB_CA_EV_REJECTED_EXCODE
{
   STB_CA_CMCardFailure           = STB_CA_BitMask(0),
   STB_CA_CMNANoRights            = STB_CA_BitMask(1),
   STB_CA_CMNAExpired             = STB_CA_BitMask(2),
   STB_CA_CMNABlackout            = STB_CA_BitMask(3),
   STB_CA_CMNARating              = STB_CA_BitMask(4),
   STB_CA_CMNAPPPV                = STB_CA_BitMask(5),
   STB_CA_CMNAIPPV                = STB_CA_BitMask(6),
   STB_CA_CMNAIPPT                = STB_CA_BitMask(7),
   STB_CA_CMNAMaxShot             = STB_CA_BitMask(8),
   STB_CA_CMNANoCredit            = STB_CA_BitMask(9),
   STB_CA_CMNAOthers              = STB_CA_BitMask(10)
}STB_CA_EV_REJECTED_EXCODE_E;

/* ��Ӫ����Ϣ��ȡ�������� */
typedef enum tagSTB_CA_OP_TYPE
{
   STB_CA_OP_GET_ONE,      /* ��ȡָ��OPI����Ӫ����Ϣ */
   STB_CA_OP_GET_FIRST,    /* ��ȡ��һ����Ӫ����Ϣ */
   STB_CA_OP_GET_NEXT      /* ��ȡ��һ����Ӫ����Ϣ */
}STB_CA_OP_TYPE_E;

/* ���˼���������Ч�� */
typedef enum tagSTB_CA_RATING_VALID
{
	STB_CA_Rating			= 0x01, /* ��ǰ������rating */
	STB_CA_RatingCheck	    = 0x02  /* ��ǰ������ratingCheck */
} STB_CA_RATING_VALID_E;


/* CA��ʼ������ */
typedef  struct  tagSTB_CA_CONFIG
{
   BOOL                 bCheckPairing;      /* �Ƿ���Ҫ��������� */
   BOOL                 bCheckBlackList;    /* �Ƿ�������� */
   BOOL                 bCardRatingBypass;  /* �Ƿ���Ҫ���˼� */
}STB_CA_CONFIG_S;

/* CA״̬ */
typedef  struct  tagSTB_CA_STATUS
{
   BOOL                 bCAStarted; /* CA״̬ TRUE��ʾCA�Ѿ����� FALSE��ʾδ����*/
   BOOL                 bIsPaired;  /* �������״̬,TRUE��ʾ�Ѿ����,������ʼ
                                       ��ΪҪ����������ʱ,��ֵ��Ч */
   STB_CA_CARD_REPORT_E enScStatus; /* ���ܿ�״̬ */
}STB_CA_STATUS_S;

/* ��Ӫ����Ϣ */
typedef struct tagSTB_CA_OPERATOR
{
   UINT16               uwOPI;            /* ��Ӫ��ID */
   UINT16               uwDate;           /* �������� */
   UINT8                aucName[16];       /* ��Ӫ������ */
   UINT8                aucOffers[8];      /* ��Ȩ������ ���庬����ο���Ӫ�̹涨 */
   UINT8                ucGeo;            /* ������ */
   UINT8                aucReserved[3];   /* ����δ�� */
} STB_CA_OPERATOR_S;

/* canal CA�İ汾��Ϣ */
typedef struct tagSTB_CA_REVISION
{
   CHAR         Name[24];       /* CA�Ŀ��� */
   CHAR         Date[9];        /* �ⴴ������ dd/mm/yy */
   CHAR         Release[15];    /* ��汾�� */
} STB_CA_REVISION_S;

/* canal CA��ID��Ϣ�ṹ */
typedef struct tagSTB_CA_ID
{
   STB_CA_REVISION_S    astVer[STB_CA_MaxLib];  /* ��汾��Ϣ */
   CHAR                 cCopyright[48];         /* ��Ȩ��Ϣ */
} STB_CA_ID_S;

/* ���ܿ�Ӧ�����ݽṹ */
typedef struct tagSTB_CA_CARD_APP
{
   UINT8        ID;             /* Ӧ��ID �ο�tagSTB_CA_CARD_APPID */
   UINT8        Major;          /* ���汾�� */
   UINT8        Minor;          /* ���汾�� */
   UINT8        ucReserved;     /* ����δ�� */
} STB_CA_CARD_APP_S; 

/* ���ܿ����ݽṹ */
typedef struct tagSTB_CA_CARD_CONTENT
{
   UINT8              ucProductCode[STB_CA_CardProductCodeLength];    /* ��Ʒ���� */
   UINT8              ucSerialNumber[STB_CA_CardSerialNumberLength];  /* ���к� */
   UINT8              ucApp;       /* ���ܿ�Ӧ�ø�������ǰֵΪ1 */
   UINT8              ucType;      /* ���ܿ����� �ο� tagSTB_CA_CARD_TYPE */
   UINT16             uwReserved;  /* ����δ�� */
   STB_CA_CARD_APP_S* pstAppData;  /* ���ܿ�Ӧ������ */
} STB_CA_CARD_CONTENT_S;

/* ���˼��ṹ */
typedef struct tagSTB_CA_RATING
{
   UINT8               ucValid;        /* �ο�tagSTB_CA_RATING_VALID */
   UINT8               ucRating;       /* ���˼�ֵ */
   UINT8               ucIsChecked;    /* �Ƿ�����˼� */
   UINT8               ucReserved;     /* ����δ�� */
} STB_CA_RATING_S;

/* ���ӹ����¼���Ӧ����Ӫ��������Ϣ */
typedef struct tagSTB_CA_AFD
{
   UINT16       uwOPI;                  /* ��Ӫ��ID */
   UINT8        aucData[10];            /* ���ӹ������� */
   UINT8        ucID;                   /* ���ӹ���ID */
   UINT8        aucReserved[3];             /* ����δ�� */
} STB_CA_AFD_S;

/* ��Ȩ��Ϣ��Ӧ��PID�б�ṹ */
typedef struct tagSTB_CA_PID_LIST
{
   UINT16*      puwPID;                 /* PID �б� */
   UINT8        ucNumber;               /* �б������ݸ��� */
   UINT8        aucReserved[3];            /* ����δ�� */
} STB_CA_PID_LIST_S;

/* canal CA�ʼ�ͷ�ṹ */
typedef struct tagSTB_CA_EMAIL_HEAD
{
    UINT8        ucTID;              /* table_id, Ӧ��Ϊ0x86 */
    UINT8        ucReserved;         /* ����δ�� */
    UINT8        ucNS[6];            /* ���ܿ����к� */
    UINT16       uwLong;             /* bit0��bit11: Length on 12 bits starting from the following byte
                                        bit12��bit13: MPEG reserved
                                        bit14: TBD
                                        bit15: Equal to 0
                                     */
    UINT16       uwOPI;              /* ��Ӫ��ID */
    UINT16       uwReserved;         /* ����δ�� */
    UINT8        ucPU1;
    UINT8        ucPU2;   
} STB_CA_EMAIL_HEAD_S;

/* canal CA�ʼ����ݽṹ */
typedef struct tagSTB_CA_EMAIL_DATA
{
   UINT8        ucType;             /* �ʼ����ȼ�. 0x02:��ͨ, 0x0B:���� */
   UINT8        ucIndex;            /* �ʼ����� */
   CHAR         szMessage[86];      /* ��Ϣ���� */
} STB_CA_EMAIL_DATA_S;

/* canal CA�ʼ��ṹ */
typedef struct tagSTB_CA_EMIAL_INFO
{
   STB_CA_EMAIL_HEAD_S   stEmailHeader;   /* �ʼ�ͷ */
   STB_CA_EMAIL_DATA_S   stEmailData;     /* �ʼ����� */
   UINT8                 aucReserved[3];  /* ����δ�� */
} STB_CA_EMIAL_INFO_S;

typedef VOID (*STBCAAppCallback)(UINT32 ulEventId, UINT32 ulResult, UINT32 ulDataLen, void* pData);
/*******************************************************************************
*
*     CA Ӧ��API����
*
*******************************************************************************/
extern INT32 STBInitCA(STB_CA_CONFIG_S  stConfig, STBCAAppCallback pfnNotify);
extern INT32 STBGetCAStatus(STB_CA_STATUS_S* pstStatus);
extern INT32 STBSetSysPassword(CHAR* szOldPwd, CHAR* szNewPwd);
extern INT32 STBCheckSysPassword(CHAR* szPwd);
extern INT32 STBGetOperatorInfo(STB_CA_OP_TYPE_E enType, UINT16 uwOpi);
extern INT32 STBStartRatingCheck(CHAR* szPwd);
extern INT32 STBStopRatingCheck(CHAR* szPwd);
extern INT32 STBGetRating(CHAR* szPwd);
extern INT32 STBSetRating(UINT8 ucRating, CHAR* szPwd);
extern INT32 STBGetCAId(void);
extern INT32 STBGetCardContent(void);
extern INT32 STBGetFuncData(UINT16 uwOpi, UINT8 ucIndex);
extern INT32 STBCardUpdate(void);
extern INT32 CAEventNotify(UINT32 udwEventId, UINT32 udwResult, UINT32 udwDataLen, void* pData);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* ifndef __STB_CA_APP_H__ */
