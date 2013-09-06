/******************************************************************************/
/*    Copyright (c) 2005 iPanel Technologies, Ltd.                            */
/*    All rights reserved. You are not allowed to copy or distribute          */
/*    the code without permission.                                            */
/*    This is the demo implenment of the basic type definitions needed by     */
/*    iPanel MiddleWare.                                                      */
/*                                                                            */
/*    ���ļ�ֻ�ܷ���ȫ���Ե��������Ͷ�����ߺ궨�壬���������κα��ͷ�ļ�!!! */	
/*										---huangsl  2008-02                   */
/******************************************************************************/

/*
** �κ�struct/union��, �����ģ��˽�е�(����������ģ��䰴Լ���������ݵ�), �Ͻ����ڹ���header��! ��, ���Է�����
** ����: typedef struct tagMyStruct MyStruct; ��struct tagMyStruct��ʵ�嶨��һ��Ҫ����ģ���ڲ���.c�ļ���!
*/

#ifndef __IPANEL_TYPEDEF_H__
#define __IPANEL_TYPEDEF_H__

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
/* generic type redefinitions */
typedef int					INT32_T;
typedef unsigned int		UINT32_T;
typedef short				INT16_T;
typedef unsigned short		UINT16_T;
typedef char				CHAR_T;
typedef unsigned char		BYTE_T;
#define CONST				const
#define VOID				void

typedef struct
{
    UINT32_T uint64_32h;
    UINT32_T uint64_32l;
}IPANEL_UINT64_T;

typedef struct
{
    UINT32_T uint64_32h;
    UINT32_T uint64_32l;
}IPANEL_INT64_T;

/* general return values */
#define IPANEL_OK	0
#define IPANEL_ERR	(-1)
/* �������۵Ľ��, Ӧtujz��Ҫ��, ��һ�������Ķ���, ����ܶ�ı�������. --McKing 2008-12-4
#define IPANEL_NULL (void *)0 */
#define IPANEL_NULL 0

/* general iPanel version values */
#define IPANEL_MAJOR_VERSION   3
#define IPANEL_MINOR_VERSION   0

/******************************************************************************/
typedef struct tagGeneralControl GeneralControl;
typedef struct STBContext STBContext;
typedef struct tagiPanelAppParams iPanelAppParams;

/** Abstract browser object. */
typedef struct tagiPanelBrowser iPanelBrowser;
/** Control parameters for \ref iPanelBrowser_new. */
typedef struct tagiPanelBrowserParams iPanelBrowserParams;
/* 
** other type definitions *****************************************************
*/

/* AV sources */
typedef enum 
{
	IPANEL_AV_SOURCE_DEMUX,	//��Demuxģ����������
	IPANEL_AV_SOURCE_MANUAL	//ϵͳ������������
} IPANEL_AV_SOURCE_TYPE_e;

/* iPanel switch modes */
typedef enum{
	IPANEL_DISABLE,
	IPANEL_ENABLE
} IPANEL_SWITCH_e;

typedef enum
{
    IPANEL_DEV_USE_SHARED,               /* �������û�����ʹ���豸 */
    IPANEL_DEV_USE_EXCUSIVE              /* ��ռʹ���豸 */
} IPANEL_DEV_USE_MODE;


/**************************Defines for  stream data exchange******************/
// �������ݿ��ͷź���ָ��
typedef VOID (*IPANEL_XMEM_FREE)(VOID *pblk);

typedef struct
{
	VOID *pdes;                          /* pbuf�е������������� */
	IPANEL_XMEM_FREE pfree;   /* IPANEL_XMEMBLK���ݿ��ͷź���ָ�� */    
	UINT32_T *pbuf;                      /* ���ݻ�������ַ */
	UINT32_T len;                        /* ���������� */
} IPANEL_XMEMBLK, *pIPANEL_XMEMBLK;

/* XMEM block descriptor types*/
typedef enum
{
	IPANEL_XMEM_PCM = 1,
	IPANEL_XMEM_MP3	= 2,
	IPANEL_XMEM_TS	= 3,
	IPANEL_XMEM_ES	= 4,
	IPANEL_XMEM_GEN	= 5
} IPANEL_XMEM_PAYLOAD_TYPE_e;

typedef struct 
{
	IPANEL_XMEM_PAYLOAD_TYPE_e destype;    
	UINT32_T len;      /* buffer�����ݳ��� */
} IPANEL_XMEM_GEN_DES;


/* MP3 descriptor definition*/
typedef struct
{
	IPANEL_XMEM_PAYLOAD_TYPE_e destype;  /* 1����ʾPCM���������������ʽ������������Ͷ��� */
	UINT32_T samplerate;                 /* ����Ƶ�� HZ */
	UINT16_T channelnum;                 /* ͨ��������1��������2˫���� */
	UINT16_T bitspersample;              /* �������ȣ�bpp */
	UINT16_T bsigned;                    /* �з��Ż����޷���, 1�з��ţ�0�޷��� */
	UINT16_T bmsbf;                      /* �Ƿ��λ��ǰ��most significant bit first����1����λ��ǰ��0��λ��ǰ */
	UINT32_T samples;                    /* �������� */
} IPANEL_PCMDES, *pIPANEL_PCMDES;

/* ES descriptor definition*/
typedef struct 
{
	IPANEL_XMEM_PAYLOAD_TYPE_e destype;  /* IPANEL_XMEM_ES����ʾES���������������ʽ������������Ͷ��� */
	UINT32_T timestamp;    /* ʱ��� ����90KΪ��λ[��Ϊdvb����33bit��¼90kʱ�ӵģ��Ƿ��б�Ҫ����λ�����180K] */
	BYTE_T *ppayload;  /*��Ч������ʼ��ַ*/   
	UINT32_T len;      /*��Ч���ݳ��� */
} IPANEL_XMEM_ES_DES;

/**************************Defines for  stream data exchange end *******************/


/******************************************************************************/
typedef struct _StbGlobalVar StbGlobalVar;
struct _StbGlobalVar {
	char *name;
	char *value;
	StbGlobalVar *next;
};


/******************************************************************************/
#ifndef MEDIA_URL_LENGTH
#define MEDIA_URL_LENGTH		255
#endif
#ifndef MEDIA_TYPE_LENGTH
#define MEDIA_TYPE_LENGTH		32
#endif

struct tagEisMediaInstanceCtx
{
#if defined(PROJECT_HUAWEI_SPECIAL)
#define MEDIA_URL_LENGTH 1024
	char url[MEDIA_URL_LENGTH];
	char mediaType[MEDIA_TYPE_LENGTH];
#else
#define MEDIA_URL_LENGTH 255
	char url[MEDIA_URL_LENGTH];
	char mediaType[MEDIA_TYPE_LENGTH];
#endif
};
typedef struct tagEisMediaInstanceCtx EisMediaInstanceCtx;

/******************************************************************************/
#ifdef HAVE_IPANEL_LOADER
typedef struct{
    unsigned int man_id;
    unsigned int hard_ver;
    unsigned int soft_ver;	
    unsigned int freq;
    unsigned int symbol;
    unsigned short qam;
    unsigned short pid;
    unsigned char tid;
    unsigned char load_seq;
    unsigned char used;
} iPanelOTADownloadPara;
#endif


#ifdef __cplusplus
}
#endif

#endif // __IPANEL_TYPEDEF_H__
