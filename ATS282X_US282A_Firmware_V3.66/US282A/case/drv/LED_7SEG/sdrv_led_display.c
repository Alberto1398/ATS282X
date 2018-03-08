
/*******************************************************************************
*                              US282A
*                 Copyright(c) 2014-2015 Actions (Zhuhai) Microelectronics Co., Limited,
*                            All Rights Reserved.
*    brief    ��ʾLED����AP����
*   ICON���ϵ�ĳһ����������ICON˳��ת����,0xff��ʾ��������û�д�ICON
*   ��������ֵΪ��������ICON������˳�򣻴˱�˳������ʾӲ��ģ���б�˳��һ��
*      <author>       <time>           
*       Wekan       2015-3-26pm            
*******************************************************************************/


#include "led_inner.h"


//04311�а���˳��ΪK1-K6; LED_PLAY->LED_PAUSE->LED_USB->
//                        LED_SD->LCD_COL->LED_AUX->LED_FM
//for 04311
// #define LED_SD  S1   //0
// #define LED_USB S2   //1
// #define LED_AUX S6   //5
// #define LCD_COL S7   //6
// #define LED_FM  S8   //7 ;"FM"��"."Ϊͬһ��ICON��ͬʱ��/��
// #define LED_PLAY S17 //16
// #define LED_PAUSE S18    //17

//7 segment matrix
/********************************************************
 -----------------------------------------
 7    6    5    4    3    2    1    0
 ------------------------------------------
 6    /    4A   K8   /    K6   /    /
 ------------------------------------------
 5   4B    /    4F   4D   3E   K3   3D
 -----------------------------------------
 4   4G   4C    /    3A   3G   2C   1D
 -----------------------------------------
 3   /    3E   3C    /    3F   2G   1C
 -----------------------------------------
 2   K7   K2   3B   K5    /    2F   1G
 -----------------------------------------
 1   /    2D   2E   2B    2A   /    1F
 -----------------------------------------
 0   /    K1   K4   1E    1B   1A    /
 -----------------------------------------
 *******************************************************/

uint8 cur_disval[8] =
{ 0 };

//�������������iconΪMP3,FM��ģ��//������demo��
const uint8 icon_set[ICON_TOTAL] =
{ 0x00, 0x02, 0xff, 0xff, 0xff, 0xff, 0x04, 0x01, 0xff, 0xff, 0xff, 0xff, 0x05, 0x07, 0xff, 0xff, 0x06, 0x03 };

const uint8 seg_val[] =
{
    0x3f, //number0
    0x06, //number1
    0x5b, //number2
    0x4f, //number3
    0x66, //number4
    0x6d, //number5
    0x7d, //number6
    0x07, //number7
    0x7f, //number8
    0x6f, //number9
    0x77, //'A'
    0x7f, //'b'
    0x39, //'C'
    0x3f, //'d'
    0x79, //'E'
    0x71, //'F'
    0x7d, //'G'
    0x76, //'H'
    0x06, //'I'
    0x0f, //'J'
    0x00, //'K'����ʾ
    0x38, //'L'
    0x00, //'M'����ʾ
    0x37, //'n'	big 0x37
    0x3f, //'O'
    0x73, //'P'
    0x67, //'Q'
    0x72, //'R'
    0x6d, //'S'
    0x78, //'T'
    0x3e, //'U'
    0x3e, //'V'
    0x00, //'W'
    0x00, //'X'
    0x6e, //'y'
    0x5b, //'Z'
    0x7C, //'b'
    0x40 //error'-'
};

//icon show or hide vals
const uint8 icon_vals[8][2] =
{
    { 0x20, 0x00 }, //k1 byte 5 bit0 TF
    { 0x20, 0x02 }, //k2 byte 5 bit2 FM
    { 0x02, 0x05 }, //k3 byte 1 bit5 USB
    { 0x10, 0x00 }, //k4 byte 4 bit0 BAT2
    { 0x08, 0x02 }, //k5 byte 3 bit2 COL
    { 0x04, 0x06 }, //k6 byte 2 bit6 BAT1
    { 0x40, 0x02 }, //k7 byte 6 bit2
    { 0x10, 0x06 } //k8 byte 4 bit6  DOT
};

//number display vals 4 number and 7 segment
const uint8 num_vals[4][7][2] =
{
    //first layer number 0
    {
        //countent 7segment
        { 0x02, 0x00 }, //1a byte 0 bit1
        { 0x04, 0x00 }, //lb byte 0 bit2
        { 0x01, 0x03 }, //lc byte 3 bit0
        { 0x01, 0x04 }, //ld byte 4 bit0
        { 0x08, 0x00 }, //le byte 0 bit3
        { 0x01, 0x01 }, //lf byte 1 bit0
        { 0x01, 0x02 }//lg byte 2 bit0
    }, //endof number 0

    //first layer number 1
    {
        //countent 7segment
        { 0x04, 0x01 }, //2a byte 1 bit2
        { 0x08, 0x01 }, //2b byte 1 bit3
        { 0x02, 0x04 }, //2c byte 4 bit1
        { 0x20, 0x01 }, //2d byte 1 bit5
        { 0x10, 0x01 }, //2e byte 1 bit4
        { 0x02, 0x02 }, //2f byte 2 bit1
        { 0x02, 0x03 } //2g byte 3 bit1
    },//end of number 1
    //first layer number 2
    {
        //countent 7segment
        { 0x08, 0x04 }, //3a byte 4 bit3
        { 0x10, 0x02 }, //3b byte 2 bit4
        { 0x10, 0x03 }, //3c byte 3 bit4
        { 0x01, 0x05 }, //3d byte 5 bit0
        { 0x04, 0x05 }, //3e byte 5 bit2
        { 0x04, 0x03 }, //3f byte 3 bit2
        { 0x04, 0x04 } //3g byte 4 bit2
    },//end of number 2
    //first layer number 3
    {
        //countent 7segment
        { 0x20, 0x06 }, //4a byte 6 bit5
        { 0x40, 0x05 }, //4b byte 5 bit6
        { 0x20, 0x04 }, //4c byte 4 bit5
        { 0x08, 0x05 }, //4d byte 5 bit3
        { 0x20, 0x03 }, //4e byte 3 bit5
        { 0x10, 0x05 }, //4f byte 5 bit4
        { 0x40, 0x04 } //4g byte 4 bit6
    }//end of number 3
};

/******************************************************************************
* \par  Description: �������룬��ʾ�����ض���������Ӧ�Ķ�
*     addr: segment address
*     content: show content;0xff��ʾΪICON
*     type: signature it is show or hide��1-show��0-hide
* \param[out]  none
* \return      none
*   <author>    <time>     
*    Wekan   2015-3-26pm  
*******************************************************************************/

void sdrv_led_display(uint8 addr, uint8 content, uint8 type)
{
    uint8 dis_val;
    uint8 real_addr, i;
    uint8 tab_index;
    //judge it is icon or number
    if (content == 0xff)
    {
        //icon number to real index of icons in led
        real_addr = icon_set[addr];
        //get table values index --- the true table byte idnex
        tab_index = icon_vals[real_addr][1];

        if (type == 1)
        {
            //display or
            cur_disval[tab_index] |= icon_vals[real_addr][0];
        }
        else
        {
            //hide and
            cur_disval[tab_index] &= (~icon_vals[real_addr][0]);
        }
    }
    else
    {
        //get number display address
        if (type == 0)
        {
            //hide
            dis_val = 0;
        }
        else
        {
            //show
            dis_val = seg_val[content];
        }

        //get number display value
        for (i = 0; i < 7; i++)
        {
            //byte numbers
            tab_index = num_vals[addr][i][1];

            if ((dis_val & (1 << i)) != 0)
            {
                //show
                cur_disval[tab_index] |= num_vals[addr][i][0];
            }
            else
            {
                //hide
                cur_disval[tab_index] &= (~num_vals[addr][i][0]);
            }

        }

    }
    hdrv_data_send((uint32*) cur_disval);
}
