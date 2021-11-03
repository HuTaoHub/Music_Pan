#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

//连接服务器
int connect_ser(char* ip, int port);

//获取ip和端口
int get_ip_port(int argc, char* argv[], char **ip, int* port);

//获取指令
char* get_cmd(char* buff, char* myargv[]);