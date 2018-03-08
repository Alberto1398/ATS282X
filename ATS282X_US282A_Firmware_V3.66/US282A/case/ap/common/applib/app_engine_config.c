/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ����������Ӧ�ù�����ģ�飬ͳһ�����������ͺ�APP ID��ת����
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "applib.h"

typedef struct
{
    uint8 engine_app_id;
    uint8 engine_type;
} app2engine_type_t;

const app2engine_type_t applib_app2eg_type[] =
{
    { APP_ID_MENGINE, ENGINE_MUSIC },
    { APP_ID_FMENGINE, ENGINE_RADIO },
    { APP_ID_LINEIN_EG, ENGINE_LINEIN },
    { APP_ID_BTPLAYEG, ENGINE_BTPLAY },
    { APP_ID_BTCALLEG, ENGINE_BTCALL },
    { APP_ID_UENGINE, ENGINE_USOUND },
};

//�ж��Ƿ�����
engine_type_e get_engine_type_by_appid(uint8 app_id)
{
    uint8 i;

    for (i = 0; i < (sizeof(applib_app2eg_type) / sizeof(app2engine_type_t)); i++)
    {
        if (applib_app2eg_type[i].engine_app_id == app_id)
        {
            return applib_app2eg_type[i].engine_type;
        }
    }

    return ENGINE_NULL;
}

//��ȡ�����Ӧ app id
uint8 get_engine_appid_by_type(engine_type_e engine_type)
{
    uint8 i;

    if (engine_type == ENGINE_NULL)
    {
        return APP_ID_MAX;
    }

    for (i = 0; i < (sizeof(applib_app2eg_type) / sizeof(app2engine_type_t)); i++)
    {
        if (applib_app2eg_type[i].engine_type == engine_type)
        {
            return applib_app2eg_type[i].engine_app_id;
        }
    }

    return APP_ID_MAX;
}

