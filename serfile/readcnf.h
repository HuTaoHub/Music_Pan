#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//用于存储读取serinfo.cnf文件中的ip,端口,监听队列大小
typedef struct sock_info
{
    char ips[32];
    short port;
    short lismax;

}sock_info;

//读取serinfo.cnf文件中信息,成功０，失败-1
int read_serinfo(sock_info* ser);
