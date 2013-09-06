/******************************************************************************/
/*    Copyright (c) 2005 iPanel Technologies, Ltd.                     */
/*    All rights reserved. You are not allowed to copy or distribute          */
/*    the code without permission.                                            */
/*    This is the demo implenment of the Porting APIs needed by iPanel        */
/*    MiddleWare.                                                             */
/*    Maybe you should modify it accorrding to Platform.                      */
/*                                                                            */
/*    $author Zouxianyun 2005/04/28                                           */
/******************************************************************************/

#ifndef _IPANEL_MIDDLEWARE_PORTING_OSFILE_API_FUNCTOTYPE_H_
#define _IPANEL_MIDDLEWARE_PORTING_OSFILE_API_FUNCTOTYPE_H_

#include "ipanel_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    IPANEL_FILE_SEEK_SET 		= 1, 
    IPANEL_FILE_SEEK_CUR 		= 2,
    IPANEL_FILE_SEEK_END 		= 3,
    IPANEL_FILE_TRUNCATE 		= 4,
    IPANEL_FILE_FLUSH 			= 5,
    IPANEL_FILE_GET_POSITION 	= 6,
    IPANEL_FILE_GET_CREATE_TIME = 7,
    IPANEL_FILE_GET_MODIFY_TIME = 8,
    IPANEL_FILE_GET_LENGTH		= 9
}IPANEL_FILE_IOCTL_e;

typedef enum
{
    IPANEL_DIR_RENAMCE_NODE 	= 1
}IPANEL_DIR_IOCTL_e;

typedef struct
{
    CHAR_T *oldname;
    CHAR_T *newname;
}IPANEL_RENAME_T;

typedef struct
{
    UINT32_T flag; 		//0 ��ʾ�Ǹ��ļ��ڵ㣬1��ʾ��Ŀ¼�ڵ�/
    CHAR_T 	 name[256]; //�ڵ����ַ������������ϼ�·��/
    VOID 	*handle; 	//�������ײ�ʵ���߹���������Ϣʹ��/
}IPANEL_DIR;

typedef struct
{
 const char *szfilename; //�ļ���
 IPANEL_UINT64_T ilength;//�ļ�����
}IPANEL_FILELENGTH_T;


/* open a OS file */
UINT32_T ipanel_porting_file_open(CONST CHAR_T *filename, CONST CHAR_T *mode);

/* read data from a OS file */
INT32_T ipanel_porting_file_read(UINT32_T fd, BYTE_T *buffer, INT32_T nbytes);

/* write data to a OS file */
INT32_T ipanel_porting_file_write(UINT32_T fd, CONST BYTE_T *buffer, INT32_T nbytes);

/* close a OS file */
INT32_T ipanel_porting_file_close(UINT32_T fd);

/* delete a OS file, or folder(last char of name shall be '/' or '\') */
INT32_T ipanel_porting_file_delete(CONST CHAR_T *name);

INT32_T ipanel_porting_file_ioctl(UINT32_T fd, IPANEL_FILE_IOCTL_e op, VOID *arg);

/*create a directory*/
INT32_T ipanel_porting_dir_create(CONST CHAR_T *dirname);

/*delete a directory*/
INT32_T ipanel_porting_dir_remove(CONST CHAR_T *dirname);

/*open a directory for searching */
UINT32_T ipanel_porting_dir_open(CONST CHAR_T *dirname);

/*close a directory */
INT32_T ipanel_porting_dir_close(UINT32_T dd);

/*reset position to the start of a directory */
INT32_T ipanel_porting_dir_rewind(UINT32_T dd);

/*read one entry from a directory */
INT32_T ipanel_porting_dir_read(UINT32_T dd, IPANEL_DIR *pdir);

INT32_T ipanel_porting_dir_ioctl(IPANEL_DIR_IOCTL_e op,VOID *arg);

#ifdef __cplusplus
}
#endif

#endif//_IPANEL_MIDDLEWARE_PORTING_OSFILE_API_FUNCTOTYPE_H_
