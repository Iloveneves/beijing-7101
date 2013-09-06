#ifndef _BR_FILE__H_
#define _BR_FILE__H_
#include "stdio.h"

typedef struct _linkedobj_struct
{
	unsigned long int linked_obj_id;
	unsigned char* linked_obj_addr;/*��ǰ�ļ����׵�ַ*/
	unsigned char* tmpbuf;        /*��ǰ���ļ�ָ��buffer*/
	unsigned char obj_type;
	unsigned char cur_version;/*������ǰ汾��*/
	unsigned char section;/*ʵ������obj section number�� ���Ǳ�ʾ�ڼ���SECTION*/
	unsigned char last_section;/*���һ��section number */
	unsigned long int CRC32;/*crc У��*/
	unsigned long int obj_size; /*��ǰ�ļ��Ĵ�С*/
	unsigned char compressed_flag;
	unsigned long int compressed_size;
	int received;
	struct _linkedobj_struct * next;
}z_linkedobj_struct;

#if 0
#define graph_read fread
#define graph_open fopen
#define graph_seek fseek
#define graph_close fclose
#define graph_file	FILE
#define graph_setmode setmode
#define graph_fdopen	fdopen
#define graph_setvbuf	setvbuf
#define graph_bopen	open
#define graph_getc		getc
#define graph_bclose	close
#define graph_ungetc	ungetc
#else
#define graph_read chz_fread
#define graph_open chz_fopen
#define graph_seek chz_fseek
#define graph_close chz_fclose
#define graph_file	z_linkedobj_struct
#define graph_setmode chz_setmode
#define graph_fdopen	chz_fdopen
#define graph_setvbuf	chz_setvbuf
#define graph_bopen	chz_open
#define graph_getc		chz_getc
#define graph_bclose	chz_close
#define graph_ungetc	chz_ungetc


/*
���ļ��ĵ�ǰλ�ÿ�ʼ�ж�ȡsize*count���ֽڵ�����
buf ��Ŷ������ݵ�ָ��
size ÿ�����ݵ�λ���ֽ���
count ��������ݵ�λ����
*/
int chz_fread(void *buf, int size, int count, graph_file *fp);

/*
��һ���ļ�
filename �ļ�����
*/
graph_file *chz_fopen(char *filename, char *mode,int ri_len);

/*
�趨�ļ�����ָʾ��λ��
p �ļ�ָ��
offset �����origin�涨��ƫ��λ����
origin ָ���ƶ�����ʼλ�ã�������Ϊ�������������     
SEEK_SET �ļ���ʼλ��              
SEEK_CUR �ļ���ǰλ��               
SEEK_END �ļ�����λ��
*/
int chz_fseek(graph_file *fp, long offset, int origin);

int chz_fclose(graph_file *fp);
int chz_close(int handle);

/*��������һ���Ѵ��ļ��Ĳ���ģʽ*/
int chz_setmode(int handle, int amode);


/*�õ�һ����UNIX�ļ����handle�ȼ۵���ʽ�ļ�ָ��*/
graph_file *chz_fdopen(int handle, char *mode);


/*��ָ���ļ�fpָ������������*/
int chz_setvbuf(graph_file *fp, char *buf, int type, size_t size);


/*�Ը��ַ�ʽ���ļ�*/
int chz_open(const char *path, int mode,int ri_len);


/*���ļ��ж���һ���ַ�*/
int chz_getc(graph_file *fp);

/*���ַ�c�ŵ��ļ���fp���ײ�*/
int chz_ungetc(int c, graph_file *fp);
#endif
#endif


