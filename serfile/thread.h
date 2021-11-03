#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "md5sum_check.h"

//启动一个线程
void start_thread(int c);

//线程函数
void* fun_thread(void* arg);

//获取命令
char* get_cmd(char buff[], char* myargv[]);

//执行命令
int run_cmd(int c, char* cmd, char* myargv[]);
