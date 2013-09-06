#ifndef _GRAPHCOM__H_
#define _GRAPHCOM__H_

#include "status.h"
#include "graphconfig.h"
#include "common.h"
/*#include "ch_CQInfoService.h" */
extern ST_Partition_t          *appl_partition ;


#define gz_STATIC  /*��̬����*/

/*�ڴ������  ��  ��Ҫ����ƽ̨����*/




#define	CH_MIN(x,y)	((x)<(y)?(x):(y))
#define	CH_MAX(x,y)	((x)>(y)?(x):(y))
#define CH_ABS(x) (((x)<0)?(-(x)):(x))
#define CH_SQU(x)	((x) * (x))
#define CH_ZERO(x)	memset(&(x), 0, sizeof(x))

#ifndef EOF
#define EOF -1
#endif

#ifndef NULL
#undef  NULL
#define NULL (void*)0
#endif

/*�޷�������,���ݲ�ͬ��ƽ̨Ӧ�������ı�*/
#ifndef DEFINED_U8
#define DEFINED_U8
#define U8 unsigned char
#endif

#ifndef DEFINED_U16
#define DEFINED_U16
#define U16 unsigned short
#endif

#ifndef DEFINED_U32
#define DEFINED_U32
#define U32 unsigned  long
#endif

#ifndef DEFINED_U64
#define DEFINED_U64
#define U64 CH_UNSIGNED_64
#endif

#ifndef UINT
#define UINT unsigned int/* �������ã���ò���,����U32�������������������뾯�� */
#endif

/*�������������ݲ�ͬ��ƽ̨Ӧ�������ı�*/
#ifndef DEFINED_S8
#define DEFINED_S8
#define S8 signed char
#endif

#ifndef DEFINED_S16
#define DEFINED_S16
#define S16 signed short
#endif

#ifndef DEFINED_S32
#define DEFINED_S32
#define S32 signed  long
#endif

#ifndef DEFINED_S64
#define DEFINED_S64
#define S64 CH_SIGNED_64
#endif

#if 0
#ifndef INT
#define INT int /* �������ã���ò���,����S32�������������������뾯�� */
#endif
#endif/*5107ƽ̨�Ѿ�����*/

#ifndef CH_FLOAT 
#define CH_FLOAT float
#endif

#ifndef CH_BOOL
#define CH_BOOL int
#endif

#define CH_TRUE 1
#define CH_FALSE 0

#ifndef PU64
#define PU64 unsigned long int
#endif

/*ȡ�����Сֵ*/
#if 0
#define	CH_MIN(x,y)	((x)<(y)?(x):(y))
#define	CH_MAX(x,y)	((x)>(y)?(x):(y))
#define CH_ABS(x) (((x)<0)?(-x):(x))
#endif
#define ch_SCREEN_WIDTH (720 - 1)/*��ʾ��Ļ��С*/
#define ch_SCREEN_HIGH (576 - 1)

#define ch_SCREEN_ADJUST_X 	(0)/*��Ļ����ƫ��*/
#define ch_SCREEN_ADJUST_Y	(0)

#define ch_MaxWindowsNum 20/*�������򿪵Ĵ��ڸ���*/

#define DashedLength 4 /*���߶μ��������׵ĳ���*/

#define CH_IfColorGet(a)	{if(CH_NotCaseSensitiveStrCompare(ColorName,(S8*)(#a)) == 0)		\
						return (PCH_RGB_COLOUR)&ch_gcolor_##a;}	

#ifdef USE_ARGB1555

 #define CHZ_DrawColorPoint(r, g, b, buf)	\
 {		\
 	*((unsigned short*)(buf)) = 0x8000  		\
 						+ ((((unsigned short)(r) >> 3) & 0x1f) << 10)		\
 						+ ((((unsigned short)(g) >> 3) & 0x1f) << 5)		\
 						+ ((((unsigned short)(b) >> 3) & 0x1f));		\
 }
 #elif USE_ARGB4444
  #define CHZ_DrawColorPoint(r, g, b, buf)	\
 {		\
 	*((unsigned short*)(buf)) = 0xf000  		\
 						+ ((((unsigned short)(r) >> 4) & 0x0f) << 8)		\
 						+ ((((unsigned short)(g) >> 4) & 0x0f) << 4)		\
 						+ ((((unsigned short)(b) >> 4) & 0x0f));		\
 }
#elif defined(USE_ARGB8888)
 #define CHZ_DrawColorPoint(r, g, b, buf)	\
 {		\
 	*((unsigned char*)(buf + 3)) = 0xff;		\
 	*((unsigned char*)(buf + 2)) = r;		\
 	*((unsigned char*)(buf + 1)) = g;		\
 	*((unsigned char*)(buf + 0)) = b;		\
 }
#else
 #define CHZ_DrawColorPoint(r, g, b, buf)	\
 {		\
 	*((unsigned short*)(buf)) = ((g & 0xf0) + (b >> 4)) + (((unsigned short)(0xf0 + (r >> 4))) << 8);		\
 }
#endif
 
/* ���������CH_RGB_COLOUR �ṹ
 * ���ض�Ӧƽ̨��RBG��ɫֵ*/
#define  CH_GET_RGB_COLOR(RgbColor) RgbColor->Rgb##32##Entry


/*ͨ��ͼ�νӿں���*/


/*�����Ļ�ͼ���ݽṹ*/
typedef struct
{
	S16 	x;
	S16 	y;
}CH_POINT,*PCH_POINT;

typedef struct
{
	U16 	Left;
	U16 	Right;
	U16 Top;
	U16 Bottom;
}CH_AREA_LIMIT,*PCH_AREA_LIMIT;

typedef struct
{
	U8			uRed;
	U8			uGreen;
	U8			uBlue;
	U8			bAlphaState;
}CH_RGB15_ENTRY,*PCH_RGB15_ENTRY;

typedef	struct
{
	U8			uRed;
	U8			uGreen;
	U8			uBlue;
}CH_RGB16_ENTRY,*PCH_RGB16_ENTRY;

typedef	struct
{
	U8			uRed;
	U8			uGreen;
	U8			uBlue;
	U8			uAlpha;
}CH_RGB32_ENTRY,*PCH_RGB32_ENTRY;

typedef	struct
{
	U8			uRed;
	U8			uGreen;
	U8			uBlue;
	U8		       uAlpha;
}CH_RGB12_ENTRY;

typedef	union
{
	U8	iColourIndex;
	CH_RGB15_ENTRY	Rgb15Entry;
	CH_RGB16_ENTRY	Rgb16Entry;
	CH_RGB32_ENTRY	Rgb32Entry;
	CH_RGB12_ENTRY	Rgb12Entry;
}CH_RGB_COLOUR,*PCH_RGB_COLOUR;

typedef struct 
{
	int iStartX;
	int iStartY;/*������ڻ��߶�������Ͻǵ������*/
	int iWidth;
	int iHeigh;/*���ڻ��߶���ĸ߶ȺͿ��*/
}CH_RECT,*PCH_RECT;

typedef struct
{
	U16 iStartX;/*��ʼ������*/
	U16 iStartY;
	U16 iLeftLimit;/*����*/
	U16 iRightLimit;/*����*/
	U16 iEndX;/*����������*/
	U16 iEndY;
}CH_LimitArea, *PCH_LimitArea;/*�����壬��Ϊ��һ���ǹ������Σ�������Ҫ��������*/

typedef struct _CH_WINDOW_BUF
{
	U32 dwHandle;/*��Ļ��������ַ*/
	U16 width;
	U16 height;
	struct _CH_WINDOW_BUF* pNext;
}CH_WINDOW_BUF, *PCH_WINDOW_BUF;

typedef enum
{
	CH_HYPERLINK_T = 1,/*������*/
	CH_CHECKBOX_T,/*��ѡ��*/
	CH_RADIO_T,/*��ѡ��Ŧ*/
	CH_SELECT_T,/*�����˵�*/
	CH_BUTTON_T,/*��Ŧ*/
	CH_BASICDLG_T,/*��Ϣ�Ի���*/
	CH_EDIT_T,/*�����*/
	/*���϶����˳���ܸģ��Ҳ����Լ�����������*/
	
	CH_WINDOWS_MENU_T,/*����ϵͳ�Ĳ˵�*/
	CH_USER_DEFINED_EVENT_T/*�û��Զ��������Ԫ��*/
}CH_WinDlg_TYPE;

typedef struct  _CH_ACTIVE_NODE
{
	S8 Enable;/*0Ϊδ�����0Ϊ����*/
	PCH_RECT Place;/*λ�ô�С����Ϣ*/
	PCH_WINDOW_BUF ActFrame;/*��ʾĿǰ��ʾ�������ĵڼ�����*/
	U32 DelayTime;/*�ӳ�ʱ��*/
	U32 TimeCount ;/*��ʱ��*/
	PCH_WINDOW_BUF PicAddr;/*�ӵ�����*/
	void* buf;
	struct _CH_ACTIVE_NODE *Next;
} CH_ACTIVE_NODE, *PCH_ACTIVE_NODE;


typedef struct 
{
	CH_WINDOW_BUF scrBuffer;
	CH_RECT Screen;/*��ǰ���ڴ�С����Ϣ*/
	U16 SingleScrWidth;/*������*/
	U16 SingleScrHeight;/*������*/
	S16 ColorByte;/*��ʾһ���������õ��ֽ�����ע�⣬����λ��!*/
	PCH_RGB_COLOUR BackgroundColor;
	PCH_RGB_COLOUR CurrentColor;
	PCH_ACTIVE_NODE ActNode;/*��̬�ڵ㣬��Ҫ��ʱˢ�µĲ��֣��綯������������*/
	struct tagCONTROL* DlgControl;/*�ؼ����簴ť�ȵ�*/
	struct _ControlCom* ControlHead;/*�ؼ���������ͷ*/
}CH_WINDOW,*PCH_WINDOW;



typedef enum
{
	LR_TB = 1,/*�����ң����ϵ���*/
	RL_TB,/*���ҵ��󣬴��ϵ���*/
	TB_RL/*���ϵ��£����ҵ���*/
}CH_TEXT_DIRECTIOIN;/*���ַ���*/

typedef enum
{
	XING_KAI = 1,/*�п�*/
	SONG_TI,/*����*/
	FANG_SONG,/*����*/
	LI_SHU/*����*/
}CH_FONT_STYLE;/*����*/

typedef enum
{
	NONE = 1,/*��*/
	UNDERLINE ,/*�»���*/
	LINE_THROUGH,/*�ᴩ��*/
	OVERLINE/*�ϻ���*/
}CH_FONT_DICORATION;/*װ��*/

typedef struct
{
	S16 	iWidth;
	S16 	iHeight;
}CH_SIZE;/*��С*/


typedef struct 
{
	CH_FONT_STYLE Style;/*����*/
	CH_SIZE FontSize;/*��С*/
	PCH_RGB_COLOUR Color;/*��ɫ*/
	CH_TEXT_DIRECTIOIN Drct;/*���ַ���*/
	CH_FONT_DICORATION UnderLine;/*�»���*/
	S16 Oblique;/*������б��0Ϊ��������0Ϊ��б*/
	S16 WordSpacing;/*��������˼��Ӣ�ĵ���֮��ľ��룬�����Ϊ����������*/
	S16 LetterSpacing;/*Ӣ����ĸ֮��ļ��*/
	S16 LineHeight;/*�иߣ����������϶�������������*/
}CH_FONT, *PCH_FONT;/*�ֵ��й���Ϣ*/

 typedef enum
{
	CPT_JPG = 1,
	CPT_JPEG ,
	CPT_JPE,
	CPT_JFIF,
	CPT_GIF,
	CPT_BMP,
	CPT_DIB,
	CPT_PNG,
	CPT_PCX
}CH_PictureType;/*ͼ������*/

#define	id_backgroundRepeat(a)	id_backgroundRepeat_##a
typedef enum
{
	id_backgroundRepeat(repeat) = 1,/*����ͼ��������ͺ�����ƽ��*/
	id_backgroundRepeat(no_repeat),/*����ͼ��ƽ��*/
	id_backgroundRepeat(repeat_x),/*����ͼ���ں�����ƽ��*/
	id_backgroundRepeat(repeat_y),/*����ͼ��������ƽ��*/
	id_backgroundRepeat(room)/*���ŵ�ָ����Χ��С*/
}id_backgroundRepeat;
 

extern CH_STATUS CH_PutBitmapToScr3(PCH_WINDOW ch_this, PCH_RECT psrcRect, PCH_RECT pdestRect, int trans, S8* bitmap);
extern PCH_WINDOW CH_CreateWindow(S16 iWindowWidth, S16 iWindowHigh);
CH_PictureType CH_GetPicType(S8* FileName);
void CH_FreeMem(void * pAddr) ;
void *CH_AllocMem(U32 size);/*����size���ֽڵ��ڴ�*/


#define CH_MemSet memset
#define CH_MemCopy memcpy
	
#define J_OSP_FreeMemory CH_FreeMem


#endif


