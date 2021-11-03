#include "getfilename.h"

//申请空间
char** allocate_memory(int n)
{

    char** temp = (char**)malloc(sizeof(char*) * n);
    if (temp == NULL){
        return NULL;
    }

    //每一个指针元素指向堆区内存
    for (int i = 0; i < n;i ++)
    {
        //将malloc空间的首地址赋给每一个指针数组中每一个元素
        temp[i] = (char*)malloc(64);
        memset(temp[i], 0, 64);
    }

    return temp;
}
//释放空间
void free_memory(char* arr[], int len)
{
    if (NULL == arr)
    {
        return;
    }

    for (int i = 0; i < len; i++)
    {
        free(arr[i]);
        arr[i] = NULL;
    }

    free(arr);
    arr = NULL;
}



//获取所有文件名
int get_file_name(char* path, char* filename[])
{
    if(path == NULL || filename == NULL)
    {
        return -1;
    }

    DIR* dir = opendir(path);   //目录指针
    if(dir == NULL)
    {
        return -1;
    }
    struct dirent* p;   //用于指向读到的目录文件属性
    struct stat buff;
    int i = 0;
    int count = 0; // 普通文件个数

    while((p = readdir(dir)) != NULL)
    {
        memset(&buff, 0, sizeof(buff));
        //将p->d_name的文件属性保存在buff中
        lstat(p->d_name, &buff);

        //如果是目录文件
        if(S_ISDIR(buff.st_mode))
        {
            continue;
        }
        //判断p->d_name是不是 . ..跳过
        if(strcmp(p->d_name, ".") == 0|| 
        strcmp(p->d_name, ".." ) == 0)
        {
            continue;
        }
        strcpy(filename[i++], p->d_name);
        count++;
    }

    closedir(dir);
    return count;

}

