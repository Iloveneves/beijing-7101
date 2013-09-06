#include "br_file.h"
#include "graphcom.h"
#if 1
graph_file   g_graphicFile;

/*
���ļ��ĵ�ǰλ�ÿ�ʼ�ж�ȡsize*count���ֽڵ�����
buf ��Ŷ������ݵ�ָ��
size ÿ�����ݵ�λ���ֽ���
count ��������ݵ�λ����
*/
int chz_fread(void *buf, int size, int count, graph_file *fp)
{
 
	int buf_long = EOF;
	if(fp == NULL)
		return 0;
	buf_long = CH_MIN(size * count, (fp->obj_size  + fp->linked_obj_addr- fp->tmpbuf));
	if(buf_long > 0)
	{
		memcpy(buf, fp->tmpbuf, buf_long);
		fp->tmpbuf += buf_long;
	}
	return buf_long;
 
}

/*
��һ���ļ�
filename �ļ�����
*/
graph_file *chz_fopen(char *filename, char *mode, int ri_Len)
{
#if 0
	return 
	 	(graph_file *)chz_find_obj_id(filename);
#else
   g_graphicFile.linked_obj_addr = g_graphicFile.tmpbuf=(U8*)filename;
if(ri_Len != 0)
   g_graphicFile.obj_size = ri_Len;
   return &g_graphicFile;
#endif
}

/*
�趨�ļ�����ָʾ��λ��
p �ļ�ָ��
offset �����origin�涨��ƫ��λ����
origin ָ���ƶ�����ʼλ�ã�������Ϊ�������������     
SEEK_SET �ļ���ʼλ��              
SEEK_CUR �ļ���ǰλ��               
SEEK_END �ļ�����λ��
*/
int chz_fseek(graph_file *fp, long offset, int origin)
{
 	int len;
	if(fp == NULL)
		return 0;
	switch(origin)
	{
		case SEEK_SET:
			fp->tmpbuf = fp->linked_obj_addr + offset;
			break;
		case SEEK_CUR:
			fp->tmpbuf = fp->tmpbuf + offset;
			break;
		case SEEK_END:
			fp->tmpbuf = fp->linked_obj_addr + fp->obj_size + offset;
			break;
		default:
			break;
			
	}
	return (fp->tmpbuf - fp->linked_obj_addr);
 }

int chz_fclose(graph_file *fp)
{
	if(fp == NULL)
		return -1;
	fp->tmpbuf = fp->linked_obj_addr;
	return 0;
}
int chz_close(int handle)
{
	return 
		chz_fclose((graph_file *)handle);
}

/*��������һ���Ѵ��ļ��Ĳ���ģʽ*/
int chz_setmode(int handle, int amode)
{
	return 0;
}


/*�õ�һ����UNIX�ļ����handle�ȼ۵���ʽ�ļ�ָ��*/
graph_file *chz_fdopen(int handle, char *mode)
{
	return (graph_file *)handle;
}


/*��ָ���ļ�fpָ������������*/
int chz_setvbuf(graph_file *fp, char *buf, int type, size_t size)
{
	return 0;
}


/*�Ը��ַ�ʽ���ļ�*/
int chz_open(const char *path,int mode,int ri_len)
{
	graph_file *fp;
	fp = chz_fopen((char*)path, NULL,ri_len);
	if(fp)
		return (int)fp;
	else
		return -1;
}


/*���ļ��ж���һ���ַ�*/
int chz_getc(graph_file *fp)
{
	char c = EOF;
	if(fp && fp->tmpbuf < fp->linked_obj_addr + fp->obj_size)
	{
		c = *fp->tmpbuf;
		 fp->tmpbuf++;
	}
	 return (int)c;
}

/*���ַ�c�ŵ��ļ���fp���ײ�*/
int chz_ungetc(int c, graph_file *fp)
{
	if(fp)
	{
		*fp->linked_obj_addr = (char)(c & 0xff);
		return c;
	}
	return EOF;
}
#else
int close(int handle)
{
return 0;
}
int setmode(int handle, int amode)
{
return 0;
}
#endif

