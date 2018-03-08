#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>

char f_buffer[1024*1024];

int main()
{
    FILE *in, *in_flist, *out;
    char file_line[128];
    char pcm_file[32];
    char ch;
    long in_len;
    int n = 0;
    
    in_flist = fopen("pcm_flist.txt", "r");
    if (in_flist == NULL)
    {
    	printf("�� pcm_flist.txt ����\n");
    	return 0;
    }
    
    while(1)
    {
        if (feof(in_flist))
        {
            break;
        }
        
        if (fgets(file_line, 127, in_flist) == NULL)
        {
            break;
        }
        
        //��ȡһ���ļ���
        n = 0;
        while (1)
        {
            if ((file_line[n] == '\n') || (file_line[n] == '\r') || (file_line[n] == 0))
            {
                pcm_file[n] = 0;
                break;
            }
            
            pcm_file[n] = file_line[n];
            n++;
        }

        if (strstr(pcm_file, ".pcm") == NULL)
		{
            printf("pcm_flist.txt ����\n");
            break;
        }
        
        in = fopen(pcm_file, "rb");
        if (in == NULL)
        {
            printf("[%s] ", pcm_file);
            printf("û�д��ļ���\n");
            break;
        }
        
        fseek(in, 0, 2);
        in_len = ftell(in);
        fseek(in, 0, 0);
        
        fread(f_buffer, in_len, 1, in);
        
        fclose(in);
        
        out = fopen(pcm_file, "wb");
        if (out == NULL)
        {
            printf("���� [%s] ʧ�ܣ�\n", pcm_file);
            break;
        }
        
        f_buffer[0] = in_len&0xff;
        f_buffer[1] = (in_len&0xff00)>>8;
        f_buffer[2] = (in_len&0xff0000)>>16;
        f_buffer[3] = (in_len&0xff000000)>>24;
        
        for (n = 0; n < in_len; n++)
        {
        	fputc(f_buffer[n], out);
        }
        
        fclose(out);
        
        printf("[%s] ��ɣ�\n", pcm_file);
    }
    
    fclose(in_flist);
    
    printf("��������˳�...");
	ch = getchar();

    return 0;
}
