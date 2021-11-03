#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>

//获取单个文件的32位序列，参数文件名，序列存放在md5中
char *get_md5sum(char *filename, char* md5);