/********************************************************************************
*                              5003
*                            Module: storage io
*                 Copyright(c) 2003-2008 Actions Semiconductor,
*                            All Rights Reserved.
*
* History:
*      <author>    <time>           <version >             <desc>
*       kkli     2008-09-02 10:00     1.0             build this file
********************************************************************************/
/*!
* \file     storageio.h
* \brief    ����Դ洢�豸����������ӿ�
* \author   kkli
* \version 1.0
* \date  2008/09/02
*******************************************************************************/
#ifndef __STORAGEIO_H__
#define __STORAGEIO_H__

#define LOOP_PLAY_ENERGY  400 //�޷첥�Ź��������ż�ֵ



typedef struct{
    char *file_name;//��չ�����ļ����ļ���
    uint16 id;//�ļ���id
    uint16 loop_interval_ms;//���׸���ʱ�䣬Ϊ0������
}sdd_open_arg_t;



//�޷첥������ֵ
typedef struct{
    uint32 *mp3_id3_size;//mp3��id3��С��Ҳ��mp3���������ݿ�ʼ��ַ
    uint32 *clr_Energy;//��������ֵ����Ҫʵʱ����
}sdd_loop_play_t;


/*!
 * \brief
 *      �ṩ�ͱ�׼�ļ�ϵͳ�����ӿ�һ�µĴ洢�豸��������ӿ�
 */
typedef struct storage_io_s {
    /*! ������ ,����ʵ�ʶ����Ĵ�С�����귵��-1*/
    int (*read)(void *buffer, int size, int len, struct storage_io_s *io);
    /*! д���� */
    int (*write)(void *buffer, int size, int len, struct storage_io_s *io);
    /*! ��λ */
    int (*seek)(struct storage_io_s *io, int offset, int whence);
    /*! �ļ�λ�� */
    int (*tell)(struct storage_io_s *io);
    /*�ر��ļ�*/
   // int (*close)(struct storage_io_s *io);
    /*���ļ�*/
    int (*open)(void*arg);
    /*open�����Ĳ���*/
    void* open_arg;
    
    /* �ļ����� 0: VFS�ļ���1��SD�ļ� */
    int file_type;
    /* �ļ���� */
    void *handel;

    sdd_loop_play_t loop_play;
    
} storage_io_t;

extern sdd_open_arg_t g_sdd_open_parm;

extern int sdd_open(sdd_open_arg_t* arg);
extern int sdd_read(uint8*buff, uint32 size, uint32 len, storage_io_t *io);
extern int sdd_seek(storage_io_t *io, int32 ofs, int32 whence) ;       
extern int sdd_tell(storage_io_t *io);


#endif // __STORAGEIO_H__
