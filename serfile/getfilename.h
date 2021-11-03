#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <malloc.h>


//申请空间
char** allocate_memory(int n);
//释放空间
void free_memory(char* arr[], int len);

//获取所有文件名
int get_file_name(char* path, char* filename[]);