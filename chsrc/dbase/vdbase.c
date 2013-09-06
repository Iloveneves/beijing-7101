/*
 * VDBASE.C			ver 0.1
 *
 * (c) Copyright SGS-THOMSON Microelectronics Limited 1996.
 *
 * Source file name : VDBASE.C
 * Author(s)	T.H.Thillai Rajan (tht.rajan@st.com)
 *
 * =======================
 * IMPROVEMENTS	THOUGHT	OF
 * =======================
 *
 * =====================
 * MODIFICATION	HISTORY:
 * =====================
 *
 * Date	       Modification										Initials
 * ----	       ------------										--------

 * 07.09.96		CREATED															THT
 * 10-11-2003   changed                                          zxg
 * 15-06-2004   changed                                          yxl
 * 2005-02-     changed   �γ�һ��NORMAL Release�����ݿ�         yxl
						  ��IPANEL HTML��BEIJING_DATABASE��������
						  �����BEIJING_DBASE��ʾGeHua Release �����ݿ�  ����ΪNORMAL Release�����ݿ�
	2005-08-07    changed   1)�޸�������ݽṹ���γ��ɺ����        yxl
							DVBC��DVBT���ݵ����ݿ�
						  2��ɾ�����ݿ����뱱���軪����Ҫ�����
						  �Ĳ��֣��γɳ���ͨ�ð����ݿ�
						  
*/
/*{{{  Defines and includes*/
#include <string.h>
#include "stddefs.h"  /* standard definitions			 */
/*#include "app_data.h"*/

#include "stcommon.h"
#include "st_nvm.h"
#include "sttbx.h"
#include "..\report\report.h"
#include "..\main\initterm.h"
#include "vdbase.h"
#include "stdlib.h"
#include "appltype.h"





#define	SOFTWARE_UPDATE_ID		"1.2.0"



 BOX_INFO_STRUCT	*pstBoxInfo;    /* Box information from e2prom */

 opaque_t     idNvmBoxInfoId = 0;         /* Box  identify from e2prom */
 
SHORT sCurTransponderId	= INVALID_LINK;        /*��ǰ��гƵ������*/
SHORT sCurProgramId		= INVALID_LINK;            /*��ǰ��������*/
semaphore_t *CurAcessSema;

SHORT CH_GetsCurTransponderId(void)
{
	SHORT Temp;
	semaphore_wait(CurAcessSema);
	Temp=sCurTransponderId;
    semaphore_signal(CurAcessSema);
	return Temp;
}

SHORT CH_GetsCurProgramId(void)
{ 
	SHORT Temp;
	semaphore_wait(CurAcessSema);
	Temp=sCurProgramId;
    semaphore_signal(CurAcessSema);
	return Temp;
}


SHORT CH_UpdatesCurTransponderId(SHORT Temp)
{
	semaphore_wait(CurAcessSema);
	sCurTransponderId=Temp;
	semaphore_signal(CurAcessSema);
}

SHORT CH_UpdatesCurProgramId(SHORT Temp)
{
	semaphore_wait(CurAcessSema);
	sCurProgramId=Temp;
	semaphore_signal(CurAcessSema);
}




/*{{{  DVBDbaseInit()*/
BOOLEAN	DVBDbaseInit(void)
{
  	
    CurAcessSema = semaphore_create_fifo ( 1 );
   	
    return FALSE;
}



