#include "app_radio.h"

//����Standby
bool radio_modual_standby(void)
{
    bool bret = TRUE;
    msg_apps_t msg;

    //FM ģ�����standby
    msg.type = MSG_FMENGINE_STANDBY;
    //�����첽��Ϣ
    bret = (bool) (send_async_msg(APP_ID_FMENGINE, &msg) == ERR_NO_ERR);
    return bret;
}
