##############################################
#   File name: makefile
#   Description: makefile for on CHANGHONG 7100 platform
#   Author: Yixiaoli
#   Date:2006-08-07
#   COPYRIGHT (C) 2006 CHANGHONG
##############################################

MODULE_NAME = main
$(MODULE_NAME)_OBJ_DIR = $(MODULE_NAME).dir
$(MODULE_NAME)_PLATFORM_DIR = $(DVD_PLATFORM)_$(DVD_BACKEND)
include $(DVD_MAKE)/generic.mak
BDIR = $($(MODULE_NAME)_OBJ_DIR)$(SLASH)$($(MODULE_NAME)_PLATFORM_DIR)$(SLASH)
##$(MODULE_NAME)_OBJECTS = main.tco  initterm.tco Key.tco NECDecode.tco ch_hdmi.tco Yxl_GraphicTest.tco chevent.tco
$(MODULE_NAME)_OBJECTS = initterm.tco ch_hdmi.tco  
$(MODULE_NAME)_OBJECTS+=CH_VideoOutputBase.tco 
$(MODULE_NAME)_OBJECTS+=chevent.tco CH_sys.tco 


BUILD_$(MODULE_NAME)_OBJECTS = $(addprefix $(BDIR), $($(MODULE_NAME)_OBJECTS))


SRC     = $(subst $(BAD_SLASH),$(GOOD_SLASH),$(CURDIR))$(SLASH)
INCDIR  = -I$(subst $(BAD_SLASH),$(GOOD_SLASH),$(CURDIR))
INCDIR += -I$(subst $(BAD_SLASH),$(GOOD_SLASH),$(DVD_INCLUDE))
INCDIR += -I$(subst $(BAD_SLASH),$(GOOD_SLASH),$(DVD_CONFIG)/chip)
INCDIR += -I$(subst $(BAD_SLASH),$(GOOD_SLASH),$(CURR_DIR)/include)
INCDIR += -I$(subst $(BAD_SLASH),$(GOOD_SLASH),$(DVD_CONFIG)/platform/$(DVD_PLATFORM))
INCDIR += -I$(subst $(BAD_SLASH),$(GOOD_SLASH),$(DVD_ROOT)$(SLASH)multicom$(SLASH)include)
INCDIR += -I$(subst $(BAD_SLASH),$(GOOD_SLASH),$(OSPLUSROOT)/include)


INCLIB  = $(subst $(BAD_SLASH),$(GOOD_SLASH),$(DVD_LIBS))
INCLIB +=$(DVD_EXPORTS)/$(LIB_PREFIX)stapi_$(DVD_TRANSPORT)$(LIB_SUFFIX)
##INCLIB +=$(BDIR)$(MODULE_NAME).lib
INCLIB +=$(CURR_DIR)\chlib\main.lib
##INCLIB +=$(CURR_DIR)\chlib\YTest.lib
INCLIB +=$(CURR_DIR)\chlib\key.lib
INCLIB +=$(CURR_DIR)\chlib\sectionbase.lib
INCLIB +=$(CURR_DIR)\chlib\graphicbase.lib
INCLIB +=$(CURR_DIR)\chlib\channelbase.lib
INCLIB +=$(CURR_DIR)\chlib\chdemux.lib

INCLIB +=$(CURR_DIR)\chlib\symbol.lib
##INCLIB +=$(CURR_DIR)\chlib\mosaic.lib
INCLIB +=$(CURR_DIR)\chlib\ch_flash.lib
##INCLIB +=$(CURR_DIR)\chlib\Network_Stack.lib
INCLIB +=$(CURR_DIR)\chlib\chmid_tcpip.lib
INCLIB +=$(CURR_DIR)\chlib\chmid_cm.lib
INCLIB +=$(CURR_DIR)\chlib\chdrv_net.lib

INCLIB +=$(CURR_DIR)\chlib\usif.lib
INCLIB +=$(CURR_DIR)\chlib\dbase.lib

##INCLIB +=$(CURR_DIR)\chlib\net.lib


ifeq "$(Broadbus_VOD)" "1"
INCLIB +=$(OSPLUSROOT)\lib\st40\libngbt.a
INCLIB +=$(OSPLUSROOT)\lib\st40\libngip.a
INCLIB +=$(OSPLUSROOT)\lib\st40\libngos.a
INCLIB +=$(OSPLUSROOT)\lib\st40\libngppp.a
INCLIB +=$(OSPLUSROOT)\lib\st40\libngrt.a
##INCLIB +=$(CURR_DIR)\chlib\chdrv_tcpip.lib
INCLIB +=$(CURR_DIR)\chlib\broadbus.lib
endif

##INCLIB +=$(CURR_DIR)\chlib\ch_graphcom.lib
##INCLIB +=$(CURR_DIR)\chlib\ch_picapi.lib

ifeq "$(CH_7710_PLATFORM)" "0"
INCLIB +=$(CURR_DIR)\chlib\ch_tuner.lib
##INCLIB +=$(CURR_DIR)\chlib\ch_tuner_ST.lib
else
ifeq "$(YXL_ST_PLATFORM)" "1"
INCLIB +=$(CURR_DIR)\chlib\ch_tunerLG.lib
else
INCLIB +=$(CURR_DIR)\chlib\ch_tuner.lib
endif
endif

INCLIB +=$(CURR_DIR)\chlib\report.lib
INCLIB +=$(CURR_DIR)\chlib\errors.lib
##INCLIB +=$(CURR_DIR)\chlib\ch_card.lib
INCLIB +=$(CURR_DIR)\chlib\ch_led.lib
INCLIB +=$(CURR_DIR)\chlib\util.lib
INCLIB +=$(CURR_DIR)\chlib\ch_zip.lib
INCLIB +=$(CURR_DIR)\chlib\ch_hdmi.lib

INCLIB +=$(CURR_DIR)\chlib\libste2p.a
INCLIB +=$(CURR_DIR)\chlib\libstgfx.a

INCLIB +=$(CURR_DIR)\chlib\stsmart.lib
INCLIB +=$(CURR_DIR)\chlib\stuart.lib

ifeq "$(CH_EPG)" "1"
INCLIB +=$(CURR_DIR)\chlib\ch_epg.lib
endif


ifeq "$(USE_VOD)" "1"
INCLIB +=$(CURR_DIR)\chlib\vod.lib
INCLIB +=$(CURR_DIR)\chlib\itvapplib_st7101.lib
##INCLIB +=$(CURR_DIR)\chlib\itv_st7101.lib
endif
INCLIB +=$(CURR_DIR)\chlib\CH_AVSYNBaseline.lib


ifeq "$(INTEGRATE_NVOD)" "1"
INCLIB +=$(CURR_DIR)\chlib\ch_nvod.lib
endif
ifeq "$(BEIJING_MOSAIC)" "1"
INCLIB +=$(CURR_DIR)\chlib\mosaic.lib
endif
ifeq "$(USE_IPANEL)" "1"
INCLIB +=$(CURR_DIR)\chlib\eis.lib
#INCLIB +=$(CURR_DIR)\chlib\eis_7100.lib
#INCLIB +=$(CURR_DIR)\chlib\lib_ipanel_ST7100.lib
#INCLIB +=$(CURR_DIR)\chlib\debug_lib_ipanel_ST7101_10006.lib
INCLIB +=$(CURR_DIR)\chlib\debug_lib_ipanel_ST7101.lib
INCLIB +=$(CURR_DIR)\chlib\libcloud.a
INCLIB +=$(CURR_DIR)\chlib\ch_cloud.lib

INCLIB +=$(CURR_DIR)\chlib\st_ch_stock.lib
endif
ifeq "$(SUMA_SECURITY)" "1"
INCLIB +=$(CURR_DIR)\chlib\ch_security.lib
INCLIB +=$(CURR_DIR)\chlib\tsm.lib
INCLIB +=$(CURR_DIR)\chlib\ch_pic.lib
endif

##20070720 add security lib
INCLIB +=$(CURR_DIR)\chlib\libstsectoolfuse.a
INCLIB +=$(CURR_DIR)\chlib\libstsectoolnuid.a
INCLIB +=$(CURR_DIR)\chlib\ch_produce.lib

INCLIB +=$(CURR_DIR)\chlib\libsttkdma_m1.a
INCLIB +=$(CURR_DIR)\chlib\ch_tkdma.lib
##########################
ifeq "$(NAFR_KERNEL)" "1"
INCLIB +=$(CURR_DIR)\chlib\MgDDI.lib
ifeq "$(NAGRA_PRODUCE_VERSION)" "0"
INCLIB +=$(CURR_DIR)\MgKernel\mgcak_Core_debug_st7101_st40_3.1.1.lib 
INCLIB +=$(CURR_DIR)\MgKernel\mgopi_changhong_bgctv_dev_st7101_st40_3.1.1.lib
INCLIB +=$(CURR_DIR)\MgKernel\mgcak_Pairing_debug_st7101_st40_3.1.1.lib
INCLIB +=$(CURR_DIR)\MgKernel\mgcak_PPV_debug_st7101_st40_3.1.1.lib
else
INCLIB +=$(CURR_DIR)\MgKernel\mgcak_Core_release_st7101_st40_3.1.1.lib 
INCLIB +=$(CURR_DIR)\MgKernel\mgopi_changhong_bgctv_prod_st7101_st40_3.1.1.lib 
INCLIB +=$(CURR_DIR)\MgKernel\mgcak_Pairing_release_st7101_st40_3.1.1.lib
INCLIB +=$(CURR_DIR)\MgKernel\mgcak_PPV_release_st7101_st40_3.1.1.lib
endif

ifeq "$(CH_IPANEL_MGCA)" "1"
INCLIB +=$(CURR_DIR)\chlib\MGCA_Ipanel.lib 
endif


##INCLIB +=$(CURR_DIR)\chlib\MgKernel.lib 
INCLIB +=$(CURR_DIR)\chlib\ChCABase.lib
endif
ifeq "$(HTRD_STOCK)" "1"
INCLIB +=$(CURR_DIR)\chlib\htrd_stock.lib 
INCLIB +=$(CURR_DIR)\chlib\htrdstock_changhong_7101_jiangsu_honey300_test.lib
endif

ifeq "$(ENABLE_USB)" "1"
INCLIB +=$(CURR_DIR)\chlib\ch_usb.lib 
INCLIB +=$(OSPLUSROOT)\lib\st40\libusblink_d.a
endif

##zhw add
ifeq "$(TF_CA)" "1"
##INCLIB +=$(CURR_DIR)\chlib\stsmart.lib
##INCLIB +=$(CURR_DIR)\chlib\stuart.lib

INCLIB +=$(CURR_DIR)\chlib\cas21.lib
INCLIB +=$(CURR_DIR)\chlib\tfcas.lib
endif

##20070720 add security lib
##INCLIB +=$(CURR_DIR)\chlib\libstsectoolfuse.a
##INCLIB +=$(CURR_DIR)\chlib\libstsectoolnuid.a

##INCLIB +=$(CURR_DIR)\chlib\libsttkdma_m1.a
##INCLIB +=$(CURR_DIR)\chlib\ch_tkdma.lib
##########################
INCLIB +=$(CURR_DIR)\chlib\ch_mp3.lib


ifeq "$(CH_STOCK)" "1"
   INCLIB +=$(CURR_DIR)\chlib\stock.lib 
   INCLIB +=$(CURR_DIR)\chlib\ch_stock.lib
endif
 ##  INCLIB +=$(CURR_DIR)\chlib\ethernet.lib
   INCLIB +=$(OSPLUSROOT)\lib\st40\mb411stb7109\libregistry.a
   INCLIB +=$(OSPLUSROOT)\lib\st40\libospluslib.a
  
ifeq "$(TCPIP_ENABLE)" "1"
INCLIB +=$(OSPLUSROOT)\lib\st40\libngbt.a
INCLIB +=$(OSPLUSROOT)\lib\st40\libngip.a
INCLIB +=$(OSPLUSROOT)\lib\st40\libngos.a
INCLIB +=$(OSPLUSROOT)\lib\st40\libngppp.a
INCLIB +=$(OSPLUSROOT)\lib\st40\libngrt.a
INCLIB +=$(CURR_DIR)\chlib\chdrv_tcpip.lib   
INCLIB +=$(CURR_DIR)\chlib\ch_net.lib
endif
##C_CC_GLOBAL  = -c -DSTFAE -DST_$(DVD_BACKEND) -DST_$(DVD_OS) -D$(DVD_PLATFORM) -D$(DVD_TRANSPORT) -D$(DVD_SERVICE) -DPROCESSOR_$(PROC_DEFINE) $(INCDIR) $(OPTIONAL_CFLAGS) $(DVD_CFLAGS)
C_CC_GLOBAL  = -c -DST_$(DVD_BACKEND) -DST_$(DVD_OS) -D$(DVD_PLATFORM) -D$(DVD_TRANSPORT) -D$(DVD_SERVICE) -DPROCESSOR_$(PROC_DEFINE) $(INCDIR) $(OPTIONAL_CFLAGS) $(DVD_CFLAGS)

ifeq "$(ARCHITECTURE)" "ST20"
C_CC_default  = $(PROCESSOR) -g -O0 $(C_CC_GLOBAL) 
endif
 
ifeq "$(ARCHITECTURE)" "ST40"
C_CC_default     = -mruntime=os21 -g -O0 $(C_CC_GLOBAL)
endif



all : create_directories $(BDIR)$(MODULE_NAME).lib copy_chlib $(BDIR)main$(OBJ_SUFFIX) main.lku
$(BDIR)errors.tco : $(SRC)errors.c 
	$(CC) $<        -o $@		$(if $(C_CC_$(basename $(notdir $@))),$(C_CC_$(basename $(notdir $@))), $(C_CC_default))	


##yxl 2007-03-03 add below section for create rom file

rom_info :
	$(ECHO) -------------------------------------------------------------------------------------
	$(ECHO) INFO: Creating a rom file for CHANGHONG 7100 Board
	$(ECHO) -------------------------------------------------------------------------------------

ifeq "$(ARCHITECTURE)" "ST40"
rom : rom_info create_directories $(BDIR)$(MODULE_NAME).lib copy_chlib $(BDIR)main$(OBJ_SUFFIX) flash.bin
endif


$(BDIR)bootrom.bin : $(BDIR)bootrom.out 
	-@sh4objcopy -O binary $(BDIR)bootrom.out $(BDIR)bootrom.bin --set-start 0xA0120004 --pad-to 0xA0120FF4

$(BDIR)bootrom.out : $(BDIR)bootrom.o
	-@sh4ld -o $(BDIR)bootrom.out $(BDIR)bootrom.o -EL --defsym _start=0xA0120004  -e 0xA0120004 --defsym _stack=0

$(BDIR)bootrom.o : $(DVD_PLATFORM)_$(DVD_BACKEND)_bootrom.S
	-@sh4gcc $< -c -g -Wa,--gdwarf2 -D__$(DVD_PLATFORM)__ -o $@ -fpic 

##$(BDIR)main.bin : main.out 
##	-@sh4outbin >NUL: main.out main.bin 
main.bin : main.out 
	-@sh4outbin >NUL: main.out main.bin 



##flash.bin: $(BDIR)main.bin $(BDIR)bootrom.bin 
##	-@$(CP) >NUL: /B $(BDIR)bootrom.bin+$(BDIR)main.bin flash.bin
flash.bin: main.bin $(BDIR)bootrom.bin 
	-@$(CP) >NUL: /b $(BDIR)bootrom.bin+main.bin flash.bin



##end yxl 2007-01-31 add below section for create rom file 


$(BDIR)main1.lib :					   \
       $(BDIR)errors.tco
	@$(ECHO) Building $@...
ifeq "$(ARCHITECTURE)" "ST20"
	@$(AR) $^ -o $@
endif
ifeq "$(ARCHITECTURE)" "ST40"
	@$(AR) -rs $@ $^
endif


##main.lku : $(BDIR)sgglue_main$(OBJ_SUFFIX) $(BDIR)sgglue_sysmem$(OBJ_SUFFIX) $(BDIR)$(LIB_PREFIX)sgglue$(LIB_SUFFIX) $(DVD_EXPORTS)/$(DVD_PLATFORM)_$(DVD_BACKEND)/$(LIB_PREFIX)stapi_$(DVD_TRANSPORT)$(LIB_SUFFIX) $(INCLIB)
##main.lku : $(DVD_EXPORTS)/$(DVD_PLATFORM)_$(DVD_BACKEND)/$(LIB_PREFIX)stapi_$(DVD_TRANSPORT)$(LIB_SUFFIX) $(INCLIB)
##ifeq "$(ARCHITECTURE)" "ST40"
##	$(CC_PLUS)  -Wl,--start-group $(BDIR)sgglue_main$(OBJ_SUFFIX) $(BDIR)sgglue_sysmem$(OBJ_SUFFIX) $(BDIR)$(LIB_PREFIX)sgglue$(LIB_SUFFIX) $(INCLIB) $(DVD_EXPORTS)/$(DVD_PLATFORM)_$(DVD_BACKEND)/$(LIB_PREFIX)stapi_$(DVD_TRANSPORT)$(LIB_SUFFIX)  -Wl,--end-group -specs=platform/$(DVD_PLATFORM)/$(DVD_BACKEND)/$(DVD_PLATFORM)_$(DVD_BACKEND)_board.mem -mruntime=os21 -mboard=board_$(DVD_PLATFORM)_map -g -o main.out -Wl,-Map=main.map
##endif

##main.lku : $(DVD_EXPORTS)/$(DVD_PLATFORM)_$(DVD_BACKEND)/$(LIB_PREFIX)stapi_$(DVD_TRANSPORT)$(LIB_SUFFIX) $(INCLIB)
main.lku :
ifeq "$(ARCHITECTURE)" "ST40"
##	$(CC_PLUS)  -Wl,--start-group $(BDIR)main.tco -Wl,--end-group -specs=$(STB7100ROOT)/config/platform/$(DVD_PLATFORM)/ch_7100_board.mem -mruntime=os21 -mboard=board_$(DVD_PLATFORM)_map -g -o main.out -Wl,-Map=main.map
## 	$(CC_PLUS)  -Wl,--start-group $(BDIR)main.tco $(DVD_EXPORTS)/$(DVD_PLATFORM)_$(DVD_BACKEND)/$(LIB_PREFIX)stapi_$(DVD_TRANSPORT)$(LIB_SUFFIX)  -Wl,--end-group -specs=$(STB7100ROOT)/config/platform/$(DVD_PLATFORM)/$(DVD_PLATFORM)_$(DVD_BACKEND)_board.mem -mruntime=os21 -mboard=board_$(DVD_PLATFORM)_map -g -o main.out -Wl,-Map=main.map
##	$(CC_PLUS)  -Wl,--start-group $(BDIR)main.tco $(DVD_EXPORTS)/$(DVD_PLATFORM)_$(DVD_BACKEND)/$(LIB_PREFIX)stapi_$(DVD_TRANSPORT)$(LIB_SUFFIX)  -Wl,--end-group -specs=$(STB7100ROOT)/config/platform/$(DVD_PLATFORM)/$(DVD_PLATFORM)_$(DVD_BACKEND)_board.mem -mruntime=os21 -mboard=board_$(DVD_PLATFORM)_map -g -o main.out -Wl,-Map=main.map
##	$(CC_PLUS)  -Wl,--start-group $(BDIR)main.tco $(DVD_EXPORTS)/$(DVD_PLATFORM)_$(DVD_BACKEND)/$(LIB_PREFIX)stapi_$(DVD_TRANSPORT)$(LIB_SUFFIX)  -Wl,--end-group -specs=$(PRJROOT)/config/platform/$(DVD_PLATFORM)/$(DVD_PLATFORM)_$(DVD_BACKEND)_board.mem -mruntime=os21 -mboard=board_$(DVD_PLATFORM)_map -g -o main.out -Wl,-Map=main.map
##	$(CC_PLUS)  -Wl,--start-group $(INCLIB) $(BDIR)main.tco $(DVD_EXPORTS)/$(DVD_PLATFORM)_$(DVD_BACKEND)/$(LIB_PREFIX)stapi_$(DVD_TRANSPORT)$(LIB_SUFFIX)  -Wl,--end-group -specs=$(PRJROOT)/config/platform/$(DVD_PLATFORM)/$(DVD_PLATFORM)_$(DVD_BACKEND)_board.mem -mruntime=os21 -mboard=board_$(DVD_PLATFORM)_map -g -o main.out -Wl,-Map=main.map
##	$(CC_PLUS)  -Wl,--start-group $(BDIR)main$(OBJ_SUFFIX) $(INCLIB) $(BDIR)main.tco -Wl,--end-group -specs=$(PRJROOT)/config/platform/$(DVD_PLATFORM)/$(DVD_PLATFORM)_$(DVD_BACKEND)_board.mem -mruntime=os21 -mboard=board_$(DVD_PLATFORM)_map -g -o main.out -Wl,-Map=main.map
##	$(CC_PLUS)  -Wl,--start-group $(BDIR)main$(OBJ_SUFFIX) $(INCLIB) -Wl,--end-group -specs=$(PRJROOT)/config/platform/$(DVD_PLATFORM)/$(DVD_PLATFORM)_$(DVD_BACKEND)_board.mem -mruntime=os21 -mboard=board_$(DVD_PLATFORM)_map -g -o main.out -Wl,-Map=main.map
##	$(CC_PLUS)  -Wl,--start-group $(BDIR)main$(OBJ_SUFFIX) $(INCLIB) -Wl,--end-group -specs=$(PRJROOT)/config/platform/$(DVD_PLATFORM)/boardA.mem -mruntime=os21 -mboard=board_$(DVD_PLATFORM)_map -g -o main.out -Wl,-Map=main.map
##	$(CC_PLUS)  -Wl,--start-group $(BDIR)main$(OBJ_SUFFIX) $(INCLIB) -Wl,--end-group -specs=$(PRJROOT)/config/platform/$(DVD_PLATFORM)/boardA.mem -mruntime=os21 -mboard=mb411stb7109p1 -g -o main.out -Wl,-Map=main.map
	$(CC_PLUS)  -Wl,--start-group $(BDIR)main$(OBJ_SUFFIX) $(INCLIB) -Wl,--end-group -specs=$(PRJROOT)/config/platform/$(DVD_PLATFORM)/$(DVD_BACKEND)/CH7101_board.mem -mruntime=os21 -mboard=mb411stb7109p1 -g -o main.out -Wl,-Map=main.map

##	$(CC_PLUS)  -Wl,--start-group $(INCLIB) -Wl,--end-group -specs=$(STB7100ROOT)/config/platform/$(DVD_PLATFORM)/ch_7100_board.mem -mruntime=os21 -mboard=board_$(DVD_PLATFORM)_map -g -o main.out -Wl,-Map=main.map

endif


##yxl 2007-01-31 add
##ifeq "$(ARCHITECTURE)" "ST40"
##flash.bin : $(BDIR)main$(OBJ_SUFFIX) $(INCLIB)
##	-@$(MAKE) -C platform/$(DVD_PLATFORM)/$(DVD_BACKEND)/bootrom
##	-@$(MAKE) -C $(PRJROOT)/config/platform/$(DVD_PLATFORM)/$(DVD_BACKEND)/bootrom
##endif



$(BDIR)main$(OBJ_SUFFIX) : main.c 
	$(CC) main.c       -o $(BDIR)main$(OBJ_SUFFIX)    -mruntime=os21 -g -O0 $(C_CC_GLOBAL)


##yxl 2006-11-22




##yxl 2006-11-22
##blower_$(DVD_PLATFORM)_$(DVD_BACKEND).lku : $(BDIR)sgglue_sysini$(OBJ_SUFFIX) $(BDIR)sgglue_sysmem$(OBJ_SUFFIX) $(BDIR)sgglue_blower$(OBJ_SUFFIX) $(BDIR)sgglue_flash$(OBJ_SUFFIX) $(BDIR)sgglue_pios$(OBJ_SUFFIX) $(BDIR)sgglue_trace$(OBJ_SUFFIX) $(BDIR)sgglue_uart$(OBJ_SUFFIX) $(BDIR)sgglue_i2c$(OBJ_SUFFIX) $(DVD_EXPORTS)/$(DVD_PLATFORM)_$(DVD_BACKEND)/$(LIB_PREFIX)stapi_$(DVD_TRANSPORT)$(LIB_SUFFIX) $(INCLIB)
##ifeq "$(ARCHITECTURE)" "ST40"
##	$(CC) $(BDIR)sgglue_sysini$(OBJ_SUFFIX) $(BDIR)sgglue_sysmem$(OBJ_SUFFIX) $(BDIR)sgglue_blower$(OBJ_SUFFIX) $(BDIR)sgglue_flash$(OBJ_SUFFIX) $(BDIR)sgglue_pios$(OBJ_SUFFIX) $(BDIR)sgglue_trace$(OBJ_SUFFIX) $(BDIR)sgglue_uart$(OBJ_SUFFIX) $(BDIR)sgglue_i2c$(OBJ_SUFFIX) $(INCLIB) $(DVD_EXPORTS)/$(DVD_PLATFORM)_$(DVD_BACKEND)/$(LIB_PREFIX)stapi_$(DVD_TRANSPORT)$(LIB_SUFFIX) -specs=platform/$(DVD_PLATFORM)/$(DVD_BACKEND)/$(DVD_PLATFORM)_$(DVD_BACKEND)_board.mem -mruntime=os21 -mboard=board_$(DVD_PLATFORM)_map -g -o blower_$(DVD_PLATFORM)_$(DVD_BACKEND).out -Xlinker -Map=blower_$(DVD_PLATFORM)_$(DVD_BACKEND).map
##endif
##yxl 2006-11-22




	

blower_$(DVD_PLATFORM)_$(DVD_BACKEND).lku : $(BDIR)sgglue_sysini$(OBJ_SUFFIX) $(BDIR)sgglue_sysmem$(OBJ_SUFFIX) $(BDIR)sgglue_blower$(OBJ_SUFFIX) $(BDIR)sgglue_flash$(OBJ_SUFFIX) $(BDIR)sgglue_pios$(OBJ_SUFFIX) $(BDIR)sgglue_trace$(OBJ_SUFFIX) $(BDIR)sgglue_uart$(OBJ_SUFFIX) $(BDIR)sgglue_i2c$(OBJ_SUFFIX) $(DVD_EXPORTS)/$(DVD_PLATFORM)_$(DVD_BACKEND)/$(LIB_PREFIX)stapi_$(DVD_TRANSPORT)$(LIB_SUFFIX) $(INCLIB)
ifeq "$(ARCHITECTURE)" "ST40"
	$(CC) $(BDIR)sgglue_sysini$(OBJ_SUFFIX) $(BDIR)sgglue_sysmem$(OBJ_SUFFIX) $(BDIR)sgglue_blower$(OBJ_SUFFIX) $(BDIR)sgglue_flash$(OBJ_SUFFIX) $(BDIR)sgglue_pios$(OBJ_SUFFIX) $(BDIR)sgglue_trace$(OBJ_SUFFIX) $(BDIR)sgglue_uart$(OBJ_SUFFIX) $(BDIR)sgglue_i2c$(OBJ_SUFFIX) $(INCLIB) $(DVD_EXPORTS)/$(DVD_PLATFORM)_$(DVD_BACKEND)/$(LIB_PREFIX)stapi_$(DVD_TRANSPORT)$(LIB_SUFFIX) -specs=platform/$(DVD_PLATFORM)/$(DVD_BACKEND)/$(DVD_PLATFORM)_$(DVD_BACKEND)_board.mem -mruntime=os21 -mboard=board_$(DVD_PLATFORM)_map -g -o blower_$(DVD_PLATFORM)_$(DVD_BACKEND).out -Xlinker -Map=blower_$(DVD_PLATFORM)_$(DVD_BACKEND).map
endif
ifeq "$(ARCHITECTURE)" "ST200"
	$(CC) $(BDIR)stapp_sysini$(OBJ_SUFFIX) $(BDIR)stapp_sysmem$(OBJ_SUFFIX) $(BDIR)stapp_blower$(OBJ_SUFFIX) $(BDIR)stapp_flash$(OBJ_SUFFIX) $(BDIR)stapp_pios$(OBJ_SUFFIX) $(BDIR)stapp_trace$(OBJ_SUFFIX) $(BDIR)stapp_uart$(OBJ_SUFFIX) $(BDIR)stapp_i2c$(OBJ_SUFFIX) -L$(DVD_EXPORTS)/$(DVD_PLATFORM)_$(DVD_BACKEND) -Wl,--start-group $(INCLIB) -lstapi_$(DVD_TRANSPORT) -Wl,--end-group -mruntime=os21 -g -O0 -mboard=$(DVD_PLATFORM) -msoc=sti5301 -mcore=st231 -lm -o blower_$(DVD_PLATFORM)_$(DVD_BACKEND).out -Wl,-Map=blower_$(DVD_PLATFORM)_$(DVD_BACKEND).map 
endif



purge :
	@$(ECHO) Cleaning $(MODULE_NAME)...
	-@$(RM) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BDIR))$(SLASH)*.tco
	-@$(RM) $(subst $(BAD_SLASH),$(GOOD_SLASH),$(BDIR))$(SLASH)*.lib
	-@$(RM) *.bak
    ifneq "$(wildcard $(BDIR))" ""
		-@$(RMDIR) $(BDIR)
    endif
    ifneq "$(wildcard $($(MODULE_NAME)_OBJ_DIR))" ""
		-@$(RMDIR) $($(MODULE_NAME)_OBJ_DIR)
    endif

$($(MODULE_NAME)_OBJ_DIR) :
	-@$(MKDIR) $($(MODULE_NAME)_OBJ_DIR)
	
$($(MODULE_NAME)_PLATFORM_DIR) :
	-@$(MKDIR) $($(MODULE_NAME)_OBJ_DIR)$(SLASH)$($(MODULE_NAME)_PLATFORM_DIR)

create_directories : 
	-@$(MAKE) $($(MODULE_NAME)_OBJ_DIR) --quiet --ignore-errors
	-@$(MAKE) $($(MODULE_NAME)_PLATFORM_DIR) --quiet --ignore-errors

$(BDIR)$(MODULE_NAME).lib :					   \
       $(BUILD_$(MODULE_NAME)_OBJECTS) 
	@$(ECHO) Building $@...
ifeq "$(ARCHITECTURE)" "ST20"
	@$(AR) $^ -o $@
endif
ifeq "$(ARCHITECTURE)" "ST40"
	@$(AR) -rs $@ $^
endif


$(BUILD_$(MODULE_NAME)_OBJECTS): $(BDIR)\%.tco : $(SRC)\%.c
	$(CC) $<        -o $@		$(if $(C_CC_$(basename $(notdir $@))),$(C_CC_$(basename $(notdir $@))), $(C_CC_default))	



copy_chlib:
	-@$(CP) $(BDIR)$(MODULE_NAME).lib $(CURR_DIR)\chlib\$(MODULE_NAME).lib



## Run Targets ##
## ----------- ##

run :
ifeq "$(ARCHITECTURE)" "ST20"
	st20run -i platform/$(DVD_PLATFORM)/$(DVD_BACKEND)/$(DVD_PLATFORM)_$(DVD_BACKEND).cfg -l platform/$(DVD_PLATFORM)/$(DVD_BACKEND) -l $(DVD_CONFIG)/board -t $(TARGET) main.lku	-v
endif
ifeq "$(ARCHITECTURE)" "ST40"
#	$(DB) -nw --directory platform/$(DVD_PLATFORM)/$(DVD_BACKEND) -x platform/$(DVD_PLATFORM)/$(DVD_BACKEND)/$(DVD_PLATFORM)_$(DVD_BACKEND)_start_script.cmd
	$(DB) -nw --directory $(subst $(BAD_SLASH),$(GOOD_SLASH),$(DVD_CONFIG)/platform/$(DVD_PLATFORM))/$(DVD_BACKEND) -x $(subst $(BAD_SLASH),$(GOOD_SLASH),$(DVD_CONFIG)/platform/$(DVD_PLATFORM))/$(DVD_BACKEND)/chsrc_start_script.cmd 
##	$(DB) -nw --directory $(subst $(BAD_SLASH),$(GOOD_SLASH),$(DVD_CONFIG)/platform/$(DVD_PLATFORM))/$(DVD_BACKEND) -x $(subst $(BAD_SLASH),$(GOOD_SLASH),$(DVD_CONFIG)/platform/$(DVD_PLATFORM))/$(DVD_BACKEND)/$(DVD_PLATFORM)_$(DVD_BACKEND)_start_script.cmd

endif


debug :
ifeq "$(ARCHITECTURE)" "ST20"
	st20run -i platform/$(DVD_PLATFORM)/$(DVD_BACKEND)/$(DVD_PLATFORM)_$(DVD_BACKEND).cfg -l platform/$(DVD_PLATFORM)/$(DVD_BACKEND) -l $(DVD_CONFIG)/board -t $(TARGET) main.lku -v -zinquestgui
endif
ifeq "$(ARCHITECTURE)" "ST40"
#	$(DB) -w --directory platform/$(DVD_PLATFORM)/$(DVD_BACKEND) -x platform/$(DVD_PLATFORM)/$(DVD_BACKEND)/$(DVD_PLATFORM)_$(DVD_BACKEND)_start_script.cmd
##	$(DB) -w --directory $(subst $(BAD_SLASH),$(GOOD_SLASH),$(DVD_CONFIG)/platform/$(DVD_PLATFORM))/$(DVD_BACKEND) -x $(subst $(BAD_SLASH),$(GOOD_SLASH),$(DVD_CONFIG)/platform/$(DVD_PLATFORM))/$(DVD_BACKEND)/$(DVD_PLATFORM)_$(DVD_BACKEND)_start_script.cmd
	$(DB) -w --directory $(subst $(BAD_SLASH),$(GOOD_SLASH),$(DVD_CONFIG)/platform/$(DVD_PLATFORM))/$(DVD_BACKEND) -x $(subst $(BAD_SLASH),$(GOOD_SLASH),$(DVD_CONFIG)/platform/$(DVD_PLATFORM))/$(DVD_BACKEND)/chsrc_start_script.cmd

endif

debug_flash :
ifeq "$(ARCHITECTURE)" "ST20"
	st20run -i platform/$(DVD_PLATFORM)/$(DVD_BACKEND)/$(DVD_PLATFORM)_$(DVD_BACKEND).cfg -l platform/$(DVD_PLATFORM)/$(DVD_BACKEND) -l $(DVD_CONFIG)/board -t $(TARGET)_flash -v -zinquestgui
endif
ifeq "$(ARCHITECTURE)" "ST40"
	$(DB) -w -x platform/$(DVD_PLATFORM)/$(DVD_BACKEND)/$(DVD_PLATFORM)_$(DVD_BACKEND)_start_script.cmd
endif


### END OF FILE ###
