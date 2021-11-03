#include "md5sum_check.h"
#include "getfilename.h"


//获取path文件夹下所有文件的md5序列,存放在filemd5中
int get_all_filemd5(char* path, char* filename[], char* filemd5[])
{
    if(path == NULL|| filemd5 == NULL)
    return -1;

    //count普通文件的个数
    int count = get_file_name(path, filename);
    //获取所有文件名存放在filemd5中
    for( int i = 0; i < count; i++)
    {
       //现在filemd5[]中存放的是文件名,第一个参数文件名，第二个参数使用文件名用过的空间地址
        get_md5sum(filename[i], filemd5[i]);
    }

  
    return count;
}

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
        read(fd[0], buff, 32);
        strcpy(md5, buff);
        
        return md5;
    }
}
//文件是否重复，实现快传,返回重复的文件名
char* is_same(int c, char *md5, char* filename[], char* serfilemd5[])
{
    if (md5 == NULL || filename == NULL || serfilemd5 == NULL)
    {
        //erro4可以作为能否上传的依据
        send(c, "err04", 5, 0);
        return NULL;
    }

    //读取md5文件,先留着，后期实现   
    char* path = ".";
    int count = get_all_filemd5(path, filename, serfilemd5);
    for(int i = 0; i < count; i++)
    {
        //如果上传的文件md5值已存在，就做个硬链接
       if(strncmp(md5, serfilemd5[i], 32) == 0)
       {
           return filename[i];
       }
    }
    
    return NULL;
}