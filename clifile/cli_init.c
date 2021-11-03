#include "cli_init.h"

//连接服务器
int connect_ser(char *ip, int port)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("socket error!\n");
        return -1;
    }

    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = inet_addr(ip);

    //连接服务器
    int res = connect(sockfd, (const struct sockaddr *)&saddr, sizeof(saddr));
    if (-1 == res)
    {
        return -1;
    }

    return sockfd;
}

//获取ip和端口
int get_ip_port(int argc, char *argv[], char **ip, int *port)
{
    if (argc == 1)
        return 0;
    if (argc < 3 || argv == NULL || ip == NULL || port == NULL)
        return -1;

    int count = 0;
    for (int i = 1; i < argc; i++)
    {
        if (0 == strncmp(argv[i], "ip=", 3))
        {
            *ip = argv[i] + 3;
            count++;
            continue;
        }

        if (0 == strncmp(argv[i], "port=", 5))
        {
            *port = atoi(argv[i] + 5);
            count++;
            continue;
        }
    }

    if (count == 2)
    {
        return 0;
    }
    return -1;
}

//获取指令
char *get_cmd(char *buff, char *myargv[])
{
    if (buff == NULL || myargv == NULL)
    {
        return NULL;
    }

    int i = 0;
    char *s = strtok(buff, " ");
    while (s != NULL)
    {
        myargv[i++] = s;
        s = strtok(NULL, " ");
    }

    return myargv[0];
}

