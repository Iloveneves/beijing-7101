/*
  * ===================================================================================
  * CopyRight By CHANGHONG NET L.T.D.
  * �ļ�: 	eis_api_file.c
  * ����: 	ʵ���ļ�ϵͳ��صĽӿ�
  * ����:	 ��ս�ң�������
  * ʱ��:	2008-10-23
  * ===================================================================================
  */

#include "eis_api_define.h"
#include "eis_api_globe.h"
#include "eis_api_debug.h"
#include "eis_api_file.h"
#include "eis_api_globe.h"

/*
����˵����
	�򿪲���ϵͳ�е��ļ�������ļ������ڣ�Ҫ��������ļ�������ļ�����Ŀ¼��
	���ڣ���Ҫ�������Ŀ¼��
����˵����
	���������
		filename������ϵͳ֧���ļ������ַ�����
		mode����ģʽ�ַ�����
	�����������
	�� �أ�
		����IPANEL_NULL���ļ������
		����IPANEL_NULL:ʧ�ܡ�
*/
UINT32_T ipanel_porting_file_open(const CHAR_T*filename, const CHAR_T *mode)
{
		return IPANEL_NULL;
}

/*
����˵����
	�رմ򿪵��ļ���
����˵����
	���������
		fd���ļ��������ipanel_porting_file_open ��á�
	�����������
	�� �أ�
		IPANEL_OK:�ɹ�;
		IPANEL_ERR:ʧ�ܡ�
  */
INT32_T ipanel_porting_file_close(UINT32_T fd)
{
	return IPANEL_OK;	
}

/*
����˵����
	ɾ��ָ�����ļ���Ŀ¼�������Ŀ¼Ӧ�����ԡ�/����\����β��
����˵����
	���������
		name��·�����ļ�����Ŀ¼������
	�����������
	�� �أ�
		IPANEL_OK:�ɹ�;
		IPANEL_ERR:ʧ�ܡ�
  */
INT32_T ipanel_porting_file_delete(const CHAR_T *name)
{
	eis_report("\n ipanel_porting_file_delete ");
	
	return IPANEL_OK;
}

/*
����˵����
	���ļ��ĵ�ǰλ�ö�ȡ������󲻳���len �����ݡ�
����˵����
	���������
		fd��ipanel_porting_file_open �������صĴ��ļ��ľ����
		buf����������ַ��
		len�����������ȡ�
	���������
		buf����ȡ�����ݡ�
	�� �أ�
		>��0:ʵ�ʶ������ݵĳ��ȣ�0 ��С��len����ʾ���ļ���β�ˡ�
		IPANEL_ERR:������
*/
INT32_T ipanel_porting_file_read(UINT32_T fd, BYTE_T *buf, INT32_T len)
{
	eis_report("\n ipanel_porting_file_read ");

	return 0;
}

/*
����˵����
	дlen �ֽڵ����ݵ��ļ��С�
����˵����
	���������
		fd��ipanel_porting_file_open �������صĴ��ļ��ľ����
		buf�����ݿ���ʼ��ַ��
		len�����ݿ鳤�ȡ�
	����������ޡ�
	�� �أ�
		>��0:ʵ��д�����ݵĳ��ȡ���
		IPANEL_ERR:������
*/
INT32_T ipanel_porting_file_write(UINT32_T fd,const CHAR_T *buf, INT32_T len)
{
		return IPANEL_ERR;
}

#if 0
/*
  * Func: CHMID_FILE_Init
  * Desc: �ļ�ϵͳ�ĳ�ʼ��
  * Notic: ������ϵͳ��ʼ����ʱ�����
  */
void CHMID_FILE_Init ( void )
{
	int i;
	/* ���������ź��� */
	gp_SemaFileSystem = semaphore_create_fifo ( 1 );

	semaphore_wait ( gp_SemaFileSystem );
	for ( i = 0; i < CHMID_FILE_MAX_NUM; i ++ )
	{
		gt_FileSystem[i].cp_FileName 			= NULL;
		gt_FileSystem[i].mp_Data				= NULL;
		gt_FileSystem[i].mp_SemaFile			= NULL;
		gt_FileSystem[i].mu32_CurrFileLen[0]	= 0;
		gt_FileSystem[i].mu32_CurrFileLen[1]	= 0;
		gt_FileSystem[i].mu32_CurrFileLen[2]	= 0;
		gt_FileSystem[i].mu32_CurrFileLen[3]	= 0;
		gt_FileSystem[i].mu32_CurrFileLen[4]	= 0;
		gt_FileSystem[i].mu32_TotalFileLen		= 0;
		gt_FileSystem[i].mu8_OpenCount		= 0;
	}
	Semaphore_Signal ( gp_SemaFileSystem );

	return;
}
#endif
/*--eof-------------------------------------------------------------------------------------------------*/

