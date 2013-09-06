

#ifndef __mgif__
#define __mgif__
#include "stlite.h"
#include "pub_st.h"
#define ICHG(c1,c2)	c2*256+c1

#define boolea boolean
/*#define WORD long  /*00_GIF*/
#define LPCSTR char*

/*#define No_GIF_Back  00_060808*/



typedef struct
{	
	boolea 	active;					/* ��չ���ƿ��Ƿ���Ч */
	U32 		disposalMethod;			/* ���÷��� */
	boolea 	userInputFlag;			/* �û������־ */
	boolea 	trsFlag;					/* ͸��ɫ��־����λ��ʾʹ��͸��ɫ */
	WORD 	delayTime;				/* ����ͼ���ӳ�ʱ�� */
	U32 		trsColorIndex;			/* ͸��ɫ���� */
}GCTRLEXT;

typedef struct
{
	WORD 	imageLPos;				/* ��ͼX����ƫ���� 		*/
	WORD 	imageTPos;				/* ��ͼY����ƫ���� 		*/
	WORD 	imageWidth;				/* ��ͼ��� 					*/
	WORD 	imageHeight;			/* ��ͼ�߶� 					*/
	boolea 	lFlag;					/* �ֲ�ɫ���� 			*/
	boolea 	interlaceFlag;			/* ��֯��� 					*/
	boolea 	sortFlag;				/* �����ǣ������λ��ʾ�ֲ�ɫ��������� */
	U32 		lSize;					/* �ֲ�ɫ����� 			*/
	U8 		*pColorTable;			/* ָ����ɫ���ָ�� 	*/
	U8 		*dataBuf;				/* ָ�����ݵ�ָ�� 		*/
	GCTRLEXT ctrlExt;				/* ��չ���ƿ� 				*/
}FRAME;

typedef FRAME *LPFRAME;
typedef const  FRAME *LPCFRAME;

typedef struct
{	
	U32 		frames;					/* ������ͼ���� */
	WORD 	scrWidth, scrHeight;		/* �߼���Ⱥ͸߶� */
	boolea 	gFlag;					/* ȫ��ɫ���־ */
	U32 		colorRes;				/* ��ɫ��� */
	boolea 	gSort;					/* ȫ��ɫ���Ƿ�Ҫ�������� */
	U32 		gSize;					/* ȫ��ɫ���С */
	U32 		BKColorIdx;				/* ����ɫ���� */
	U32 		pixelAspectRatio;		/* ���ر� */
	U8 		*gColorTable;			/* ɫ������ָ�� */
}GLOBAL_INFO;

typedef GLOBAL_INFO *LPGLOBAL_INFO;
typedef const GLOBAL_INFO *LPCGLOBAL_INFO;

/* �ַ������ */
typedef struct
{	
	U32 				len;			/* ���� */
	unsigned char* 	p;			/* ����ָ�� */
}STRING_TABLE_ENTRY;

/*
 * �ṹ: ÿ֡ͼ�����Ϣ
 */
typedef struct mgif_frame_info_s{
	unsigned int	m_OffsetX;		/* x����ƫ��	*/
	unsigned int	m_OffsetY;		/* y����ƫ��	*/
	unsigned char	m_HandleMethod;	/* ���÷���		*/
	unsigned char	m_DelayTime;	/* �ӳ�ʱ��		*/
	unsigned char	m_LocalPalette;	/* �Ƿ�ʹ�þֲ���ɫ�� */
	FRAME		p_Frame;		/* ��ͼָ�� */
}mgif_frame_info_t;

/*
 * �ṹ: ÿ��gif����Ϣ
 */
typedef struct m_gif_file_info_s{
	unsigned char*		p_ActData;		/* ʵ������ָ��			*/
	unsigned char*		p_BackupData;	/* ��������ָ��			*/
	unsigned char*		p_DrawData;	/* ��������ָ��			*/
	unsigned char*		p_PaletteData;	/* ɫ������ָ�� 			*/
	unsigned char*		p_MethodData;	/* ���ڴ��÷�����ָ��  */
	unsigned int		m_StartX;		/* ��ͼ��ʼX */
	unsigned int		m_StartY;		/* ��ͼ��ʼY */
	unsigned int		m_Size;			/* �ļ���С				*/
	unsigned int		m_Width;		/* ͼ����				*/
	unsigned int		m_Height;		/* ͼ��߶�				*/
	unsigned int 		u_Width;		/*��ʾ����ĸ߶ȺͿ��*/
	unsigned int 		u_Height;
	unsigned char		m_TotalFrame;	/* ���ж���֡ͼ��		*/
	unsigned char		m_CurFrameIndex;/* ��ǰͼ��֡�����		*/
	char				m_PreFrameIndex;/* ��һ֡ͼ������ */
	char				m_PrevPrevIndex; /* ��һ֡����һ֡״̬ */
	int 			b_enalbe;/*���涯��ʱ����Ч��־*/
	mgif_frame_info_t	m_FrameInfo[100];/* ÿ֡ͼ�����Ϣ����	*/
	LPCGLOBAL_INFO	p_GlobeInfo;		/* ȫ��ָ�� */
}mgif_info_t, *mgif_handle;

extern	mgif_handle gifhandle;

#endif


extern void chz_ShowADGif( U8* rp_Data, U32 ru_Size, U16 ru_StartX, U16 ru_StartY, U8 ru_Nouse );
extern void chz_GetADGifSize( U8* rp_Data, U32 ru_Size, U16* ru_Width, U16* ru_Height);
extern void* mgif_malloc ( unsigned int size );
extern 	void mgif_DestroyHandle ( mgif_handle r_GifHandle );
//extern  STGFX_Handle_t GFXHandle;
/*--eof----------------------------------------------------------------------------*/

