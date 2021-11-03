#include "md5.h"


//获取单个文件的32位序列，参数文件名，序列存放在md5中
char *get_md5sum(char* filename, char* md5)
{
    if (filename == NULL || md5 == NULL)
    {
        return NULL;
    }

    //无名管道
    int fd[2] = {0};
    if (pipe(fd) == -1)
    {
        return NULL;
    }

    pid_t pid = fork();
    if(pid == -1)
    {
        return NULL;
    }

    if(pid == 0)
    {
        close(fd[0]);
        //存放md5 filename
        char* argv[5] = {0};
        char* s = "md5sum";
        argv[0] = s;
        argv[1] = filename;
    
        //子进程用无名管道的写端替换标准输出、标准错误输出
        dup2(fd[1], 1);
        dup2(fd[1], 2);

        execvp(s, argv);
        exit(0);
    }
    else
    {
        wait(NULL);
        close(fd[1]);
        char buff[64] = {0};
        int num = read(fd[0], buff, 32);
        if(strncmp(buff, "md5sum:", 7) == 0)
        {
            return NULL;
        }

        strcpy(md5, buff);
        
        return md5;
    }
}