
#include "app_alarm.h"


#if 1

int sdd_open(sdd_open_arg_t* arg)
{
    g_loop_play_ms = arg->loop_interval_ms;
    return ((int)sys_sd_fopen(arg->file_name));
}



//SD��չ��������,�ص����������ܷ���bank

int sdd_read(uint8*buff, uint32 size, uint32 len, storage_io_t *io)
{
    uint32 read_total = 0;
    int ret = 0;
    sd_file_t *fp = io->handel;
    uint32 f_size = (fp->endaddr - fp->startaddr) + 1;
    uint8 seek_cnt = 0;
    uint32 mp3_id3_size = *io->loop_play.mp3_id3_size;

    read_again:
    
    ret = (int32) sys_sd_fread(fp, buff, len);
    if(g_loop_play_ms != 0)
    {
        goto jump_end;
    }
    read_total = sys_sd_ftell(fp) - fp->startaddr;//tell���ַ�ʽ���ɿ� 

    //seek�󣬿��ܲ���512���룬��� �����ݲ�0
    if((ret < len) && (ret > 0))
    {
        libc_memset(buff + ret, 0x00, len - ret);
        ret = len;
    }
    
    if((read_total <= (mp3_id3_size + 2048))||(f_size <= (read_total + 2048)))
    {
        *io->loop_play.clr_Energy = LOOP_PLAY_ENERGY;//�޷첥������ֵ
    }
    else
    {
        *io->loop_play.clr_Energy = 0;
    }
    if(f_size <= read_total)//��ǰseek
    {
        sys_sd_fseek(fp, SEEK_SET, mp3_id3_size);
    }
    
    if((ret < 1) && (seek_cnt < 3))
    {
        seek_cnt++;
        goto read_again;
    }
    jump_end: 
    return ret;
}



//SD��չ����������seek,�ص����������ܷ���bank
int sdd_seek(storage_io_t *io, int32 ofs, int32 whence)
{
    return ((int32)sys_sd_fseek(io->handel, whence, ofs));
}


//SD��չ����������tell�����ߵ�ǰ��λ��
//�ص����������ܷ���bank
int sdd_tell(storage_io_t *io)
{
    return ((int32) sys_sd_ftell(io->handel));
}

#endif

