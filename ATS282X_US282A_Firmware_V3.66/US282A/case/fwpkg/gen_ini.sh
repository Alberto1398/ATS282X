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
# �ýű���Ҫ����������������һ��������֧�ֵİ���(EVB, DVB_ATS2825, DVB_ATS2823, EVB_ATS2823)
# �ڶ����������Ƿ�֧��OTA
# �������������Ƿ�֧��TWS
# ĿǰOTA��TWS���ܲ���ͬʱ��

FW_NAME_EVB=US282A_BTBOX_EVB
FW_NAME_DEMO=US282A_BTBOX_DEMO
FW_NAME_DVB_2825=US282A_BTBOX_DVB_ATS2825
FW_NAME_DVB_2823=US282A_BTBOX_DVB_ATS2823
FW_NAME_EVB_2823=US282A_BTBOX_EVB_ATS2823

echo "generate depend files"

ini_name=temp.ini
touch ./$ini_name

echo "##############################################################" | tee ./$ini_name
echo "##            �̼������ļ�                                  ##" | tee -a ./$ini_name
echo "##            ����SDK��ع��ܵ�����                         ##" | tee -a ./$ini_name
echo "#    ���ļ���gen_ini.sh�Զ����ɣ��벻Ҫ�ֶ��༭             ##" | tee -a ./$ini_name
echo "##############################################################" | tee -a ./$ini_name
echo -e '\n' | tee -a ./$ini_name

#########ѡ������ʹ�õ�fwimage.cfg######################
echo "��ѡ��fwimage�ļ���������ű�ʾ:"
filelist=`ls ./config_txt/*.cfg`
idx=0
for file in $filelist
do 
    echo [$idx] $(basename $file) 
    let idx=$idx+1
done

read -n1 selno
echo -e '\n'
idx=0
for file in $filelist
do 
    if [ $idx == $selno ]
    then
        cfgname=$(basename $file)
	fi
	let idx=$idx+1
done
echo "fwimage_cfg=$cfgname" | tee -a $ini_name

#########ѡ������ʹ�õ�config.txt######################
echo "��ѡ��config.txt�ļ���������ű�ʾ:"
filelist=`ls ./config_txt/*.txt`
idx=0
for file in $filelist
do 
    echo [$idx] $(basename $file) 
    let idx=$idx+1
done

read -n1 selno
echo -e '\n'

idx=0
for file in $filelist
do 
    if [ $idx == $selno ]
    then
        cfgname=$(basename $file)
	fi
	let idx=$idx+1
done

echo "config_txt=$cfgname" |tee -a $ini_name

#########ѡ��֧�ֵİ���######################
echo "Enter Board type:"
echo "[0] US282A_BTBOX_EVB"
echo "[1] US282A_BTBOX_DEMO"
echo "[2] CASE_BOARD_ATS2823"
echo "[3] CASE_BOARD_DVB_ATS2825"
echo "[4] CASE_BOARD_DVB_ATS2823"
    
read -n1 BoardNum
  
case $BoardNum in
0)
      BoardType=$FW_NAME_EVB
      echo "BoardType=$FW_NAME_EVB" | tee -a $ini_name;; 
1)
      BoardType=$FW_NAME_DEMO
      echo "BoardType=$FW_NAME_DEMO" | tee -a $ini_name;;  
2)
      BoardType=$FW_NAME_EVB_2823
      echo "BoardType=$FW_NAME_EVB_2823" | tee -a $ini_name;;             
3)
      BoardType=$FW_NAME_DVB_2825
      echo "BoardType=$FW_NAME_DVB_2825" | tee -a $ini_name;;
4)
      BoardType=$FW_NAME_DVB_2823
      echo "BoardType=$FW_NAME_DVB_2823" | tee -a $ini_name;;           
*)
      echo "error Board type:$BoardType"
      exit 1;;
esac

#########ѡ�����ɹ̼�����######################
echo "Enter MakeFW type:"
echo "[0] All(include *.fw *.hex *.atf)"
echo "[1] FW"
echo "[2] FW+CheckSize"
echo "[3] FW+CheckSize+HEX"
    
read -n1 SelNum
  
case $SelNum in
0)
      echo "FWType=0" | tee -a $ini_name;; 
1)
      echo "FWType=1" | tee -a $ini_name;;  
2)
      echo "FWType=2" | tee -a $ini_name;; 
3)
      echo "FWType=3" | tee -a $ini_name;;                           
*)
      echo "error MakeFW type:$SelNum"
      exit 1;;
esac

#########�Ƿ�֧��OTA######################
read -n1 -p "Support OTA or Not:[y/n]" SupportOTA

echo -e '\n'

case $SupportOTA in
Y|y)
      echo "SupportOTA=1" | tee -a $ini_name
      echo "SDK support OTA";;
N|n)
      echo "SupportOTA=0" | tee -a $ini_name
      echo "SDK Not support OTA";; 
*)
      echo "error choice"
      exit 1;;    
esac

#########�Ƿ�֧��TWS######################
read -n1 -p "Support TWS or Not:[y/n]" SupportTWS

echo -e '\n'
case $SupportTWS in
Y|y)
      echo "SupportTWS=1" | tee -a $ini_name
      echo "SDK support TWS";; 	
N|n)
      echo "SupportTWS=0" | tee -a $ini_name
      echo "SDK Not support TWS";; 
*)
      echo "error choice"
      exit 1;;
esac

#########�Ƿ�֧��WAVES��Ч######################
read -n1 -p "Support waves sound effect:[y/n]" SupportWaves

echo -e '\n'
case $SupportWaves in
Y|y)
      echo "SupportWaves=1" | tee -a $ini_name;;  	
N|n)
      echo "SupportWaves=0" | tee -a $ini_name;;
*)
      echo "error choice"
      exit 1;;
esac

#########�Ƿ�֧��SPP/BLE######################
read -n1 -p "Support SPP/BLE:[y/n]" SupportSPPBLE

echo -e '\n'
case $SupportSPPBLE in
Y|y)
      echo "SupportSPPBLE=1" | tee -a $ini_name;;  	
N|n)
      echo "SupportSPPBLE=0" | tee -a $ini_name;;
*)
      echo "error choice"
      exit 1;;
esac

#########�Ƿ��ESD����######################
read -n1 -p "Support ESD:[y/n]" SupportESD

echo -e '\n'
case $SupportESD in
Y|y)
      echo "SupportESD=1" | tee -a $ini_name;;  	
N|n)
      echo "SupportESD=0" | tee -a $ini_name;;
*)
      echo "error choice"
      exit 1;;
esac

#########�Ƿ��Ƿ����汾######################
read -n1 -p "SDKRelease:[y/n]" SDKRelease

echo -e '\n'
case $SDKRelease in
Y|y)
      echo "SDKRelease=1" | tee -a $ini_name;;  	
N|n)
      echo "SDKRelease=0" | tee -a $ini_name;;
*)
      echo "error choice"
      exit 1;;
esac

#########����DAE��Ч�����ļ�����######################
read -p "������DAE�����ļ���(������׺):" dae_name 

echo "cfg_dae_txt=$dae_name" | tee -a $ini_name

#########���ݹ������ɹ̼��ļ���######################
##�����Ǹ���FWNAME + [_OTA] + [_TWS] + [_W]
##�������ɵ�ʱ�򻹿��Լ�������������

read -p "������ini�ļ���:" fw_name 
#fw_name=$BoardType

if [ $SupportOTA == y ]
then
    fw_name="$fw_name"_OTA
fi

if [ $SupportTWS == y ]
then
    fw_name="$fw_name"_TWS
fi

if [ $SupportWaves == y ]
then
    fw_name="$fw_name"_W
fi

if [ $SupportSPPBLE == y ]
then
    fw_name="$fw_name"_SPP
fi

echo "Firmware_name=$fw_name" | tee -a $ini_name

mv ./$ini_name ./config_txt/"$fw_name".ini
		