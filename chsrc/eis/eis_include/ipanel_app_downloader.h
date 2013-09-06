#ifndef	IPANEL_APP_DOWNLOADER_H__
#define IPANEL_APP_DOWNLOADER_H__

#ifdef __cplusplus
extern "C" {
#endif

enum{
	IPANEL_APP_EVENT_TYPE_NONE,
	IPANEL_APP_EVENT_TYPE_DOWNLOADER,//��������Ϣ
	IPANEL_APP_EVENT_TYPE_UNDEFINED
};

enum{
	IPANEL_APP_DOWNLOADER_NONE,
	IPANEL_APP_DOWNLOADER_START,//֪ͨ������Ӧ���ѿ�ʼ�����ļ�
	IPANEL_APP_DOWNLOADER_STOP,//֪ͨ������Ӧ����ֹͣ�����ļ�
	IPANEL_APP_DOWNLOADER_SUCCESS,//֪ͨ������Ӧ��ĳ���ļ����سɹ�
	IPANEL_APP_DOWNLOADER_FAILED,//֪ͨ������Ӧ��ĳ���ļ�����ʧ��
	IPANEL_APP_DOWNLOADER_UNDEFINED
};

struct FileInfo{
	char *name;
	char *buf;
	int32 len;
};

int32 ipanel_app_process_event(int msg, unsigned int p1, unsigned int p2);
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/* ������ͨ���������ã��ͻ�ֻG�Ҫʵ���ⲿ�ֽӿڣ������Ľӿ�IPANEL���ڿ��Gʵ�� */
enum {
	IPANEL_DOWNLOADER_UNDEF,
	IPANEL_DOWNLOADER_SUCCESS,
	IPANEL_DOWNLOADER_TIMEOUT,
	IPANEL_DOWNLOADER_NOT_FOUND,
	IPANEL_DOWNLOADER_SAVE_FAILED,
	IPANEL_DOWNLOADER_HALT
};

typedef INT32_T (*IPANEL_DOWNLOADER_NOTIFY)(VOID *tag, CONST CHAR_T *uri, INT32_T status);

INT32_T ipanel_download_file(VOID *handle, CONST CHAR_T *uri, UINT32_T timeout, IPANEL_DOWNLOADER_NOTIFY func, VOID *tag);
//////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif /* IPANEL_APP_DOWNLOADER_H__ */

