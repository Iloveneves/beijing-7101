/*
  * ===================================================================================
  * CopyRight By CHANGHONG NET L.T.D.
  * �ļ�: 	eis_api_osd.c
  * ����: 	ʵ�ֻ�ͼ��صĽӿ�
  * ����:	 ��ս�ң�������
  * ʱ��:	2008-10-23
  * ===================================================================================
  */

#include "eis_api_define.h"
#include "eis_api_globe.h"
#include "eis_api_debug.h"
#include "eis_api_osd.h"
#include "..\dbase\vdbase.h"
#include "eis_include\ipanel_graphics.h"
#include "channelbase.h"
#include "..\main\initterm.h"

#ifdef __EIS_API_DEBUG_ENABLE__
//#define __EIS_API_OSD_DEBUG__
#endif

#ifdef SUMA_SECURITY
extern semaphore_t *pst_OsdSema ;/* ����IPNAEL����ʾ����*/
#endif
static unsigned char 	*draw_buffer 	= NULL;
#ifdef EIS_OSD_YASUO
unsigned char 	*cvbs_draw_buffer 	= NULL;
#endif
unsigned char 	*Bak_OSD_buffer 	= NULL;

int act_src_w=720,act_src_h=576;

IPANEL_GRAPHICS_WIN_RECT   g_IpanelDiaplyRect;

 CH_RESOLUTION_MODE_e HighSolution_change=CH_1280X720_MODE;

void CH_RestoreOsd();
void CH_PutBakOsd( );


/*
����˵����
	��ȡ��ʾ���ڵ���Ϣ����iPanel MiddleWareʹ����ʾ���ڵĳߴ硢��ʾ��Ļ����
	���ĵ�ַ����ʾ��Ļ�������ĳߴ硣�ú�������iPanel Middleware ϵͳ���г�ʼ��
	ʱ�����һ�Σ����������й��̵��в��ٵ��á�
	ע�⣺�����Ļ���������ɫʧ�棬�������������ȷ�ϵײ����õ���ɫ��ʽ�Ƿ�
	�Ϳ����ɫ��ʽһ�£������ɫ��ʽ�����ڵ�����Ϣ���ҵ�����������к���������
	���������ȷ��һ�µײ�͸��ɫ�����á�
����˵����
	���������
		��
	���������
		width,height: iPanel MiddleWare������ڵĴ�С��
		buf_width��buf_height: ʵ����ʾ��Ļ�������Ĵ�С��ĿǰiPanel
		Middlewareֻ����������ں���ʾ��Ļ��������С��ȵ����������buf_width��
		�����width������buf_height�������height��
		pbuf:������ʾ�Ļ�������ַָ��,���*pbuf = IPANEL_NULL,iPanel
		Middleware�Լ����仺������Ȼ�����ipanel_porting_draw_image �������Ļ
		��ʾ�������С����*pbuf!=IPANEL_NULL, iPanel Middlewareֱ�ӽ���������
		��Ļ��ʾ�������У�ipanel_porting_draw_image�����ǿպ�����
	�� �أ�
		IPANEL_OK:�ɹ�;
		IPANEL_ERR:ʧ�ܡ�
*/
INT32_T ipanel_porting_graphics_get_info(INT32_T *width, INT32_T *height, VOID **pbuf, INT32_T *buf_width, INT32_T *buf_height)
{

	if(width == NULL
		|| height == NULL
		|| buf_width == NULL
		|| pbuf == NULL)
	{
		eis_report("\nipanel_porting_graphics_get_info para error!");
		return IPANEL_ERR;
	}
	*width			= EIS_REGION_WIDTH;
	*height			= EIS_REGION_HEIGHT;
	*buf_width		= EIS_REGION_WIDTH;
	*buf_height		= EIS_REGION_HEIGHT;

	*pbuf 			= /*NULL*/draw_buffer;

      g_IpanelDiaplyRect.x = 0;
      g_IpanelDiaplyRect.y = 0;
      g_IpanelDiaplyRect.w = EIS_REGION_WIDTH;
      g_IpanelDiaplyRect.h=   EIS_REGION_HEIGHT;
	eis_report ( "\n++>eis osd ipanel_porting_graphics_get_info=<%d,%d,%d,%d>", *width, *height, *buf_width, *buf_height );
	
	return IPANEL_OK;	
	
}

/*
����˵����
	��װ��ɫ�壬iPanel MiddleWare Ҫ���ɫ���е�0 ��ɫΪ͸��ɫ���������ʽ
	С�ڵ���8bppʱ����Ҫʵ�ָú�����������ʽʱ�ú���Ӧ�ÿա�
����˵����
	���������
		npals����ɫ���С��8 λ��ɫʱ256��
		pal[]������ɫ�����ݣ����е���ɫ�������32bit �����ͣ���ʾ��ɫ��ʽ�ǣ�
		0x00RRGGBB���ú�������IPanel Middleware ϵͳÿ�ο�ʼ����ʱ����һ�Σ���
		�������й��̵��в��ٵ��á�����8λ����ɫģʽ����ʹ�õ�ɫ�塣
	���������
		�ޡ�
	�� �أ�
		IPANEL_OK:�ɹ�;
		IPANEL_ERR:ʧ�ܡ�		
*/
INT32_T ipanel_porting_graphics_install_palette(UINT32_T *pal, INT32_T npals)
{
	return IPANEL_OK;
}

/*
����˵����
	��ʹ�õ�ɫ�幤��ģʽʱ�����õ�ɫ������ɫֵ͸���ȡ�ע���������õ�alpha��
	����Ϊ����alpha�Ŀ��Ʒ�ʽ��
����˵����
	���������
		alpha��0��100��0Ϊȫ͸��100Ϊ��ȫ��͸����
	���������
		�ޡ�
	�� �أ�
		IPANEL_OK:�ɹ�;
		IPANEL_ERR:ʧ�ܡ�
*/
INT32_T ipanel_porting_graphics_set_alpha(INT32_T alpha)
{
	U8 u8_ActAlpha;

      if(alpha > 100)
	{
	  	alpha= 100;
	}
	else if(alpha < 0)
	{
		alpha = 0;
	}
	u8_ActAlpha = alpha * 128 / 100;
       
	CH_SetViewPortAlpha_HighSolution (  u8_ActAlpha );/**/

	return IPANEL_OK;
} 
/*
  * Func: eis_clear_all
  * Desc: �������OSD��
  */
void eis_clear_all()
{
	CH6_AVControl ( VIDEO_BLACK, false, CONTROL_VIDEO );
}

/*
 * del the eis region and restore the osd region
 */
void eis_del_region( void )
{
#ifdef CH_MUTI_RESOLUTION_RATE
{
     CH_RESOLUTION_MODE_e enum_Mode;
     enum_Mode = CH_GetReSolutionRate() ;
    if(enum_Mode == CH_OLD_MOE)
    	{
	   CH6_ClearFullArea( GFXHandle, CH6VPOSD.ViewPortHandle );
    	}
	else
	{
           CH_ClearFUll_HighSolution();
	}
}
#else
	/* Clear full viewport */
	CH6_ClearFullArea( GFXHandle, CH6VPOSD.ViewPortHandle );
#endif
	/* �򿪱����� 
	CH6_MixLayer( true, true, true );*/

	CH6_AVControl( VIDEO_CLOSE, false, CONTROL_VIDEO );

	if ( NULL != draw_buffer )
	{
		memory_deallocate( CHSysPartition, draw_buffer );
		draw_buffer = NULL;
	}

	return;
}

/*
 * init the eis region
 */
void eis_init_region( void )
{

	/* ������ϵͳ���ڷ��� */
#if 1
	draw_buffer = (unsigned char *)memory_allocate ( CHSysPartition, EIS_REGION_WIDTH * EIS_REGION_HEIGHT * 4 +10);
	if(NULL!=draw_buffer)
	memset(draw_buffer,0,EIS_REGION_WIDTH * EIS_REGION_HEIGHT * 4);
#endif
	HighSolution_change= CH_GetReSolutionRate() ;

#ifdef EIS_OSD_YASUO
	cvbs_draw_buffer = (unsigned char *)memory_allocate ( CHSysPartition, EIS_REGION_WIDTH*576 * 4 +10);
	if(NULL!=cvbs_draw_buffer)
	memset(cvbs_draw_buffer,0, EIS_REGION_WIDTH*576 * 4 );
#endif	
	
	Bak_OSD_buffer=(unsigned char *)memory_allocate ( CHSysPartition, 640*526*4+10);
	if(NULL!=Bak_OSD_buffer)
		memset(Bak_OSD_buffer,0, 640*526 * 4 );
}

/*
����˵����
	��(x,y)���꿪ʼ������Ļ�ϻ�һ�����εĵ���ͼ����Ļ�ϡ�
����˵����
	���������
		x, y, w, h: ��ʾ��������λ�úʹ�С��
		bits: �������������ʼ��ַ��ע��:bits �������������ģ����ӣ�x��y��
		��ʼ��һ���н�����һ���У�w_src���������ο�����Ҫ�����ݲ�һ���������ģ�
		���ӣ�x��y����ʼ��ȡ����w �����أ��ٴ���һ�п�ʼ������ͼ��ʾ��
		w_src: iPanel Middleware �ڲ�������ÿ�еĿ�ȣ�������Ϊ��λ����
	�����������
	�� �أ�
		IPANEL_OK:�ɹ�;
		IPANEL_ERR:ʧ�ܡ�
����˵����
	�ú�����iPanel Middleware �������й��̵���Ƶ�����ã����Ը���OSD ��
	��ʾ����������Ҫ�����е�Ч�ʺܸߡ����⣬��ע�⣬x��y �����꣬��ʱ��δ֪
	����£��ᳬ������ʾ��Χ����ʵ�ִ�����������Խ����жϡ�
  */
INT32_T ipanel_porting_graphics_draw_image(INT32_T x, INT32_T y, INT32_T w, INT32_T h, BYTE_T *bits,INT32_T w_src)
{
#if 1
	int i,j,k;
	U32 uColor = 0xffff0000;
	U32	uRed, uGreen, uBlue;
	U32	*pBuff, *p32Src;
	U16	*pSrc;
	
	#ifdef __EIS_API_OSD_DEBUG__
	eis_report ( "\n++>draw_image start time=%d,x=%d,y=%d,w=%d,h=%d",ipanel_porting_time_ms(),x,y,w,h );
	#endif

	if(NULL==draw_buffer)
	{
		 return 0;
	}
	
#ifdef SUMA_SECURITY
		semaphore_wait(pst_OsdSema);
#endif
#ifndef USE_EIS_2D
#ifdef SUMA_SECURITY	
	if( CH_CheckSKBDisplay(x,y,w,h) )
	{
			do_report(0,"\n SumaSecure_SKB_updateSKB \n");
			SumaSecure_SKB_updateSKB();
			do_report(0,"\n SumaSecure_SKB_updateSKB over) \n");
	}
#endif
#endif
	if( x < 0 )
		x = 0;
	if( y < 0 )
		y = 0;
	if( ( x + w ) > EIS_REGION_WIDTH ) 
			w = EIS_REGION_WIDTH - x;
	if( ( y + h ) > EIS_REGION_HEIGHT )
			h = EIS_REGION_HEIGHT - y;


#ifdef CH_MUTI_RESOLUTION_RATE
{
	CH_RESOLUTION_MODE_e enum_Mode;
	int  i_DesX;
	int  i_DesY;
	enum_Mode = CH_GetReSolutionRate() ;

	 if(HighSolution_change != enum_Mode)
 	{
	
	       CH_SetReSolutionRate(HighSolution_change);
		CH_ChangeOSDViewPort_HighSolution(HighSolution_change,pstBoxInfo->VideoOutputAspectRatio,pstBoxInfo->HDVideoTimingMode);
		enum_Mode=HighSolution_change;
		if((enum_Mode == CH_720X576_MODE)&&((w<act_src_w)||(h<act_src_h)))
		{
			CH_PutBakOsd();
		}
		eis_report("\n CH_ChangeOSDViewPort_HighSolution");
 	}

#ifdef EIS_OSD_YASUO
	if((enum_Mode == CH_1280X720_MODE)&&(NULL!=cvbs_draw_buffer)	)
	{
		int y_src,y_dst;
		
		if((y / 5 ) > 0)
		{
			y_src=(y / 5) * 5 - 1;
			y_dst=(y / 5) * 4 - 1;
			memcpy(cvbs_draw_buffer+y_dst*EIS_REGION_WIDTH*4,draw_buffer+y_src*EIS_REGION_WIDTH*4,EIS_REGION_WIDTH* 4 * 4);
			y_src+=5;
			y_dst+=4;
		}
		else
		{
			y_src=0;
			y_dst=0;

			memcpy(cvbs_draw_buffer,draw_buffer,EIS_REGION_WIDTH * 4 * 3);
			y_src+=4;
			y_dst+=3;
		}
		
		while((y_src-y)<h)
		{
			if((y_src+5)<g_IpanelDiaplyRect.h)
			{
				memcpy(cvbs_draw_buffer+y_dst*EIS_REGION_WIDTH*4,draw_buffer+y_src*EIS_REGION_WIDTH*4,EIS_REGION_WIDTH * 4 * 4);
				y_src+=5;
				y_dst+=4;
			}
			else
			{
				while(y_src<g_IpanelDiaplyRect.h)
				{
					memcpy(cvbs_draw_buffer+y_dst*EIS_REGION_WIDTH*4,draw_buffer+y_src*EIS_REGION_WIDTH*4,EIS_REGION_WIDTH * 4 );
					y_src+=1;
				}
				break;
			}
			
		}
		
	}
#endif
	
    if(enum_Mode == CH_OLD_MOE)
    	{
		CH6_PutRectangle ( GFXHandle, CH6VPOSD.ViewPortHandle, &gGC, 
				 g_IpanelDiaplyRect.x , y+ g_IpanelDiaplyRect.y,  g_IpanelDiaplyRect.w, h, draw_buffer );
		CH_DisplayCHVP(CH6VPOSD,CH6VPOSD.ViewPortHandle);
    	}
    else if(enum_Mode == CH_1280X720_MODE)
    	{
    	   i_DesX = x+g_IpanelDiaplyRect.x;
	   i_DesY = y+ g_IpanelDiaplyRect.y;
          CH_PutRectangle_HighSolution(  x+g_IpanelDiaplyRect.x , y+ g_IpanelDiaplyRect.y, w, h, draw_buffer,i_DesX,i_DesY);
    	}
	else
	{
	   i_DesX = x+g_IpanelDiaplyRect.x+EIS_REGION_STARTX;
	   i_DesY = y+ g_IpanelDiaplyRect.y+EIS_REGION_STARTY;
          CH_PutRectangle_HighSolution(  x+g_IpanelDiaplyRect.x , y+ g_IpanelDiaplyRect.y, w, h, draw_buffer,i_DesX ,i_DesY);
	}
}
#else
	CH6_PutRectangle ( GFXHandle, CH6VPOSD.ViewPortHandle, &gGC, 
					EIS_REGION_STARTX, y+EIS_REGION_STARTY, w_src, h, draw_buffer );
	CH_DisplayCHVP(CH6VPOSD,CH6VPOSD.ViewPortHandle);
#endif	

#ifdef SUMA_SECURITY
		semaphore_signal(pst_OsdSema);
#endif

#ifdef __EIS_API_OSD_DEBUG__
eis_report ( "\n++>eis ipanel_porting_graphics_draw_image end time=%d",ipanel_porting_time_ms());
#endif
	return 0;
#endif
}


/*
����˵����
	��Graphics ����һ�������������������úͻ�ȡGraphics �豸�Ĳ��������ԡ�
����˵����
	���������
		op �� ��������
		typedef enum
		{
			IPANEL_GRAPHICS_AVAIL_WIN_NOTIFY =1,
		} IPANEL_GRAPHICS_IOCTL_e;
		arg �C �������������Ĳ�����������ö���ͻ�32 λ����ֵʱ��arg ��ǿ��ת��
		�ɶ�Ӧ�������͡�
	op, arg ȡֵ���±�
	op 									arg 											˵��
	
										ָ��IPANEL_GRAPHICS_WIN_RECT				֪ͨ�ⲿ
										�ṹ��ָ��;							ģ�飬���
										typedef struct {								��ʵ����
												int x,								���Ĵ���
	IPANEL_GRAPHICS_AVAIL_WIN_NOTIFY			int y,								�Ĵ�С��
												int w,								λ�á�
												int h,
										}IPANEL_GRAPHICS_WIN_RECT;
	
	�����������
	�� �أ�
	IPANEL_OK:�ɹ�;
	IPANEL_ERR:ʧ�ܡ�
  */
INT32_T ipanel_porting_graphics_ioctl(IPANEL_GRAPHICS_IOCTL_e op, VOID *arg)
{
	IPANEL_GRAPHICS_WIN_RECT *p_Struct;
	static short win_h_temp=0;
	eis_report ( "\n++>eis ipanel_porting_graphics_ioctl op=%d",op );
	
	switch ( op )
	{
		case IPANEL_GRAPHICS_AVAIL_WIN_NOTIFY:
		p_Struct = ( IPANEL_GRAPHICS_WIN_RECT* )arg;
		eis_report("\n ipanel_porting_graphics_ioctl p_Struct->h =%d",p_Struct->h );
	
      		if(p_Struct->h  <= 576 )/*����OSD*/
	      	{
			g_IpanelDiaplyRect.x = p_Struct->x;
			g_IpanelDiaplyRect.y = p_Struct->y;
			g_IpanelDiaplyRect.w = p_Struct->w;
			g_IpanelDiaplyRect.h = p_Struct->h;

			act_src_w=p_Struct->w-1;
			act_src_h=p_Struct->h-1;
			if(win_h_temp==0)
			{
				win_h_temp=act_src_h;
			}

			if((win_h_temp!=act_src_h)&&(HighSolution_change==CH_720X576_MODE))
			{

				CH_ChangeOSDViewPort_HighSolution(HighSolution_change,pstBoxInfo->VideoOutputAspectRatio,pstBoxInfo->HDVideoTimingMode);
			}

			HighSolution_change=CH_720X576_MODE;
			win_h_temp=act_src_h;
			CH_RestoreOsd();

	      	}
  		else
		{                           /*����OSD����*/
			g_IpanelDiaplyRect.x = p_Struct->x;
			g_IpanelDiaplyRect.y = p_Struct->y;
			g_IpanelDiaplyRect.w = p_Struct->w;
			g_IpanelDiaplyRect.h = p_Struct->h;
			// CH_DeleteOSDViewPort_HighSolution();
#if 0
			CH_SetReSolutionRate(CH_1280X720_MODE);
			CH_ChangeOSDViewPort_HighSolution(CH_1280X720_MODE,pstBoxInfo->VideoOutputAspectRatio,pstBoxInfo->HDVideoTimingMode);
#endif
			HighSolution_change=CH_1280X720_MODE;
       	} 
		return IPANEL_OK;
		break;
	default:
		return IPANEL_ERR;
		break;
	}
	return IPANEL_ERR;
}

/*
����˵����
	��ȡ���ĵ�ǰλ�ã���Ļ���ϽǶ���Ϊԭ�㡣
����˵����
	���������
		�ޡ�
	���������
		x��y ����ֵ��
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_porting_cursor_get_position(INT32_T *x, INT32_T *y)
{
	#ifdef __EIS_API_OSD_DEBUG__
	eis_report ( "\n++>eis ipanel_porting_cursor_get_position δ���" );
	#endif

	return IPANEL_OK;
}

/*
����˵����
	��������λ�á�x, y �������Ļ���ϽǶ����λ�á�
����˵����
	���������
		x��y������ֵ��
	����������ޣ�
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_porting_cursor_set_position(INT32_T x,INT32_T y)
{
#ifdef __EIS_API_OSD_DEBUG__
	eis_report ( "\n++>eis ipanel_porting_cursor_set_position δ���" );
#endif

	return IPANEL_OK;
}

/*
����˵����
	��ȡ��ǰ���/������״���롣��1 ~ 4��1����ͷ��״��2������״��3��ɳ©��״��
	4����I����״��
����˵����
	�����������
	�����������
	�� �أ�
		>=0:���/������״���롣
		IPANEL_ERR: ����ִ��ʧ�ܡ�
  */
INT32_T ipanel_porting_cursor_get_shape(VOID)
{
#ifdef __EIS_API_OSD_DEBUG__
	eis_report ( "\n++>eis ipanel_porting_cursor_get_shape δ���" );
#endif

	return 0;
}

/*
����˵����
	�������/�������͡���1 ~ 4��1����ͷ��״��2������״��3��ɳ©��״��4����I��
	��״��
����˵����
	���������
		shape�����/�������͡���1 ~ 4����Ч,������Ч��
	����������ޣ�
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_porting_cursor_set_shape(INT32_T shape)
{
#ifdef __EIS_API_OSD_DEBUG__
	eis_report ( "\n++>eis ipanel_porting_cursor_set_shape δ���" );
#endif

	return 0;
}

/*
����˵����
	�������/����Ƿ�ɼ���
����˵����
	���������
		flag��0�����ع�ꣻ1����ʾ��ꡣ
	����������ޣ�
	�� �أ�
		IPANEL_OK: ����ִ�гɹ�;
		IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_porting_cursor_show(INT32_T flag)
{
#ifdef __EIS_API_OSD_DEBUG__
	eis_report ( "\n++>eis ipanel_porting_cursor_show δ���" );
#endif

	return 0;
}


#define EIS_EEPROM_ADDR	256	
#define EIS_EEPROM_SIZE		0x1000	/* 4K */

int ipanel_porting_eeprom_burn(U32 addr, CONST BYTE_T *buf_addr, U32 len)
{
	if(buf_addr!=NULL)
	{
		eis_report ( "\n++>eis ipanel_porting_eeprom_burn addr=%x, buf_addr=%x,len=%d", addr,buf_addr,len);
		WriteNvmData(addr,len,buf_addr);
	}
	return IPANEL_OK;
}

int ipanel_porting_eeprom_read(U32 addr,BYTE_T *buf_addr, U32 len)
{
	if(buf_addr!=NULL)
	{
		ReadNvmData(addr,len,buf_addr);
		eis_report ( "\n++>eis ipanel_porting_eeprom_read addr=%x, buf_addr=%x,len=%d", addr,buf_addr,len);
	}
	return IPANEL_OK;
}

int ipanel_porting_eeprom_info(BYTE_T **addr, U32 *size)
{
	if((addr!=NULL )&&(size!=NULL))
	{
		*addr=(BYTE_T *)EIS_EEPROM_ADDR;
		*size=EIS_EEPROM_SIZE;
		eis_report ( "\n++>eis ipanel_porting_eeprom_info addr=%x, size=%d", *addr,*size);
	}
	return IPANEL_OK;
}
/*�л���ʽʱ�ָ����е�OSD�˵�*/
void CH_RstoreALLOSD(void)
{
     CH_RESOLUTION_MODE_e enum_Mode;
     int  i_DesX;
     int  i_DesY;
     int y = 0;
     int h = 0;
#ifdef USE_EIS_2D
	 ipanel_porting_graphics_update_osd( 0,0,g_IpanelDiaplyRect.w,g_IpanelDiaplyRect.h);
	 return;
#else	 
	enum_Mode = CH_GetReSolutionRate() ;
	if(enum_Mode == CH_OLD_MOE)
	{
		CH6_PutRectangle ( GFXHandle, CH6VPOSD.ViewPortHandle, &gGC, 
		 0, 0,  g_IpanelDiaplyRect.w, g_IpanelDiaplyRect.h, draw_buffer );
		CH_DisplayCHVP(CH6VPOSD,CH6VPOSD.ViewPortHandle);
	}
	else if(enum_Mode == CH_1280X720_MODE)
	{
		CH_PutRectangle_HighSolution( 0 , 0, g_IpanelDiaplyRect.w, g_IpanelDiaplyRect.h, draw_buffer,0,0);
	}
	else
	{
		CH_PutRectangle_HighSolution(  0 , 0, g_IpanelDiaplyRect.w, g_IpanelDiaplyRect.h, draw_buffer,0 ,0);
	}
#endif
}
  /*�ָ���ʾ����*/
  /*�л���ʽʱ�ָ����е�OSD�˵�*/
void CH_RestoreOsd()
  {
  	int i,j,k;
	if(NULL==Bak_OSD_buffer)
		return;
	
	for(k=0;k<2;k++)
	{
		for(i=0;i<263;i++)
		{
			for(j=0;j<640;j++)
			{
				memcpy(Bak_OSD_buffer+k*263*640*4+i*640*4+j*4,
					draw_buffer+k*360*EIS_REGION_WIDTH*4+/*(osd_640_526[i])*/(i*720/526)*EIS_REGION_WIDTH*4+j*2*4,4);
			}
		}
	}

  }
  
  void CH_PutBakOsd( )
{
	if(NULL==Bak_OSD_buffer)
		return;
	CH_PutBakOSD(  0 , 0, 640, 526, Bak_OSD_buffer,0,0);

}

void CH_SwitchIOOsd(int ri_index)
{
  	CH_ChangeOSDIO_HighSolution(CH_GetReSolutionRate(),pstBoxInfo->VideoOutputAspectRatio,pstBoxInfo->HDVideoTimingMode);
}

int CH_Switch_CVBS_Osd_y(int y)
{
	int yushu=y%5;
	int cvbs_y=0;
	switch(yushu)
	{
		case 0:
			cvbs_y=(y/5)*4;
			break;
		case 1:
		case 2:
			cvbs_y=(y/5)*4+1;
			break;
		case 3:
			cvbs_y=(y/5)*4+2;
			break;
		case 4:
			cvbs_y=(y/5)*4+3;
			break;
		default:
			break;
			
	}
	return cvbs_y;
}

#ifdef SUMA_SECURITY
void CH_Eis_DrawDot(int x,int y,int color)
{
	int*  Dot = NULL;
	Dot = (int*)((int)draw_buffer + y*g_IpanelDiaplyRect.w*4+x*4);
	*Dot = color;

}


void CH_Eis_drawLine (int x1, int y1,int x2,int y2, int color)
{
	int x,y;
	int temp;
	

	if(x1 == x2)/*����*/
	{
		if(y1 < y2)
		{
			temp = 1;
		}
		else
		{
			temp = -1;
		}

		x = x1;
		y = y1;
		while(y != y2)
		{
			CH_Eis_DrawDot(x,y,color);
			y += temp;
		}

	}
	else if( y1 == y2)
	{
		if(x1 < x2)
		{
			temp = 1;
		}
		else
		{
			temp = -1;
		}

		x = x1;
		y = y1;
		while(x != x2)
		{
			CH_Eis_DrawDot(x,y,color);
			x += temp;
		}	


	}

	
}
void CH_Eis_DrawRectangle (int x, int y, int w, int h, int color)
{
    	int i,i_y;
	for(i  = 0 ;i <= h ; i ++)
	{
		i_y = y+i;
		CH_Eis_drawLine( x,  i_y, (x+w), i_y, color);
	}

}
#ifndef USE_EIS_2D
U8* CH_Eis_GetRectangle(int PosX, int PosY, int Width, int Height)
{
  	U8*  pTemp = NULL;
	int     i_bufsize = Width*Height*4;
	int i;
	U8* pStart;

	U8* temp;
	
	pTemp=memory_allocate(CHSysPartition,i_bufsize);
	if(pTemp==NULL) 	
	{
		partition_status_t Teststaus;
		partition_status_flags_t flags=0;
		if( partition_status(CHSysPartition,&Teststaus,flags)==0)
		{
			do_report(0,"i_bufsize \n");
			do_report(0,"chsys_partition  free size=%x,total used size%x,lArge free=%x\n",
				Teststaus.partition_status_free,Teststaus.partition_status_size,
				Teststaus.partition_status_free_largest);
		}
		return NULL;	

	}
	else
	{
		temp = pTemp;
		memset(pTemp,0,i_bufsize);
	}
	pStart = draw_buffer + PosY*g_IpanelDiaplyRect.w*4+PosX*4;
	for(i = 0;i < Height;i++)
	{
		memcpy(pTemp,pStart, Width*4);
		pTemp += Width*4;
		pStart +=  g_IpanelDiaplyRect.w*4;	
	}
	
	pTemp = pTemp -Height*Width*4;
	if(pTemp != temp)
	{
		return NULL;
	}

	return  pTemp;
}
#endif

BOOL CH_Eis_PutRectangle( int PosX, int PosY,int Width,int Height,U8* pBMPData)
{
	U8*  pTemp = pBMPData;

	int i;
	U8* pStart;
	

	pStart = draw_buffer + PosY*g_IpanelDiaplyRect.w*4+PosX*4;
	for(i = 0;i < Height;i++)
	{
		memcpy(pStart,pTemp, Width*4);
		pTemp += Width*4;
		pStart +=  g_IpanelDiaplyRect.w*4;	
	}
	  	
	return FALSE;
}


BOOL CH_Eis_DisplayPicdata( int PosX, int PosY,int Width,int Height,U8* pBMPData)
{
	U8*  pTemp = pBMPData;

	int i,j;
	U8* pStart;
	

	pStart = draw_buffer + PosY*g_IpanelDiaplyRect.w*4+PosX*4;
	for(i = 0;i < Height;i++)
	{
		for(j = 0;j < Width;j++)
		{
			if(*(U32*)pTemp != 0X80000000)
			{
				memcpy(pStart,pTemp,4);
			}
			
			pTemp += 4;
			pStart +=  4;	
		}
		pStart += ( g_IpanelDiaplyRect.w -Width)*4;	
	}
	 ipanel_porting_graphics_draw_image( PosX, PosY,Width, Height, NULL, EIS_REGION_WIDTH);
	return FALSE;
}

#endif


/*--eof----------------------------------------------------------------------------------------------------*/

