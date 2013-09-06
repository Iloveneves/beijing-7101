/******************************************************************************/
/*    Copyright (c) 2009 iPanel Technologies, Ltd.                            */
/*    All rights reserved. You are not allowed to copy or distribute          */
/*    the code without permission.                                            */
/*    This is the demo implenment of the porting APIs needed by iPanel        */
/*    MiddleWare.                                                             */
/*    Maybe you should modify it accorrding to Platform.                      */
/*                                                                            */
/******************************************************************************/

#ifndef __IPANEL_GRAPHICS2_H__
#define __IPANEL_GRAPHICS2_H__

#include "ipanel_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ���� */
typedef struct
{
	INT32_T		x;
	INT32_T		y;
	INT32_T		width;
	INT32_T		height;
}IPANEL_GRAPHICS_RECT;


/* colorkeyģʽ */
typedef enum
{
	/* ����color_key */
	IPANEL_COLORKEY_DISABLE = 0,
	/* RGB��Ϊcolorkey */
	IPANEL_COLORKEY_RGB,
	/* ALPHA��Ϊcolorkey */
	IPANEL_COLORKEY_ALPHA,
	/* ARGB��������Ϊcolorkey */
	IPANEL_COLORKEY_ARGB,
}IPANEL_GRAPHICS_COLORKEY_MODE;

/* alphaģʽ */
typedef enum
{
	/* ����alpha */
	IPANEL_ALPHA_DISABLE = 0,
	/* ȫ��alpha */
	IPANEL_ALPHA_GLOBAL,
	/* SrcOver��ϲ��� */
	IPANEL_ALPHA_SRCOVER,
}IPANEL_GRAPHICS_ALPHA_MODE;

/* �����alpha_flag��colorkey_flag������, �ȼ���colorkey_flag, �ټ���alpha_flag */
typedef struct {
	/* alpha_flag
	1. alpha_flag==IPANEL_ALPHA_DISABLE ��ʾ����alpha
	2. alpha_flag==IPANEL_ALPHA_GLOBAL ��ʾ����alpha, ���㹫ʽΪ: 
	alpha = alpha*0xff/100
	Dst.Red		= ( alpha * Src.Red   + (0xff - Src.Alpha) * Dst.Red ) / 0xff
	Dst.Green	= ( alpha * Src.Green + (0xff - Src.Alpha) * Dst.Green ) / 0xff
	Dst.Blue	= ( alpha * Src.Blue  + (0xff - Src.Alpha) * Dst.Blue ) / 0xff
	Dst.Alpha	= ( alpha * Src.Alpha  + (0xff - Src.Alpha) * Dst.Alpha ) / 0xff
	3. alpha_flag==IPANEL_ALPHA_SRCOVER ��ʾ����alpha, ���㹫ʽΪ:		//by willa,09/10/15,Java����lyreҪ��ʵ��
	����srcAlpha��opaque���͵ģ���0��ʶ͸���ȣ�0xff��ʶ��͸��				
	Dst.Red		= ( Src.Alpha * Src.Red   + (0xff - Src.Alpha) * Dst.Red ) / 0xff
	Dst.Green	= ( Src.Alpha * Src.Green + (0xff - Src.Alpha) * Dst.Green ) / 0xff
	Dst.Blue	= ( Src.Alpha * Src.Blue  + (0xff - Src.Alpha) * Dst.Blue ) / 0xff
	Dst.Alpha	= ( Src.Alpha  + (0xff - Src.Alpha) * Dst.Alpha ) / 0xff
	*/
	IPANEL_GRAPHICS_ALPHA_MODE		alpha_flag;
	/* colorkeyģʽ
	Src, Dst����32λ��˵��,
	Src , Dst, �ǰ�ARGB32��˵��: 
		|   Alpha |  Red | Green | Blue |
	1. colorkey_flag ��IPANEL_COLORKEY_ARGB
		���Src==colorkey, ��Dst����;
		���Src!=colorkey, ��Dst=Src;
	2. colorkey_flag ��IPANEL_COLORKEY_ALPHA, , ���㹫ʽΪ:
		Dst.Red		= ( Src.Alpha * Src.Red   + (0xff - Src.Alpha) * Dst.Red ) / 0xff
		Dst.Green	= ( Src.Alpha * Src.Green + (0xff - Src.Alpha) * Dst.Green ) / 0xff
		Dst.Blue	= ( Src.Alpha * Src.Blue  + (0xff - Src.Alpha) * Dst.Blue ) / 0xff
		Dst.Alpha	=  Dst.Alpha
	*/
	IPANEL_GRAPHICS_COLORKEY_MODE	colorkey_flag;
	/* alpha: 0=��ȫ͸��,100=��ȫ��͸�� */
	UINT32_T	alpha;
	/* colorkey, �ǰ�ƽ̨֧�ֵĸ�ʽ��� */
	UINT32_T	colorkey;
}IPANEL_GRAPHICS_BLT_PARAM;


/* ��ʼ��ͼ�ο� */
INT32_T ipanel_porting_graphics_init();

/* �ر�ͼ�ο� */
INT32_T ipanel_porting_graphics_exit();

/* �����߼�ͼ��: surface�������߲����ٱ��ı�ֱ��destroy
*	width, height��surface�Ĵ�С. �������Ż������г�����Ч��surface �Ŀ�� ��ͼ��仯�����еĿ�ߵ����ֵ��������ͼ�񳬳�surface�ı߽�
*  phSurface��surface���
*/
INT32_T ipanel_porting_graphics_create_surface( INT32_T type, INT32_T width, INT32_T height, VOID** phSurface );

/* �����߼�ͼ�� */
INT32_T ipanel_porting_graphics_destroy_surface( VOID* hSurface );


/* ȡ��ͼ����󻺳�����Ϣ */
INT32_T ipanel_porting_graphics_get_surface_info( VOID* hSurface, VOID** pbuffer, INT32_T* width, INT32_T* height, INT32_T* bufWidth, INT32_T* bufHeight, INT32_T* format );


/* ͼ��Ԥ���� */
INT32_T ipanel_porting_graphics_prepare_surface( VOID* hSurface, IPANEL_GRAPHICS_RECT* rect, IPANEL_GRAPHICS_BLT_PARAM* param );

/* ͼ��֮�俽��(��stretch/alpha/colorkey����)
*  hSurface ��Ŀ��ͼ�㣬hSurface==NULL��ʾֱ�ӿ�����OSD��
*  rectĿ��ͼ�������
*  hSrcSurface ��Դͼ��
*  srcRect��srcSurface�ϵ�����
*/
INT32_T ipanel_porting_graphics_stretch_surface( VOID* hSurface, IPANEL_GRAPHICS_RECT* rect, VOID* hSrcSurface, IPANEL_GRAPHICS_RECT* srcRect, IPANEL_GRAPHICS_BLT_PARAM* param );

/* ͼ��֮�俽��(��alpha/colorkey����)
*  hSurface ��Ŀ��ͼ�㣬hSurface==NULL��ʾֱ�ӿ�����OSD��
*  rectĿ��ͼ�������
*  hSrcSurface ��Դͼ��
*  srcx,srcy��srcSurface�ϵ�λ��
*/
INT32_T ipanel_porting_graphics_blt_surface( VOID* hSurface, IPANEL_GRAPHICS_RECT* rect, VOID* hSrcSurface, INT32_T srcx, INT32_T srcy, IPANEL_GRAPHICS_BLT_PARAM* param );


/* ��ͼ����������: colorΪƽ̨֧�ֵĸ�ʽ��rect������ */
INT32_T ipanel_porting_graphics_fill_surface_rect( VOID* hSurface, IPANEL_GRAPHICS_RECT* rect, UINT32_T color );

/* ��ͼ���ϻ���: x1,y1����ʼ���꣬x2, y2���յ����� colorΪƽ̨֧�ֵĸ�ʽ thicknessΪ�ߵĿ��*/
INT32_T ipanel_porting_graphics_draw_line(void* hSurface, INT32_T x1, INT32_T y1, INT32_T x2,INT32_T y2, INT32_T color, INT32_T thickness);


/* ͼƬԤ���� */
INT32_T ipanel_porting_graphics_prepare_buffer( VOID* src, INT32_T srcLineWidth, IPANEL_GRAPHICS_RECT* rect, IPANEL_GRAPHICS_BLT_PARAM* param );
/* ��bufferͼ�񿽱���ͼ��(��alpha/colorkey����)
*  hSurface ��Ŀ��ͼ�㣬hSurface==NULL��ʾֱ�ӿ�����OSD��
*  rectĿ��ͼ�������
*  src ��ԴͼƬָ��
*  srcx, srcy ��ԴͼƬλ��
*  srcLineWidth ��Դͼ�����ؿ��
*/
INT32_T ipanel_porting_graphics_blt_buffer( VOID* hSurface, IPANEL_GRAPHICS_RECT* rect, VOID* src, INT32_T srcx, INT32_T srcy, INT32_T srcLineWidth, IPANEL_GRAPHICS_BLT_PARAM* param );

/* ��bufferͼ�񿽱���ͼ��(��stretch/alpha/colorkey����)
*  hSurface ��Ŀ��ͼ�㣬hSurface==NULL��ʾֱ�ӿ�����OSD��
*  rectĿ��ͼ�������
*  src ��ԴͼƬָ��
*  srcx, srcy ��ԴͼƬλ��
*  srcLineWidth ��Դͼ�����ؿ��
*/
INT32_T ipanel_porting_graphics_stretch_buffer( VOID* hSurface, IPANEL_GRAPHICS_RECT* rect, VOID* src, IPANEL_GRAPHICS_RECT* srcRect, INT32_T srcLineWidth, IPANEL_GRAPHICS_BLT_PARAM* param );


/* �ǰ�osd��ָ�������͵���Ļ����ʾ 
*/
INT32_T ipanel_porting_graphics_update_osd( INT32_T x, INT32_T y, INT32_T width, INT32_T height );


/*
IPANEL_GRAPHICS_AVAIL_WIN_NOTIFY    argΪָ��IPANEL_GRAPHICS_WIN_RECT�ṹ��ָ��;
IPANEL_GRAPHICS_SET_ALPHA_NOTIFY    argΪָ�� int ��ָ��,alphaֵ;��ʹ�õ�ɫ�幤��ģʽʱ�����õ�ɫ������ɫֵ
	͸���ȡ�ע���������õ�alpha������Ϊ����alpha�Ŀ��Ʒ�ʽ��alpha��0��100��0Ϊȫ͸��100Ϊ��ȫ��͸����
*/
typedef enum
{
	IPANEL_GRAPHICS_AVAIL_WIN_NOTIFY =1,
	IPANEL_GRAPHICS_GET_2D_STATUS,
	IPANEL_GRAPHICS_SET_ALPHA_NOTIFY
} IPANEL_GRAPHICS_IOCTL_e;

/*֪ͨ�ⲿģ�飬�����ʵ������Ĵ��ڵĴ�С��λ�á�*/
typedef struct TAG_IPANEL_GRAPHICS_WIN_RECT
{
	int x;
	int y;
	int w;
	int h;  //willa ��ipanel_graphics.h���Ѿ����壬�������
}IPANEL_GRAPHICS_WIN_RECT;

/*
����˵����
��Graphics����һ�������������������úͻ�ȡGraphics�豸�Ĳ��������ԡ�
����˵����
���������
op �� ��������
arg - �������������Ĳ�����������ö���ͻ�32λ����ֵʱ��arg��ǿ��ת���ɶ�Ӧ�������͡�
op, argȡֵ���±�
op    arg    ˵��
�����������
��    �أ�
IPANEL_OK:�ɹ�;
IPANEL_ERR:ʧ�ܡ�
*/
INT32_T ipanel_porting_graphics_ioctl_ex(IPANEL_GRAPHICS_IOCTL_e op, VOID *arg);
#ifdef __cplusplus
}
#endif

#endif//__IPANEL_GRAPHICS2_H__
