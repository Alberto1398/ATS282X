#!/bin/bash

# This script build these files:
# *.FW---> firmware 
# *.OTA--> OTA firmare
# *.HEX--> upgrade file for end user
# *.FWU--> upgrade file for factory product
# *.ATF--> autotest file for ATT
# 2016-06-12	-	wuyufan

# �ýű��ļ�������������������fw�ļ���OTA�ļ���HEX�����ļ��Լ�ATT���������atf�ļ�
# ͨ��makefile���øýű�������Ҫ����һ����������Ӧ��ini�ļ�

echo "making fw"

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

#��ȡ�̼�������
FW_NAME=`grep -a "Firmware_name" $int_file | awk -F '=' '{print $2}'`

FW_DIR=$FW_NAME

#�ɸ���ʵ����Ҫȷ���̼����Ƿ���Ҫ��ʱ����Ϣ
#FW_NAME="$FW_NAME"_`date +%y%m%d`

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

FWType=`grep -a "FWType" $int_file | awk -F '=' '{print $2}'`

#��ȡdae.txt����
cfg_dae_txt_name=`grep -a "cfg_dae_txt" $int_file | awk -F '=' '{print $2}'`

#�ۺ�atf�ļ���ԭ�е�fw�ļ��γ��µ�fw�ļ�
FW_ALL_NAME=${FW_NAME%}"_ALL"

echo $FW_EXT_NAME

function error_exit()
{
    echo "!!!Warning: Build Error"
    echo "Press [Ctrl+C] to exit..."
    while true;
    do
        sleep 5;
    done    
}

#�ж�config_txt_name�ļ��Ƿ���� -f��ʾ�ж��ļ� -d��ʾ�ж�Ŀ¼    
if [ ! -f "./config_txt/$config_txt_name" ]
then
    echo "ConfigError: ./config_txt/$config_txt_name not exist"
    error_exit;
fi   

#�ж�config_cfg_name�ļ��Ƿ���� -f��ʾ�ж��ļ� -d��ʾ�ж�Ŀ¼  
if [ ! -f "./config_txt/$config_cfg_name" ]
then
    echo "ConfigError: ./config_txt/$config_cfg_name not exist"
    error_exit;
fi 

#�ж�֮ǰ����Ľ���Ƿ���ȷ,�������������ȷ���������̼����ɲ���
grep -a "compile error" make.log 
if [ $? == 0 ]
then
    echo "make error-->exit"
    error_exit;
fi

#�ж��ڴ��Ƿ���ȷ
grep -a "Error :section" make.log 
if [ $? == 0 ]
then
    echo "make error-->exit"
    error_exit;
fi

echo "Support OTA: $SupportOTA" 
echo "Support TWS: $SupportTWS" 
 
#if [ -d "../ap/bluetooth_stack" ]
#then
#    if [ $SupportTWS == 1 ]
#    then 
#        echo "copy btstack_tws.ap" 
#        cp -f ../ap/bluetooth_stack/btstack.ap ./ap/btstack_tws.ap        
#    else
#        echo "copy btstack_normal.ap" 
#        cp -f ../ap/bluetooth_stack/btstack.ap ./ap/btstack_normal.ap
#    fi
#fi

#if [ $SupportTWS == 1 ]
#then  
#    cp -f ./ap/btstack_tws.ap  ./ap/btstack.ap      
#else
#    cp -f ./ap/btstack_normal.ap ./ap/btstack.ap 
#fi

if [ $SupportWaves == 1 ]
then  
    cp -f ./dsplib/audiopp_waves.dsp  ./dsplib/audiopp.dsp      
else
    cp -f ./dsplib/audiopp_normal.dsp ./dsplib/audiopp.dsp   
fi

#�ж�cfg_dae_txt_name�ļ��Ƿ���� -f��ʾ�ж��ļ� -d��ʾ�ж�Ŀ¼    
if [ ! -f "./config_txt/$cfg_dae_txt_name" ]
then
    echo "ConfigError: ./config_txt/$cfg_dae_txt_name not exist"
    error_exit;
fi 

cp -f ./config_txt/$config_txt_name ./bin/config.txt
cp -f ./config_txt/$cfg_dae_txt_name ./bin/cfg_dae.txt
cp -f ./config_txt/$config_cfg_name ./fwimage.cfg

../tools/Gen_config/Genbin.exe          ./config_txt/$config_txt_name ./config_txt/$cfg_dae_txt_name        ./bin/config.bin

#���³���������һ���汾�Ž��µĹ̼���OTA�ļ�������OTA�̼��������ԣ������ļ��汾�ű��벻һ�£�����MODULE_NUM
#����һ��
#���������config_txtĿ¼���Ƿ���������ļ���+"_ver_new"�������ļ���������ڣ�������¹̼�������
#�ⲿ�ֳ����ǲ��Գ��򣬷���OTA���ԣ���ʽ���������ȥ���ⲿ�ֹ̼��Ĵ��
OTA_new_cfg=0
if [ $SupportOTA == 1 ]
then
    new_config_cfg_name=${config_cfg_name%.cfg}"_ver_new.cfg"
    echo $new_config_cfg_name
    if [ -f "./config_txt/$new_config_cfg_name" ]
    then
        OTA_new_cfg=1
    else
        OTA_new_cfg=0
    fi 
fi

chgrp Users -R ./
chmod -R 777 ./
mkdir -p $FW_DIR

echo "*************************Reduce AP Size******************************"
../../psp_rel/tools/ap_post_builder.exe ./fwimage.cfg config.ap


echo "*************************Build AFI *****************************"
if [ -f "../../psp_rel/bin_original/buildAFI.bat" ]
then
    chgrp Users ../../psp_rel/bin_original/.*
    chmod -R 777 ../../psp_rel/bin_original/.*
    ./buildAFI.bat
fi    
				
echo "*************************Build Firmware *****************************"
../../psp_rel/tools/maker_py/Maker.exe -c ./fwimage.cfg -o  ./$FW_NAME.FW -mf

if [ $OTA_new_cfg == 1 ]
then
    cp -f ./config_txt/$new_config_cfg_name ./fwimage.cfg 
    ../../psp_rel/tools/maker_py/Maker.exe -c ./fwimage.cfg -o  ./"$FW_NAME"_ver_new.FW -mf   
fi

#rm -f ./bin/config.txt
#rm -f ./bin/config.bin
rm -f ./fwimage.cfg

if [ $FWType == 1 ]
then
    mv -f $FW_NAME.FW ./$FW_DIR
    exit 0;
fi 

echo "*************************CheckSize *****************************"	
../../psp_rel/tools/maker_py/FW2X.exe -fsss $FW_NAME.FW MakeBin FLASHSIZE 123

grep -a "ERROR!! Firmware size is BIGGER than flash size" ./build.log
if [ $? == 0 ]
then
    echo "build error-->exit"
    error_exit;   
fi

echo $FWType
if [ $FWType == 2 ]
then
    exit 0;
fi    

echo "*************************Build UPGRADE.HEX*****************************"
../../psp_rel/tools/maker_py/FW2X.exe -fssf  $FW_NAME.FW  MakeBin MakeUPG $FW_NAME.FWU
cp -f $FW_NAME.FWU $FW_NAME.HEX

if [ $FWType == 3 ]
then
    mv -f $FW_NAME.FW ./$FW_DIR
    exit 0;
fi 

echo "*************************Build ATF*****************************"
if [ -d "../ap_test/" ]
then
    cp -f ./$FW_NAME.FWU ../ap_test/ATT/UPGRADE.HEX
    cp -f ./$FW_NAME.FW ../ap_test/ATT/US282A.FW
    cd ../ap_test/ATT
    pwd
    make clean -f Makefile
    make -f Makefile    
    cp -f actionstest.fw ../../fwpkg/$FW_DIR/$FW_ALL_NAME.FW  
    cd ../../fwpkg 
fi

cp $FW_NAME.FW TEST.FW

chmod 777 TEST.FW

mv -f $FW_NAME.FW $FW_NAME.FWU $FW_NAME.HEX ./$FW_DIR

#�ж��Ƿ�֧��OTA���ܣ�֧��OTA���ܵĹ̼�������OTA�ļ�
#FW2X f�������ܴ����·��
if [ $SupportOTA == 1 ]
then
    echo "*************************Build OTA*****************************"
    ../../psp_rel/tools/maker_py/FW2X.exe -fssf  TEST.FW  MakeBin MakeOTA $FW_NAME.OTA 
    mv -f $FW_NAME.OTA ./$FW_DIR
    rm -f TEST.FW
fi

if [ $OTA_new_cfg == 1 ]
then
    cp "$FW_NAME"_ver_new.FW TEST.FW
    chmod 777 TEST.FW
    mv -f "$FW_NAME"_ver_new.FW  ./$FW_DIR
    ../../psp_rel/tools/maker_py/FW2X.exe -fssf  TEST.FW  MakeBin MakeOTA "$FW_NAME"_ver_new.OTA 
    mv -f "$FW_NAME"_ver_new.OTA ./$FW_DIR
    rm -f TEST.FW    
fi

chmod -R 777 ./
	
