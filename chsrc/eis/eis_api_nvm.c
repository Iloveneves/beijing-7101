/*
  * ===================================================================================
  * CopyRight By CHANGHONG NET L.T.D.
  * �ļ�: 	eis_api_nvm.c
  * ����: 	ʵ�����ô洢��صĽӿ�
  * ����:	 ��ս�ң�������
  * ʱ��:	2008-10-23
  * ===================================================================================
  */

#include "eis_api_define.h"
#include "eis_api_globe.h"
#include "eis_api_debug.h"
#include "eis_api_nvm.h"
#include "../main/initterm.h"

#include "eis_include\Ipanel_nvram.h"
#ifdef SUMA_SECURITY
#include "..\ch_security\stb_tsm.h"
extern semaphore_t *p_SumaMemorySema; 
U8 SumaSecurity_write_flash ( U32 ru32_FlashAddr, U32 ru32_WriteSize, U8 *rp_Data );
#endif

#ifdef __EIS_API_DEBUG_ENABLE__
#define __EIS_API_NVM_DEBUG__
#endif


typedef enum eisflashcmd
{
	EIS_FLASH_ERASE,
	EIS_FLASH_WRITE,
#ifdef SUMA_SECURITY
	SUMA_FLASH_WRITE,
#endif
	EIS_FLASH_NONE
}EIS_FLASH_CMD;


typedef struct eisflash
{ 
    EIS_FLASH_CMD                       eis_cmd;
    U32                       i_flashAddr;
    U8                      *puc_EisFlashRamData;
    U32                      eis_flashsize;	
}CH_Eis_FlashCmd;

#define CH_FLASH_MIRROR
/*#define SKIN_FUNCTION*/

#ifdef CH_FLASH_MIRROR
static BYTE_T *mirror_basic=NULL;
static BYTE_T *mirror_appmgr=NULL;
static BYTE_T *mirror_third=NULL;
static BYTE_T *mirror_quick=NULL;
#endif

message_queue_t  *gp_EisCmd_Message = NULL;    


INT32_T eis_Nvm_buf_len;/* ��¼IPANEL���ݽ�Ҫд��ĳ���*/
static U8 eis_flash_write_or_erase=EIS_FLASH_NONE;/* ��¼IPANEL���ݶ���*/
	
static semaphore_t *gpsema_eis_nvm	= NULL;	/* ����nvm�ڴ滥�� */

static INT32_T eis_flash_state=IPANEL_NVRAM_UNKNOWN;/* ��¼FLASH����״̬*/

static void Eis_NVM_Monitor(void *ptr);
#ifdef CH_FLASH_MIRROR
int EIS_Get_ActFALSHAddr(unsigned int mirrie_addr)
{
	if((unsigned int)mirror_basic == mirrie_addr)
	{
		eis_report("\n EIS_FLASH_OFFSET_BASIC");
		return (EIS_FLASH_OFFSET_BASIC);
	}
	else
	if((unsigned int)mirror_appmgr == mirrie_addr)
	{
		return (EIS_FLASH_OFFSET_APPMGR);
	}
	else
	if((unsigned int)mirror_third == mirrie_addr)
	{
		eis_report("\n EIS_FLASH_OFFSET_THIRD_PART");
		return (EIS_FLASH_OFFSET_THIRD_PART );
	}
	else
	if((unsigned int)mirror_quick == mirrie_addr)
	{
		eis_report("\n EIS_FLASH_OFFSET_QUICK");
		return (EIS_FLASH_OFFSET_QUICK);
	}
	else
		{
		eis_report("\n FLASH addr find failed");
		return 0;
		}
}
#endif

static void Eis_NVM_Monitor(void *ptr);

/*
  * Func: ipanel_porting_term_nvm
  * Desc: ����nvm�õ��ľ����ڴ�
  */
void ipanel_porting_term_nvm ( void )
{
	if ( gpsema_eis_nvm )
	{
		semaphore_delete ( gpsema_eis_nvm );
		gpsema_eis_nvm = NULL;
	}
}

boolean ipanel_porting_init_nvm ( void )
{
	message_queue_t  *pstMessageQueue = NULL;	

	gpsema_eis_nvm = semaphore_create_fifo(1);

	eis_flash_write_or_erase=EIS_FLASH_NONE;
    /*������Ϣ����*/
	if (( pstMessageQueue = message_create_queue_timeout ( 20,50 ) ) == NULL )
   	{
             return true;
	 }
   	else
   	{
		symbol_register (	"EisCmd_Message", (	void * ) pstMessageQueue );
   	}

	if ( symbol_enquire_value ( "EisCmd_Message", ( void ** ) &gp_EisCmd_Message ) )
	{
		STTBX_Print(("Cant find EisCmd_Message message queue\n" ));
		return true;
	}
	
	#ifdef CH_FLASH_MIRROR
	CH_FlashLock();
	mirror_basic=(BYTE_T *)memory_allocate(CHSysPartition,EIS_FLASH_BASIC_SIZE);
	if(mirror_basic!=NULL)
	{
		memcpy ( mirror_basic, (void*)(EIS_FLASH_OFFSET_BASIC+EIS_FLASH_BASE_ADDR), EIS_FLASH_BASIC_SIZE );
	}
	else
	{
		CH_FlashUnLock();
		return true;
	}
	
	mirror_appmgr=(BYTE_T *)memory_allocate(CHSysPartition,EIS_FLASH_APPMGR_SIZE);
	if(mirror_appmgr!=NULL)
	{
		memcpy ( mirror_appmgr, (void*)(EIS_FLASH_OFFSET_APPMGR+EIS_FLASH_BASE_ADDR), EIS_FLASH_APPMGR_SIZE );
	}
	else
	{
		CH_FlashUnLock();
		return true;
	}
	
	mirror_third=(BYTE_T *)memory_allocate(CHSysPartition,EIS_FLASH_THIRDPART_SIZE);
	if(mirror_third!=NULL)
	{
		memcpy ( mirror_third, (void*)(EIS_FLASH_OFFSET_THIRD_PART+EIS_FLASH_BASE_ADDR), EIS_FLASH_THIRDPART_SIZE );
	}
	else
	{
		CH_FlashUnLock();
		return true;
	}
	
	mirror_quick=(BYTE_T *)memory_allocate(CHSysPartition,EIS_FLASH_QUICK_SIZE);
	if(mirror_quick!=NULL)
	{
		memcpy ( mirror_quick, (void*)(EIS_FLASH_OFFSET_QUICK+EIS_FLASH_BASE_ADDR), EIS_FLASH_QUICK_SIZE );
	}
	else
	{
		CH_FlashUnLock();
		return true;
	}

	CH_FlashUnLock();
	
	#endif
	
	if ( ( Task_Create( Eis_NVM_Monitor, NULL,1024*20, 7,"Eis_DVM_Monitor", 0 ) ) == NULL )
	{
		eis_report ( "\n Eis_DVM_Monitor creat failed\n");
		return true;
	}
	
	return false;
}
/*
  * Func: ipanel_porting_erase_flash
  * Desc: ����FLASH
  * In:	ru32_FlashAddr 	��ʼƫ����
  		ru32_EraseSize	�����ռ�ĳ���
  */
  /*����ֵ0:δ�ȵ��ź��� ��1 �ɹ� ��2ʧ��*/
U8 ipanel_porting_erase_flash ( U32 ru32_FlashAddr, U32 ru32_EraseSize )
{
	U32 	u32_ActOffset;
	int 	u32_ActLen;
	U8	u8_Status;
	int	i_OffsetTmp;
	int	i_Try;
	int	i_Error;
	/* 1. ������� */
	/* 1.1 ������� */
	if ( NULL==gpsema_eis_nvm )
	{
		eis_report ( "\n++>eis ipanel_porting_nvram_read failed" );
		return  0;
	}

#ifdef CH_FLASH_MIRROR
#ifdef SKIN_FUNCTION
	if((ru32_FlashAddr >= (EIS_FLASH_OFFSET_SKIN+EIS_FLASH_BASE_ADDR)) && (ru32_FlashAddr < (EIS_FLASH_OFFSET_SKIN+EIS_FLASH_BASE_ADDR+EIS_FLASH_SKIN_SIZE) ) )
	{
		u32_ActOffset = ru32_FlashAddr -EIS_FLASH_BASE_ADDR;
	}
	else
#endif		
	u32_ActOffset =EIS_Get_ActFALSHAddr(ru32_FlashAddr);
#else
	u32_ActOffset = ru32_FlashAddr -EIS_FLASH_BASE_ADDR;
#endif

	if(u32_ActOffset==0)
	{
		return 0;
	}

	
	u32_ActLen	= ru32_EraseSize ;
	
	u8_Status = IPANEL_FLASH_STATUS_ERASING;

	i_OffsetTmp = u32_ActOffset;
	CH_FlashLock();
	while ( u32_ActLen > 0 )
	{
#ifndef SKIN_FUNCTION
       		if((i_OffsetTmp>=EIS_FLASH_OFFSET_BASIC)&&(i_OffsetTmp<EIS_FLASH_MAX_ADDR))
#endif
			{
		     i_Try = 1000;
		       do
		      {   
		        CH_PUB_UnLockFlash(i_OffsetTmp);
			i_Error = CHFLASH_Erase ( FLASHHandle, i_OffsetTmp, EIS_FLASH_BLOCK_SIZE );
				i_Try --;

		   	}        while ( i_Error != 0 &&  i_Try > 0);
		
			eis_report ( "\n++>eis erase1 [0x%x] <%s>", i_OffsetTmp, GetErrorText(i_Error) );
		
		
                    CH_PUB_LockFlash(i_OffsetTmp);
       		}
		
			if ( i_Try <= 0 )
			{
				break;
			}
		
		      i_OffsetTmp 	+= EIS_FLASH_BLOCK_SIZE;
		      u32_ActLen	-= EIS_FLASH_BLOCK_SIZE;
	}
	CH_FlashUnLock();

	if ( i_Try <= 0 )
	{
		u8_Status = IPANEL_FLASH_STATUS_FAIL;
		return 2;
	}
	else
	{
		u8_Status = IPANEL_FLASH_STATUS_OK;
		return 1;
	}
}

/*
  * Func: ipanel_porting_write_flash
  * Desc: ����FLASH
  * In:	ru32_FlashAddr 	��ʼƫ����
  		ru32_WriteSize	д��ռ�ĳ���
  		rp_Data			д�����ݵĻ���ָ��
  */
  /*����ֵ0:δ�ȵ��ź��� ��1 �ɹ� ��2ʧ��*/
U8 ipanel_porting_write_flash ( U32 ru32_FlashAddr, U32 ru32_WriteSize, U8 *rp_Data )
{
	U32 	u32_ActOffset;
	int 	u32_ActLen ;
	U8	u8_Status;
	int	i_OffsetTmp;
	int	i_Try;
	int	i_Error;
	int	i_ActWrite;
	U8   *puc_ActualBlockData= NULL;
	int   i_BlockWriteSize;
	eis_report ( "\n ipanel_porting_write_flash  " );
	/* 1. ������� */
	/* 1.1 ������� */

#ifdef CH_FLASH_MIRROR
#ifdef SKIN_FUNCTION
	if((ru32_FlashAddr >= (EIS_FLASH_OFFSET_SKIN+EIS_FLASH_BASE_ADDR)) && (ru32_FlashAddr < (EIS_FLASH_OFFSET_SKIN+EIS_FLASH_BASE_ADDR+EIS_FLASH_SKIN_SIZE) ) )
	{
		u32_ActOffset = ru32_FlashAddr -EIS_FLASH_BASE_ADDR;
	}
	else
#endif	
	u32_ActOffset =EIS_Get_ActFALSHAddr(ru32_FlashAddr);
#else
	u32_ActOffset = ru32_FlashAddr -EIS_FLASH_BASE_ADDR;
#endif


	u32_ActLen	= ru32_WriteSize;

	if(u32_ActOffset==0)
	{
		return 0;
	}
	
	u8_Status= IPANEL_FLASH_STATUS_WRITING;

	i_OffsetTmp = u32_ActOffset;
	
	 CH_FlashLock();
	 
	while ( u32_ActLen > 0 )
	{
                boolean b_NeedUpadate  = false;
     
	           i_Try =1000;
          /*��ǰ��д������ݴ�С*/
		  i_BlockWriteSize = (u32_ActLen>=EIS_FLASH_BLOCK_SIZE)?EIS_FLASH_BLOCK_SIZE:u32_ActLen;
		  
	         /*���ȱȽ�ԭ�������ݺ�ϣ��д��������Ƿ�仯*/
                if(puc_ActualBlockData  == NULL)
                {
                   puc_ActualBlockData = memory_allocate(CHSysPartition, EIS_FLASH_BLOCK_SIZE);
		   
                }
	              if(puc_ActualBlockData != NULL)
		    	{
		    		#ifdef CH_FLASH_MIRROR
					memcpy(puc_ActualBlockData,(U8 *)(u32_ActOffset+EIS_FLASH_BASE_ADDR+(ru32_WriteSize-u32_ActLen)),EIS_FLASH_BLOCK_SIZE);
				#else
					 memcpy(puc_ActualBlockData,ru32_FlashAddr+(ru32_WriteSize-u32_ActLen),EIS_FLASH_BLOCK_SIZE);
				#endif
			   if(memcmp(puc_ActualBlockData,rp_Data+(ru32_WriteSize-u32_ActLen),i_BlockWriteSize) == 0)/* ��ͬ���������ݣ�����Ҫ����*/
			   {
					 eis_report ( "same flash address[%x],size[%d]\n ", ru32_FlashAddr,i_BlockWriteSize);
			   }
			  else
			  {
				        memcpy(puc_ActualBlockData,rp_Data+(ru32_WriteSize-u32_ActLen),i_BlockWriteSize);

					eis_report ( "Need update  flash address[%x],size[%d]\n ", ru32_FlashAddr,i_BlockWriteSize);
					b_NeedUpadate = true;
			  }
			   
		    	}
		  if(b_NeedUpadate == true)
		  {
#ifndef SKIN_FUNCTION
		  	 if((i_OffsetTmp>=EIS_FLASH_OFFSET_BASIC)&&(i_OffsetTmp<EIS_FLASH_MAX_ADDR))
#endif
			 {
		     i_Try =1000;
		      do
		    {
		        CH_PUB_UnLockFlash(i_OffsetTmp);
			i_Error = CHFLASH_Erase ( FLASHHandle, i_OffsetTmp, EIS_FLASH_BLOCK_SIZE );

			i_Try--;
				}while ( i_Error != 0 && i_Try > 0);
			
			i_Try =1000;
		      do		  
		      {
			i_Error = CHFLASH_Write ( FLASHHandle, i_OffsetTmp, 
									/*rp_Data+(ru32_WriteSize-u32_ActLen)*/puc_ActualBlockData, 
									EIS_FLASH_BLOCK_SIZE/*(u32_ActLen>=EIS_FLASH_BLOCK_SIZE)?EIS_FLASH_BLOCK_SIZE:u32_ActLen*/, 
									&i_ActWrite );
			i_Try--;
			 }while ( i_Error != 0 &&  i_Try > 0);
	
            	     CH_PUB_LockFlash(i_OffsetTmp);
			}
		      if ( i_Try <= 0 )
			{
				break;
			}
		  }
		i_OffsetTmp 	+= EIS_FLASH_BLOCK_SIZE;
		u32_ActLen	-= EIS_FLASH_BLOCK_SIZE;
	}
	
	CH_FlashUnLock();
       if(puc_ActualBlockData != NULL)
       {
               memory_deallocate(CHSysPartition, puc_ActualBlockData);
       }
	if ( i_Try <= 0 )
	{
		u8_Status = IPANEL_FLASH_STATUS_FAIL;
		return 2;
	}
	else
	{
		u8_Status= IPANEL_FLASH_STATUS_OK;
		return 1;
	}
}

/* EIS�첽дFLASH����*/
static void Eis_NVM_Monitor(void *ptr)
{
	int i;
	clock_t timeout;
	CH_Eis_FlashCmd *msg_p ;
	while(1)
	{
	       timeout = time_plus(time_now(), ST_GetClocksPerSecond()*5);		
		msg_p =	(CH_Eis_FlashCmd *)message_receive_timeout(gp_EisCmd_Message,&timeout);
	      if(msg_p != NULL)
	      	{
			      semaphore_wait(gpsema_eis_nvm);
	           
	                  if(msg_p->eis_cmd ==  EIS_FLASH_ERASE)
	                  	{
				      if(ipanel_porting_erase_flash(msg_p->i_flashAddr,msg_p->eis_flashsize)==1)
					{
						eis_flash_write_or_erase = EIS_FLASH_NONE;
						eis_flash_state = IPANEL_NVRAM_SUCCESS;
					} 	
					else
					{
						eis_flash_write_or_erase = EIS_FLASH_NONE;
						eis_flash_state = IPANEL_NVRAM_FAILED;
					}
	                  	}
	                  else if(msg_p->eis_cmd  == EIS_FLASH_WRITE)
	                  	{
					if(ipanel_porting_write_flash(msg_p->i_flashAddr,msg_p->eis_flashsize,msg_p->puc_EisFlashRamData)==1)
					{
						eis_flash_write_or_erase = EIS_FLASH_NONE;
						eis_flash_state = IPANEL_NVRAM_SUCCESS;
					}	
					else
					{
						eis_flash_write_or_erase = EIS_FLASH_NONE;
						eis_flash_state = IPANEL_NVRAM_FAILED;
					}
	                  	}
#ifdef SUMA_SECURITY
				   else if(msg_p->eis_cmd  == SUMA_FLASH_WRITE)
	                  	{
						if(SumaSecurity_write_flash(msg_p->i_flashAddr,msg_p->eis_flashsize,msg_p->puc_EisFlashRamData)==1)
							{
							if(msg_p->i_flashAddr == FLASH_SECURITY_ADDR)
							{
								CH_SetSecurityFlashStatus(0x8A);
							}
							else if(msg_p->i_flashAddr == FLASH_SECURITY_ADDR_SPARE)
							{
								CH_SetSpareSecurityFlashStatus(0x8A);

							}
						
							}	
						else
							{
							do_report(0,"SUMA Write flash error\n");

							}
	                  	}
#endif

				semaphore_signal(gpsema_eis_nvm);
				
			#ifndef CH_FLASH_MIRROR 
			if(msg_p->puc_EisFlashRamData != NULL)
			{
		              memory_deallocate(CHSysPartition, msg_p->puc_EisFlashRamData);
				msg_p->puc_EisFlashRamData  = NULL;
			}
			#endif
			
			message_release(gp_EisCmd_Message,msg_p);
	      	}
		else
		{
               
		}
	}
}

/*
����˵����
	����ṩ���ϲ��NVRAM(ͨ������flash)�ڴ�������ʼ��ַ��������ÿ��Ĵ�С��
ע�⣺IPANEL_NVRAM_DATA_BASIC��IPANEL_NVRAM_DATA_QUICK�����ݵĴ洢�ռ���
	�����ṩ�ġ������������ݴ洢�ռ����ipanel���øú���û�л�ȡ������Ӧ�Ĺ��ܾ�
	����ʵ�֡�
����˵����
	���������
		flag: Ҫ��ȡ����������
		typedef enum
		{
			IPANEL_NVRAM_DATA_BASIC, / ����NVRAM /
			IPANEL_NVRAM_DATA_SKIN, / Skinר�� /
			IPANEL_NVRAM_DATA_THIRD_PART, / ���������� /
			IPANEL_NVRAM_DATA_QUICK, / ����д���NVRAM /
			IPANEL_NVRAM_DATA_UNKNOWN
		} IPANEL_NVRAM_DATA_TYPE_e;
		IPANEL_NVRAM_DATA_BASIC - ��ʾ����ipanel�洢ͨ�����ݵ�NVRAM����ʼ
		��ַ���������ÿ�����С��
		IPANEL_NVRAM_DATA_SKIN - ��ʾ���ع�skin�ļ��洢��NVRAM����ʼ��ַ��
		�������ÿ����Ĵ�С��
		IPANEL_NVRAM_DATA_THIRD_PART - ��ʾ���ع��������洢��NVRAM����ʼ��
		ַ���������ÿ����Ĵ�С��
		IPANEL_NVRAM_DATA_QUICK - ��ʾ���ع�ipanel�洢�������Ǳ仯Ƶ�ʺܸ�
		�����ݵ�NVRAM����ʼ��ַ���������ÿ����Ĵ�С��
	���������
		**addr��FLASH�ռ����ʼ��ַ��
		*sect_num��FLASH�ռ�Ŀ���
		*sect_size��FLASH��ÿ���С��
	�� �أ�
		IPANEL_OK:�ɹ�;
		IPANEL_ERR:ʧ�ܡ�
  */
INT32_T ipanel_porting_nvram_info(BYTE_T **addr, INT32_T *sect_num, INT32_T *sect_size, INT32_T flag)
{

#ifdef EIS_API_NVM_DEBUG
	eis_report ( "++>eis ipanel_porting_nvram_info flag=%d", flag );
#endif

	switch ( flag )
	{
	case IPANEL_NVRAM_DATA_BASIC: 			/*����VRAM */
		*sect_num 	= EIS_FLASH_BASIC_SIZE/EIS_FLASH_BLOCK_SIZE;
		*sect_size	= EIS_FLASH_BLOCK_SIZE;
		
		#ifdef CH_FLASH_MIRROR
		if(mirror_basic==NULL)
		{
			return IPANEL_ERR;
		}
		else
		{
			*addr = mirror_basic;
		}
		#else
		*addr		= (BYTE_T *)(EIS_FLASH_OFFSET_BASIC+EIS_FLASH_BASE_ADDR)/*gpsta_eis_BasicNvmMirror*/;
		#endif
		
		eis_report ( "++>addr=0x%x,sect_size=%d ,sect_num=%d", *addr, *sect_size,*sect_num);	
		break;
	case IPANEL_NVRAM_DATA_APPMGR: 
		*sect_num 	= EIS_FLASH_APPMGR_SIZE/EIS_FLASH_BLOCK_SIZE;
		*sect_size	= EIS_FLASH_BLOCK_SIZE;
		
		#ifdef CH_FLASH_MIRROR
		if(mirror_appmgr==NULL)
		{
			return IPANEL_ERR;
		}
		else
		{
			*addr = mirror_appmgr;
		}
		#else
			*addr= (U8*)(EIS_FLASH_OFFSET_APPMGR+EIS_FLASH_BASE_ADDR);
		#endif

		eis_report ( "++>addr=0x%x,sect_size=%d sect_num=%d", *addr, *sect_size,*sect_num);
		break;
		
	case IPANEL_NVRAM_DATA_THIRD_PART:/* ���������� */
		*sect_num 	= EIS_FLASH_THIRDPART_SIZE/EIS_FLASH_BLOCK_SIZE;
		*sect_size	= EIS_FLASH_BLOCK_SIZE;
		#ifdef CH_FLASH_MIRROR
		if(mirror_third==NULL)
		{
			return IPANEL_ERR;
		}
		else
		{
			*addr = mirror_third;
		}
		#else
		*addr		= (U8*)(EIS_FLASH_OFFSET_THIRD_PART+EIS_FLASH_BASE_ADDR)/*gpsta_eis_APPMGRNvmMirror*/;
		#endif

		eis_report ( "++>addr=0x%x,sect_size=%d sect_num=%d", *addr, *sect_size,*sect_num);
		break;
	case IPANEL_NVRAM_DATA_AUX:/* ����д���NVRAM */
		*sect_num 	= EIS_FLASH_QUICK_SIZE/EIS_FLASH_BLOCK_SIZE;
		*sect_size	= EIS_FLASH_BLOCK_SIZE;
		#ifdef CH_FLASH_MIRROR
		if(mirror_quick==NULL)
		{
			return IPANEL_ERR;
		}
		else
		{
			*addr = mirror_quick;
		}
		#else
		*addr = (U8*)(EIS_FLASH_OFFSET_QUICK+EIS_FLASH_BASE_ADDR)/*gpsta_eis_APPMGRNvmMirror*/;
		#endif

		eis_report ( "++>addr=0x%x,sect_size=%d sect_num=%d", *addr, *sect_size,*sect_num);
		break;
#ifdef SKIN_FUNCTION
		case IPANEL_NVRAM_DATA_SKIN:/* ����д���NVRAM */
			*sect_num 	= EIS_FLASH_SKIN_SIZE/EIS_FLASH_BLOCK_SIZE;
			*sect_size	= EIS_FLASH_BLOCK_SIZE;

			*addr = (U8*)(EIS_FLASH_OFFSET_SKIN+EIS_FLASH_BASE_ADDR)/*gpsta_eis_APPMGRNvmMirror*/;

			eis_report ( "++>addr=0x%x,sect_size=%d sect_num=%d", *addr, *sect_size,*sect_num);
		break;
#endif		
	default:
		{
		*sect_num 	= 0;
		*sect_size	= 0;
		*addr		= NULL;	
		return IPANEL_ERR;
			}
		break;
	}
#ifdef EIS_API_NVM_DEBUG
	eis_report ( "++>eis ipanel_porting_nvram_info end" );
#endif

	return IPANEL_OK;
}

/*
����˵����
	��NVRAM (ͨ������flash)�ж�ȡָ�����ֽ��������ݻ������У�ʵ��ʱע�����
	�ռ�Խ�硣�����ȡ����ʼ��ַ��������Ч��ַ��Χ������IPANEL_ERR�������ȡ��
	���ݵ�ַ�ռ䷶Χ������Ч�ռ䣬��buffer�з�����Ч�ռ�����ݣ���������ֵָʾ
	ʵ�ʶ�ȡ�����ֳ��ȡ�
����˵����
	���������
		flash_addr����Ҫ��ȡ��Ŀ��Flash Memory����ʼ��ַ��
		buf_addr�������ȡ���ݵĻ�������
		len����Ҫ��ȡ���ֽ�����
	���������
		buf_addr����ȡ�����ݡ�
	�� �أ�
		>�� 0:ʵ�ʶ�ȡ�����ݳ���;
		IPANEL_ERR:ʧ�ܡ�
*/
INT32_T ipanel_porting_nvram_read ( UINT32_T flash_addr, BYTE_T *buf_addr, INT32_T len )
{
#ifdef EIS_API_NVM_DEBUG
	eis_report ( "\n++>eis ipanel_porting_nvram_read flash_addr=%x, buf_addr=%x, len=%d" ,flash_addr,buf_addr,len);
#endif
	if((0==flash_addr)||(NULL==buf_addr)||(len==0))
		return IPANEL_ERR;
	
	if ( NULL==gpsema_eis_nvm )
	{
		eis_report ( "\n++>eis ipanel_porting_nvram_read failed" );
		return IPANEL_ERR;
	}
       semaphore_wait(gpsema_eis_nvm);

	memcpy ( buf_addr, (void*)(flash_addr), len );
	
	semaphore_signal ( gpsema_eis_nvm );
#ifdef EIS_API_NVM_DEBUG
	eis_report ( "++>eis ipanel_porting_nvram_read end" );
#endif

	return len;
}

/*
����˵����
	������д��NVRAM(ͨ������flash)�У������ֲ�ͬ�Ĳ���ģʽ����̨д
	��IPANEL_NVRAM_BURN_DELAY���ͼ�ʱд��IPANEL_NVRAM_BURN_NOW��ģ
	ʽ��
	ʵ��ʱע������ռ�Խ�����⣬���ں�̨д��ģʽ������ʹ���µ��߳���ͬ������
	�Ҹú��������������أ�������������ʵ�ʴ���ʱ��ͨ���Ὠ��һ�������ڴ�������
	������д���ڴ澵��ȥ���������أ�Ȼ���̨����/�߳̽������ڴ����е�����д��
	NVRAM�С���̨д���flash�ռ�Ĳ��������ɵײ㸺��
	���ڼ�ʱд��ģʽ����Ҫ�ڽӿ����������д������������첽ʵ�֡���ʱд��ģ
	ʽ��flash�ռ�Ĳ���������ipanel����
ע�⣺�˺��������ĵ�ַ�����ݳ��������ֽ�Ϊ��λ�ģ�������ģʽ������flash��
		��ע�Ᵽ���������ԡ�
		1�� �����ʼ��ַ�����ַ����һ���ֽڱ���ǰ��0xFF���γ�����д��flash��
		2�� ���������ַ�����ַ�����һ���ֽڱ����0xFF�γ�����д��flash��
		3�� Ϊ���Ż�ִ��Ч�ʣ��û��������¿��������������������ݺ��ʣ����߽���β
			���ַ�ֽڵ��������Դ������ݺ��ʣ�
����˵����
	���������
		flash_addr����Ҫд���Ŀ����ʼ��ַ��
		buf_addr��д�����ݿ����ʼ��ַ��
		len����Ҫд����ֽ�����
		mode��д�����ģʽ
		typedef enum
		{
			IPANEL_NVRAM_BURN_DELAY,
			IPANEL_NVRAM_BURN_NOW
		}IPANEL_NVRAM_BURN_MODE_e;
	�����������
	�� �أ�
		>�� 0:ʵ��д������ݳ���;
		IPANEL_ERR:ʧ�ܡ�
*/
INT32_T ipanel_porting_nvram_burn(UINT32_T flash_addr, const CHAR_T *buf_addr, INT32_T len,IPANEL_NVRAM_BURN_MODE_e mode)
{
#ifdef EIS_API_NVM_DEBUG
	eis_report ( "\n++>eis ipanel_porting_nvram_burn flash_addr=%x, buf_addr=%x, len=%d" ,flash_addr,buf_addr,len);
#endif
      clock_t timeout;
      CH_Eis_FlashCmd *msg_p;

	 if((0==flash_addr)||(NULL==buf_addr)||(len==0))
		return IPANEL_ERR;
#ifndef SKIN_FUNCTION
	if(len>(EIS_FLASH_BLOCK_SIZE*8))
		return IPANEL_ERR;

#else	
	if(IPANEL_NVRAM_BURN_NOW == mode)
	{
		if( ipanel_porting_write_flash(flash_addr,len,buf_addr) == 1)
		{
			return len;
		}
		else
		{
			return 0;
		}
	}
#endif	
	if ( NULL==gpsema_eis_nvm )
	{
		eis_report ( "\n++>eis ipanel_porting_nvram_burn failed" );
		return IPANEL_ERR;
	}
	
       semaphore_wait(gpsema_eis_nvm);
#ifdef CH_FLASHWRITE_SYN
	eis_flash_state = IPANEL_NVRAM_BURNING;
	if( ipanel_porting_write_flash(flash_addr,len,buf_addr) == 1)
	{
		eis_flash_state = IPANEL_NVRAM_SUCCESS;
	}
	else
	{
		eis_flash_state = IPANEL_NVRAM_FAILED;
	}
#else
	eis_flash_write_or_erase = EIS_FLASH_WRITE;/* ��¼��������*/

	  
      timeout = time_plus(time_now(), ST_GetClocksPerSecond()*3);		
      msg_p =	(CH_Eis_FlashCmd *)message_claim_timeout(gp_EisCmd_Message,&timeout);
      if(msg_p!=NULL)
	{
		msg_p->eis_cmd = EIS_FLASH_WRITE;
		msg_p->eis_flashsize = len;
		msg_p->i_flashAddr = flash_addr;

#ifdef CH_FLASH_MIRROR
		msg_p->puc_EisFlashRamData=(U8 *)flash_addr;
#else
		msg_p->puc_EisFlashRamData = memory_allocate(CHSysPartition,len);
#endif
		if(msg_p->puc_EisFlashRamData != NULL)
		{
			eis_flash_state = IPANEL_NVRAM_BURNING;

			memcpy(msg_p->puc_EisFlashRamData,buf_addr,len);
			message_send(gp_EisCmd_Message,msg_p);
		}
	} 
#endif
	semaphore_signal ( gpsema_eis_nvm );
#ifdef EIS_API_NVM_DEBUG
	eis_report ( "++>eis ipanel_porting_nvram_burn end" );
#endif
	
	return IPANEL_OK;
}

/*
����˵����
	����ָ����ַ�����ݣ�ͬ��ʵ�ֲ���������
����˵����
	���������
		flash_addr�����������ʼ��ַ��
		len�������ռ�ĳ��ȡ�
	���������
		��
	�� �أ�
		IPANEL_OK:�����ɹ�;
		IPANEL_ERR:����ʧ�ܡ�
*/
INT32_T ipanel_porting_nvram_erase(UINT32_T flash_addr, INT32_T len)
{
      clock_t timeout;
      CH_Eis_FlashCmd *msg_p;

#ifdef EIS_API_NVM_DEBUG
	eis_report ( "\n++>eis ipanel_porting_nvram_erase flash_addr=%x" ,flash_addr);
#endif
	if((EIS_FLASH_OFFSET_THIRD_PART+EIS_FLASH_BASE_ADDR)==flash_addr)
	{
		return IPANEL_OK;
	}
	
	if((0==flash_addr)||(len==0))
		return IPANEL_ERR;
	
#ifndef SKIN_FUNCTION
	if(len>(EIS_FLASH_BLOCK_SIZE*8))
		return IPANEL_ERR;
#else
	 if( ipanel_porting_erase_flash(flash_addr,len) == 1)
	 {
		return IPANEL_OK;
	 }
	 else
	 {
		return IPANEL_ERR;
	 }
#endif
	
	
	if ( NULL==gpsema_eis_nvm )
	{
		eis_report ( "\n++>eis ipanel_porting_nvram_erase failed" );
		return IPANEL_ERR;
	}

       semaphore_wait(gpsema_eis_nvm);
#ifdef CH_FLASH_MIRROR
	memset((U8 *)flash_addr,0,len);
#endif

 #ifdef CH_FLASHWRITE_SYN
	eis_flash_state = IPANEL_NVRAM_BURNING;
	if( ipanel_porting_erase_flash(flash_addr,len) == 1)
		eis_flash_state = IPANEL_NVRAM_SUCCESS;
	else
		eis_flash_state = IPANEL_NVRAM_FAILED;
	
#else
	eis_flash_write_or_erase=EIS_FLASH_ERASE;/* ��¼��������*/

      timeout = time_plus(time_now(), ST_GetClocksPerSecond()*3);		
      msg_p =	(CH_Eis_FlashCmd *)message_claim_timeout(gp_EisCmd_Message,&timeout);
      if(msg_p!=NULL)
	{
		msg_p->eis_cmd = EIS_FLASH_ERASE;
		msg_p->eis_flashsize = len;
		msg_p->i_flashAddr = flash_addr;
		msg_p->puc_EisFlashRamData = NULL;
		eis_flash_state = IPANEL_NVRAM_BURNING;
		message_send(gp_EisCmd_Message,msg_p);

	} 
    #endif	  
	semaphore_signal ( gpsema_eis_nvm );
#ifdef EIS_API_NVM_DEBUG
	eis_report ( "++>eis ipanel_porting_nvram_erase end" );
#endif

	return IPANEL_OK;
}

/*
����˵����
	�ں�̨д��ģʽ�£����ô˺����ж�������NVRAM(ͨ������flash)��д��״̬��
����˵����
	���������
		flash_addr �C burn ����ʼ��ַ��
		len - д����ֽ���
	�����������
	�� �أ�
		IPANEL_NVRAM_BURNING������д��
		IPANEL_NVRAM_SUCCESS���Ѿ�д�ɹ���
		IPANEL_NVRAM_FAILED��дʧ�ܡ�
*/
INT32_T ipanel_porting_nvram_status(UINT32_T flash_addr, INT32_T len)
{
    int   i_status;
#ifdef EIS_API_NVM_DEBUG
	eis_report ( "\n ipanel_porting_nvram_status  " );
#endif
  	if ( NULL==gpsema_eis_nvm )
	{
		eis_report ( "\n++>eis ipanel_porting_nvram_status failed" );
		return IPANEL_ERR;
	}	
  semaphore_wait(gpsema_eis_nvm);
    i_status =  eis_flash_state;
    semaphore_signal ( gpsema_eis_nvm );
		
    return i_status;
}


#ifdef SUMA_SECURITY


/*
  * Func: ipanel_porting_write_flash
  * Desc: ����FLASH
  * In:	ru32_FlashAddr 	��ʼƫ����
  		ru32_WriteSize	д��ռ�ĳ���
  		rp_Data			д�����ݵĻ���ָ��
  */
  /*����ֵ0:δ�ȵ��ź��� ��1 �ɹ� ��2ʧ��*/
U8 SumaSecurity_write_flash ( U32 ru32_FlashAddr, U32 ru32_WriteSize, U8 *rp_Data )
{
	U32 	u32_ActOffset;
	int 	u32_ActLen;
	U8	u8_Status;
	int	i_OffsetTmp;
	int	i_Try =1000;
	int	i_Error;
	int	i_ActWrite;
	U8   *puc_ActualBlockData= NULL;
	int   i_BlockWriteSize;
	
	eis_report ( "\n ipanel_porting_write_flash  " );
	/* 1. ������� */
	/* 1.1 ������� */

	u32_ActOffset = ru32_FlashAddr -EIS_FLASH_BASE_ADDR;
	
	u32_ActLen	= ru32_WriteSize;

	if(u32_ActOffset==0)
	{
		return 0;
	}
	
	u8_Status= IPANEL_FLASH_STATUS_WRITING;

	i_OffsetTmp = u32_ActOffset;
	
	semaphore_wait(p_SumaMemorySema);
	
	CH_FlashLock();
	
	while ( u32_ActLen > 0 )
	{
                boolean b_NeedUpadate  = false;
     
	           i_Try =1000;
          	/*��ǰ��д������ݴ�С*/
		  i_BlockWriteSize = (u32_ActLen>=EIS_FLASH_BLOCK_SIZE)?EIS_FLASH_BLOCK_SIZE:u32_ActLen;
		  
	         /*���ȱȽ�ԭ�������ݺ�ϣ��д��������Ƿ�仯*/
                if(puc_ActualBlockData  == NULL)
                {
                   	puc_ActualBlockData = memory_allocate(CHSysPartition, EIS_FLASH_BLOCK_SIZE);   
                }
	              if(puc_ActualBlockData != NULL)
		    {
#ifdef CH_FLASH_MIRROR
				memcpy(puc_ActualBlockData,(U8 *)(u32_ActOffset+EIS_FLASH_BASE_ADDR+(ru32_WriteSize-u32_ActLen)),EIS_FLASH_BLOCK_SIZE);
#else
				memcpy(puc_ActualBlockData,ru32_FlashAddr+(ru32_WriteSize-u32_ActLen),EIS_FLASH_BLOCK_SIZE);
#endif
			   if(memcmp(puc_ActualBlockData,rp_Data+(ru32_WriteSize-u32_ActLen),i_BlockWriteSize) == 0)/* ��ͬ���������ݣ�����Ҫ����*/
			   {
				eis_report ( "same flash address[%x],size[%d]\n ", ru32_FlashAddr,i_BlockWriteSize);						
			   }
			  else
			  {
				        memcpy(puc_ActualBlockData,rp_Data+(ru32_WriteSize-u32_ActLen),i_BlockWriteSize);

					eis_report ( "Need update  flash address[%x],size[%d]\n ", ru32_FlashAddr,i_BlockWriteSize);
					b_NeedUpadate = true;
			  }
			   
		    	}
		  if(b_NeedUpadate == true)
		  {
				 CH_PUB_UnLockFlash(i_OffsetTmp);
				 i_Try =1000;
		      		do
			    	{
					     
						i_Error = CHFLASH_Erase ( FLASHHandle, i_OffsetTmp, EIS_FLASH_BLOCK_SIZE );

						i_Try--;
				}while ( i_Error != 0 && i_Try > 0);
			
				i_Try =1000;
			      do		  
			      {
					i_Error = CHFLASH_Write ( FLASHHandle, i_OffsetTmp, 
											puc_ActualBlockData, 
											EIS_FLASH_BLOCK_SIZE, 
											&i_ActWrite );
					i_Try--;
				 }while ( i_Error != 0 &&  i_Try > 0);
				
            	 		CH_PUB_LockFlash(i_OffsetTmp);
			      if ( i_Try <= 0 )
				{
					break;
				}
		  }
		i_OffsetTmp 	+= EIS_FLASH_BLOCK_SIZE;
		u32_ActLen	-= EIS_FLASH_BLOCK_SIZE;
	}
	
	CH_FlashUnLock();
	
       if(puc_ActualBlockData != NULL)
       {
               memory_deallocate(CHSysPartition, puc_ActualBlockData);
       }
	  semaphore_signal(p_SumaMemorySema);
	if ( i_Try <= 0 )
	{
		u8_Status = IPANEL_FLASH_STATUS_FAIL;
		return 2;
	}
	else
	{
		u8_Status= IPANEL_FLASH_STATUS_OK;
		return 1;
	}
}


boolean  CH_Security_Flash_burn(UINT32_T flash_addr, const CHAR_T *buf_addr, INT32_T len)
{
	clock_t timeout;
	CH_Eis_FlashCmd *msg_p;

	if (  buf_addr == NULL || len <= 0)
	{
		return TRUE;
	}
	  
      timeout = time_plus(time_now(), ST_GetClocksPerSecond()*3);		
      msg_p =	(CH_Eis_FlashCmd *)message_claim_timeout(gp_EisCmd_Message,&timeout);
      if(msg_p!=NULL)
     	{
		msg_p->eis_cmd = SUMA_FLASH_WRITE;
		msg_p->eis_flashsize = len;
		msg_p->i_flashAddr = flash_addr;
#ifdef CH_FLASH_MIRROR
		msg_p->puc_EisFlashRamData=(U8 *)buf_addr;
#else
		msg_p->puc_EisFlashRamData = memory_allocate(CHSysPartition,len);
#endif
		if(msg_p->puc_EisFlashRamData != NULL)
		{
			memcpy(msg_p->puc_EisFlashRamData,buf_addr,len);
			message_send(gp_EisCmd_Message,msg_p);
		}
     	} 

#ifdef EIS_API_NVM_DEBUG
	eis_report ( "++>eis CH_Security_Flash_burn end" );
#endif
	
	return FALSE;
}


void CH_CheckSecurityDataStatus(void)
{
	char * tempbuf = NULL;
	if(CH_GetSecurityFlashStatus() != 0X8A)
	{
		if(CH_GetSpareSecurityFlashStatus() == 0x8A)/*����������Ч*/
		{

			tempbuf = (char *)memory_allocate ( CHSysPartition , FLASH_SECURITYDATA_LEN);
			if(tempbuf != NULL)
			{
				memset(tempbuf,0,FLASH_SECURITYDATA_LEN);

				ipanel_porting_nvram_read(FLASH_SECURITY_ADDR_SPARE,tempbuf,FLASH_SECURITYDATA_LEN);
				
				CH_SetSecurityFlashStatus(0x8b);
				if(CH_Security_Flash_burn(FLASH_SECURITY_ADDR,tempbuf,FLASH_SECURITYDATA_LEN)== false)
				{
					int time = 3000;
				
					while(time >0)
					{
						if(CH_GetSecurityFlashStatus()==0x8a)
						{
							break;
						}
						else
						{
							task_delay(ST_GetClocksPerSecond()/1000);
							time--;
						}
						
					}
					
				}
			

				memory_deallocate(CHSysPartition,tempbuf);
				tempbuf == NULL;
			}
			else
			{
				return;
			}



		}

	}


}


#endif



/*--eof----------------------------------------------------------------------------------------------------*/

