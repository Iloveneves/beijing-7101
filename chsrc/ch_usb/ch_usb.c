/*��USBʶ��ģ��*/
/*���ڱ��ļ�����OSD �����OSD��ʾ����
���鷢��Ϣ�������̣�����������ʾ��ʾ��*/

/*����ֻ��Ҫ֪����һ�����ӳɹ��󣬷��ʵ�һ���̵ĸ�·��Ϊ
gDiskMountPoint[0].DiskMountPoint,�ڶ���ΪgDiskMountPoint[1].DiskMountPoint*��
�Դ����ƣ�������װ汾֧��4������
����:���������ʵ�һ��������pvr �ļ����µ�readme.txt
sprintf(readpath,"%s%s","gDiskMountPoint[0].DiskMountPoint","/pvr/readme.txt");
����vfs_open(readpath,OSPLUS_O_WRONLY) �ڶ���������ʾ��д��ʽ��*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "stddefs.h"
#include "stdevice.h"
#include "stcommon.h"
#include <stpio.h>




/* osplus includes */
#include <osplus/registry.h>
#include <osplus/device.h>
#include <osplus/lvm.h>
#include <osplus/pci.h>
#include <osplus.h>
#include <usblink.h>


#include "osplus/vfs.h"
#include "osplus/e2fs.h"
#include "osplus/fat.h"

#include "ch_usb.h"
#include "..\main\initterm.h"
#include "..\key\keymap.h"
typedef struct OSPLUS_CallbackInfo_s
{
	device_event_t    Event;                                  /*�¼�����*/

	char             *DeviceName;                            /*�豸����*/

	char             *TypeName;                              /*�豸��������*/

	device_type_t     DeviceType;                         /*�豸����*/

}OSPLUS_CallbackInfo_t;



static message_queue_t *DeviceCallbackMsg_p = NULL;     
static semaphore_t*    Device_Exist_Protect = NULL;           /*�豸���ʻ����ź���*/


MOUNT_t gDiskMountPoint[MAXPARTITION];
int gPartitionNum=0;/*������*/
char gPVRMountRoot[32];/*PVR ROOT*/
int gCurMountIndex=-1;/*MOUNT ������*/
int gExt2=0;/* 0����EXT2��1��*/

int		USBINSERT = 0;


int CHMID_HDD_UMOUNTLVM(const char *mountname,const int patitionindex)
{
	
	if(gDiskMountPoint[patitionindex].gIsMountOk == 0)
	{
		do_report(0,"ysharp->>have umounted yet\n");
		return 1;
	}
	else
	{
		
		if(vfs_umount(mountname) != -1)
		{
			
			do_report(0,"ysharp->>the %d disk umount is ok\n",patitionindex);
			gDiskMountPoint[gCurMountIndex].gIsMountOk=0;
			gDiskMountPoint[gCurMountIndex].block=0;
			return 0;
		}
		else
		{
			do_report(0,"ysharp->>the %d disk umount is fail\n",patitionindex);
			return 1;
		}
	}
}

/*��һ���߼����ϴ���һ����·��
mountname: �ƶ�Ӳ��Ϊ��Ӧ��ע�������U��Ϊע���豸��
patitionindex:�߼�����̷������ֻ��һ���߼������ΪU�̣���0
type:OSD����Ϣ����ʾ��ʽ��0Ϊ��Ϣ���ݸ�������ʾ��1Ϊֱ�ӵ���д�ֺ�����ʾ*/
int CHMID_HDD_MOUNTLVM(char *mountname,int patitionindex,int type)
{
	vfs_info_t              DiskInfo;
	int testyy;
	
	char prmvertion[200]={0};
	char fsname[10]={0};
	char InfoName[2][MAX_LANGUAGE_ID][100]= 
	{
		"��ǰ������Ҫ��⣬���Ե�","Checking disk...",
		"Ӳ�̲���ʶ��ϵͳ��ֻ֧��FAT,FAT32,EXT2","Spot fail,only FAT,FAT32 and EXT2 can be supported"
	};
	semaphore_wait(Device_Exist_Protect);
	CHMID_HDD_UMOUNTLVM(gDiskMountPoint[patitionindex].DiskMountPoint,patitionindex);
	if(gExt2 == 1)
	{
		sprintf(fsname,"%s","E2FS");
	}
	else
	{
		sprintf(fsname,"%s","FAT");
	}
	if(vfs_sync_all() == -1)
	{
		do_report(0,"ysharp->>the disk may be exist dirty data\n");
	}
	if(vfs_mount(mountname, gDiskMountPoint[patitionindex].DiskMountPoint, fsname, OSPLUS_O_RDWR) == -1)
	{
		do_report(0,"current volume is need check...\n");
		if(type == 0)
		{
			CH_EnPopupDialog( 
								"Disk is checking,please waitting..."
								);
			/*OSD �����﷢����Ϣ����������ʾ[������Ҫ��⣬���Ե�]*/
			/*CH_USB_SendMessageToDisplay(USB_NEEDCHECK,patitionindex);*/
		}
		else
		{
			/*CH_DrawInfo(230, 160,310,80,(char *)InfoName[0][pstBoxInfo->abiLanguageInUse],3);*/
			/*CH_PVR_DrawInfoName(230, 160,310,80, (char *)InfoName[0][pstBoxInfo->abiLanguageInUse]);*/
		}
		#ifdef USB_DEBUG
		do_report(0,"ysharp->>vfs_mount(%s)=failed Errstr is %s  errcode is%d\n",mountname,strerror(errno), errno);
		do_report(0,"ysharp->>FAT32 mounting on [%s] @ mount point [%s] failed Err %d\n",mountname, gDiskMountPoint[patitionindex].DiskMountPoint, errno);
		#endif
		do_report(0,"ysharp->>Checking volumes now\n");
		
		if(gDiskMountPoint[gCurMountIndex].gIsMountOk == 1)
		{
			if(vfs_umount(gDiskMountPoint[gCurMountIndex].DiskMountPoint)  != -1)
			{
				gDiskMountPoint[gCurMountIndex].gIsMountOk =0;
			}
		}
		
		if (vfs_fsck(mountname, fsname) == -1)
		{
		#ifdef USB_DEBUG
			do_report(0,"ysharp->>vfs_fsck(%s)=failed Errstr is %s  errcode is%d\n",mountname,strerror(errno), errno);
		#endif	
			
		}
		if (vfs_mount(mountname,   gDiskMountPoint[patitionindex].DiskMountPoint, NULL, OSPLUS_O_RDWR) == -1)
		{
		#ifdef USB_DEBUG
			do_report(0,"ysharp->>FAT32 mounting on [%s] @ mount point [%s] failed Err %d\n", mountname,  gDiskMountPoint[patitionindex].DiskMountPoint, errno);
		#endif	
			do_report(0,"mount fail,current volume is unusable\n");
			if(type == 0)
			{
				/*CH_USB_SendMessageToDisplay(USB_CHECKERR,patitionindex);*/
			}
			else
			{
				/*CH_DrawInfo(230, 160,310,80,(char *)InfoName[1][pstBoxInfo->abiLanguageInUse],3);*/
			}
			
			semaphore_signal(Device_Exist_Protect);
			gDiskMountPoint[patitionindex].gIsMountOk=0;
			return 1;
		}
		else
		{
			if(vfs_fscntl(gDiskMountPoint[patitionindex].DiskMountPoint, VFS_FSCNTL_INFO_GET, (void *)&DiskInfo) == -1)
			{
			#ifdef USB_DEBUG
				do_report(0,"vfs_fscntl(%s)=failed Err %d\n", gDiskMountPoint[patitionindex].DiskMountPoint, errno);
			#endif
			}
			gDiskMountPoint[patitionindex].gIsMountOk=1;
			do_report(0,"ysharp->>FAT32 mounted on volume [%s] @ point [%s] FreeSpace[%u]\n\n", mountname, gDiskMountPoint[patitionindex].DiskMountPoint, DiskInfo.free_blocks);
			gDiskMountPoint[patitionindex].block= DiskInfo.free_blocks;
			gPartitionNum++;
			semaphore_signal(Device_Exist_Protect);
			gCurMountIndex=patitionindex;
			return 0;
			
			
		}
	}
	else
	{
		if(vfs_fscntl(gDiskMountPoint[patitionindex].DiskMountPoint, VFS_FSCNTL_INFO_GET, (void *)&DiskInfo) == -1)
		{
		#ifdef USB_DEBUG
			do_report(0,"vfs_fscntl(%s)=failed Err %d\n", gDiskMountPoint[patitionindex].DiskMountPoint, errno);
		#endif
		}
		gDiskMountPoint[patitionindex].gIsMountOk=1;
	         gDiskMountPoint[patitionindex].block= DiskInfo.free_blocks;
		
		 do_report(0,"ysharp->>FAT32  mounted on volume [%s] @ point [%s] FreeSpace[%u]\n\n", mountname,  gDiskMountPoint[patitionindex].DiskMountPoint, DiskInfo.free_blocks);
		 gPartitionNum++;
		 semaphore_signal(Device_Exist_Protect);	
		 gCurMountIndex=patitionindex;
		 return 0;
		
	}
}


int CHMID_HDD_MOUNTUSBFS(char *devicename)
{
	ST_ErrorCode_t resErrCode;
	extern int CHMID_HDD_MOUNTLVM(char *mn,int patitionindex,int type);
	resErrCode=CHMID_HDD_MOUNTLVM(devicename,0,0);
	if(resErrCode != 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
	
}

/* ��ʼ���ṹ*/
void CHMID_USB_InitMountStruct(void)
{
	int i=0;
	for(i=0;i<MAXPARTITION;i++)
	{
		memset(gDiskMountPoint[i].DiskMountPoint,0,32);
		memset(gDiskMountPoint[i].lvm,0,32);
		sprintf(gDiskMountPoint[i].DiskMountPoint,"%s%d","/dvr",i);
		gDiskMountPoint[i].gIsMountOk=0;
		gDiskMountPoint[i].block=0;
	}
	return ;
}

/*ж��ʱ�Ĵ���*/
int CHMID_HDD_Unmount(void)
{
	int err=0;
	
	if ((vfs_umount_all() )== -1)
	
	{
		err++;
	}
	memset(gDiskMountPoint,0,MAXPARTITION*sizeof(MOUNT_t));
	gPartitionNum=0;
	CHMID_USB_InitMountStruct();
	if(err == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}


/*��صײ���Ϣ�Ľ��̣���Ϣͨ���豸�ص������ϴ�*/
void  CH_DeviceDetect_Task(void *pointer)
{
	U8		FoundE2FSPartition = FALSE;
	U32		NumBlocks = 0;
	U32		i = 0, j = 0;
	int TryReset=0;
	int SupportFSType[MAX_SUPPORT_FS_TYPE]={0x1,0x11,0x4,0x6,0xe,0x14,0x16,0x1e,0xb,0xc,0x1b,0x1c,0x83};/*Ŀǰ֧�ֵ��ļ�ϵͳ���ͣ���OSPLUS�ĵ��л�ȡ*/
	BOOL    RetErr = FALSE;
	char	StorageDeviceName[256];
	char	UsbStorageDeviceName[256];
	int pass=0;
	clock_t     clkTime2ReceiveMail;
	lvm_partitions_t       *PartitionList_p;
	lvm_partition_t         Partition,*Partition_p;
	vfs_info_t              DiskInfo;
	int osdstatus=-1;
	//int k=-1;/*add for debug,remove when........*/
	OSPLUS_CallbackInfo_t   *msg;
	OSPLUS_CallbackInfo_t   ch_DviceInfo;
	CHMID_USB_InitMountStruct();
	while (1)
	{
	       osdstatus=CH_GetReSolutionRate();
		clkTime2ReceiveMail = time_plus ( time_now ( ), ( clock_t ) (ST_GetClocksPerSecondLow()*15) );	
		if ((msg = message_receive_timeout(DeviceCallbackMsg_p, &clkTime2ReceiveMail)) == NULL)
		{
			/*do_report(0,"\nthe masege is NULL\n"); sqzow20100725*/
			continue ;
		}
		
             do_report(0,"\nmsg->Event======%d",msg->Event);
		if (msg->Event == device_created)/*���豸����*/
		{
		    if(osdstatus==0)
		    	{
		    	    //do_report(0,"\nthe cishu1===========%d",k);/*add for debug*/
			    //k++;/*add for debug*/
		    	    do_report(0,"\nthe osdstatus==%d",osdstatus);
#if 0/*remove for debug*/
			    CH6_PopupDialog( GFXHandle, 
								CH6VPOSD.ViewPortHandle, 
								"���ƶ��豸����",
								0, 
								0,
								0 
								);
#endif
				
		    	}
			//makekeypress(VKEY_USB_MSG1);
		/*OSD   �������﷢��Ϣ����������ʾ [���ƶ��豸����]*/
			pass=0;/*reset value*/
			memcpy(&ch_DviceInfo,msg,sizeof(OSPLUS_CallbackInfo_t));	
			strcpy(StorageDeviceName, msg->DeviceName);/*�õ��豸����*/
			
			if(ch_DviceInfo.DeviceType == device_type(OSPLUS_DEVICE_TYPE_REMOVABLE_STORAGE))/*�豸����ΪU�̻�洢��*/
			{
				do_report(0,"Udisk is connecting...\n ");
				
			}
			else if(ch_DviceInfo.DeviceType  == device_type(OSPLUS_DEVICE_TYPE_KEYBOARD))/*�豸����ΪUSB���̣��Ժ��ͨ����Ӽ��̽��������������*/
			{
				do_report(0,"USB keybord is connecting...\n ");
			}
			else if(ch_DviceInfo.DeviceType  == device_type(OSPLUS_DEVICE_TYPE_MOUSE))/*�豸����Ϊ��꣬Ϊ������������OSD�ṩ����*/
			{
				do_report(0,"USB keybord is connecting...\n ");
			}
			else if(ch_DviceInfo.DeviceType == device_type(OSPLUS_DEVICE_TYPE_MODEM))/*è*/
			{
				do_report(0,"modem is connectting...\n");
			}
			else if(ch_DviceInfo.DeviceType == device_type(OSPLUS_DEVICE_TYPE_FIXED_STORAGE))/*�豸����Ϊ�ƶ�Ӳ�̻�̶�Ӳ��*/
			{
				do_report(0,"Hard disk is connecting...\n");
				
				PartitionList_p = lvm_partitions(StorageDeviceName);/*��ȡTABLE��Ϣ*/
				if(PartitionList_p != NULL)
				{
					for(i = 0; i < PartitionList_p->length; i++)
					{
						int typepass=0;
						int j=0;
						Partition_p = &PartitionList_p->partitions[i];	
						do_report(0,"Partition[%s]: Type[%d] RawType[%02X] @ LBA[%u] Size[%u] blocks\n",
									Partition_p->volume_name,
									Partition_p->type,
									Partition_p->raw_type,
									Partition_p->start_lba,
									Partition_p->size);
						for(j=0;j<MAX_SUPPORT_FS_TYPE;j++)
						{
							if(Partition_p->raw_type == SupportFSType[j])
							{
								typepass=1;
								if(Partition_p->raw_type == 0x83)/*�߼���EXT2��ʽ��*/
								{
									gExt2=1;
									do_report(0,"current volume is formatted by ext2\n");
								}
								else
								{
									gExt2=0;
									do_report(0,"current volume is formatted by fat\n");
								}
								break;
							}
						}
						if(typepass == 1)
						{
							sprintf(gDiskMountPoint[gPartitionNum].lvm,"%s",Partition_p->volume_name);
							gDiskMountPoint[gPartitionNum].block=Partition_p->size; /*1 block == 2k BYTE*/
							if(gPartitionNum >= 4)
							{
								break;
							}/* ֻͬʱ��4����·��*/
							if(CHMID_HDD_MOUNTLVM(Partition_p->volume_name,gPartitionNum,0) == 0)/*Ϊ��ǰ��MOUNT��·��*/
							{
								pass+=1;
								do_report(0,"mount is ok\n");
							}
							else
							{
								pass-=1;
                                					do_report(0,"mount is err\n");
							}
						}
						
					}
					free(PartitionList_p);
				}
				else /*û�з�����Ϣ����Ҫ������������ʽ����ͨ�����������Ӳ�̻����н���������Զ������͸�ʽ������
				���������豸��ͨ����������ֻ��OSD��������Ӧ��ʾ*/
				{
					do_report(0,"OSPLUS_DEVICE_TYPE_FIXED_STORAGE NO Partion infomation\n");
				}	
				if(pass >= 1)
				{

				
				if(osdstatus==0)

				     {
                                            do_report(0,"\nthe osdstatus==%d",osdstatus);
						  do_report(0,"\nshebei yijing keyi yong le");
						#if 1/*remove for debug*/
						 CH_EnPopupDialog( 
								"USB Device is ready"
								);
						 #endif	
					}
					//makekeypress(VKEY_USB_MSG2);
					/*OSD   �������﷢��Ϣ����������ʾ [�豸�ѿ���]*/			
					pass=0;/*reset value*/
				}
					
				else
				{

				    if(osdstatus==0)
				    	{
                                              do_report(0,"\nthe osdstatus==%d",osdstatus);
	                                   #if 0/*remove for debug*/
						   CH6_PopupDialog( GFXHandle, 
								CH6VPOSD.ViewPortHandle, 
								"�豸��ȡʧ��",
								0, 
								0,
								0 
								);
						#endif
				    	}
					//makekeypress(VKEY_USB_MSG3);
					/*OSD   �������﷢��Ϣ����������ʾ [�豸��ȡʧ��]*/
				}
				
			}
			else if(ch_DviceInfo.DeviceType == device_type(OSPLUS_DEVICE_TYPE_USB_UNKNOWN))
			{
				do_report(0,"ysharp->>unkown device connect\n");
			}
		
			else if(ch_DviceInfo.DeviceType == device_type(OSPLUS_DEVICE_TYPE_VOLUME))/*�߼��������ﴦ��U��*/
			{
				
				if(CHMID_HDD_MOUNTUSBFS(msg->DeviceName) == 0)
				{
					pass=1;
					do_report(0,"ysharp->>mount fs suc\n");
				}
				else
				{
					do_report(0,"ysharp->>mount fs err\n");
					pass=0;
				}
		if(osdstatus==0)
			{
				if(pass == 1)
				{
					CH_EnPopupDialog( 
								"USB detect success"
								);
					//makekeypress(VKEY_USB_MSG2);
					/*OSD   �������﷢��Ϣ����������ʾ [�豸�ѿ���]*/
					pass=0;/*reset value*/
				}
					
				else
				{
					CH_EnPopupDialog( 
								"USB detect Failed"
								);
					//makekeypress(VKEY_USB_MSG3);
					/*OSD   �������﷢��Ϣ����������ʾ [�豸��ȡʧ��]*/
				}
			}
				
			}	

		}
		
		else/*�豸ж�أ�ж��ʱ���߼���Ϊ��������ݲ������߼�����������豸��ж��*/
		{
			if(ch_DviceInfo.DeviceType == device_type(OSPLUS_DEVICE_TYPE_VOLUME))/*�����USB���豸*/
			{
				do_report(0,"Usb device remove\n");
				if(CHMID_HDD_Unmount() != 0)
				{

				    if(osdstatus==0)
				    	{
				    	           do_report(0,"\nthe osdstatus==%d",osdstatus);
						#if 1/*remove for debug*/
					           CH_EnPopupDialog( 
								"USB remove sucess"
								);
						#endif
				    	}
					//makekeypress(VKEY_USB_MSG4);
					/*OSD   �������﷢��Ϣ����������ʾ [�豸�Ƴ��쳣]*/
					do_report(0,"ysharp->>unmount err\n");
				}
				else
				{
				     if(osdstatus==0)
				     	{
				     	         do_report(0,"\nthe osdstatus==%d",osdstatus);
						#if 1/*remove for debug*/
					         CH_EnPopupDialog(
								"USB remove sucess"
								);
						#endif
				     	}
					//makekeypress(VKEY_USB_MSG5);
					/*OSD   �������﷢��Ϣ����������ʾ [�豸�Ƴ�]*/
					do_report(0,"ysharp->>unmount suc\n");
					
				}
				semaphore_signal(Device_Exist_Protect);
			}
		} 
	   
		free(msg->DeviceName);
		free(msg->TypeName);
		message_release(DeviceCallbackMsg_p, msg);
	}//end while
}
/*����PIO���磬���ݾ���ĵ�·ͼ��ȡ��Ӧ�Ŀ���PIO�ڵ�����*/
ST_ErrorCode_t CH_USB_PIOSetPower( void )
{
    extern ST_DeviceName_t PIO_DeviceName[];

    ST_DeviceName_t    DeviceName = {0};
    STPIO_OpenParams_t OpenParam = {0};
    STPIO_Handle_t 	   DeviceHandle = 0;
    ST_ErrorCode_t 	   RetVal = 0;

    strcpy(DeviceName, PIO_DeviceName[5]);
    OpenParam.ReservedBits 	  = PIO_BIT_6|PIO_BIT_7;
    OpenParam.BitConfigure[6] = STPIO_BIT_OUTPUT;
    OpenParam.BitConfigure[7] = STPIO_BIT_OUTPUT;

    RetVal = STPIO_Open(DeviceName, &OpenParam, &DeviceHandle);
    if (RetVal != ST_NO_ERROR)
        return RetVal;
#ifdef STB_TV  /*s7 add 2008-06-02*/
    RetVal = STPIO_Set(DeviceHandle,(STPIO_BitMask_t)PIO_BIT_7);
#else
    RetVal = STPIO_Clear(DeviceHandle,(STPIO_BitMask_t)PIO_BIT_7);
#endif

    if (RetVal != ST_NO_ERROR)
        return RetVal;

/*20080614 add*/
	task_delay(ST_GetClocksPerSecond()/10);
	

    RetVal = STPIO_Set (DeviceHandle, (STPIO_BitMask_t)PIO_BIT_6);
    if (RetVal != ST_NO_ERROR)
        return RetVal;

    RetVal = STPIO_Close (DeviceHandle);
    return RetVal;

}


static void CH_USB_CallBack( device_event_t   event,
                         device_notify_t *notifications[],
                         int              total,
                         void            *cookie)
{
	U32                     i=0;
	int pass=0;
	OSPLUS_CallbackInfo_t  *Msg;
	device_type_t dev_logical_volume;
	device_type_t dev_fixed_storage;
	device_type_t dev_removable_storage;
	dev_logical_volume    = device_type (OSPLUS_DEVICE_TYPE_VOLUME);
	dev_removable_storage = device_type (OSPLUS_DEVICE_TYPE_REMOVABLE_STORAGE);
	dev_fixed_storage     = device_type (OSPLUS_DEVICE_TYPE_FIXED_STORAGE);
	//do_report(0,"\nwo  haishi huidiao le1");
	for(i = 0; i < total; i++)
	{

		if(notifications[i]->type == dev_logical_volume || notifications[i]->type == dev_removable_storage || notifications[i]->type == dev_fixed_storage
		)
		{
		       //do_report(0,"\nwo  haishi huidiao le2");
			pass=1;
		}

		if(pass == 1)
		{
		       //do_report(0,"\nwo  haishi huidiao le3");
			if ((Msg = message_claim_timeout(DeviceCallbackMsg_p, TIMEOUT_IMMEDIATE)) != NULL)
			{
				Msg->Event      = event;
				Msg->DeviceName = osplus_strdup (notifications[i]->device_name);
				Msg->TypeName   = osplus_strdup (notifications[i]->type_name);
				Msg->DeviceType = notifications[i]->type;
                            do_report(0,"\nwo  haishi huidiao le4");
				message_send(DeviceCallbackMsg_p, Msg);
			}
			else
			{
				do_report(("DeviceCallback: message_claim_timeout failed \n"));
			}
		}
	}
}

/*USB��ʼ������*/

ST_ErrorCode_t CH_USB_Setup(void)
{
	ST_ErrorCode_t  resErrCode = ST_NO_ERROR;

	/*����DEVICE CALLBACK ��Ϣ����*/
	DeviceCallbackMsg_p = message_create_queue_timeout( sizeof(OSPLUS_CallbackInfo_t), 16 );
	/* ע��callback���� */
	if(device_callback_attach (CH_USB_CallBack, NULL, 1) == -1)
	{
		
		return ST_ERROR_FEATURE_NOT_SUPPORTED;
	}
#if 0 /* ���������Ӳ�̣������δ���*/
	if(ATAPI_OSPLUS_Setup() != ST_NO_ERROR)
	{
	do_report(0,"ATAPI create class failed Err %d\n", errno);
	return ST_ERROR_FEATURE_NOT_SUPPORTED;
	}
#endif	

/*��ʼ��USB�豸����*/
	resErrCode = CH_USB_PIOSetPower();

	if(resErrCode != ST_NO_ERROR)
	{
		return  -1  ; 
	}
#if 1
	{
        void    *uncached = NULL;

        /* Uncached pool setup */
        uncached = osplus_cache_safe_allocate(NULL, SCC_OSPLUS_NONCACHED_MEM_POOL_SIZE);
        if (uncached == NULL)
        {
            return -1;
        }

        uncached = OSPLUS_UNCACHED_ADDR(uncached);

        /* Ensure cache coherency by purging the whole cache */
        cache_purge_data_all();

        /* Register the uncached memory */
        osplus_uncached_register(uncached, SCC_OSPLUS_NONCACHED_MEM_POOL_SIZE);

        /* SoftConnex USBLink initialisation. */
        if (USBLINK_Initialize(uncached, SCC_OSPLUS_NONCACHED_MEM_POOL_SIZE) != 0)
        {
            return -1;
        }

    }

#if 0 /* TODO: Need to enable this */
    /* register NCACHE area to OS+ */
    osplus_uncached_register((void *)NCACHE_BASE_ADDRESS, NCACHE_SIZE);
#endif
#if 0
    /* setting the E2FS cache size */
    registry_key_set(E2FS_REGISTRY_NODE, E2FS_REGISTRY_CACHE_SIZE, (void *)512);
#endif
#if 0
      registry_key_set(E2FS_REGISTRY_NODE, E2FS_REGISTRY_CACHE_MODE, (void *)e2fs_cache_mode_soft_update);
	/* set the E2FS cache mode to soft update so that file updates are always intact */
	/*registry_key_set(E2FS_REGISTRY_NODE, E2FS_REGISTRY_CACHE_MODE, (void *)e2fs_cache_mode_soft_update);*/

	/*��ʼ���ļ�ϵͳ e2fs and fat*/
	if(e2fs_vfs_init() == -1)
	{
		return ST_ERROR_FEATURE_NOT_SUPPORTED;
	}
#endif	
	if (fat_vfs_init () == -1)
	{
		do_report(0,"fat_vfs_init()=failed Err %d\n", errno);
		return ST_ERROR_FEATURE_NOT_SUPPORTED;
	}
     if (lvm_initialize(2) == -1)
      {
        do_report(0,"lvm_initialize()=failed Err %d\n", errno);
        return ST_ERROR_FEATURE_NOT_SUPPORTED;
      }
#endif

	return 0;

}

#if 1
ST_ErrorCode_t CH_DeviceDetectInit(void)
{
	int             params;
	task_t          *Device_Task_Id = NULL;
	ST_ErrorCode_t  ErrCode = ST_NO_ERROR;
	/*device_callback_attach (CH_USB_CallBack, NULL, 1);*/
	
	Device_Task_Id =Task_Create(CH_DeviceDetect_Task, NULL, 10*1024, 5, "Detect Task", 0);
	if (Device_Task_Id == NULL)
	{
		do_report(0, "Can not create CH_DeviceDetect Task");
		return ST_ERROR_BAD_PARAMETER;
	}
	else
	{
		do_report(0,"CH_DeviceDetect  Task Created");
	}


	return ErrCode;
}


#endif
int CH_USB_Init( void )
{
#if 1
	char InfoName[1][MAX_LANGUAGE_ID][50]= 
	{
	"���USB�豸...","Detect USB Device..."

	};
	ST_ErrorCode_t st_ErrorCode;

	st_ErrorCode = CH_USB_Setup();

	Device_Exist_Protect = semaphore_create_fifo(1);


	
	/*if(CHMID_HDD_CHKWHENSTART() == 1)
	{
		do_report(0,"ysharp->>not find usb hdd\n");
	}
	else
	{
		do_report(0,"ysharp->> find usb hdd\n");
	}
	*/

	if(st_ErrorCode == ST_NO_ERROR)
	{ 
		return  CH_DeviceDetectInit();
	}
	else
	{
		return 1;
	}
#endif
}
#if 0
void filecraetest(void)
{
	vfs_file_t         *InfoFileHandle;
	char path[255]={0};
	sprintf(path,"%s%s",gDiskMountPoint[gCurMountIndex].DiskMountPoint,"testfile.txt");
	vfs_unlink(path);
	 InfoFileHandle=vfs_creat(path);
	if(InfoFileHandle == NULL)
	{
		do_report(0,"file create fail\n");
	}
	else
	{
		do_report(0,"create ok\n");
		vfs_unlink(path);
		vfs_close(InfoFileHandle);
	}
}
#endif


