#include "eh_id3.h"

/******************************************************************************/
/*
 * \par  Description:����aac���͵�id3��Ϣ
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
bool get_id3_aac(void)
{
    if (FALSE != get_id3_aax())
    {
        return TRUE;
    }

    vfs_file_seek(eh_vfs_mount, 0, SEEK_SET, id3_handle);
    if (FALSE != get_id3_mp3())
    {
        return TRUE;
    }
    vfs_file_seek(eh_vfs_mount, 0, SEEK_SET, id3_handle);
    return get_id3_ape();
}

