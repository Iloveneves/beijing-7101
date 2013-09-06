/******************************************************************************/
/*    Copyright (c) 2005 iPanel Technologies, Ltd.                     */
/*    All rights reserved. You are not allowed to copy or distribute          */
/*    the code without permission.                                            */
/*    This is the demo implenment of the Porting APIs needed by iPanel        */
/*    MiddleWare.                                                             */
/*    Maybe you should modify it accorrding to Platform.                      */
/*                                                                            */
/*    $author huzh 2007/11/22                                                 */
/******************************************************************************/

#ifndef _IPANEL_MIDDLEWARE_PORTING_UPGRADE_API_FUNCTOTYPE_H_
#define _IPANEL_MIDDLEWARE_PORTING_UPGRADE_API_FUNCTOTYPE_H_

#include "ipanel_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct loaderinfo
{
	unsigned int Frequency;	// example: 2990000  
	unsigned int Symbolrate;// example: 69000
 	unsigned int Modulation;// example: "1"(16-QAM)  "2"(32-QAM)"3"(64-QAM)     "4"(128-QAM)  "5"(256-QAM)unsigned int Pid;
 	// example: 0x137
	//������Щ����ֻ����ͨ����������ʱipanel�Żḳֵ������������
	unsigned int  TableId;			// ������Ҫ��ֵ������Ϊ0
	unsigned char Version;			// ��Ҫ����������İ汾(һ�㲻��)     
	unsigned char Upgrade_Type;		// ��ʾ����:0 ; ǿ�Ƹ���:1
	unsigned char Upgrade_flag;		// currently no use,pass 1
	unsigned char Software_Type;	// ������Ҫ������������ͣ�Ӧ�����:0 ; ϵͳ���:1
}IPANEL_LOADER_INFO;

/*
�������յ���������������������������ģ�顣
return��
-1 	�C �������ݴ�����Ǳ��豸�����������
 0 	- �汾��ȣ���������;
 1 	- ���°汾���ֶ�����;
 2 	- ���°汾��ǿ��������
*/
INT32_T ipanel_upgrade_check(CHAR_T *des, UINT32_T len);

/*֪ͨ����ִ��ģ��ִ������������*/
INT32_T ipanel_upgrade_start(CHAR_T *des, UINT32_T len);

//ipanel�ṩ��������ʹ�õĺ�������������ʹ�øú���ʱ������ipanel���õ�ipanel_upgrade_start()��������
INT32_T ipanel_upgrade_getparams(UINT32_T onid, UINT32_T tsid, UINT32_T srvid, UINT32_T comp_tag, IPANEL_LOADER_INFO *info);

#ifdef __cplusplus
}
#endif

#endif // _IPANEL_MIDDLEWARE_PORTING_UPGRADE_API_FUNCTOTYPE_H_
