#include "downfile.h"

//发送get命令并接受回复
int send_reply(int sockfd, char *cmd, char *filename)
{
    if (cmd == NULL || filename == NULL)
    {
        return -1;
    }
    //发送 get a.txt
    send(sockfd, cmd, strlen(cmd), 0);

    char recv_buff[32] = {0};
    if (recv(sockfd, recv_buff, 32, 0) <= 0)
    {
        printf("ser close\n");
        return -1;
    }
    if (strncmp(recv_buff, "ok#", 3) != 0)
    {
        printf("file none\n");
        return -1;
    }
    printf("%s\n", recv_buff);

    //获取文件大小
    int filesize = atoi(recv_buff + 3);

    //先不管y/n,先打开创建文件，打开失败自动回复n
    int fd = open(filename, O_WRONLY | O_CREAT, 0600);
    //文件打开失败
    if (fd == -1)
    {
        printf("file open err\n");
        send(sockfd, "n", 1, 0);
        return -1;
    }

    //客户端文件成功打开并创建，然后回复服务器y/n
    char reply_buff[8] = {0};
    fgets(reply_buff, 8, stdin);
    //去掉回车
    reply_buff[strlen(reply_buff) - 1] = 0;
    if (strcmp(reply_buff, "y") == 0)
    {
        //同意下载且文件大小为0
        if (filesize == 0)
        {
            send(sockfd, "n", 1, 0);
            close(fd);
            return 0;
        }
        //文件大小不为0，传输数据
        else
        {
            send(sockfd, "y", 1, 0);
            accept_data(sockfd, fd, filesize, filename);
            return 0;
        }
    }

    //发送n或者其他
    else
    {
        send(sockfd, reply_buff, strlen(reply_buff), 0);
        return -1;
    }
}

//接收数据
void accept_data(int sockfd, int fd, int filesize, char* filename)
{
    if(fd == -1)
    {
        printf("file close\n");
        return;
    }
    //存放数据
    char databuff[1024] = {0};
    //每次读的字节数
    int recv_size = 0;

    //已经接受的文件大小
    int cur_size = 0;
    //百分比
    double bar = 0.0;

    while(1)
    {
        //接收服务器发来的数据
        recv_size = recv(sockfd, databuff, 1024, 0);
        if(recv_size <= 0)
        {
            printf("ser close\n");
            close(fd);
        }

        write(fd, databuff, recv_size);
        memset(databuff, 0, sizeof(databuff));
        
        cur_size += recv_size;

        //计算一下当前传输的百分比
        bar = cur_size * 100.0 / filesize;
        printf("downloading: %s: %.1f%%\r", filename, bar);
        fflush(stdout);

        if(cur_size >= filesize)
        {
            break;
        }
    }

    printf("\n");
    close(fd);
}