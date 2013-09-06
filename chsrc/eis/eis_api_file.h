/*
  * ===================================================================================
  * CopyRight By CHANGHONG NET L.T.D.
  * �ļ�: 	eis_api_file.h
  * ����: 	�����ļ�ϵͳ��صĽӿ�
  * ����:	 ��ս�ң�������
  * ʱ��:	2008-10-23
  * ===================================================================================
  */

#ifndef __EIS_API_FILE__
#define __EIS_API_FILE__

#if 0
#define CHMID_FILE_MAX_NUM	100			/* ���ֻ�ܴ�100���ļ� */
#define CHMID_FILE_MAX_OPEN	5			/* ͬʱ�ܴ��ļ����߳��� */
#define CHMID_DIRE_MAX_DEEP	100			/* Ŀ¼��� */

/* �ļ��ṹ */
typedef struct
{
	semaphore_t		*mp_SemaFile;
	char				*cp_FileName;
	U8				mu8_OpenCount;							/* ��ǰ�򿪸��ļ��Ľ������� */
	U32				mu32_TotalFileLen;
	U32				mu32_CurrFileLen[CHMID_FILE_MAX_OPEN];	/* ���ֻ����5������ͬʱ��һ���ļ� */
	U8				*mp_Data;								/* �ļ�ʵ������ָ�� */
}chmid_file_t;
#endif

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
extern UINT32_T ipanel_porting_file_open(const CHAR_T*filename, const CHAR_T *mode);

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
extern INT32_T ipanel_porting_file_close(UINT32_T fd);

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
extern INT32_T ipanel_porting_file_delete(const CHAR_T *name);


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
extern INT32_T ipanel_porting_file_read(UINT32_T fd, BYTE_T *buf, INT32_T len);

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
extern INT32_T ipanel_porting_file_write(UINT32_T fd,const CHAR_T *buf, INT32_T len);

#endif

/*--eof-----------------------------------------------------------------------------------------------------*/

