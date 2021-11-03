#include <sys/socket.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>


//上传文件
int up_file(int c, char* filename, char* md5, char* filesize);

//创建新文件
int create_new_file(int c, char* filename, char* filesize);

//给已有文件制作硬链接
int hard_link(char* src_filename, char* des_filename);