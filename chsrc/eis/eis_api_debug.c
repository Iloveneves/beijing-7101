/*
  * ===================================================================================
  * CopyRight By CHANGHONG NET L.T.D.
  * �ļ�: 	eis_api_debug.c
  * ����: 	ʵ��debug�������ؽӿ�
  * ����:	������
  * ʱ��:	2008-10-22
  * ===================================================================================
  */

#include "eis_api_define.h"
#include "eis_api_globe.h"
#include "eis_api_debug.h"
#include "stdarg.h"              	/* for argv argc  */

semaphore_t *p_EisDebugSmp=NULL; /*�Դ�ӡ�ӿ����ӱ�����ֹ������������*/
#if 1
#define __EIS_API_DEBUG_ENABLE__
#endif
int    gi_DebugControl = true;

#ifdef __EIS_API_DEBUG_ENABLE__
static char eis_string_p[1*1024] = "";
#endif

#ifdef __EIS_API_DEBUG_ENABLE__
static char eis_debug_string[512] = "";
#endif

void ipanel_EnableDebugControl (void)
{
   gi_DebugControl = true;
   
}
void ipanel_DisableDebugControl (void)
{
   gi_DebugControl = false;
}

/*
����˵����
	iPanel Middleware ���øú��������̨���������Ϣ���ú����ĵ��÷�����C ��
	׼���printf ��ͬ��ʵ��ʱ��ʹ�ÿ��������ƣ��Է�������Ƿ����������Ϣ��
ע�⣺
	iPanel MiddleWare Ҫ�������Ϣ�������������Ϊ4K�������п��ܵ����̵߳��쳣��
����˵����
	���������
		fmt����ʽ���ַ���������˵�����ձ�׼��printf ������
	�����������
	�� �أ�
		>0: ʵ��������ַ�����
		IPANEL_ERR: �����쳣���ء�
  */
INT32_T ipanel_porting_dprintf( const CHAR_T *fmt , ...)
{
     if(gi_DebugControl == false)
     	{
			return 0;
     	}
#ifdef __EIS_API_DEBUG_ENABLE__
	{
		va_list Argument;
		if(p_EisDebugSmp==NULL)
		{
			p_EisDebugSmp = semaphore_create_fifo(1);
			if(p_EisDebugSmp==NULL)
				return 0;
		}
		
		if(strlen(fmt)>1023)
		{
			return 0;
		}
		 semaphore_wait(p_EisDebugSmp);
		/* Translate arguments into a character buffer */
		va_start ( Argument, fmt );
		vsprintf ( eis_string_p, fmt, Argument );
		va_end ( Argument );
	 
		/*STTBX_Print((String_p));*/
		sttbx_Print( "%s", eis_string_p );
		semaphore_signal ( p_EisDebugSmp );
	}
#endif
	return 0;
}
/*
����˵����
	eis �ӿ�debug��Ϣ����
����˵����
	���������
		fmt����ʽ���ַ���������˵�����ձ�׼��printf ������
	�����������
	�� �أ�
		>0: ʵ��������ַ�����
		IPANEL_ERR: �����쳣���ء�
  */
INT32_T eis_report( const CHAR_T *fmt , ...)
{

#ifdef __EIS_API_DEBUG_ENABLE__
	va_list Argument;
	if(strlen(fmt)>511)
	{
		return 0;
	}
	
	if(p_EisDebugSmp==NULL)
	{
		p_EisDebugSmp = semaphore_create_fifo(1);
		if(p_EisDebugSmp==NULL)
			return 0;
	}
	semaphore_wait(p_EisDebugSmp);

	/* Translate arguments into a character buffer */
	va_start ( Argument, fmt );
	vsprintf ( eis_debug_string, fmt, Argument );
	va_end ( Argument );
 
	/*STTBX_Print((String_p));*/

	sttbx_Print( " %s", eis_debug_string );
	semaphore_signal ( p_EisDebugSmp );

#endif
	return 0;
}

/*--eof---------------------------------------------------------------------------------------------------*/

