#include <sys/socket.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "md5.h"

//向sockfd,发送cmd myargv[1]
int up_file(int sockfd, char* cmd, char* filename);

//打开并读文件
void send_data(int sockfd, char* filename);