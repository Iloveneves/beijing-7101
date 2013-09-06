#ifndef _GRAPHCNFIG_H__/*ͼ�β�����������������*/
#define _GRAPHCNFIG_H__
#include "br_file.h"
#include "os_plat.h"
#define _USE_VECTORFONT_ /*ʹ��ʸ������*/

/*#define ENABLE_CH_PRINT*/
#define SQ_MDF_20061103
/*
1��Ϊ���AV INITΪ��Ƶʱ��Ϊ��Ч������������
2�� Ϊ���"����ר��"��� gif�и���һֱ�Ѳ���,���ƺ���������������,
	����������ķ����Ƕ��ڲ�������ͼƬ������
	ͬʱ�����ڲ�ͬ��������ڶ����ͬ�Ķ��������ͬ��PID, ���
	����������ʱ��Ҫ������һ��PID�ı�ʯ
3.  ��ֹ���źŲ��������ղ�����, ������һ���ĵȴ�ʱ��
*/


#define SQ_MDF_1218 
/*�������һ��ҳ���ǿ�Ƶ�㲥�ţ� ����һ��ҳ���INIT����3ʱ�� ������������������*/

#define MDF_070306
/*
1. ���������Ļ���з�ʽ
2��STBNUMֻ�������ڳ�������к�
3��SRC���Ϊ0�� ����ͻ��˲�����
*/

#define DISABLE_GIF_CACHE
/*
�ر�gif��������
*/

#define TXTLINK_SCROLL
/*
�����ӵ���Ļ����Ч��
*/

#define H070529 
/*
1�� �رշ���ʱ��������
2�� ��AV DAV VODAVʧ�ܺ� ���ص�ǰƵ��
*/

#define H070714
/*
VOD���ź� �����ж��Ƿ�ɹ�

*/

#if 0
#ifndef NULL
#undef  NULL
#define NULL 0
#endif

/*�޷�������,���ݲ�ͬ��ƽ̨Ӧ�������ı�*/
#ifndef DEFINED_U8
#define DEFINED_U8
#define U8 unsigned char
#endif

#ifndef DEFINED_BOOL
#define DEFINED_BOOL
#define CH_BOOL int
#endif

#ifndef DEFINED_U16
#define DEFINED_U16
#define U16 unsigned short
#endif

#ifndef DEFINED_U32
#define DEFINED_U32
#define U32 unsigned  long
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
#define S16 signed short int
#endif

#ifndef DEFINED_S32
#define DEFINED_S32
#define S32 signed  long int
#endif

#if 1
#ifndef INT
#define INT int /* �������ã���ò���,����S32�������������������뾯�� */
#endif
#endif
#ifndef CH_FLOAT 
#define CH_FLOAT float
#endif
/*
#ifndef DEFINED_EOF
#define DEFINED_EOF
#define EOF -1
#endif
*/
#endif
#ifdef CH_BOOL
#undef CH_BOOL 
#define CH_BOOL int
#endif

/*CQ_BORWSERģ��Ĵ�ӡ���ƣ� ��Ϊ1���ӡ �� ��Ϊ0�������ӡ*/
/*{{{*/
#define BR_BIDIRECT_C 		1
#define BR_CACHE_C 			1
#define BR_CQHTMLMAIN_C 	1
#define BR_RECEIVE_C 		1
#define BR_SOCKET_C 			0
#define BR_CQINFOSERVICE_C	0
#define BR_DRAWJPEG_C		0
#define J_AV_C				0

/*}}}*/
#endif


