#include "eis_api_define.h"
#include "eis_api_upgrade.h"
#include "eis_include\ipanel_upgrade.h"
#include "..\main\initterm.h"
#include "..\dbase\vdbase.h"

extern semaphore_t  *gp_semNvmAccessLock;
extern BOX_INFO_STRUCT	*pstBoxInfo;    /* Box information from e2prom */
static U8 temp_serach_tag=0;
/* ��������������*/

static CDTTable CDTInfo;
static UpdatePara CurUpdatePara;
static HL_INFOR  hl_infor; 


#define IPANEL_UPGRADE_DEBUG   
/*
��ȡ��������ڲ�����Ipanelʵ�֣������г��ҵ��á�
����˵����
���������
onid�����������ݰ��´�����Ƶ��Original_network_id ֵ
tsid���������Ƶ����Ϣ,QAM ��Ϣ,�ͷ�������Ϣ��NIT���ID��
srvid��������������ID����ֵӦΪ�����������ݰ���PMT �ڸ�Ƶ���Ľ�Ŀ�ţ�һ�㶨��һ��Service_id��Ӧһ�����̡�
comp_tag�����屾�汾��Loader PID
info������������Ҫ�����Ľṹ��ָ�롣
�����������
��    �أ�
IPANEL_OK: ����ִ�гɹ�;
IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_upgrade_getparams(UINT32_T onid, UINT32_T tsid, UINT32_T srvid, UINT32_T comp_tag, IPANEL_LOADER_INFO *info);

/* ��Ҫ���ĵĲ����ͺ���*/
#if 1
#define MANUFACTURE_ID   01 /* ����ID*/
#define  NVM_APPLICATION_MEM_START  8100  /* EEPROM���������Ϣ����ƫ�Ƶ�ַ*/
#define  CPCB_START_ADDRESS        0xA0108000 /* LOADER��FLASH�����Ӳ��ID ��STB��Ϣ��ƫ�Ƶ�ַ*/

/************************������: CH_LDR_DispFPProgress ����*******************/
int CH_LDR_ReadSTBConfig( void )
{
	U32 check_sum = 0;

	memcpy( (U8 *)&hl_infor, (U8 *)(CPCB_START_ADDRESS),
		      sizeof(HL_INFOR ));
	
	check_sum  =  hl_infor.hardware_id;
	check_sum +=  hl_infor.loader_ver_id;
	check_sum +=  hl_infor.uiImageStartAddress;
	check_sum +=  hl_infor.uiImageSize;
	check_sum +=  hl_infor.ucSoftwareVerMajor;
	check_sum +=  hl_infor.ucSoftwareVerMinor;
	check_sum +=  hl_infor.iDateCode ;
	check_sum +=  hl_infor.iTimeCode ;
	check_sum +=  hl_infor.HL_CHECKSUM ;

	 if(check_sum == hl_infor.HL_INFOR_CHECKSUM )
        {  
	        eis_report("\n ver = %d hl_len:0x%x    <downloaded time:0x%x>\n ",      \
				hl_infor.ucSoftwareVerMinor,hl_infor.uiImageSize, 							hl_infor.iDateCode);
	        return 0;
        }
        else
        {
	          eis_report("\ncpcb err\n");
	        return -1;
        }
	 if(   hl_infor.uiImageSize  == 0)
	        return -1;
        return -1;
	
}


/*�õ���ǰӲ��ID*/
short CH_GetHardwareID(void)
{
	return hl_infor.hardware_id%100;
}
/*�õ���ǰSTB����汾*/
void GetSTBVersion(U8 *STBSoftVersionMajor,U8 *STBSoftVersionMinor)
{

  *STBSoftVersionMajor=hl_infor.ucSoftwareVerMinor /256;
  *STBSoftVersionMinor=hl_infor.ucSoftwareVerMinor %256;

}
#endif

SHORT CH_GetLoaderVersion(void)
{
 return hl_infor.loader_ver_id;
}

 int NVM_DRV_Write( U8 *pbBuffer, U32 wOffset, U32 wLength)
 {
	 int i;
	 U32 len;
	 ST_ErrorCode_t          ReturnError;

	semaphore_wait( gp_semNvmAccessLock );
	 for ( i=0;i<100; i++)
	 {
		 ReturnError= STE2P_Write( hE2PDevice,wOffset+NVM_APPLICATION_MEM_START ,pbBuffer, wLength, &len);
		 if(ReturnError==ST_NO_ERROR && len == wLength)
		 {
		 	semaphore_signal ( gp_semNvmAccessLock );
			 return 0;
		 }
	 }
	 semaphore_signal ( gp_semNvmAccessLock );
	 return -1;
 }

void CH_LDR_WriteReservedFreq(U32 rST_Freq )
{
     U8  ST_TmpBuf[10];
     U8  ST_CheckSum  = 0;

     ST_TmpBuf[0] = (rST_Freq & 0xff);
     ST_TmpBuf[1] = ((rST_Freq >> 8 ) & 0xff);
     ST_TmpBuf[2] = ((rST_Freq >> 16) & 0xff);
     ST_TmpBuf[3] = ((rST_Freq >> 24) & 0xff);
     NVM_DRV_Write(ST_TmpBuf,36,4);
	 
     ST_CheckSum  =  ST_TmpBuf[0];
     ST_CheckSum +=  ST_TmpBuf[1];
     ST_CheckSum +=  ST_TmpBuf[2];	
     ST_CheckSum +=  ST_TmpBuf[3];	 
     NVM_DRV_Write(&ST_CheckSum,40,1);
	 
}

/*����LOADER���ղ���*/
 int eis_write_LoaderPara()
 {
	 U8 tmp[10];
	 U8  check_sum  = 0;
	 int enable_OTA_download;
	 /*4 bytes*/
	int MainFre=CDTInfo.download_frequency/**10 20060320 del*/;
	int AuxFre=CDTInfo.abiDateStamp/**10 20060320 del*/;
	int Qam=CDTInfo.download_modulation;
	int Symbol=CurUpdatePara.Sym_Rate;

	CH_LDR_WriteReservedFreq(AuxFre*10);/*��KΪ��λ*/

	 
	 tmp[0] = (CurUpdatePara.Freq & 0xff);
	 tmp[1] = ((CurUpdatePara.Freq >> 8 ) & 0xff);
	 tmp[2] = ((CurUpdatePara.Freq >> 16) & 0xff);
	 tmp[3] = ((CurUpdatePara.Freq >> 24) & 0xff);

	 NVM_DRV_Write(tmp,20,4);
	 check_sum += tmp[0];
	 check_sum += tmp[1];
	 check_sum += tmp[2];
	 check_sum += tmp[3];
	 /*4 ,8*/

	 tmp[0] = (CurUpdatePara.Sym_Rate & 0xff);
	 tmp[1] = ((CurUpdatePara.Sym_Rate >> 8) & 0xff);
	 tmp[2] = ((CurUpdatePara.Sym_Rate >> 16) & 0xff);
	 tmp[3] = ((CurUpdatePara.Sym_Rate >> 24) & 0xff);
	 NVM_DRV_Write(tmp,24,4);
	 
	 check_sum += tmp[0];
	 check_sum += tmp[1];
	 check_sum += tmp[2];
	 check_sum += tmp[3];
	 /*1, 9*/

	 tmp[0] = CurUpdatePara.Pol;
	 NVM_DRV_Write(tmp,28,1);
	 check_sum += tmp[0];
	 /*2 ,11*/

	 tmp[1] = (CurUpdatePara.pid & 0xff);
	 tmp[0] = ((CurUpdatePara.pid >> 8) & 0xff);
	 NVM_DRV_Write(tmp,29,2);
	 check_sum += tmp[0];
	 check_sum += tmp[1];
	 
	 /*1,12*/
	 tmp[0] = CurUpdatePara.tableid;
	 NVM_DRV_Write(tmp,31,1);
	 check_sum += tmp[0];
	 /*1,13*/
	 tmp[0] = 0xff;
	 NVM_DRV_Write(tmp,32,1);
	 check_sum += tmp[0];
	 /*1,14*/
	 enable_OTA_download = 1;
	 tmp[0] = enable_OTA_download;
	 NVM_DRV_Write(tmp,33,1);
	 check_sum += tmp[0];
	 /*1,15*/

	 tmp[0] = CurUpdatePara.flash_corrupted ;
	 NVM_DRV_Write(tmp,34,1);
	 check_sum += tmp[0];
	 
	 NVM_DRV_Write(&check_sum,35,1);
 }

/*�ж��Ƿ���Ҫ�������*/
boolean eis_NeedUpdateSoftware()
{
	U8 STBSoftVersionMajor;
	U8 STBSoftVersionMinor;
	boolean Update=false;

	/*return true;only for test */
#ifdef IPANEL_UPGRADE_DEBUG						
	GetSTBVersion(&STBSoftVersionMajor,&STBSoftVersionMinor);
	eis_report("\n changhong loader STBSoftVersionMajor = %d,STBSoftVersionMinor=%d",STBSoftVersionMajor,STBSoftVersionMinor);  
	eis_report("\n  ipanel abiSoftVersionMajor=%d  abiSoftVersionMinor = %d\n", CDTInfo.abiSoftVersionMajor,CDTInfo.abiSoftVersionMinor);
#endif					

	switch(CDTInfo.Download_type)
	{		
	case UPDATE_REPLACE_LOWER:  /*�滻��������汾���͵����(��ͬ��manufacturer_id��Hardware_Id)*/
		GetSTBVersion(&STBSoftVersionMajor,&STBSoftVersionMinor);
		if(STBSoftVersionMajor<CDTInfo.abiSoftVersionMajor)
		 {
			Update=true;
		 }
		 else if(STBSoftVersionMajor==CDTInfo.abiSoftVersionMajor)
		 {
			 if(STBSoftVersionMinor<CDTInfo.abiSoftVersionMinor)
			 {
				 Update=true;
			 }
		 }
		 break;
	case UPDATE_REPLACE_ALL:      /*�滻���а汾���(��ͬ��manufacturer_id��Hardware_Id)*/
		GetSTBVersion(&STBSoftVersionMajor,&STBSoftVersionMinor);
		 if(STBSoftVersionMajor!=CDTInfo.abiSoftVersionMajor||
			STBSoftVersionMinor!=CDTInfo.abiSoftVersionMinor)
		 Update=true;
		 break;
	case UPDATE_REPLACE_TARGET:    /*�滻Ŀ��汾���(��ͬ��manufacturer_id��Hardware_Id)*/

		 GetSTBVersion(&STBSoftVersionMajor,&STBSoftVersionMinor);
		 if(STBSoftVersionMajor!=CDTInfo.abiSoftVersionMajor||
			STBSoftVersionMinor!=CDTInfo.abiSoftVersionMinor)
		 {
			 if(STBSoftVersionMajor==CDTInfo.Target_main_software&&STBSoftVersionMinor==CDTInfo.Target_minor_software)
			 {
				 Update=true;
			 }
		 }
		 break;

	}
	return       Update;
	
}
/*
����˵����
�������յ���������������������������ģ�顣
����˵����
���������
des��������������ַ
len����������������
���������
��
�� �أ�
-1 �C �������ݴ�����Ǳ��豸�����������
0 - �汾��ȣ���������;
1 - ���°汾���ֶ�����;
2 - ���°汾��ǿ��������
*/
INT32_T ipanel_upgrade_check(CHAR_T *des, UINT32_T len) 
{
	  unsigned char  *pucSectionData = NULL;
	  unsigned char    linkage_type,descriptor_tag,force_flag;
	int	  Descriptor_length,download_loop_length,Length;
	U16   transport_stream_id,original_network_id;
	U32   start_STB_id,end_STB_id;
	  unsigned char temp,temp0,temp1,temp2,temp3;
	boolean upgrade_type=false;
	boolean manufacturer_id_check=false;
	boolean hardware_ver_check=false;
#ifdef IPANEL_UPGRADE_DEBUG				
	eis_report("\n ipanel_upgrade_check start \n");  
#endif
	pucSectionData=(  unsigned char  *)des;
	if(pucSectionData[0]!=0x81)
	{
#ifdef IPANEL_UPGRADE_DEBUG				
		eis_report("\n ipanel_upgrade_check error descriptor tag  != 0x81  \n");  
#endif
		return -1;
	}
	Descriptor_length=pucSectionData[1];   				
	pucSectionData  = pucSectionData + 2;
	
	CDTInfo.manufacturer_id = *(pucSectionData ); /*manufacturer_id*/				
	
	pucSectionData  = pucSectionData + 1;
	CDTInfo.hardware_id =  *(pucSectionData );   /*Hardware_Id*/
				
#ifdef IPANEL_UPGRADE_DEBUG	
	eis_report("\n changhong manufacturer_id=%d,hardware_id = %d",MANUFACTURE_ID,CH_GetHardwareID());  
	eis_report("\n ipanel manufacturer_id=%d,hardware_id = %d\n",CDTInfo.manufacturer_id,CDTInfo.hardware_id);  
#endif

	if( (CDTInfo.manufacturer_id == MANUFACTURE_ID))
	{ /*�����жϹ���ID�����ǹ���ID���*/
		manufacturer_id_check = true;
	}
	else
	{
		manufacturer_id_check = false;
	}
	
	if( (CDTInfo.hardware_id == CH_GetHardwareID()))
	{ /*�����ж�Ӳ���汾������Ӳ���汾���*/
		hardware_ver_check = true;
	}
	else
	{
		hardware_ver_check = false;
	}
	
	if ( manufacturer_id_check && hardware_ver_check )
	{
		
		pucSectionData  = pucSectionData + 1;
		temp =    *pucSectionData ;
		pucSectionData  = pucSectionData + 1;
		CDTInfo.usMagicNo = temp << 8|(*pucSectionData );     /*usMagicNo*/
#ifdef IPANEL_UPGRADE_DEBUG						
	eis_report("\n changhong loader ver = %d",CH_GetLoaderVersion());  
	eis_report("\n  ipanel loader ver = %d\n", CDTInfo.usMagicNo);
#endif					
		
		pucSectionData  = pucSectionData + 1;
		CDTInfo.abiSoftVersionMajor = *(pucSectionData );    /*SoftVersionMajor*/
#ifdef IPANEL_UPGRADE_DEBUG						
		//eis_report("\n abiSoftVersionMajor = %d\n", CDTInfo.abiSoftVersionMajor);
#endif					
					
		pucSectionData  = pucSectionData + 1;										
		CDTInfo.abiSoftVersionMinor = *(pucSectionData );  /*SoftVersionMinor*/
#ifdef IPANEL_UPGRADE_DEBUG
		//eis_report("abiSoftVersionMinor = %d\n", CDTInfo.abiSoftVersionMinor);
#endif
		pucSectionData  = pucSectionData + 1;
		temp0 =    *pucSectionData ;
		pucSectionData  = pucSectionData + 1;
		temp1 =    *pucSectionData ;
		pucSectionData  = pucSectionData + 1;
		temp2 =    *pucSectionData ;
		pucSectionData  = pucSectionData + 1;
		temp3 =    *pucSectionData ;
					
		CDTInfo.abiDateStamp = \
			( temp0 <<16)|(temp1 <<8)|(temp2 ); /*20060226 add ��Ϊ����Ƶ��*/
		pucSectionData  = pucSectionData + 1;
		temp0 =    *pucSectionData ;
		pucSectionData  = pucSectionData + 1;
		temp1 =    *pucSectionData ;
		pucSectionData  = pucSectionData + 1;
		temp2 =    *pucSectionData ;
		pucSectionData  = pucSectionData + 1;
		temp3 =    *pucSectionData ;
		CDTInfo.abiTimeStamp = \
			( temp0 <<24)|(temp1 <<16)|(temp2 <<8)|temp3;/*abiTimeStamp*/
					
		pucSectionData  = pucSectionData + 1;
		temp0 =    *pucSectionData ;
		pucSectionData  = pucSectionData + 1;
		temp1 =    *pucSectionData ;
		pucSectionData  = pucSectionData + 1;
		temp2 =    *pucSectionData ;
		pucSectionData  = pucSectionData + 1;
		temp3 =    *pucSectionData ;
		CDTInfo.iImageCRC = \
			( temp0 <<24)|(temp1 <<16)|(temp2 <<8)|temp3;/*Imager crc*/
		
		pucSectionData  = pucSectionData + 1;
		
		CDTInfo.iImageSize = *(pucSectionData );         /*eserved*/
					
		pucSectionData  = pucSectionData + 1;
		temp0 =    *pucSectionData ;
		
		pucSectionData  = pucSectionData + 1;
		temp1 =    *pucSectionData ;
		pucSectionData  = pucSectionData + 1;
		
		CurUpdatePara.Freq=CDTInfo.download_frequency = (temp0 <<16)+(temp1 <<8)+ (*(pucSectionData ));/*download_frequency*/
               CurUpdatePara.Freq=CDTInfo.download_frequency=CDTInfo.download_frequency*10;
#ifdef IPANEL_UPGRADE_DEBUG					
		eis_report("frequency = %d\n", CDTInfo.download_frequency );
#endif
					
		pucSectionData  = pucSectionData + 1;
		temp =    *pucSectionData ;
		pucSectionData  = pucSectionData + 1;
		
		CurUpdatePara.Sym_Rate=CDTInfo.download_symbol = temp <<8|*(pucSectionData );                 /*Download_Symb*/
		
		pucSectionData  = pucSectionData + 1;
		
		CurUpdatePara.Pol=CDTInfo.download_modulation = *(pucSectionData );                     /*download_modulation*/
#ifdef IPANEL_UPGRADE_DEBUG
		eis_report("download_modulation = %d\n", CDTInfo.download_modulation);
#endif					
		pucSectionData  = pucSectionData + 1;
		temp =    *pucSectionData ;
		pucSectionData  = pucSectionData + 1;
		CurUpdatePara.pid=CDTInfo.download_pid = temp <<8|*(pucSectionData );                  /*download_pid*/
#ifdef IPANEL_UPGRADE_DEBUG					
		eis_report("download_pid = %d\n", CDTInfo.download_pid  );
#endif
					
		pucSectionData  = pucSectionData + 1;
		CDTInfo.Header_table_id = *(pucSectionData );                 /*Header_table_id*/
		
		pucSectionData  = pucSectionData + 1;
		CurUpdatePara.tableid=CDTInfo.mage_table_id = *(pucSectionData );	                 /*mage_table_id*/		
		
		pucSectionData  = pucSectionData + 1;
		CDTInfo.Target_main_software = *(pucSectionData );           /*Target_main_software*/
					
#ifdef IPANEL_UPGRADE_DEBUG					
		eis_report("Target_main_software = %d\n",CDTInfo.Target_main_software  );
#endif
					
		pucSectionData  = pucSectionData + 1;
		CDTInfo.Target_minor_software = *(pucSectionData );	          /*Target_minor_software*/
					
#ifdef IPANEL_UPGRADE_DEBUG					
		eis_report("Target_minor_software = %d\n",CDTInfo.Target_minor_software  );
#endif
					
		pucSectionData  = pucSectionData + 3;
       	//pstBoxInfo->serach_tag = ( (*(pucSectionData )) & 0x01);	             /*serach_tag*/
		temp_serach_tag= ( (*(pucSectionData )) & 0x01);

		pucSectionData  = pucSectionData + 1;
#ifdef IPANEL_UPGRADE_DEBUG					
		eis_report("serach_tag = %d\n",temp_serach_tag  );
#endif
		
	  /*0	�滻��������汾���͵����(��ͬ��manufacturer_id��Hardware_Id)
		1	�滻���а汾���(��ͬ��manufacturer_id��Hardware_Id)
		2	�滻Ŀ��汾���(��ͬ��manufacturer_id��Hardware_Id)
		*/
       	CDTInfo.Download_type = *(pucSectionData );	             /*Download_type*/
		CurUpdatePara.flash_corrupted=0x34;

#ifdef IPANEL_UPGRADE_DEBUG					
		eis_report("Download_type = %d\n",CDTInfo.Download_type  );
#endif
		if(( CDTInfo.usMagicNo == 0) ||((CDTInfo.usMagicNo)==CH_GetLoaderVersion()))
		{	/*�����ж�LOADER�汾������LOADER�汾���*/
			if(CDTInfo.abiSoftVersionMajor!=0)
			{ /*��Ҫ�ж�����汾*/
		              upgrade_type=eis_NeedUpdateSoftware();
			}
			else
			{
				upgrade_type=true;
			}
		}
		else
		{
			eis_report("\n loader ver check error \n");  
		}

		eis_report("\n SoftVersion=0 return %d \n",upgrade_type);  
		/*2009014 change*/
		if(upgrade_type)
		{
			pstBoxInfo->ucSoftwareVerMajor=CDTInfo.abiSoftVersionMajor ;
			pstBoxInfo->ucSoftwareVerMinor=CDTInfo.abiSoftVersionMinor;
		}
		return upgrade_type;
	}
	else
	{
		eis_report("\n LoaderVersion check error \n");  
		return -1;
	}
}
/*
����˵����
֪ͨ����ִ��ģ��ִ������������
����˵����
���������
des��������������ַ
len����������������
���������
��
�� �أ�
IPANEL_OK: ����ִ�гɹ�;
IPANEL_ERR: ����ִ��ʧ�ܡ�
*/
INT32_T ipanel_upgrade_start(CHAR_T *des, UINT32_T len) 
{
	UINT32_T onid;
	UINT32_T tsid;
	UINT32_T srvid;
	UINT32_T comp_tag;
	IPANEL_LOADER_INFO info;
	U8 errno;

	//eis_report(0,"\n enter ipanel_upgrade_start\n");
	
	onid=tsid=srvid=comp_tag=0xffffffff;
	errno=ipanel_upgrade_getparams( onid,  tsid,  srvid,  comp_tag,&info);	
	if(IPANEL_OK==errno)
	{
		CurUpdatePara.Freq=CDTInfo.download_frequency = info.Frequency/10;/*download_frequency*/
		CurUpdatePara.Sym_Rate=CDTInfo.download_symbol = info.Symbolrate/10; /*Download_Symb*/
		CurUpdatePara.Pol=CDTInfo.download_modulation =  info.Modulation;   /*download_modulation*/
		
		eis_write_LoaderPara();
		pstBoxInfo->upgrade=1; /*��¼Ϊ����״̬*/
       	pstBoxInfo->serach_tag = temp_serach_tag;	             /*serach_tag*/

		CH_NVMUpdate(idNvmBoxInfoId); 
		CH_Restart();
		return IPANEL_OK;
	}
	else
	{
		return IPANEL_ERR;
	}
	
	//eis_report(0,"\n exit ipanel_upgrade_start\n");
}

