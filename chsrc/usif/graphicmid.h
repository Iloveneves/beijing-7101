/*
  Name:graphicmid.h
  Description:header file

Modifiction:

Date:Yixiaoli 2007-09-03 by yixiaoli  
1.Add ARGB8888 ��ʽ֧�֣���MACRO YXL_ARGB8888���ƣ����޸����º���֧��ARGB8888
����Ҫ�ǽ�����ɫ��صĲ�����int �ȸ�Ϊunsigned int)
(1)CH6_DisRectTxtInfoNobck(...);
(2)CH6_MulLine(...);
(3)DispLaySelect(...);
(4)CH6_DrawLinkLine(STGFX_Handle_t GHandle,STLAYER_ViewPortHandle_t VPHandle,int StartPosX,
		int StartPosY,int EndPosX,int EndPosY,U8 Ratio,
		int LineThickness,unsigned int LineColor)(...);
(5)CH6_DrawLinkLine(...);
(6)DrawTriangle(...);
(7)CH6_MulLine_ChangeLine(...);
(8)CH6_MulLine_ChangeLine1(...);
(9)CH6_Displayscoll(...);
(10)DrawTrigon1(...);
(11)DrawTrigon2(...);
*/


#include "stlayer.h"
#include "stgfx.h"
#include "stgxobj.h"
#include "graphicbase.h"

#ifndef GRAPH_MID
#define GRAPH_MID

#ifndef USE_GFX
#define USE_GFX             /*��Եײ�һЩ����ͼ�β�����ʾ�������޸�*/
#endif

/*ϵͳλ�����ö���*/

#define SYSTEM_TITLE_FRAM_X       75           /*ϵͳ���ñ����Xλ��*/
#define SYSTEM_TITLE_FRAM_Y       46           /*ϵͳ���ñ����Yλ��*/

#define SYSTEM_TIME_FRAM_X        502          /*ϵͳ����ʱ���Xλ��*/
#define SYSTEM_TIME_FRAM_Y        42           /*ϵͳ����ʱ���Yλ��*/
#define SYSTEM_TIME_FRAM_WIDTH        160      /*ϵͳ����ʱ�����*/
#define SYSTEM_TIME_FRAM_HEIGHT        50      /*ϵͳ����ʱ���߶�*/

#define SYSTEM_OUTER_FRAM_X       48          /*ϵͳ������߿�Xλ��*/
#define SYSTEM_OUTER_FRAM_Y       92          /*ϵͳ������߿�Yλ��*/
#define SYSTEM_OUTER_FRAM_WIDTH   604         /*ϵͳ������߿���*/
#define SYSTEM_OUTER_FRAM_HEIGHT  393         /*ϵͳ������߿�߶�*/

#define SYSTEM_INNER_FRAM_X       238        /*ϵͳ�����ڱ߿�Xλ��*/
#define SYSTEM_INNER_FRAM_Y       108        /*ϵͳ�����ڱ߿�Yλ��*/
#define SYSTEM_INNER_FRAM_WIDTH   403        /*ϵͳ�����ڱ߿���*/
#define SYSTEM_INNER_FRAM_HEIGHT  363        /*ϵͳ�����ڱ߿�߶�*/
#define SYSTEM_INFO_FRAM_X       246        /*ϵͳ���öԻ���Xλ��*/
#define SYSTEM_INFO_FRAM_Y       170        /*ϵͳ���öԻ���Yλ��*/


#define SYSTEM_FIRSTITEM_X   57             /*ϵͳ������ĿXλ��*/
#define SYSTEM_FIRSTITEM_Y   146              /*ϵͳ������ĿYλ��*/
#define SYSTEM_ITEM_WIDTH   175             /*ϵͳ������Ŀ���*/
#define SYSTEM_ITEM_HEIGHT  35                /*ϵͳ������Ŀ�߶�*/

#define SYSTEM_ITEM_GAP_BETWEEN   15        /*ϵͳ������Ŀ����*/
#define SYSTEM_ITEM_GAP_ARROW     15        /*ϵͳ������Ŀ�����¼�ͷ���*/
#define SYSTEM_ARROW_GAP_OUTER    20        /*ϵͳ�������¼�ͷ����߿���*/


#define SET_BANNER_X_START 300
#define SET_BANNER_Y_START 140
#define SET_BANNER_WIDTH   340
#define SET_BANNER_HEIGHT  250

/*20051115  WZ add  the  file  for AV  setting */

#define AVSET_ITEM_LEFT_X      238+10+10
#define AVSET_ITEM_LEFT_WIDTH       170     /* AV������Ŀ�Ŀ�*/
#define AVSET_ITEM_RIGHT_X    238+10+10+170+10
#define AVSET_ITEM_RIGHT_WIDTH     180
#define  AVSET_ITEM_Y               108+30+/*45 20060923 change*/30
#define AVSET_ITEM_HEIGH        30
#define AVSET_ITEM_H_GAP       20    /*AV������Ŀ��ˮƽ���*/
#define AVSET_ITEM_V_GAP       /*15 20060923 change*/10     /*AV������Ŀ�Ĵ�ֱ���*/


#define  AVSET_FRAM_X_START  238+10
#define  AVSET_FRAM_Y_START  108+30
#define  AVSET_FRAM_WIDTH      380
#define  AVSET_FRAM_HEIGH       300



#ifdef OSD_COLOR_TYPE_RGB16
#define TEXT_COLOR			0x910e/*F247*/
#define RECT_FILL_COLOR		0xF3DE/*FEFF*/

#else
#define TEXT_COLOR			0xff204070/*F247*/
#define RECT_FILL_COLOR		0xffe0f0f0/*FEFF*/
#endif

/***********/
#define CHARACTER_WIDTH 12
/*****all the pic name define***/



#ifndef OSD_COLOR_TYPE_RGB16  
#define RADIOBACK "BACK.MPG"
#define BLACK_PIC "BLACK.MPG"   

#define BUTTON_RETURN "b_return.ga"  /*yxl 2004-12-20 add this line*/
#define BACKGROUND_PIC "back.gam"       
#define FAV_PIC "Favo.gam"              
#define SKIP_PIC "skip.gam"
#define  LOCK_PIC "lock.gam"
#define  UNREAD_MAIL_PIC "mailnew.bmp"
#define  READ_MAIL_PIC "mailold.bmp"
#define  CLOCK_PIC    "clock.gam"
#define  MAIL_UNREAD_PIC    "mail.gam"
#define  MAIL_READ_PIC  "rmail.gam"
#define DLG1_PIC   "clock.gam"  
#define DLG2_PIC   "red.bmp"    
#define ADD1_PIC     "add1.gam"
#define ADD_PIC     "add.gam"
#define SUB1_PIC     "sub1.gam"
#define SUB_PIC     "sub.gam"
#define RADIO_PIC   "Radio.gam"
#define BAR_PIC     "bar.gam"
#define RED_BUTTON   "Rbutt.gam"
#define GREEN_BUTTON "Gbutt.gam"
#define YELLOW_BUTTON "Ybutt.gam"
#define BLUE_BUTTON  "Bbutt.gam"
#define  LOGMPG     "LOGO.MPG"
#define  RADIOMPG   "RADIO.MPG"  
#define MUTE_PIC    "mute.gam"
#define UNMUTE_PIC  "Unmute.gam"
#define BLUEFILL_PIC "Bfill.gam"
#define YELLOWFILL_PIC "Yfill.gam"
#define DIALOG1_PIC     "Dlg1.gam"
#define DIALOG2_PIC     "Dlg1.gam"/*"DlgTran.gam"*/
#define CH_TITLE_PIC       "CHTitle.gam"
#define EN_TITLE_PIC       "ENTitle.gam"
#define NVOD_CHI_TITLE_PIC "CHnvod.gam"
#define NVOD_ENG_TITLE_PIC "ENnvod.gam"
#define EN_EPGTITLE		"ENepg.gam"
#define CH_EPGTITLE		"CHepg.gam"
#define PROG_PIC	"prog.gam"
#define SPROG_PIC	"Sprog.gam"
/*ljg 040903 add*/
#define LEFTRIGHT_PIC "b_lr.gam"
#define UPARRF_PIC	"Uarrowf.gam"
#define UPARR_PIC	"Uarrow.gam"
#define DOWNARRF_PIC	"Darrowf.gam"
#define DOWNARR_PIC	"Darrow.gam"
#define BUTTON_A		"butt_A.gam"
#define BUTTON_B		"butt_B.gam"
#define BUTTON_C		"butt_C.gam"
#define BUTTON_PAGE	"b_page.gam"
#define BUTTON_OK		"b_ok.gam"
#define BUTTON_EXIT		"b_exit.gam"
#define LEFT_PROG_B 	"Left_B.gam"
#define LEFT_PROG_Y 	"Left_Y.gam"
#define RIGHT_PROG_B 	"Right_B.gam"
#define RIGHT_PROG_Y 	"Right_Y.gam"
#define RIGHT_PROG_W 	"Right_W.gam"
#define BUTTON_DOWN	"b_down.gam"
#define BUTTON_UP		"b_up.gam"
#define BUTTON_LEFT		"b_left.gam"
#define BUTTON_RIGHT	"b_right.gam"
#define BUTTON_MENU	"b_menu.gam"
#define BUTTON_INFO "b_info.gam"
#define CH_GAMETITLE	"CHgame.gam"
#define EN_GAMETITLE	"ENgame.gam"
#define YELLOW_PROG_2   "proy_2.gam"
#define YELLOW_PROG_5   "proy_5.gam"
#define YELLOW_PROG_10   "proy_10.gam"
#define YELLOW_PROG_20   "proy_20.gam"
#define YELLOW_PROG_50   "proy_50.gam"
#define BLUE_PROG_2   	"prob_2.gam"
#define BLUE_PROG_5   	"prob_5.gam"
#define BLUE_PROG_10  	 "prob_10.gam"
#define BLUE_PROG_20   	"prob_20.gam"
#define BLUE_PROG_50   	"prob_50.gam"
/*END ljg 040903 add*/

#define BLACK_MPG		"BLACK.MPG"

#define SEARCH_MPG		"Search.mpg"
#define MULTI_AUDIO                "MultAud.gam"
#define MULTI_CH                   "MultC.gam"
#define MULTI_EN                   "MultE.gam"
#define AC3_SIGN                    "ac3.gam"   /* 20061203 wz add*/


#define SYSTEM_FONT_NORMA_COLORL         0xff204070
#define SYSTEM_FONT_FOCUS_COLOR      /*  0XDB5C*/0xfff8f8f8
#define SYSTEM_TIME_TEXT_COLOR           	0xff205080
#define SYSTEM_SELECT_ITEM_FILL_COLOR  0xfff08020
#define SYSTEM_SELECT_ITEM_BORDER_COLOR  0xffc0d0e0
#define SYSTEM_NORNAL_ITEM_FILL_COLOR  0xffc0d0e0
#define SYSTEM_NORMAL_ITEM_BORDER_COLOR 0xff5080b0
#define SYSTEM_INNER_FRAM_BORDER_COLOR	 0xff5080b0
#define SYSTEM_OUTER_FRAM_BORDER_COLOR   0xffd0d0e0
#define SYSTEM_WAITFONT_BACKCOLOR      0xff7090b0
#define SYSTEM_TIME_BACK_COLOR            0xff80a0d0/*ϵͳ����ʱ�����屳����ɫ*/
#define SYSTEM_INNER_FRAM_COLOR          0xffc0d0e0  /*ϵͳ�����ڿ���ɫ*/
#define SYSTEM_OUTER_FRAM_COLOR         0xff80a0c0/*   ϵͳ���������ɫ*/
#define SYSTEM_lIST_FILL_COLOR          0xffe0f0f0
#define SYSTEM_lIST_BORDER_COLOR         0xff5080b0
#define SYSTEM_BUTTON_COLOR		0xffe0f0f0
#define SYSTEM_BUTTONBORDER_COLOR	0xff70a0c0
/*yxl 2004-12-18 modify this section*/
#if 0
#define SEARCH_INFO_COLOR          0x8CDE    /*ϵͳ�����ڿ���ɫ*/
#else
#define SEARCH_INFO_COLOR          0xffd0e0f0/*0x3fff */   /*ϵͳ�����ڿ���ɫ*/
#endif
/*end yxl 2004-12-18 modify this section*/

#define SYSTEM_HELP_FILL_COLOR           0xff6080a0/*  ϵͳ���ð�����Ϣ���ɫ*/
#define SYSTEM_HELP_BORDER_CLOR       0xffc0d0e0  /*ϵͳ���ð�����Ϣ�߿�ɫ*/      

#define SCROLL_FILL_COLOR                0xff6080a0   /*���ͱ���ɫ*/
#define SCROLL_SLIDER_COLOR              0xffd0e0f0    /*������ɫ*/


#define whitecolor                       0xfff8f8f8
#define RedColor                         0xffa02030
#define BlueColor                        0xff305080
#define GreenColor                       0xff00c000
#define orange_color				0xfff08020
#define PROG_YELLOW_COLOR			     0xfff07020
#define PROG_BLUE_COLOR			         0xff7090c0

#if 0 /*yxl 2004-12-22 modify this line*/
#define SEARCH_TEXT_COLOR				0xf106
#else
#define SEARCH_TEXT_COLOR				0xff6090d0
#endif/*end yxl 2004-12-22 modify this line*/


#else
#define RADIOBACK "BACK.MPG"
#define BLACK_PIC "BLACK.MPG"   

#define BUTTON_RETURN "b_return.ga"  /*yxl 2004-12-20 add this line*/
#define BACKGROUND_PIC "back.gam"       
#define FAV_PIC "Favo.gam"              
#define SKIP_PIC "skip.gam"
#define  LOCK_PIC "lock.gam"
#define  UNREAD_MAIL_PIC "mailnew.bmp"
#define  READ_MAIL_PIC "mailold.bmp"
#define  CLOCK_PIC    "clock.gam"
#define  MAIL_UNREAD_PIC    "mail.gam"
#define  MAIL_READ_PIC  "rmail.gam"
#define DLG1_PIC   "clock.gam"  
#define DLG2_PIC   "red.bmp"    
#define ADD1_PIC     "add1.gam"
#define ADD_PIC     "add.gam"
#define SUB1_PIC     "sub1.gam"
#define SUB_PIC     "sub.gam"
#define RADIO_PIC   "Radio.gam"
#define BAR_PIC     "bar.gam"
#define RED_BUTTON   "Rbutt.gam"
#define GREEN_BUTTON "Gbutt.gam"
#define YELLOW_BUTTON "Ybutt.gam"
#define BLUE_BUTTON  "Bbutt.gam"
#define  LOGMPG     "LOGO.MPG"
#define  RADIOMPG   "RADIO.MPG"  
#define MUTE_PIC    "mute.gam"
#define UNMUTE_PIC  "Unmute.gam"
#define BLUEFILL_PIC "Bfill.gam"
#define YELLOWFILL_PIC "Yfill.gam"
#define DIALOG1_PIC     "Dlg1.gam"
#define DIALOG2_PIC     "DlgTran.gam"/*"Dlg2.gam"*/

#define CH_TITLE_PIC       "CHTitle.gam"
#define EN_TITLE_PIC       "ENTitle.gam"
#define NVOD_CHI_TITLE_PIC "CHnvod.gam"
#define NVOD_ENG_TITLE_PIC "ENnvod.gam"
#define EN_EPGTITLE		"ENepg.gam"
#define CH_EPGTITLE		"CHepg.gam"
#define PROG_PIC	"prog.gam"
#define SPROG_PIC	"Sprog.gam"
/*ljg 040903 add*/
#define LEFTRIGHT_PIC "b_lr.gam"
#define UPARRF_PIC	"Uarrowf.gam"
#define UPARR_PIC	"Uarrow.gam"
#define DOWNARRF_PIC	"Darrowf.gam"
#define DOWNARR_PIC	"Darrow.gam"
#define BUTTON_A		"butt_A.gam"
#define BUTTON_B		"butt_B.gam"
#define BUTTON_C		"butt_C.gam"
#define BUTTON_PAGE	"b_page.gam"
#define BUTTON_OK		"b_ok.gam"
#define BUTTON_EXIT		"b_exit.gam"
#define LEFT_PROG_B 	"Left_B.gam"
#define LEFT_PROG_Y 	"Left_Y.gam"
#define RIGHT_PROG_B 	"Right_B.gam"
#define RIGHT_PROG_Y 	"Right_Y.gam"
#define RIGHT_PROG_W 	"Right_W.gam"
#define BUTTON_DOWN	"b_down.gam"
#define BUTTON_UP		"b_up.gam"
#define BUTTON_LEFT		"b_left.gam"
#define BUTTON_RIGHT	"b_right.gam"
#define BUTTON_MENU	"b_menu.gam"
#define BUTTON_INFO "b_info.gam"
#define CH_GAMETITLE	"CHgame.gam"
#define EN_GAMETITLE	"ENgame.gam"
#define YELLOW_PROG_2   "proy_2.gam"
#define YELLOW_PROG_5   "proy_5.gam"
#define YELLOW_PROG_10   "proy_10.gam"
#define YELLOW_PROG_20   "proy_20.gam"
#define YELLOW_PROG_50   "proy_50.gam"
#define BLUE_PROG_2   	"prob_2.gam"
#define BLUE_PROG_5   	"prob_5.gam"
#define BLUE_PROG_10  	 "prob_10.gam"
#define BLUE_PROG_20   	"prob_20.gam"
#define BLUE_PROG_50   	"prob_50.gam"
/*END ljg 040903 add*/

#define BLACK_MPG		"BLACK.MPG"

#define SEARCH_MPG		"Search.mpg"
#define MULTI_AUDIO                "MultAud.gam"
#define MULTI_CH                   "MultC.gam"
#define MULTI_EN                   "MultE.gam"
#define AC3_SIGN                    "ac3.gam"   /* 20061203 wz add*/


#define SYSTEM_FONT_NORMA_COLORL         0x910e
#define SYSTEM_FONT_FOCUS_COLOR      /*  0XDB5C*/0xFFFF
#define SYSTEM_TIME_TEXT_COLOR           	0x9150
#define SYSTEM_SELECT_ITEM_FILL_COLOR  0XFA04
#define SYSTEM_SELECT_ITEM_BORDER_COLOR  0XE35C
#define SYSTEM_NORNAL_ITEM_FILL_COLOR  0XE35C
#define SYSTEM_NORMAL_ITEM_BORDER_COLOR 0XAA16
#define SYSTEM_INNER_FRAM_BORDER_COLOR	 0XAA16
#define SYSTEM_OUTER_FRAM_BORDER_COLOR   0XEB5C
#define SYSTEM_WAITFONT_BACKCOLOR      0XBA56
#define SYSTEM_TIME_BACK_COLOR            0XC29A/*ϵͳ����ʱ�����屳����ɫ*/
#define SYSTEM_INNER_FRAM_COLOR          0XE35C  /*ϵͳ�����ڿ���ɫ*/
#define SYSTEM_OUTER_FRAM_COLOR         0XC298/*   ϵͳ���������ɫ*/
#define SYSTEM_lIST_FILL_COLOR          0XF3DE
#define SYSTEM_lIST_BORDER_COLOR         0XAA16
#define SYSTEM_BUTTON_COLOR		0XF3DE
#define SYSTEM_BUTTONBORDER_COLOR	0XBA98
/*yxl 2004-12-18 modify this section*/
#if 0
#define SEARCH_INFO_COLOR          0x8CDE    /*ϵͳ�����ڿ���ɫ*/
#else
#define SEARCH_INFO_COLOR          0xeb9E/*0x3fff */   /*ϵͳ�����ڿ���ɫ*/
#endif
/*end yxl 2004-12-18 modify this section*/

#define SYSTEM_HELP_FILL_COLOR           0xB214/*  ϵͳ���ð�����Ϣ���ɫ*/
#define SYSTEM_HELP_BORDER_CLOR       0XE35C  /*ϵͳ���ð�����Ϣ�߿�ɫ*/      

#define SCROLL_FILL_COLOR                0xB214   /*���ͱ���ɫ*/
#define SCROLL_SLIDER_COLOR              0xEB9E    /*������ɫ*/


#define whitecolor                       0xFFFF
#define RedColor                         0xD086
#define BlueColor                        0x9950
#define GreenColor                       0x8300
#define orange_color				0xfa04
#define PROG_YELLOW_COLOR			     0xF9C4
#define PROG_BLUE_COLOR			         0xBA58

#if 0 /*yxl 2004-12-22 modify this line*/
#define SEARCH_TEXT_COLOR				0xf106
#else
#define SEARCH_TEXT_COLOR				0XB25A
#endif/*end yxl 2004-12-22 modify this line*/
#endif


/*FlashFileStr  *GlobalFlashFileInfo;*/
#define M_CHECK(X,XLOW,XHIGH) {if((X)<(XLOW)) (X)=(XHIGH);else if ((X)>(XHIGH)) (X)=(XLOW);}/*ljg 20040712 add*/
struct Focus{U16 XPs;U16 YPs;U16 Wid; U16 Het;};


#ifdef USE_GFX

typedef enum{
	REC_STYLE_N		= 10,
  	REC_STYLE_L		= 30,
  	REC_STYLE_R		= 20,
  	REC_STYLE_U		= 100,
  	REC_STYLE_B		= 120,
  	REC_STYLE_A		= 40,
  	REC_STYLE_LU		= 50,
  	REC_STYLE_LD		= 60,
	REC_STYLE_RU		= 70,
	REC_STYLE_RD		= 80
}RECSTYLE;


extern	void CH_DrawPixel(STGFX_Handle_t GFXHandle,STLAYER_ViewPortHandle_t CHVPHandle,STGFX_GC_t* pGC,
				  			int PosX,int PosY,unsigned int Color);

extern	boolean CH6_QFilledCircle( STGFX_Handle_t Handle, STLAYER_ViewPortHandle_t VPHandle,
							U16 uCenterX, U16 uCenterY, U8 uRadius, U32 uColor );


extern	boolean CH_DrawLine( STGFX_Handle_t GHandle, STLAYER_ViewPortHandle_t VPHandle,STGFX_GC_t* pGC,
						   U16 uX1, U16 uY1, U16 uX2, U16 uY2, unsigned int uColor	) ; 


extern  void CH_Stk_DrawPoint(STGFX_Handle_t GHandle, STLAYER_ViewPortHandle_t VPHandle,int PosX,int PosY,unsigned int Color);

#endif

extern int GetKeyFromKeyQueue(S8 WaitingSeconds);


/*�������ܣ�
�ú������ڻ�һ����(x,y)Ϊ��㣬��width, heightΪ��ߵľ��Ρ�
�ӿ�˵����
int  Lcolor   ����ɫ,U16 LWidth    ���߿�
Int  color      ���ɫΪ-1����䣬
STGFX_Handle_t GHandle:the handle of the opened STGFX
STLAYER_ViewPortHandle_t VPHandle:the handle of the viewport which will be operated

����ֵ��
true   �ɹ���
false   ʧ ��
*/
extern boolean  CH6m_DrawRectangle(STGFX_Handle_t GHandle,STLAYER_ViewPortHandle_t VPHandle,STGFX_GC_t* pGC, U32 x, U32 y, U32 width, U32 height,unsigned int color,unsigned int Lcolor,U16 LWidth);

/*
 �������ܣ�
    �ú������ڻ�һ����(XStart, YStart)Ϊ��㣬��width, heightΪ��ߣ���RadiuΪԲ�ǰ뾶��Բ�Ǿ��Ρ�
�ӿ�˵����
int   Lcolor     ����ɫ,U16 LWidth ���߿�
 int  pColor    ���ɫ
STGFX_Handle_t GHandle:the handle of the opened STGFX
STLAYER_ViewPortHandle_t VPHandle:the handle of the viewport which will be operated

����ֵ��
true   �ɹ���
false   ʧ�ܡ�
*/
extern boolean  CH6m_DrawRoundRectangle(STGFX_Handle_t GHandle,STLAYER_ViewPortHandle_t VPHandle,STGFX_GC_t* pGC, U32 XStart,U32 YStart,U32 Width,U32 Height,U32 Radiu,unsigned int pColor,unsigned int LColor,U16 LWidth);


/*��*/
/* �������ܣ�
    ��ʾһ���ԣ�X1��Y1��Ϊ��㣬�ԣ�X2��Y2��Ϊ�յ���߶Ρ�
    �ӿ�˵����
    int Selectcolor ��ʾ�ߵ���ɫ��
STGFX_Handle_t GHandle:the handle of the opened STGFX
STLAYER_ViewPortHandle_t VPHandle:the handle of the viewport which will be operated
    ����ֵ��
true   �ɹ���
false   ʧ�ܡ�
*/
#ifdef USE_GFX
extern boolean  CH6m_DrawLine(STGFX_Handle_t GHandle,STLAYER_ViewPortHandle_t VPHandle,STGFX_GC_t* pGC,U16 X1,U16 Y1,U16 X2,U16 Y2,U32 Selectcolor);
#else
extern boolean  CH6m_DrawLine(STGFX_Handle_t GHandle,STLAYER_ViewPortHandle_t VPHandle,
					   STGFX_GC_t* pGC,S32 X1,S32 Y1,S32 X2,S32 Y2,unsigned int Selectcolor);
#endif

/*��Բ��Բ����ʾ*/
/* �������ܣ�
    ��ʾһ���ԣ�XStart��YStart��ΪԲ�ĵ�Բ����Բ��
    �ӿ�˵����
    U32 HRadiu,U32 VRadiu    ��������ʾ�Ĵ�ֱ�뾶��ˮƽ�뾶��
Int  pColor  ���ɫ
Int Lcolor  ,U16 Lwidth  ��������ʾ�Ĺ��ߵ���ɫ�Ϳ�
STGFX_Handle_t GHandle:the handle of the opened STGFX
STLAYER_ViewPortHandle_t VPHandle:the handle of the viewport which will be operated
  ����ֵ��
 true   �ɹ���
false   ʧ�ܡ�
*/

extern boolean CH6m_DrawEllipse(STGFX_Handle_t GHandle,STLAYER_ViewPortHandle_t VPHandle,STGFX_GC_t* pGC,U32 XStart,U32 YStart,U32 HRadiu,U32 VRadiu,unsigned int  pColor/*���ɫ*/,unsigned int  LColor/*������ɫ*/,U16 LWidth);
/*                       λͼ�Ŵ���С                                       */
//extern void bmscaler(int dest_width,int dest_height,int source_width, int source_height,  U8 *DesBitmap, U8 *bitmap);
/*�������ܣ�
��ʾһ���Զ����С��ͼƬ��
�ӿ�˵����
char * WhichBall    ����ʾ��ͼƬ��
int x,   int y     ΪͼƬ��ʾ����ʼλ��
int width,   int height    ͼƬ��ʾ�Ŀ��
����ֵ��
�ޡ�
*/

extern void CH6m_DisplayBall(char * WhichBall,int x,int y,int width,int height);


/*�б������ַ�������ʾ*/
/*�������ܣ�
�ú���������ʾ�б������ı�
�ӿ�˵����
int  x_top,   int  y_top      �ַ���ʾ����ʼλ��
int Width,  int Heigth,         �����Ŀ��
char  *TxtStr                Ҫ��ʾ���ַ���
int  BackColor   ��ʾ�ı�����ɫ
int  txtColor      ��ʾ��������ɫ
STGFX_Handle_t GHandle:the handle of the opened STGFX
STLAYER_ViewPortHandle_t VPHandle:the handle of the viewport which will be operated
  ����ֵ��
�ޡ�
*/
extern void CH6_DisRectTxtInfo(STGFX_Handle_t GHandle,STLAYER_ViewPortHandle_t VPHandle,int x_top,int y_top,int Width,int Heigth,char *TxtStr,unsigned int  BackColor,unsigned int  txtColor,int LineWidth,U8 Align);


/*�ޱ������ַ�������ʾ*/
/*�������ܣ�
������ʾһ���ޱ������ַ�������Ҫ���������б�������ʾ��
�ӿ�˵����
int  x_top,   int  y_top     ��ʾ���ַ�����ʼλ��
int Width, int Heigth          ��ʾ������
char  *TxtStr                 ��ʾ������
int  txtColor    ��ʾ���ֵ���ɫ
STGFX_Handle_t GHandle:the handle of the opened STGFX
STLAYER_ViewPortHandle_t VPHandle:the handle of the viewport which will be operated
  ����ֵ��
�ޡ�
*/
extern void CH6_DisRectTxtInfoNobck(STGFX_Handle_t GHandle,STLAYER_ViewPortHandle_t VPHandle,int x_top,int y_top,int Width,int Heigth,char *TxtStr,unsigned int  txtColor,U8 Align);


/*��ʾ��ǰ���Ͻǲ˵�����*/
/*�������ܣ�
��ʾ��ǰ���ϽǱ��⡣
�ӿ�˵����
char  CH_Title    ���ı���
char  *EN_Title    Ӣ�ı���
STGFX_Handle_t GHandle:the handle of the opened STGFX
STLAYER_ViewPortHandle_t VPHandle:the handle of the viewport which will be operated
����ֵ��
�ޡ�
*/
extern void DisplayUpTitle(STGFX_Handle_t GHandle,STLAYER_ViewPortHandle_t VPHandle,char *CH_Title,char *EN_Title);

/****�õ�λͼ�����е�ĳһ������******/
U8* GetBmpData(U32 start_x,U32 start_y,U32 width,U32 height,U32 source_width,U32 source_height,U8 *SourceData);


/*��ʾ����ĳһ����*/
/*�������ܣ�
��ʾ����ͼƬ��һ���֡�
�ӿ�˵����
U32 x1,  U32 y1     ����ͼƬ����ʼλ��
U32 width,  U32 height     ��ʾ���ֵĴ�С
U32 x2, U32  y2     ��ʾͼƬ����ʼλ��
STGFX_Handle_t GHandle:the handle of the opened STGFX
STLAYER_ViewPortHandle_t VPHandle:the handle of the viewport which will be operated
����ֵ��
�ޡ�
*/
extern void display_home_right_bottom( STGFX_Handle_t GHandle,STLAYER_ViewPortHandle_t VPHandle,char *pMessage);


/*��ʾ��ǰ������Ϣ*/
/*�������ܣ�
��ʾ��ǰ�ṩ�İ�����Ϣ��
�ӿ�˵����
char    *HelpInfo    Ҫ��ʾ�İ�����Ϣ���ݡ�
STGFX_Handle_t GHandle:the handle of the opened STGFX
STLAYER_ViewPortHandle_t VPHandle:the handle of the viewport which will be operated
����ֵ��
�ޡ�
*/
extern void DisplayRightHelp(STGFX_Handle_t GHandle,STLAYER_ViewPortHandle_t VPHandle,char *HelpInfo);




/*�������ܣ�
��ʾһ���Ի���
�ӿ�˵����
  char *DialogContent   ����ʾ���ַ���
  char Buttons          ��ʾ���ͱ�־
  int  WaitSeconds      ��ʾ��ʱ��
  STGFX_Handle_t GHandle:the handle of the opened STGFX
  STLAYER_ViewPortHandle_t VPHandle:the handle of the viewport which will be operated
  yxl 2004-12-26 add  paramter U8 DefReturnValue:
  U8 DefReturnValue:0,standfor return OK button when no key pressed in 2 buttons mode,
					1,standfor return exit button when no key pressed in 2 buttons mode,
					other value,standfor return OK button when no key pressed in 2 buttons mode
*/
#if 0 /*yxl 2004-12-26 modify below section*/ 
extern boolean	CH6_PopupDialog (STGFX_Handle_t GHandle,STLAYER_ViewPortHandle_t VPHandle,char *DialogContent,char Buttons,int WaitSeconds);
#else
extern boolean	CH6_PopupDialog (STGFX_Handle_t GHandle,STLAYER_ViewPortHandle_t VPHandle,char *DialogContent,char Buttons,int WaitSeconds,U8 DefReturnValue);

#endif/*end yxl 2004-12-26 modify below section*/ 


/*below section is added by ljg*/

 /*��������:��ȡһ�����ֵ� ���ȣ���λ����
 *����:
 		text:ָ�����ֵ�ָ��
 *����ֵ:
 		-1:����
 		Width:���ֵĳ���
*/
extern int CH6_GetTextWidth(char* text);

/*����˵��:�������ֵ�λ��
*/
extern int CH6_ReturnNumofData(int InputData);

/*��������:��ʾ������������ߣ�������������ɣ�
				��һ��ˮƽֱ�ߣ��ڶ�����ֱֱ�ߣ�������ˮƽֱ��
 *����:
 		StartPosX	:�������λ�õĺ�����
 		StartPosY		:�������λ�õ�������
 		EndPosX		:�����յ�λ�õĺ�����
 		EndPosY		:�����յ�λ�õ�������
 		Ratio		:��һ�κ͵����εı���
 		Thickness	:�ߵĴ�ϸ
 		LineColor		:�ߵ���ɫ
		STGFX_Handle_t GHandle:the handle of the opened STGFX
        STLAYER_ViewPortHandle_t VPHandle:the handle of the viewport which will be operated
 *����ֵ:��
*/
extern void CH6_DrawLinkLine(STGFX_Handle_t GHandle,STLAYER_ViewPortHandle_t VPHandle,int StartPosX,int StartPosY,int EndPosX,int EndPosY,U8 Ratio,int LineThickness,unsigned int  LineColor);


/*��������:��һ�����ɫ��Բ�Ǿ��Σ���������ʾһ������
 *����:
 		iStartX		:Բ�Ǿ�����������
 		iStartY		:Բ�Ǿ������������
 		iWidth		:Բ�Ǿ��ο��
 		iHeight		:Բ�Ǿ��θ߶�
 		Color		:�����ɫ
 		pItemMessage:Ҫ��ʾ������
		STGFX_Handle_t GHandle:the handle of the opened STGFX
		STLAYER_ViewPortHandle_t VPHandle:the handle of the viewport which will be operated
 *����ֵ:��
 */
/*extern boolean CH6_DrawRoundRecText(STGFX_Handle_t Ghandle,STLAYER_ViewPortHandle_t VPHandle,U32 StartX ,U32 StartY , U32 Width , U32 Height ,int  BackColor,int TextColor, int Align,char* pItemMessage);*/

/*��������:��һ�����򣬲������ϻ�ˮƽ�����˺ͻ����飬
				���߶����������ɫ�ľ���			     
 *����:
 		StartX	:��������������
 		StartY	:���������������
 		Height	:������߶�
 		sWidth	:��������
 		sHeight	:������߶�
 		lHeight	:�����˴�ϸ
 		ItemMax	:��󻬶�����
 		Step		:����ֵ
 		Index	:��ǰ����������λ��
 		Color	:��������ɫ
 		sColor	:��������ɫ�ͻ������ڿ���ɫ
		STGFX_Handle_t GHandle:the handle of the opened STGFX
		STLAYER_ViewPortHandle_t VPHandle:the handle of the viewport which will be operated
 *����ֵ:
 		     tempWidth:��������
*/
/*extern int CH6_DrawHorizontalSlider(STGFX_Handle_t GHandle,STLAYER_ViewPortHandle_t VPHandle,U32 StartX,U32 StartY,U32 Height,U32 sWidth, U32 sHeight,U32 sLineWidth,U32 lHeight,U32 ItemMax,U32 Step,U32 Index,int Color,int sColor);*/


extern void CH6_MulLine(STGFX_Handle_t GHandle,STLAYER_ViewPortHandle_t VPHandle,int x_top,int y_top,int Width,int Heigth,char *TxtStr,unsigned int  txtColor,U8 H_Align,U8 V_Align);
/**/
extern void CH6_DrawLog(void);
extern void CH6_DrawRadio(void);

/*ljg 040831 add*/
extern void CH6_DrawAdjustBar(STGFX_Handle_t GHandle,STLAYER_ViewPortHandle_t VPHandle,int StartX,int StartY,int PreValue,int CurValue,BOOL FirstDraw,U8 FillColorType);
extern boolean DrawTriangle(STGFX_Handle_t GHandle,STLAYER_ViewPortHandle_t VPHandle,STGFX_GC_t* pGC,STGFX_Point_t Point1,STGFX_Point_t Point2,STGFX_Point_t Point3,unsigned int Color);
extern void CH6_DisplaySomeBP(STGFX_Handle_t GHandle,STLAYER_ViewPortHandle_t VPHandle,char*SPicName,U32 x1,U32 y1,U32 SWidth,U32 SHeight,U32 width,U32 height,U32 x2,U32 y2);
extern S8 PasswordVerify(char* pPassword);

extern void CH6_DisplayKeyButton(int StartX,int StartY,char* KeyButtonName);

extern U8	CH6_FindPageCutSize( U8* uBuff, U16 uPageLength, U16* pCutPoint, U8 uMaxPage );

extern void CH_UpdateAllOSDView(void);
extern void CH6_CloseMepg(void);
#endif

