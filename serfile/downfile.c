#include "downfile.h"

//下载文件
int down_file(int c, char* filename)
{
    //文件名为空
    if(filename == NULL)
    {
        send(c, "err04", 5, 0);
        return -1;
    }

    int fd = open(filename, O_RDONLY);
    if(fd == -1)
    {
        send(c, "err05", 5, 0);
        return -1;
    }

    
    //获取文件的总大小
    /* lseek()
    重新定位与文件关联的打开文件描述的文件偏移量
    使用该方法需要将光标回退至开始
    lstat();
    int fstat(int fd, struct stat *buf);
    */
    struct stat filebuff;
    fstat(fd, &filebuff);
    int filesize = filebuff.st_size;

    char status[32] = {0};
    sprintf(status, "ok#%d byte(y/n)?", filesize);
    send(c, status, strlen(status), 0);

    //接收客户端的回复y/n
    memset(status, 0, 32);
    int res = recv(c, status, 32, 0);
    if(res == -1)
    {
        printf("one client close\n");
        close(fd);
        return -1;
    }

    if(strcmp(status, "n") == 0)
    {
        close(fd);
        return -1;
    }

    if(strcmp(status, "y") == 0)
    {
        char databuff[1024] = {0};
        int num = 0;
        while((num = read(fd, databuff, 1024)) > 0)
        {
            send(c, databuff, num, 0);
        }
        close(fd);
        return 0;
    }
    else
    {
        send(c,"err06", 5, 0);
        close(fd);
        return -1;
    }
}