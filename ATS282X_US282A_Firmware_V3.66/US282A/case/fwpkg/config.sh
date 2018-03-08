#!/bin/bash

# This script modify config.txt, fwimage.cfg and source files
# config.txt --> used for configure function support or not
# fwimage.cfg--> used for configure function support or not
# source file--> open or close some C source files
# 
# 2016-06-12	-	wuyufan

# �ýű��ļ���������Դ�ļ�������config.txt,fwimage_xx.cfg�ļ��ȣ�
# Ҳ���޸Ĳ���Դ�ļ��ĺ궨�����򿪻�ر�ĳЩ����
# �����ͻ����������ʱ�������ȵ���config.sh����Դ�ļ���Ȼ���ٽ��г���ı���
# �ýű���Ҫ����һ�����������Ƕ�Ӧ��ini�ļ�����ع������û���ini�ļ���������

echo "config source file"

FW_NAME_EVB=US282A_BTBOX_EVB
FW_NAME_DEMO=US282A_BTBOX_DEMO
FW_NAME_DVB_2825=US282A_BTBOX_DVB_ATS2825
FW_NAME_DVB_2823=US282A_BTBOX_DVB_ATS2823
FW_NAME_EVB_2823=US282A_BTBOX_EVB_ATS2823

if [ $# != 1 ]
then
    echo "missing ini file name"
    exit 1
else
    int_file=$1
fi

#shell�������:`command`,`������tab������ַ�,���������滻����command��ִ�н����ֵ��ǰ��ı���
#grep���ַ������������������˵��ַ������Լ�Ҫ���ҵ��ļ�������һ����˼����ini�ļ��в��Ұ����ؼ��ֵ��У�'|'��ʾ��������
#awk -F '=' '{print $2} �Ǹ����ض���ʽ�ָ��ַ������ָ�ı�ʶ��'=',ͬʱ�ҵ��ָ�ĵڶ����ַ������������ҵ�����Ӧ�Ĳ���
#��֮�����shell�������ini�ļ����ҵ��ؼ��ֶ����ֵ������ֵ������ı���

#��ȡ��������
BoardType=`grep -a "BoardType" $int_file | awk -F '=' '{print $2}'`

#��ȡconfig.txt����
config_txt_name=`grep -a "config_txt" $int_file | awk -F '=' '{print $2}'`

#��ȡfwimage.cfg����
config_cfg_name=`grep -a "fwimage_cfg" $int_file | awk -F '=' '{print $2}'`

#��ȡ�Ƿ�֧��TWS
SupportTWS=`grep -a "SupportTWS" $int_file | awk -F '=' '{print $2}'`

#��ȡ�Ƿ�֧��OTA
SupportOTA=`grep -a "SupportOTA" $int_file | awk -F '=' '{print $2}'`

#��ȡ�Ƿ�֧��Waves��Ч
SupportWaves=`grep -a "SupportWaves" $int_file | awk -F '=' '{print $2}'`

#��ȡ�Ƿ�֧��SPP/BLE
SupportSPPBLE=`grep -a "SupportSPPBLE" $int_file | awk -F '=' '{print $2}'`

#��ȡ�Ƿ�֧��ESD
SupportESD=`grep -a "SupportESD" $int_file | awk -F '=' '{print $2}'`

#��ȡ�Ƿ�֧�̶ֹ�������
SupportFIXSAMPLE=`grep -a "SupportFIXSAMPLE" $int_file | awk -F '=' '{print $2}'`

#��ȡdae_config.txt����
cfg_dae_txt_name=`grep -a "cfg_dae_txt" $int_file | awk -F '=' '{print $2}'`

SDKRelease=`grep -a "SDKRelease" $int_file | awk -F '=' '{print $2}'`

SupportBQB=`grep -a "SupportBQB" $int_file | awk -F '=' '{print $2}'`

SupportLowDelayLINEIN=`grep -a "SupportLOWDELAYLINEIN" $int_file | awk -F '=' '{print $2}'`

echo "BoardType: $BoardType"
echo "config.txt name: $config_txt_name"
echo "fwimage.cfg name: $config_cfg_name"
echo "SupportTWS: $SupportTWS"
echo "SupportOTA: $SupportOTA"
echo "SupportWaves: $SupportWaves"
echo "cfg_dae.txt name: $cfg_dae_txt_name"
echo "SupportESD: $SupportESD"
echo "SupportFIXSAMPLE: $SupportFIXSAMPLE"
echo "SDKRelease: $SDKRelease"
echo "SupportBQB: $SupportBQB"
echo "SupportLowDelayLINEIN: $SupportLowDelayLINEIN"
#�ж�config_txt_name�ļ��Ƿ���� -f��ʾ�ж��ļ� -d��ʾ�ж�Ŀ¼    
if [ ! -f "./config_txt/$config_txt_name" ]
then
    echo "ConfigError: ./config_txt/$config_txt_name not exist"
    exit 1;
fi   

#�ж�config_cfg_name�ļ��Ƿ���� -f��ʾ�ж��ļ� -d��ʾ�ж�Ŀ¼  
if [ ! -f "./config_txt/$config_cfg_name" ]
then
    echo "ConfigError: ./config_txt/$config_cfg_name not exist"
    exit 1;
fi  

#tws version and normal version use same btstack
grep -aq "\/\/\#define ENABLE_TRUE_WIRELESS_STEREO" ../inc/btstack_common.h
if [ $? == 0 ]
then
    echo "ConfigError: ../inc/btstack_common.h #define ENABLE_TRUE_WIRELESS_STEREO must be open"
    exit 1;
fi  

#�����ж��Ƿ�֧��OTA,֧��OTA���̼�ģʽҪ��Ϊ1����ͨ�̼�ģʽΪ0 
#sed��shell���ַ����滻���������ʽΪsed '��ʽ' �ļ�,��ʽ��'//'�������  
#sed '0, /a/s//b/' filename ���ڰ��ļ�filename�е�һ�γ���a���ַ����滻��b��0��ʾ��һ���滻��s��ʾ�滻���� 
if [ $SupportOTA == 1 ]
then   	
	sed '0,/INF_MAKE_FW_MODE.*;/s//INF_MAKE_FW_MODE = 1;/' ./config_txt/$config_cfg_name > tmpfile
	mv tmpfile ./config_txt/$config_cfg_name  
else	
	sed '0,/INF_MAKE_FW_MODE.*;/s//INF_MAKE_FW_MODE = 0;/' ./config_txt/$config_cfg_name > tmpfile
	mv tmpfile ./config_txt/$config_cfg_name 	 	
fi  

#�ж������ļ��Ƿ����otatest.ap��������ڣ����OTA����ap���������
grep -aq '//FWIM="otatest.ap"' ./config_txt/$config_cfg_name
if [ $? == 0 ]
then 
	sed '0,/SETTING_APP_SUPPORT_OTA_SONG_TEST.*;/s//SETTING_APP_SUPPORT_OTA_SONG_TEST   = 0[0,1];/' ./config_txt/$config_txt_name > tmpfile
	mv tmpfile ./config_txt/$config_txt_name     
else 
    grep -aq 'FWIM="otatest.ap"' ./config_txt/$config_cfg_name
    if [ $? == 0 ]
    then
        if [ $SupportOTA == 1 ]
        then   
        	sed '0,/SETTING_APP_SUPPORT_OTA_SONG_TEST.*;/s//SETTING_APP_SUPPORT_OTA_SONG_TEST   = 1[0,1];/' ./config_txt/$config_txt_name > tmpfile
        	mv tmpfile ./config_txt/$config_txt_name 
        else
	        sed '0,/SETTING_APP_SUPPORT_OTA_SONG_TEST.*;/s//SETTING_APP_SUPPORT_OTA_SONG_TEST   = 0[0,1];/' ./config_txt/$config_txt_name > tmpfile
	        mv tmpfile ./config_txt/$config_txt_name  
	    fi       
    else
	    sed '0,/SETTING_APP_SUPPORT_OTA_SONG_TEST.*;/s//SETTING_APP_SUPPORT_OTA_SONG_TEST   = 0[0,1];/' ./config_txt/$config_txt_name > tmpfile
	    mv tmpfile ./config_txt/$config_txt_name  
	fi   
fi

if [ $SupportSPPBLE = 0 ]
then
	sed '0,/BTMANAGER_ENABLE_SPP.*;/s//BTMANAGER_ENABLE_SPP                 = 0[0,1];/' ./config_txt/$config_txt_name > tmpfile
	mv tmpfile ./config_txt/$config_txt_name 

	sed '0,/BTMANAGER_ENABLE_BLE.*;/s//BTMANAGER_ENABLE_BLE                 = 0[0,1];/' ./config_txt/$config_txt_name > tmpfile
	mv tmpfile ./config_txt/$config_txt_name 
else			
	sed '0,/BTMANAGER_ENABLE_SPP.*;/s//BTMANAGER_ENABLE_SPP                 = 1[0,1];/' ./config_txt/$config_txt_name > tmpfile
	mv tmpfile ./config_txt/$config_txt_name 

	sed '0,/BTMANAGER_ENABLE_BLE.*;/s//BTMANAGER_ENABLE_BLE                 = 1[0,1];/' ./config_txt/$config_txt_name > tmpfile
	mv tmpfile ./config_txt/$config_txt_name 	 
fi 

if [ $SupportTWS == 1 ]
then   
	sed '0,/BTMANAGER_SUPPORT_DEVICE_NUM.*;/s//BTMANAGER_SUPPORT_DEVICE_NUM         = 2[1~2,1];/' ./config_txt/$config_txt_name > tmpfile
	mv tmpfile ./config_txt/$config_txt_name   

	sed '0,/BTMANAGER_ENABLE_ANCS .*;/s//BTMANAGER_ENABLE_ANCS                = 0[0,1];/' ./config_txt/$config_txt_name > tmpfile
	mv tmpfile ./config_txt/$config_txt_name  
	
	sed '0,/^\/\/\#define __SUPPORT_LINEIN_TWS.*/s//\#define __SUPPORT_LINEIN_TWS/' ../inc/case_independent.h > tmpfile
	mv tmpfile ../inc/case_independent.h
    if [ $SupportLowDelayLINEIN == 1 ]
    then
    	sed '0,/^\/\/\#define __SUPPORT_LINEIN_LOW_DELAY.*/s//\#define __SUPPORT_LINEIN_LOW_DELAY/' ../inc/case_independent.h > tmpfile
    	mv tmpfile ../inc/case_independent.h
    else
    	sed '0,/^\#define __SUPPORT_LINEIN_LOW_DELAY.*/s//\/\/\#define __SUPPORT_LINEIN_LOW_DELAY/' ../inc/case_independent.h > tmpfile
    	mv tmpfile ../inc/case_independent.h  	
    fi	
else
	sed '0,/BTMANAGER_SUPPORT_DEVICE_NUM.*;/s//BTMANAGER_SUPPORT_DEVICE_NUM         = 1[1~2,1];/' ./config_txt/$config_txt_name > tmpfile
	mv tmpfile ./config_txt/$config_txt_name  
	
	sed '0,/BTMANAGER_ENABLE_ANCS .*;/s//BTMANAGER_ENABLE_ANCS                = 1[0,1];/' ./config_txt/$config_txt_name > tmpfile
	mv tmpfile ./config_txt/$config_txt_name  	

	sed '0,/^\#define __SUPPORT_LINEIN_TWS.*/s//\/\/\#define __SUPPORT_LINEIN_TWS/' ../inc/case_independent.h > tmpfile
	mv tmpfile ../inc/case_independent.h  
	
    if [ $SupportLowDelayLINEIN == 1 ]
    then
    	echo "Not support low delay linein when SupportTWS == 0"
    	exit 1
    else
    	sed '0,/^\#define __SUPPORT_LINEIN_LOW_DELAY.*/s//\/\/\#define __SUPPORT_LINEIN_LOW_DELAY/' ../inc/case_independent.h > tmpfile
    	mv tmpfile ../inc/case_independent.h  	
    fi			
fi	


if [ $SupportESD == 1 ]
then     
	sed '0,/^\/\/\#define __ESD_MODE_.*/s//\#define __ESD_MODE_/' ../inc/case_independent.h > tmpfile
	mv tmpfile ../inc/case_independent.h
else
	sed '0,/^\#define __ESD_MODE_.*/s//\/\/\#define __ESD_MODE_/' ../inc/case_independent.h > tmpfile
	mv tmpfile ../inc/case_independent.h  	
fi

if [ $SupportFIXSAMPLE == 0 ]
then     
	sed '0,/^\/\/\#define	__PACFG339_H__.*/s//\#define	__PACFG339_H__/' ../drv/ccd/STA339BWS.h > tmpfile
	mv tmpfile ../drv/ccd/STA339BWS.h

	sed '0,/SETTING_AUDIO_SAMPLE_RATE_FIXED.*;/s//SETTING_AUDIO_SAMPLE_RATE_FIXED = 0[0,1];/' ./config_txt/$config_txt_name > tmpfile
	mv tmpfile ./config_txt/$config_txt_name 
else
	sed '0,/^\#define	__PACFG339_H__.*/s//\/\/\#define	__PACFG339_H__/' ../drv/ccd/STA339BWS.h > tmpfile
	mv tmpfile ../drv/ccd/STA339BWS.h  	
	
	sed '0,/SETTING_AUDIO_SAMPLE_RATE_FIXED.*;/s//SETTING_AUDIO_SAMPLE_RATE_FIXED = 1[0,1];/' ./config_txt/$config_txt_name > tmpfile
	mv tmpfile ./config_txt/$config_txt_name 

fi

if [ $SDKRelease == 1 ]
then     
	sed '0,/SETTING_UART_PRINT_ENABLE.*;/s//SETTING_UART_PRINT_ENABLE{124} = 0[0,1];/' ./config_txt/$config_txt_name > tmpfile
	mv tmpfile ./config_txt/$config_txt_name 
	
	sed '0,/SETTING_SYS_DEBUG_MODE_ENABLE.*;/s//SETTING_SYS_DEBUG_MODE_ENABLE{121} = 0[0,1];/' ./config_txt/$config_txt_name > tmpfile
	mv tmpfile ./config_txt/$config_txt_name 
	
	sed '0,/BTSTACK_LINKKEY_PRINT_ENABLE.*;/s//BTSTACK_LINKKEY_PRINT_ENABLE      = 0[0,1];/' ./config_txt/$config_txt_name > tmpfile
	mv tmpfile ./config_txt/$config_txt_name 	
else
	sed '0,/SETTING_UART_PRINT_ENABLE.*;/s//SETTING_UART_PRINT_ENABLE{124} = 1[0,1];/' ./config_txt/$config_txt_name > tmpfile		
	mv tmpfile ./config_txt/$config_txt_name 
	
	sed '0,/SETTING_SYS_DEBUG_MODE_ENABLE.*;/s//SETTING_SYS_DEBUG_MODE_ENABLE{121} = 1[0,1];/' ./config_txt/$config_txt_name > tmpfile
	mv tmpfile ./config_txt/$config_txt_name 
	
	sed '0,/BTSTACK_LINKKEY_PRINT_ENABLE.*;/s//BTSTACK_LINKKEY_PRINT_ENABLE      = 1[0,1];/' ./config_txt/$config_txt_name > tmpfile
	mv tmpfile ./config_txt/$config_txt_name 			
fi

	
#������������ж�Ҫ�޸ĵ�ͷ�ļ�λ��
if [ $BoardType = "$FW_NAME_EVB" ]
then 	
	#�޸�case_independent.hͷ�ļ�BOARD����ΪCASE_BOARD_EVB
	sed '0,/CASE_BOARD_TYPE.*CASE_BOARD.*/s//CASE_BOARD_TYPE         CASE_BOARD_EVB/' ../inc/case_independent.h > tmpfile
	mv tmpfile ../inc/case_independent.h    
elif [ $BoardType = "$FW_NAME_DVB_2825" ]
then 
	#�޸�case_independent.hͷ�ļ�BOARD����ΪCASE_BOARD_DVB_ATS2825
	sed '0,/CASE_BOARD_TYPE.*CASE_BOARD.*/s//CASE_BOARD_TYPE         CASE_BOARD_DVB_ATS2825/' ../inc/case_independent.h > tmpfile
	mv tmpfile ../inc/case_independent.h   	
elif [ $BoardType = "$FW_NAME_DVB_2823" ]
then 	
	#�޸�case_independent.hͷ�ļ�BOARD����ΪCASE_BOARD_DVB_ATS2823
	sed '0,/CASE_BOARD_TYPE.*CASE_BOARD.*/s//CASE_BOARD_TYPE         CASE_BOARD_DVB_ATS2823/' ../inc/case_independent.h > tmpfile
	mv tmpfile ../inc/case_independent.h  			
elif [ $BoardType = "$FW_NAME_EVB_2823" ]
then 	
	#�޸�case_independent.hͷ�ļ�BOARD����ΪCASE_BOARD_EVB_ATS2823
	sed '0,/CASE_BOARD_TYPE.*CASE_BOARD.*/s//CASE_BOARD_TYPE         CASE_BOARD_ATS2823/' ../inc/case_independent.h > tmpfile
	mv tmpfile ../inc/case_independent.h  	
elif [ $BoardType = "$FW_NAME_DEMO" ]
then 	
	#�޸�case_independent.hͷ�ļ�BOARD����ΪCASE_BOARD_DEMO
	sed '0,/CASE_BOARD_TYPE.*CASE_BOARD.*/s//CASE_BOARD_TYPE         CASE_BOARD_DEMO/' ../inc/case_independent.h > tmpfile
	mv tmpfile ../inc/case_independent.h  	
else
	echo "unknown case type!!!"
	echo "Support Type:US282A_BTBOX_EVB/US282A_BTBOX_DVB_ATS2825/US282A_BTBOX_DVB_ATS2823/US282A_BTBOX_EVB_ATS2823"
	exit 1
fi

if [ $SupportWaves == 1 ]
then
	sed '0,/^\/\/\#define WAVES_ASET_TOOLS/s//\#define WAVES_ASET_TOOLS/' ../inc/case_independent.h> tmpfile
	mv tmpfile ../inc/case_independent.h		 		    
else
	sed '0,/^\#define WAVES_ASET_TOOLS/s//\/\/\#define WAVES_ASET_TOOLS/' ../inc/case_independent.h > tmpfile
	mv tmpfile ../inc/case_independent.h  						
fi

#�ж�cfg_dae_txt_name�ļ��Ƿ���� -f��ʾ�ж��ļ� -d��ʾ�ж�Ŀ¼    
if [ ! -f "./config_txt/$cfg_dae_txt_name" ]
then
    echo "ConfigError: ./config_txt/$cfg_dae_txt_name not exist"
    exit 1;
fi 

if [ $SupportBQB == 1 ]
then
	sed '0,/^\/\/\#define __BQB_TEST_PROFILE_/s//\#define __BQB_TEST_PROFILE_/' ../inc/case_independent.h> tmpfile
	mv tmpfile ../inc/case_independent.h

	cp -f 	../ap/bluetooth_stack_BQB/bt_src_lib/*.o  ../ap/bluetooth_stack/bt_src_lib	
fi

#�ڱ���֮ǰ����config.h����֤����config.txt,�������������
echo "*************************Build config.bin******************************"
../tools/Gen_config/Genheader.exe       ./config_txt/$config_txt_name ./config_txt/$cfg_dae_txt_name     ../inc/config_id.h

#�����ʱ���ٲ���config.bin
#../tools/Gen_config/Genbin.exe          ./config_txt/$config_txt_name ./config_txt/$cfg_dae_txt_name     ./bin/config.bin

#�Ƚϲ�ͬ�������ɵ�config_id.h�Ƿ�һ�£������һ�£�˵����������©�ӵ��������Ҫ��ͬ��
#�������ֻ��һ�����ͣ�����ȥ�����²������
#echo "*************************Compare config.bin******************************"
#../tools/Gen_config/Genheader.exe     ./config_txt/US282A_BTBOX_EVB.txt   tmpfile   
#
#diff tmpfile ../inc/config_id.h
#if [ $? != 0 ]
#then
#  echo -e "config.bin is different!"
#	sleep 3s;
#	rm -f tmpfile
#	exit 1		
#else
#  echo -e "config.bin is same"
#  rm -f tmpfile
#fi	

#ת���ļ���ʽ
unix2dos ./config_txt/*
    


 
