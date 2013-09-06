/*****************************************************************************

File name   : skb.c
 
Description : ch_security

COPYRIGHT (C) changhong.

author: WZ 20090601


*****************************************************************************/

#include "appltype.h"
#include "stddef.h"
#include "string.h"
#include "sectionbase.h"
#include "..\dbase\vdbase.h"
#include "..\usif\graphicmid.h"
#include "..\usif\putin_pin.h"
#include "..\util\ch_time.h"
#include "..\key\keymap.h"
#include <stddefs.h>
#include"..\eis\eis_include\ipanel_porting_event.h"
#include "stb_tsm.h"
#include "skb.h"
#include "CHMID_TCPIP_api.h"
#include "..\eis\eis_api_osd.h"
#include "..\CHMID_TCPIP\utilities\sockets.h"
#include "..\main\initterm.h"

U8* p_SkbPicData = NULL;
int    i_skbx = -1;;
int    i_skby = -1;;
int    i_skbpicwidth = -1;
int    i_skbpicheight = -1;
extern STLAYER_ViewPortHandle_t 	g_VPOSD_HighSolution[];
extern semaphore_t *pst_GifSema;
task_t* skb_SecurityTask = NULL;/*����̵�����*/
boolean cardstatus = false;

#define SKB_Printf(x) 
/*#define SKB_DEBUG sqzow20100713*/
/*
	1.	�ӿڶ���
		void SumaSecure_SKB_sleep (unsigned int ms);
	2.	����˵��
		ͨ���˽ӿ�ʵ���̵߳����ߡ�
	3.	����˵��
		���������
			ms: ���ߵ�ʱ�䣬��λΪ���롣
		���������
			�ޡ�
		�� �أ�
			�ޡ�
	4.	Ӧ�ó���
		����STB����ȡ������ֵʱ������ô˺������ͷŶ�CPU��ռ�á�

*/
void SumaSecure_SKB_sleep (unsigned int ms)
{
	task_delay(ST_GetClocksPerSecond()*ms/1000);
}
/*����˵��*/
/*1.	�ӿڶ���
	void SumaSecure_SKB_notifySTB (int flag);
2.	����˵��
	ͨ���˽ӿ�ʵ�ֶ�STB��֪ͨ��������SKBģʽ��
	��STB����������SKB�������ٷ��͸����������
	�˳�SKBģʽ����SKB�����������������
3.	����˵��
	���������
	flag: 1����ʾ����SKBģʽ��
		0����ʾ�˳�SKBģʽ��
���������
		�ޡ�
�� �أ�
		�ޡ�
4.	Ӧ�ó���
	�ڳɹ�����SKB�̺߳����SumaSecure_SKB_notifySTB��1��?	���˳�SKB�̺߳����SumaSecure_SKB_notifySTB��0����
*/
static boolean gb_skbactive = false;


void CH_SKB_TerminateTask(void)
{
	
	if( skb_SecurityTask != NULL)
	{
		do_report(0,"\n CH_SKB_TerminateTask \n");
		task_wait(&skb_SecurityTask, 1, TIMEOUT_INFINITY);/*�ȴ���������ɾ�� sqzow20100706*/
		if(task_delete(skb_SecurityTask) != 0)
		{
			do_report(0,"\nERR CH_SKB_TerminateTask FAILED \n");
		}
		else
		{
			do_report(0,"\nNOTICE CH_SKB_TerminateTask SUCCESSFUL \n");
			skb_SecurityTask = NULL;
		}
	}

}

/*֪ͨ���������ģʽ��Ҫ���еĲ���*/
int  CH_Security_EnterOption(pThreadFunc pTaskFun)
{
	gb_skbactive = true;/* �˳������ģʽ*/
	do_report(0,"\n CH_Security_EnterOption ");



	CH_SKB_TerminateTask();

	if(pTaskFun != NULL)
	{
		skb_SecurityTask = Task_Create (pTaskFun,NULL,(1024*64),6,"SKB_process",0);

		if(skb_SecurityTask == NULL)
		{
			do_report(0," create skb_process failed!\n");
			return -1;
		}
		else
		{
			do_report(0," create skb_process sucessful!\n");
			return 0;
		}
	}
	else
	{
		return -1;
	}
}



/* ���˳��������ʱ��λSKB��һЩ״̬���ͷ�һЩ��Դ*/

int  CH_Security_ExitOption(void)
{
	gb_skbactive = false;/* �˳������ģʽ*/
	do_report(0,"\n CH_Security_ExitOption ");

	if(p_SkbPicData != NULL)
	{
		memory_deallocate(CHSysPartition,p_SkbPicData);
		p_SkbPicData = NULL;
	}
	i_skbx = -1;
	i_skby = -1;
	i_skbpicheight= -1;
	i_skbpicwidth= -1;


	if(cardstatus == true)
	{
		semaphore_signal(pst_GifSema);
		cardstatus = false;
	}

	MakeVirtualkeypress(VIRKEY_CLOSESKB_KEY);
	return 0;

}

int  SumaSecure_SKB_notifySTB (int flag,pThreadFunc pTaskFun)
{
	int errno_suma;
	do_report(0,"SumaSecure_SKB_notifySTB %d 0x%x",flag,pTaskFun);
	
		if(flag == 0x01)
		{
			errno_suma=CH_Security_EnterOption(pTaskFun);
		}
		else if(flag == 0x0)
		{
			errno_suma=CH_Security_ExitOption();
			
		}	
	
	return errno_suma;
}

boolean CH_GetSkbStatus(void)
{
	return gb_skbactive;
}

/*******��ͼģ��Ľӿ�*******************************************/

void ch_color_conversion(U8* barbarism_color,int* color)
{
	int  r,g,b;
	
	if(color == NULL || barbarism_color == NULL)	
		return;
	
	r = barbarism_color[2];
	g =barbarism_color[1] ;
	b = barbarism_color[0];
	
	*color =0x8000|((r & 0xf8)<<7)|((g & 0xf8)<<2)|((b & 0xf8)>>3);
}
/*
	1.	�ӿڶ���
	int SumaSecure_SKB_drawDot (int x, int y, int color);
	2.	����˵��
	ͨ���˽ӿ���osd�ϻ��㣬��Ϊ1�����ء�
	3.	����˵��
	���������
	x, y��ʾ�����ʼ���ꣻ
	color: rgbaֵ����8λΪa; ��8λΪr;�ε�8λΪg����8λΪb��
	���������
	�ޡ�
	�� �أ�
	-1��ʾʧ�ܣ�0��ʾ�ɹ���
	4.	Ӧ�ó���
	������ķ��ţ���*�š�
*/
int SumaSecure_SKB_drawDot (int x, int y, int color)
{
	int dotcolor;
	//ch_color_conversion((U8*)&color,&dotcolor);
	//do_report(0,"SumaSecure_SKB_drawDot \n");	
	CH_Eis_DrawDot(x,y,color);
	//ipanel_porting_graphics_draw_image( x,  y, 1, 1, NULL, EIS_REGION_WIDTH);

	return 0;
}
/*
1.	�ӿڶ���
	int SumaSecure_SKB_fillBox (int x, int y, int w, int h, int color);
	2.	����˵��
	ͨ���˽ӿ���osd�ϻ�ʵ�ľ��Ρ�
	3.	����˵��
	���������
	x, y��ʾ�����������ʼ���ꣻ
	w, h��ʾ��������Ŀ�͸ߣ� 
	color: rgbaֵ����8λΪa; ��8λΪr;�ε�8λΪg����8λΪb��
	���������
	�ޡ�
	�� �أ�
	-1��ʾʧ�ܣ�0��ʾ�ɹ���
	4.	Ӧ�ó���
	���˳�SKBʱ���ã�ˢ��osd����ʾ��SKB���ݡ�
*/

int SumaSecure_SKB_fillBox (int x, int y, int w, int h, int color)
{
	int dotcolor;
	//ch_color_conversion((U8*)&color,&dotcolor);
	//do_report(0,"SumaSecure_SKB_fillBox \n");
		
	CH_Eis_DrawRectangle( x,  y, w, h, color);
	//ipanel_porting_graphics_draw_image( x,  y, w, h, NULL, EIS_REGION_WIDTH);

	return 0;

}
/*
	1.	�ӿڶ���
		int SumaSecure_SKB_drawLine (int x0, int y0, int x1, int y1, int color);
	2.	����˵��
		ͨ���˽ӿ���osd�ϻ�ֱ�ߣ�������ʾSKB�Ľ��㣬ͨ��������ʾΪ����ֱ��Χ�����ľ��ο�,ֱ�߿�Ϊһ���ء�
	3.	����˵��
		���������
		X0, y0��ʾֱ�ߵ���ʼ���꣬Ϊ��Ļ���ꡣ
		x1, y1 ��ʾֱ�ߵ���ֹ���꣬Ϊ��Ļ����
		color: rgbaֵ��
		���������
		�ޡ�
		�� �أ�
		-1��ʾʧ�ܣ�0��ʾ�ɹ���
	4.	Ӧ�ó���
		�ڻ�SKB����ʱ�����á�

*/
int SumaSecure_SKB_drawLine (int x1, int y1,int x2,int y2, int color)
{
	
	int dotcolor;

	//do_report(0,"SumaSecure_SKB_drawLine \n");
	if( x1 != x2 && y1!= y2)
	{
		return -1;
	}
	if(x1== x2 && y1==y2)
	{
		SumaSecure_SKB_drawDot(x1,y1,color);
		return  0;
	}
	CH_Eis_drawLine( x1,  y1,  x2, y2,  color);
#if 0
	if(x1 == x2)
	{
		if(y1<y2)
		{
			ipanel_porting_graphics_draw_image( x1,  y1, 1, (y2-y1), NULL, EIS_REGION_WIDTH);
		}
		else
		{
			ipanel_porting_graphics_draw_image( x2,  y2, 1, (y1-y2), NULL, EIS_REGION_WIDTH);
		}
		
	}
	else if( y1 == y2)
	{
		if(x1<x2)
		{
		
			ipanel_porting_graphics_draw_image( x1,  y1, (x2-x1),1, NULL, EIS_REGION_WIDTH);
		}
		else
		{
			ipanel_porting_graphics_draw_image( x2,  y2, (x1-x2),1, NULL, EIS_REGION_WIDTH);
		}
	}
#endif
	
	return 0;
}

int SumaSecure_SKB_drawString(int size, int x, int y,  int color, char*pTextString)
{
	int dotcolor;

	return 0;
	//ch_color_conversion((U8*)&color,&dotcolor);
	
	//do_report(0,"SumaSecure_SKB_drawString \n");
	CH6_TextOut(GFXHandle, CH6VPOSD.ViewPortHandle, x, y, color, pTextString);


	
	return 0;
}

void *SumaSecure_SKB_getOsdBuf(int x, int y,  int w,  int h)
{
	//do_report(0,"SumaSecure_SKB_getOsdBuf \n");
	return (void *)(CH_Eis_GetRectangle(x, y, w, h));
}

int SumaSecure_SKB_putOsdBuf(void *buf, int x, int y, int w, int h)
{
	do_report(0,"SumaSecure_SKB_putOsdBuf  x %d y %d w %d  h %d buf 0x%x\n",x,y,w,h,buf);
	
	if(buf == NULL)
	{

		ipanel_porting_graphics_draw_image( x, y, w, h, NULL, EIS_REGION_WIDTH);
		
		return  0;
	}
	
	CH_Eis_PutRectangle( x, y, w, h,buf);
	memory_deallocate(CHSysPartition,buf);

	ipanel_porting_graphics_draw_image( x, y, w, h, NULL, EIS_REGION_WIDTH);

	buf = NULL;
	
	return 0;
}

/*
	1.	�ӿڶ���
	int SumaSecure_SKB_sendIrKey (unsigned int keycode);
	2.	����˵��
	ͨ���˽ӿڷ���irkey���������STB�յ�MSG_KEY_BACK��ʾҪ֪ͨ�����ɾ��*, �յ�MSG_KEY_NUM0֪ͨ��������*������¼C��
	3.	����˵��
	���������
	�ޡ�
	���������
	*keycode, ��ȡ�ļ�ֵ��
	�� �أ�
	0��ʾ�ɹ���-1��ʾʧ�ܡ�
	4.	Ӧ�ó���
	��SKBģʽ�У�����SKB�̺߳���á�
*/
int SumaSecure_SKB_sendIrKey (unsigned int keycode)
{

	/* װ������ΪRC6����*/
	//do_report(0,"SumaSecure_SKB_sendIrKey \n");
	ch_SecurityKEYTOWebKEY(keycode);
	
}



/*
	1.	�ӿڶ���
	int SumaSecure_SKB_openImage (char *buf, int len��const char *type);
	2.	����˵��
	ͨ���˽ӿڽ���ͼƬ�Ľ��롣
	3.	����˵��
	���������
	*buf: ͼƬ���ݻ�����, ���ݳ�����len������
	len: ���ݳ��ȡ�
	type��ͼƬ��ʽ�ַ�����Ϊ�����������֣�
	     "gif"��"bmp","jpg", "png"
	���������
	�ޡ�
	�� �أ�
	0��ʾ����ʧ�ܣ�����ֵ��ʾͼƬ�����
	4.	Ӧ�ó���
	��SKB����ʾ�����е��ã�ֻ������һ�Σ���ͼƬֻ������һ�Ρ�

*/


int SumaSecure_SKB_openImage (char *buf, int len,const char *type)
{
	int    i_skbpictype = -1;
	int    ret = -1;
	int    data;

	//do_report(0,"SumaSecure_SKB_openImage \n");
#if 0
	FILE *fp;
	fp=fopen("E:\\2.gif","wb");
	if(fp==NULL)
	{
		return;
	}
	fwrite((U8 *)buf,len,1,fp);
	fclose(fp);
#endif	
	if(p_SkbPicData != NULL)
	{
		memory_deallocate(CHSysPartition,p_SkbPicData);
		p_SkbPicData = NULL;
	}

	if( memcmp(type,"gif",3) == 0)
	{
		i_skbpictype = 5;
	}
	else if( memcmp(type,"bmp",3) == 0)
	{
		i_skbpictype = 6;
	}
	else if(memcmp(type, "jpg", 3) == 0)
	{
		i_skbpictype = 1;
	}
	else if(memcmp(type,"png",3) == 0)
	{
		i_skbpictype = -1;
	}
		

	ret = CH_ParsePicture(buf,len, i_skbpictype,&data, &i_skbpicwidth,&i_skbpicheight);

	p_SkbPicData = (U8 *)data;
	if(ret == 0&&p_SkbPicData != NULL)
	{	 
		return (int)p_SkbPicData;
	}
	else
	{
		return 0;
	}
}
/*
	1.	�ӿڶ���
	int SumaSecure_SKB_dispImage (int fd, int x, int y);
	2.	����˵��
	ͨ���˽ӿڽ���ͼƬ����ʾ��
	3.	����˵��
	���������
	fd: openImageʱ���صľ����
	x,y: ��ʾͼƬ��ʾ��ʼλ�õ����ꡣ
	���������
	�ޡ�
	�� �أ�
	-1��ʾʧ�ܣ�0��ʾ�ɹ���
	4.	Ӧ�ó���
	��SKB����ʾ�����е��ã��������ôΣ�ÿˢ��һ��SKB�Ľ��㣬��Ҫ���ô˺���������ʾһ�Ρ�


*/
int SumaSecure_SKB_dispImage (int fd, int x, int y)
{
	if(fd == (int)p_SkbPicData )
	{
		i_skbx = x;
		i_skby = y;
		CH_Eis_PutRectangle(x,y, i_skbpicwidth,i_skbpicheight, p_SkbPicData);
		//ipanel_porting_graphics_draw_image( x, y, i_skbpicwidth, i_skbpicheight, NULL, EIS_REGION_WIDTH);
		return 0;
	}
	else
	{
		return -1;
	}
}
/*
	1.	�ӿڶ���
	int SumaSecure_SKB_closeImage (int fd);
	2.	����˵��
	ͨ���˽ӿڽ���ͼƬ������Դ���ͷš�ͨ��ͼƬ�����STB��ά��һ�����н������ݵ��ڴ棬���ô˽ӿڣ������ͷŴ�Ƭ�����ڴ档
	3.	����˵��
	���������
	fd: openImageʱ���صľ����
	���������
	�ޡ�
	�� �أ�
	-1��ʾʧ�ܣ�0��ʾ�ɹ���
	4.	Ӧ�ó���
	��SKB�˳���ʾʱ���ã�ֻ������һ�Ρ�
*/
int SumaSecure_SKB_closeImage (int fd)
{
	//do_report(0,"SumaSecure_SKB_closeImage \n");
	if(p_SkbPicData != NULL)
	{
		memory_deallocate(CHSysPartition,p_SkbPicData);
		p_SkbPicData = NULL;
	}

	i_skbx = -1;
	i_skby = -1;
	i_skbpicheight= -1;
	i_skbpicwidth= -1;
	

	/*  ��ҪǶ��ʽ����ˢ��IPANELosd�����ʾ*/
	
	//CH_Referbish_osd();/* wz add 20090312*/
	return 0;
}
#if 0

int SumaSecure_SKB_putImage(void *buf, int len, const char *type, int x, int y)
{
	int    i_skbpictype = -1;
	if( memcmp(type,"gif",3) == 0)
	{
		i_skbpictype = 5;
	}
	else if( memcmp(type,"bmp",3) == 0)
	{
		i_skbpictype = 6;
	}
	else if(memcmp(type, "jpg", 3) == 0)
	{
		i_skbpictype = 1;
	}
	else if(memcmp(type,"png",3) == 0)
	{
		i_skbpictype = -1;
	}
	return CH_Eis_ShowPic(x,  y, (U8 *) buf, len, i_skbpictype);
}
int SumaSecure_SKB_showImage(int nIndex)
{
	int x,y,w,h;

	CH_RESOLUTION_MODE_e enum_Mode;

	 enum_Mode = CH_GetReSolutionRate() ;
	 
	
	 if(enum_Mode == CH_1280X720_MODE)
    	{
  		x = 400;
		y = 200;
		w = 420;
		h = 288;
		
		if(nIndex == 0)
		{
			//CH_Eis_ShowPic(x, y, BIG_MSG_1_ADDR,BIG_MSG_1_LEN,1);
			CH_Eis_DisplayPicdata(x, y,w, h, (U8 *) BIG_MSG_1_ADDR);
		}
		else if(nIndex == 1)
		{
			
			CH_Eis_DisplayPicdata(x, y,w, h, (U8 *) BIG_MSG_2_ADDR);

		}
		else if(nIndex == 2)
		{
			
			CH_Eis_DisplayPicdata(x, y,w, h, (U8 *) BIG_MSG_3_ADDR);
			
		}
        }
	else
	{
		x = 246;
		y = 170;
		w= 289;
		h = 198;
		if(nIndex == 0)
		{
			
			CH_Eis_DisplayPicdata(x, y,w, h, (U8 *) SMALL_MSG_1_ADDR);
		}
		else if(nIndex == 1)
		{
			
			CH_Eis_DisplayPicdata(x, y,w, h, (U8 *) SMALL_MSG_2_ADDR);

		}
		else if(nIndex == 2)
		{
			
			CH_Eis_DisplayPicdata(x, y,w, h, (U8 *) SMALL_MSG_3_ADDR);
			
		}
	 }

	

}
#endif
/*=============SOCKET�Ĵ����ӿ�=============================*/

/*
1.	�ӿڶ���
	SOCKET SumaSTBSecure_ Socket(int iDomain, int iType, int iProtocol);
2.	��������
����socket��
3.	����˵����
���������
iDomain������ΪAF_INET��
            iType��SOCK_STREAM ��SOCK_DGRAM��SOCK_RAW
            iProtocol��Э��š�                                   
�������: ��   
�� �أ�
�ɹ���ΪSOCKET���iSocket ������ΪNULL��
4.	Ӧ�ó���

*/

int SumaSTBSecure_Socket(int iDomain, int iType, int iProtocol)
{
	int err;
	#ifdef SKB_DEBUG
	SKB_Printf(("\n SumaSTBSecure_Socket start "));
	#endif
	if ((2 == iDomain) || (23 == iDomain))
	{
	    if ((1 == iType) || (2 == iType))
	    {
	        if (0 == iProtocol)
	        {
	            err = CHMID_TCPIP_Socket(iDomain, iType,iProtocol);		
	        }
	    }
	}
	CH_ipanel_SocketSumAdd();
	#ifdef SKB_DEBUG
	SKB_Printf(("\n SumaSTBSecure_Socket end "));
	#endif
	return err;

}
/*
	1.	�ӿڶ���
		int SumaSTBSecure_close (int iSocket  );
	2.	��������
		�ر�socket��
	3.	����˵����
		���������iSocket  Socket�ӿ�������. 
		�����������
		�� �أ�0��ʾ�ɹ���������ʾʧ�ܡ�
	4.	Ӧ�ó���

*/

int SumaSTBSecure_Close (int iSocket  )
{
	int errno;
	
	#ifdef SKB_DEBUG
	SKB_Printf(("\n SumaSTBSecure_Close start "));
	#endif
	
	if (iSocket >= 0)
	{
		CH_ipanel_SocketReduce();
		errno=CHMID_TCPIP_Close(iSocket);
		#ifdef SKB_DEBUG
		SKB_Printf(("\n SumaSTBSecure_Close end "));
		#endif
		return errno;
	}
	else
	{
		#ifdef SKB_DEBUG
		SKB_Printf(("\n SumaSTBSecure_Close end "));
		#endif
		return -1;
	}

	
}
/*
	�ӿڶ���
		Int SumaSTBSecure_ Bind(int iSocket, struct sockaddr *pName, int iNameLen);
	��������
	:	�׽ӿڰ󶨵������豸��ĳһ���˿�
	����˵����
		���������
		iSocket��socket �ļ���������Ϊsocket()���÷��ص��׽ӿ��ļ�����������
	           		*pName��ָ�����ݽṹsockaddr ��ָ�롣���ݽṹ����������
	           		iNameLen������Ϊsizeof(struct sockaddr)��
		�������: ��    
		�� �أ�
		0��ʾ�ɹ���������ʾʧ�ܡ�
	Ӧ�ó���

*/

int SumaSTBSecure_Bind(int iSocket, struct sockaddr *pName, int iNameLen)
{
	int errno;
	#ifdef SKB_DEBUG
	SKB_Printf(("\n SumaSTBSecure_Bind start "));
	#endif
	if (iSocket >= 0)
	{
		
		struct sockaddr_in  s_addr;
		CHMID_TCPIP_SockAddr_t  adr;
		
	
		memcpy(&s_addr,pName,iNameLen);
		
		adr.uc_Len = 16;
		adr.ui_IP = s_addr.sin_addr.s_addr;
		adr.us_Family = s_addr.sin_family;
		adr.us_Port = s_addr.sin_port;
		errno=CHMID_TCPIP_Bind(iSocket, &adr, iNameLen);
		#ifdef SKB_DEBUG
		SKB_Printf(("\n SumaSTBSecure_Bind end "));
		#endif
		return errno;
	}
	else
	{
		#ifdef SKB_DEBUG
		SKB_Printf(("\n SumaSTBSecure_Bind err end "));
		#endif
		return -1;
	}

}
/*
	�ӿڶ���
		Int SumaSTBSecure_Listen(int iSocket, int iBacklog);
	��������
		�ȴ�һ���������������
	����˵����
		���������
		iSocket��socket �ļ���������Ϊsocket()���÷��ص��׽ӿ��ļ�����������
		iBacklog�������������������ӵĸ�����                            
	�������: ��    
		�� �أ�
		0��ʾ�ɹ���������ʾʧ�ܡ�
	Ӧ�ó���

*/
int SumaSTBSecure_Listen(int iSocket, int iBacklog)
{
	int errno;
	#ifdef SKB_DEBUG
	SKB_Printf(("\n SumaSTBSecure_Listen start "));
	#endif
	if (iSocket >= 0)
	{
		errno=CHMID_TCPIP_Listen(iSocket, iBacklog);
		#ifdef SKB_DEBUG
		SKB_Printf(("\n SumaSTBSecure_Listen end "));
		#endif
		return errno;
	}
	else
	{
		#ifdef SKB_DEBUG
		SKB_Printf(("\n SumaSTBSecure_Listen err end "));
		#endif
		return -1;
	}

}

/*
	�ӿڶ���
		Int SumaSTBSecure_accept(int iSocket, struct sockaddr *pAddr, int *piAddrLen);
	��������
		���ܲ���iSocket��socket����
		����˵����
		���������
			iSocket��socket �ļ���������Ϊsocket()���÷��ص��׽ӿ��ļ�����������
			*pAddr��ָ�����ݽṹsockaddr ��ָ�롣���ݽṹ����������
			*piAddrlen������Ϊsizeof(struct sockaddr)��                            
		�������: ��    
	�� �أ�
		0��ʾ�ɹ���������ʾʧ�ܡ�
	Ӧ�ó���

*/

int  SumaSTBSecure_Accept(int iSocket, struct sockaddr *pAddr, int *piAddrLen)
{
	
	#ifdef SKB_DEBUG
	SKB_Printf(("\n SumaSTBSecure_Accept start "));
	#endif
	if (iSocket >= 0)
	{
		struct sockaddr_in  s_addr;
		CHMID_TCPIP_SockAddr_t  adr;
		
	
		memcpy((U8 *)(&s_addr),(U8 *)(pAddr),sizeof(s_addr));
		
		adr.uc_Len = 16;
		adr.ui_IP = s_addr.sin_addr.s_addr;
		adr.us_Family = s_addr.sin_family;
		adr.us_Port = s_addr.sin_port;
		
		return CHMID_TCPIP_Accept(iSocket,&adr, piAddrLen);
	}
	else
	{
		return -1;
	}

}

/*
	1.	�ӿڶ���
		Int SumaSTBSecure_ Connect(int iSocket, struct sockaddr *pName, int iNameLen);
	2.	��������
		��Զ����������
	3.	����˵����
		���������
			iSocket��socket �ļ���������Ϊsocket()���÷��ص��׽ӿ��ļ�����������
			*pName��ָ�����ݽṹsockaddr ��ָ�롣���ݽṹ����������
			iNameLen������Ϊsizeof(struct sockaddr)��                          
		
		�������: ��    
			�� �أ�
			0��ʾ�ɹ���������ʾʧ�ܡ�
	4.	Ӧ�ó���
*/

int  SumaSTBSecure_Connect(int iSocket, struct sockaddr *pName, int iNameLen)
{
	#ifdef SKB_DEBUG
	SKB_Printf(("\n SumaSTBSecure_Connect start "));
	#endif
	if (iSocket >= 0)
	{
		int len ;
		struct sockaddr_in  s_addr;
		CHMID_TCPIP_SockAddr_t  adr;
		
	
		memcpy(&s_addr,pName,iNameLen);
		
		adr.uc_Len = 16;
		adr.ui_IP = s_addr.sin_addr.s_addr;
		adr.us_Family = s_addr.sin_family;
		adr.us_Port = s_addr.sin_port;
		return CHMID_TCPIP_Connect(iSocket,&adr, iNameLen);
	}
	else
	{
		return -1;
	}

}

/*
	1.	�ӿڶ���
	Int SumaSTBSecure_ send(int iSocket, void *pData, int iSize, unsigned int iFlags);
	2.	��������
	ͨ��socket������Ϣ��
	3.	����˵����
	���������
	iSocket��socket �ļ�������
	*pData��ϣ�����͵����ݵ�ָ�롣
	iSize��ϣ���������ݵĳ��ȡ�
	iFlags���������ñ�־,ȱʡ����Ϊ0��                                 
	�������: ��   	
	�� �أ�
	0��ʾ�ɹ���������ʾʧ�ܡ�
	4.	Ӧ�ó���

*/

int SumaSTBSecure_Send(int iSocket, void *pData, int iSize, unsigned int iFlags)
{
	#define MAX_SEND_LEN 1460
	int ret;

	
	#ifdef SKB_DEBUG
	SKB_Printf(("\n SumaSTBSecure_Send start "));
	#endif
	if ((iSocket >= 0) && (NULL != pData))
	    {
	    	if( iSize <= MAX_SEND_LEN)
    		{
       			 ret =CHMID_TCPIP_Send(iSocket, pData, iSize, iFlags);

			return  ret;
    		}
		else
		{
			int pos=0;
			int i;
			int times = iSize/MAX_SEND_LEN;

		
			for(i = 0;i < times;i++)
			{
				do_report(0,"pos= %d\n",pos);
				
				ret	= CHMID_TCPIP_Send(iSocket,(void*)((int)pData+pos),MAX_SEND_LEN,iFlags);
				
				do_report(0,"send len %d\n",ret);
				
				if( ret  == MAX_SEND_LEN)/* ������������*/
				{
					pos += MAX_SEND_LEN;			
				}
				else 
				{
					return -1;
				}
				
			}

			if( (iSize%MAX_SEND_LEN)  !=  0)
			{
				int templen = iSize-times*MAX_SEND_LEN;
				ret = CHMID_TCPIP_Send(iSocket,(void*)((int)pData+pos),templen,iFlags);

				if( ret  ==  templen)
				{
					pos += templen;
				}
				else
				{
					return -1;
				}
					
			}
			
				ret = iSize;
				return ret;
			
			


		}
}
}

/*
	1.	�ӿڶ���
		Int SumaSTBSecure_recv (int iSocket, void *pMem, int iLen, unsigned int iFlags);
	2.	��������
		����socket��
	3.	����˵����
		���������
		iSocket��socket �ļ�������
		*pMem�����������Ϣ�ĵ�ַ��
		iLen��������Ϣ�ĳ��ȡ�
		iFlags���������ñ�־,ȱʡ����Ϊ0��                                 
		�������: ��                   	
		�� �أ�
			0��ʾ�ɹ���������ʾʧ�ܡ�
	4.	Ӧ�ó���

*/
int SumaSTBSecure_Recv (int iSocket, void *pMem, int iLen, unsigned int iFlags,int timeout)
{

	#ifdef SKB_DEBUG
	SKB_Printf(("\n SumaSTBSecure_Recv start "));
	#endif
	if (iSocket >= 0)
	{
		int i_timeout = timeout;
		SumaSTBSecure_Setsockopt( iSocket,0XFFF,4,&i_timeout,sizeof(i_timeout));
		return CHMID_TCPIP_Recv(iSocket,pMem,iLen, iFlags);
	}
	else
	{
		return -1;
	}

}

/*
	1.	�ӿڶ���
		int SumaSTBSecure_ SetSockOpt (int iSocket, int iLevel, int iOptName, void *pOptVal, int iOptLen); 
	2.	��������
		����socket״̬��
	3.	����˵����
		���������
			iSocket��socket �ļ���������Ϊsocket()���÷��ص��׽ӿ��ļ���������Ҳ��Ϊaccept()�õ����ļ�����������
			iLevel�����������õ�����㣬һ�����SOL_SOCKET �Դ�ȡsocket �㡣
			iOptName�����������õ�ѡ��		
			*pOptVal�����������õ�ֵ(������)��
			iOptLen����Ϊoptval �ĳ��ȡ�
		�����������	
		�� �أ�
			0��ʾ�ɹ���������ʾʧ�ܡ�
	4.	Ӧ�ó���

*/

int SumaSTBSecure_Setsockopt (int iSocket, int iLevel, int iOptName, void *pOptVal, int iOptLen)
{
	#ifdef SKB_DEBUG
	SKB_Printf(("\n SumaSTBSecure_Setsockopt start "));
	#endif
	if (iSocket >= 0)
	{
		struct timeval TimeOut;
		TimeOut.tv_sec = *(int *)pOptVal ;
		TimeOut.tv_usec = 0;
		return CHMID_TCPIP_SetSocketOpt(iSocket,iLevel,iOptName,&TimeOut,sizeof(TimeOut));
	}
	else
	{
		return -1;
	}

}
/*
	1.	�ӿڶ���
		int SumaSTBSecure_getsockopt (int iSocket, int iLevel, int iOptName, void *pOptVal, int iOptLen);

	2.	��������
	����socket��
	3.	����˵����
	���������
	iSocket��socket �ļ���������Ϊsocket()���÷��ص��׽ӿ��ļ���������Ҳ��Ϊaccept()�õ����ļ�����������
	iLevel�����������õ�����㣬һ�����SOL_SOCKET �Դ�ȡsocket �㡣
	iOptName�����������õ�ѡ��		
	*pOptVal�����������õ�ֵ��
	iOptLen����Ϊoptval �ĳ��ȡ�
				�����������
	�� �أ�
	0��ʾ�ɹ���������ʾʧ�ܡ�
	4.	Ӧ�ó���

*/

int SumaSTBSecure_Getsockopt (int iSocket, int iLevel, int iOptName, void *pOptVal, int iOptLen)
{
	int iOptLen_temp;
	int err_opt=iOptLen;
	#ifdef SKB_DEBUG
	SKB_Printf(("\n SumaSTBSecure_Getsockopt start "));
	#endif
	if (iSocket >= 0)
	{
		return CHMID_TCPIP_GetSocketOpt(iSocket,iLevel,iOptName,pOptVal,&iOptLen_temp);
	}
	else
	{
		return -1;
	}

}

/*�ӿڶ���
	Int SumaSTBSecure_ShutDown(int iSocket, int iHow);
	��������
	�ж�ͨ��
	����˵����
	���������
	Socket��socket �ļ���������Ϊsocket()���÷��ص��׽ӿ��ļ�����������
	iHow�� 0 ��ֹ��ȡ������
	1 ��ֹ���Ͳ�����
	2 ��ֹ��ȡ�����Ͳ���                            
	�������: ��    
	�� �أ�
	0��ʾ�ɹ���������ʾʧ�ܡ�
	Ӧ�ó���
*/

int SumaSTBSecure_Shutdown(int iSocket, int iHow)
{
	#ifdef SKB_DEBUG
	SKB_Printf(("\n SumaSTBSecure_Shutdown start "));
	#endif
	if (iSocket >= 0)
	{
		return CHMID_TCPIP_ShutDown(iSocket, iHow);
	}
	else
	{
		return -1;
	}


}

int  ch_RC6keyTOSecurityKEY(int RC6key)
{

	switch( RC6key )
	{
		/* ���ְ���*/
		case KEY_DIGIT0:	 		return  MSG_KEY_NUM0;
		case KEY_DIGIT1:	 		return  MSG_KEY_NUM1;
		case KEY_DIGIT2:	 		return  MSG_KEY_NUM2;
		case KEY_DIGIT3:	 		return  MSG_KEY_NUM3;
		case KEY_DIGIT4:	 		return  MSG_KEY_NUM4;
		case KEY_DIGIT5:	 		return  MSG_KEY_NUM5;
		case KEY_DIGIT6:	 		return  MSG_KEY_NUM6;
		case KEY_DIGIT7:	 		return  MSG_KEY_NUM7;
		case KEY_DIGIT8:	 		return  MSG_KEY_NUM8;
		case KEY_DIGIT9:	 		return  MSG_KEY_NUM9;

		
		/* �˳�����*/
		case EXIT_KEY:
			return MSG_KEY_EXIT;

		case VIRKEY_REMOVECARD_KEY:/*�γ���*/
			return MSG_KEY_TERMINATE;

		case RETURN_KEY: 	
			return MSG_KEY_BACK;	

		/*��������ȷ�ϰ���*/
		case UP_ARROW_KEY: 		return MSG_KEY_UP;
		case DOWN_ARROW_KEY: 	return MSG_KEY_DOWN;
		case LEFT_ARROW_KEY: 		return MSG_KEY_LEFT;
		case RIGHT_ARROW_KEY: 	return MSG_KEY_RIGHT;
		case SELECT_KEY: 	                 return MSG_KEY_OK;

			
	
		
		default: 
		{
			return MSG_KEY_HINT18;
		}
	 	
	}
	
}


int  ch_SecurityKEYTOWebKEY(int key)
{
#if 1
	SKB_EV_KEYCODE  keycode = key;
	switch( keycode )
	{
		/* ���ְ���*/
		case MSG_KEY_NUM0:	 		
		case MSG_KEY_NUM1:	 		
		case MSG_KEY_NUM2:	 		
		case MSG_KEY_NUM3:	 		
		case MSG_KEY_NUM4:	 		
		case MSG_KEY_NUM5:	 	
		case MSG_KEY_NUM6:	 		
		case MSG_KEY_NUM7:	 		
		case MSG_KEY_NUM8:	 		
		case MSG_KEY_NUM9:	 		

		
			eis_api_msg_send(IPANEL_EVENT_TYPE_CHAR,65,0);
			break;
			

		/*��������ȷ�ϰ���*/
		case MSG_KEY_UP: 	
			eis_api_msg_send(IPANEL_EVENT_TYPE_KEYDOWN,IPANEL_IRKEY_UP,0);
			break;
		case MSG_KEY_DOWN: 	
			eis_api_msg_send(IPANEL_EVENT_TYPE_KEYDOWN,IPANEL_IRKEY_DOWN,0);
			break;
		case MSG_KEY_LEFT: 	      
			eis_api_msg_send(IPANEL_EVENT_TYPE_KEYDOWN,IPANEL_IRKEY_LEFT,0);
			break;
		case MSG_KEY_RIGHT:   	
			eis_api_msg_send(IPANEL_EVENT_TYPE_KEYDOWN,IPANEL_IRKEY_RIGHT,0);
			break;
		case MSG_KEY_OK: 	      
			eis_api_msg_send(IPANEL_EVENT_TYPE_KEYDOWN,IPANEL_IRKEY_SELECT,0);
			break;

			
		case MSG_KEY_BACK: 

			eis_api_msg_send(IPANEL_EVENT_TYPE_KEYDOWN,IPANEL_IRKEY_BACK,0);
			break;

		case MSG_KEY_HELP:
				eis_api_msg_send(IPANEL_EVENT_TYPE_CHAR,72,0);/*H*/
			break;

		
		case MSG_KEY_DEL:
			/*eis_api_msg_send(IPANEL_EVENT_TYPE_CHAR,68,0);*/
			eis_api_msg_send(IPANEL_EVENT_TYPE_KEYDOWN,IPANEL_IRKEY_BACK,0);
			break;
			
		case MSG_KEY_CLOSESKB:
				eis_api_msg_send(IPANEL_EVENT_TYPE_CHAR,67,0);/*C*/
			break;
		case MSG_KEY_HINT1:
				eis_api_msg_send(IPANEL_EVENT_TYPE_CHAR,73,0);/*I*/
			break;
		case MSG_KEY_HINT2:
				eis_api_msg_send(IPANEL_EVENT_TYPE_CHAR,74,0);/*J*/
			break;
		case MSG_KEY_HINT3:
				eis_api_msg_send(IPANEL_EVENT_TYPE_CHAR,75,0);/*K*/
			break;
		case MSG_KEY_HINT18:
				eis_api_msg_send(IPANEL_EVENT_TYPE_CHAR,90,0);/*Z*/
			break;	
		
		default:
			eis_api_msg_send(IPANEL_EVENT_TYPE_CHAR,keycode,0);
			break;
		
	 	
	}
#endif
		return 0;
	
}

void CH_SKB_EvExtractAction(void)
{

	 if(CH_GetSkbStatus() == true)/*�������Ч*/
	{
		int i;
		clock_t timeout =  time_plus(time_now(),ST_GetClocksPerSecond()*3);;

		do_report(0,"\n<<<<<<<VIRKEY_REMOVECARD_KEY<<<<<\n");
		PutKeyIntoKeyQueue(VIRKEY_REMOVECARD_KEY);

		cardstatus = true;
		
		semaphore_wait_timeout(pst_GifSema, &timeout);

		 if(CH_GetSkbStatus() == true)
		 {
			gb_skbactive = false;
		 }
		 else
		 {
			task_delay(5000);
		 }
		do_report(0,"\n$$$$VIRKEY_REMOVECARD_KEY$$$$$$$$$$$$$n");
	}
		
}

void su_test(void)
{
#if 0	
	U8* temp;
	int i;

	for (i=0;i<50;i++)
	{
		SumaSecure_SKB_drawDot(( 200+i),200, 0xfff08020);
		SumaSecure_SKB_drawDot(( 200+i),201, 0xfff08020);
		SumaSecure_SKB_drawDot(( 200+i),202, 0xfff08020);
	}

	SumaSecure_SKB_drawLine(300, 200, 300, 500, 0xfff08020);
	
	SumaSecure_SKB_drawLine(350, 185, 450, 185, 0xfff08020);
	SumaSecure_SKB_drawLine(350, 186, 450, 186, 0xfff08020);

	SumaSecure_SKB_fillBox(350, 150, 100, 30, 0xfff08020);

	temp = SumaSecure_SKB_getOsdBuf(300, 200, 200, 100);
	if(temp != NULL)
	{
		SumaSecure_SKB_putOsdBuf(temp,300, 200, 200, 100);
	}
	


	FILE *fp;
	int fileLen = 0;
	U8* fileBuf = NULL;
	int Temp;
	SHORT Xptr ;

	int i_checkval = -1;
	
	fp=fopen("E:\\1.jpg","rb");
	if(fp==NULL)
	{
		return;
	}
	fseek(fp,0,SEEK_END);
	fileLen = ftell(fp);
	fseek(fp,0,SEEK_SET);

	fileBuf = memory_allocate(CHSysPartition,fileLen);
	if(fileBuf == NULL)
	{
		return;
	}
	memset(fileBuf,0,fileLen);
	
	Temp = fread((void *)fileBuf,1,fileLen,fp);
	fclose(fp);
	
	if(fileLen != Temp)
	{
		return ;
	}

	SumaSTBSecure_WriteFlash(0xa11e0000,&fileLen, fileBuf);

	memset(fileBuf,0,fileLen);

	SumaSTBSecure_ReadFlash(0xa11e0000, &fileLen,fileBuf);
	
	CH_Eis_ShowPic(300, 200, fileBuf, fileLen,1);

	
	if(fileBuf != NULL)
	{
		memory_deallocate(CHSysPartition,fileBuf);
		fileBuf = NULL;
	}



	int white_sock;
	struct sockaddr_in me;
	int ret = 0; 
	char buff[1024] = {0};	
	struct timeval tv={0,10};
	const char msg[] = "Socket Test";
	int time = 1;
	
	white_sock = SumaSTBSecure_Socket(AF_INET, SOCK_STREAM, 0 );
	

	memset(&me, 0, sizeof(me) );
	me.sin_addr.s_addr = Suma_inet_addr("192.163.35.100");
	me.sin_family = AF_INET;
	me.sin_port = htons(1234);
	me.sin_len = sizeof( me );
                      
	ret = SumaSTBSecure_Connect(white_sock,(const struct sockaddr *)(&me),sizeof(struct sockaddr));
	do_report(0," %d",ret);
	
	ret = SumaSTBSecure_Send(white_sock, msg, sizeof(msg), 0);
	SumaSTBSecure_Setsockopt(white_sock,0xfff,4, (char*)&time, 4);
	ret = SumaSTBSecure_Recv(white_sock, buff, 1024,0,10);

	char szCardID[20];
	int len =20;
	memset(szCardID,0,20);

	SumaSTBSecure_GetUserPIN(szCardID);

	do_report(0," %s \n",szCardID);

		FILE *fp;
	U8* fileBuf = NULL;
	int filelen = 0x1c1;
	int sixe,addr;
	fp=fopen("E:\\OTP_Read.bin","wb");
	if(fp==NULL)
	{
		return;
	}

	SumaSTBSecure_GetOTPAddr(&sixe, &addr);

	fileBuf = memory_allocate(CHSysPartition,filelen);
	if(fileBuf == NULL)
	{
		return;
	}
	
	SumaSTBSecure_ReadOTP(addr,&filelen, fileBuf);
	
	
	fwrite(fileBuf,filelen,1,fp);
	fclose(fp);




		/**ͼƬ��ʾ����**/
	FILE *fp = NULL;
	int fileLen = 0;
	U8* fileBuf = NULL;
	int Temp;
	SHORT Xptr ;
	int i;
	int i_checkval = -1;

	for(i = 1;i < 6 ;i++)
	{
	switch(i)
	{
		case 1:
			fp=fopen("E:\\r1.jpg","rb");
			break;
		case 2:
			fp=fopen("E:\\r2.jpg","rb");
			break;
		case 3:
			fp=fopen("E:\\r3.jpg","rb");
			break;
		case 4:
			fp=fopen("E:\\r4.jpg","rb");
			break;
		case 5:
			fp=fopen("E:\\r5.jpg","rb");
			break;

		default:
			break;

	}
	
	if(fp==NULL)
	{
		return;
	}
	fseek(fp,0,SEEK_END);
	fileLen = ftell(fp);
	fseek(fp,0,SEEK_SET);

	fileBuf = memory_allocate(CHSysPartition,fileLen);
	if(fileBuf == NULL)
	{
		return;
	}
	memset(fileBuf,0,fileLen);
	
	Temp = fread((void *)fileBuf,1,fileLen,fp);
	fclose(fp);
	
	if(fileLen != Temp)
	{
		return ;
	}

	SumaSecure_SKB_openImage(fileBuf, fileLen,"jpg");
	if(fileBuf != NULL)
	{
		memory_deallocate(CHSysPartition,fileBuf);
		fileBuf = NULL;
	}
		
	SumaSecure_SKB_dispImage(p_SkbPicData, 50+200*i, 200);
	SumaSecure_SKB_putOsdBuf(NULL,50+200*i, 200, 200, 200);
	SumaSecure_SKB_closeImage(p_SkbPicData);

	task_delay(ST_GetClocksPerSecond());
	}
#endif
	int adr,len ;
	FILE *fp;
	int fileLen = 0;
	U8* fileBuf = NULL;
	int Temp;
	U8* CopyBuf = NULL;
	
	fp=fopen("E:\\otp.bin","rb");
	if(fp==NULL)
	{
		return;
	}
	fseek(fp,0,SEEK_END);
	fileLen = ftell(fp);
	fseek(fp,0,SEEK_SET);

	fileBuf = memory_allocate(CHSysPartition,fileLen);
	if(fileBuf == NULL)
	{
		return;
	}
	CopyBuf = memory_allocate(CHSysPartition,fileLen);
	if(CopyBuf == NULL)
	{
		return;
	}
	memset(fileBuf,0,fileLen);
	
	Temp = fread((void *)fileBuf,1,fileLen,fp);
	fclose(fp);
	
	if(fileLen != Temp)
	{
		return ;
	}

	SumaSTBSecure_GetEepromAddr(&len,&adr);
	
	while(1)
	{
		memset(CopyBuf,0,fileLen);
		fileLen = Temp;
		SumaSTBSecure_WriteEeprom(adr,&fileLen, fileBuf);
		fileLen = Temp;
		SumaSTBSecure_ReadEeprom(adr,&fileLen,CopyBuf);

		if(memcmp(fileBuf,CopyBuf,fileLen) != 0)
		{
			do_report(0," memcmp ERR\n");
			break;
		}
	}

	if(fileBuf != NULL)
	{
		memory_deallocate(CHSysPartition,fileBuf);
		fileBuf = NULL;
	}

	if(CopyBuf != NULL)
	{
		memory_deallocate(CHSysPartition,CopyBuf);
		CopyBuf = NULL;
	}
	
	
	

}


void CH_ReadUIANDFlash(void)
{
	FILE *fp;
	fp=fopen("E:\\DVB-C8000BI.bin","wb");
	if(fp==NULL)
	{
		return;
	}
	fwrite((U8 *)0xA0120000,0xEE0000,1,fp);
	fclose(fp);
}
BOOL  CH_CheckSKBDisplay(int x,int y,int w,int h)
{
	if(CH_GetSkbStatus() == true && i_skbx != -1 && i_skby!=-1 && i_skbpicwidth != -1 && i_skbpicheight != -1)
	{
		int dis_x,dis_y;
	
		
		if(i_skbx >= x)
		{
			dis_x = i_skbx - x;
			if(dis_x < w)
			{
				if(i_skby >= y)
				{
					dis_y = i_skby - y;
					if(dis_y < h)
					{
						return true;
					}
				}
				else
				{
					dis_y = y- i_skby;
					if(dis_y < i_skbpicheight)
					{
						return true;
					}
				}

			}
			 
		}
		else
		{
			dis_x = x- i_skbx;
			 if(dis_x < i_skbpicwidth)
			 {
				if(i_skby >= y)
				{
					dis_y = i_skby - y;
					if(dis_y < h)
					{
						return true;
					}
				}
				else
				{
					dis_y = y- i_skby;
					if(dis_y < i_skbpicheight)
					{
						return true;
					}
				}


			 }
		}

		
		return false;
		
	
	}
	else
	{
		return false;

	}




}


