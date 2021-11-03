#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>

//发送回复
int send_reply(int sockfd, char *cmd, char *filename);


//接收数据
void accept_data(int sockfd, int fd, int filesize, char* filename);