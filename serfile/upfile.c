#include "upfile.h"
#include "md5sum_check.h"
#include "getfilename.h"
#define FILECOUNT 1000

//上传文件
int up_file(int c, char *filename, char *md5, char *filesize)
{
    if (c == -1 || filename == NULL || md5 == NULL || filesize == NULL)
    {
        return -1;
    }

    //存放文件名
    char **filenamebuff = allocate_memory(FILECOUNT);
    //存放文件名对应的md5
    char **serfilemd5buff = allocate_memory(FILECOUNT);
    //检索文件是否重复,p保存重复的文件名
    char *p = is_same(c, md5, filenamebuff, serfilemd5buff);

    if (p != NULL)
    {
        char repeatbuff[64] = "warn:file existed y/n?";
        //重复
        send(c, repeatbuff, strlen(repeatbuff), 0);
        memset(repeatbuff, 0, sizeof(repeatbuff));
        recv(c, repeatbuff, 40, 0);
        if (strncmp(repeatbuff, "y", 1) == 0)
        {
            //如果上传的文件存在且文件名相同，直接up ok
            if (strcmp(p, filename) == 0)
            {
                send(c, "up ok", 5, 0);
                //释放内存空间
                free_memory(filenamebuff, FILECOUNT);
                free_memory(serfilemd5buff, FILECOUNT);
                return 0;
            }
            //制作硬链接
            int res = hard_link(p, filename);
            //硬链接失败
            if (res == -1)
            {
                char *err = "ln error,file exsited";
                send(c, err, strlen(err), 0);
                //释放内存空间
                free_memory(filenamebuff, FILECOUNT);
                free_memory(serfilemd5buff, FILECOUNT);
                return -1;
            }

            //释放内存空间
            free_memory(filenamebuff, FILECOUNT);
            free_memory(serfilemd5buff, FILECOUNT);
            //发送成功
            send(c, "up ok", 5, 0);
            return 0;
        }
        return -1;
    }
    //不重复
    if (-1 == create_new_file(c, filename, filesize))
    {
        return -1;
    }
    send(c, "up ok", 5, 0);
    return 0;
}

//创建新文件
int create_new_file(int c, char *filename, char *filesize)
{
    if (filename == NULL || filesize == NULL)
    {
        return -1;
    }

    int fd = open(filename, O_WRONLY | O_CREAT, 0600);
    if (fd == -1)
    {
        return -1;
    }
    //向客户端发送文件已经打开的消息
    send(c, "ready", 5, 0);

    int size = atoi(filesize);
    int cur_size = 0;

    while (1)
    {
        char databuff[1024] = {0};
        int num = recv(c, databuff, 1024, 0);

        write(fd, databuff, num);
        cur_size += num;

        if (cur_size >= size)
        {
            break;
        }
    }

    close(fd);

    return 0;
}

//给已有文件制作软链接
int hard_link(char *src_filename, char *des_filename)
{
    if (src_filename == NULL || des_filename == NULL)
    {
        return -1;
    }
    int fd[2];
    int res = pipe(fd);
    if (res == -1)
    {
        return -1;
    }
    pid_t pid = fork();
    //创建子进程去执行硬链接操作
    if (-1 == pid)
    {
        return -1;
    }

    if (0 == pid)
    {
        close(fd[0]);
        dup2(fd[1], 1);
        dup2(fd[1], 2);
        char *argv[5] = {0};
        char *s = "ln";
        argv[0] = s;
        argv[1] = "-s";
        //argv[1]源文件名
        argv[2] = src_filename;
        //argv[3]目标文件名
        argv[3] = des_filename;

        //int execvp(const char *file, char *const argv[]);
        //参数1：命令文件，参数二：命令的参数
        execvp(s, argv);

        exit(0);
    }
    else
    {
        close(fd[1]);
        wait(NULL);
        char buff[128] = {0};
        int num = read(fd[0], buff, 127);
        if (num > 0)
        {
            return -1;
        }
        return 0;
    }
}