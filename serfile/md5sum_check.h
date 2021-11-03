#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>

//文件是否重复，实现快传,返回值是重复的文件名
char* is_same(int c, char *md5, char* filename[], char* serfilemd5[]);

//获取filename的文件md5序列号
char* get_md5sum(char* filename, char* md5);

//获取所有文件的md5序列,存放在filemd5中
int get_all_filemd5(char* path, char* filename[], char* filemd5[]);