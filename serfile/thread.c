#include "thread.h"
#include "downfile.h"
#include "upfile.h"
#define SIZE 10

void* fun_thread(void* arg)
{
    int c = (int)arg;
    while(1)
    {
        char buff[128] = {0};
        int n = recv(c, buff, 127, 0);
        if(n <= 0)
        {
           break;
        }
        /*
        printf("c = %d, %s\n", c, buff);
        send(c, "ok", 2, 0);
        */
        //将客户端的指令进行解析,存放在myargv[]指针数组中
        char* myargv[SIZE] = {0};
        char* cmd = get_cmd(buff, myargv);
        

        if(strncmp(cmd, "get", 3) == 0)
        {
            //下载
            if(-1 == down_file(c, myargv[1]))
            {
                continue;
            }
        }
        else if(strncmp(cmd, "up", 2) == 0)
        {
            //上传
            //此时myargv[2]客户端文件的md5值 myargv[3]存放文件大小
            if(-1 == up_file(c, myargv[1], myargv[2], myargv[3]))
            {
                continue;
            }
            

        }
        else
        {
            //查看命令
            if(-1 == run_cmd(c, cmd, myargv)) 
            {
                continue;
            }
        }


    }
    printf("one client close!\n");
    close(c);
}

//c是客户端的文件描述符
void start_thread(int c)
{
    //线程id，线程属性默认NULL,线程调用的函数，传给线程函数的参数

	pthread_t id;
    pthread_create(&id, NULL, fun_thread, (void*)c);
}


char* get_cmd(char buff[], char* myargv[])
{
    if(buff == NULL || myargv == NULL)
    {
        return NULL;
    }

    int i = 0;
    char* ptr = NULL;
    char* s = strtok_r(buff, " ", &ptr);

    while(s != NULL)
    {
        myargv[i++] = s;
        s = strtok_r(NULL, " ", &ptr);
    }

    return myargv[0];
}

int run_cmd(int c, char* cmd, char* myargv[])
{
    //printf("cmd = %s\n", cmd);
    //printf("myargv[0] = %s\n", myargv[0]);
    int fd[2];
    int res = pipe(fd);
    if(res == -1)
    {
        printf("pipe error\n");
        send(c, "err01", 5, 0);
        return -1;
    }

    pid_t pid = fork();
    if(-1 == pid)
    {
        printf("fork error\n");
        send(c, "err02", 5, 0);
        return -1;
    }

    if(pid == 0)
    {
        //printf("cmd = %s\n", cmd);
        //printf("myargv = %x\n", myargv);
        //0 标准输入 1 标准输出 2标准错误输出
        
        //子进程写端写入无名管道
        /*
        dup2（fd,1）的作用就是关闭标准输出，本应该写入到stdout的内容，
        标准输出已经重定向到目标文件(fd)中，
        故向标准输出写的数据将会写到目标文件（fd所表示的文件）中。
        */
        //标准输出替换成写端fd[1]
        //标准错误输出替换成写端fd[1]'
        close(fd[0]);
        dup2(fd[1], 1);
        dup2(fd[1], 2);
        

        //execvp()会从PATH 环境变量所指的目录中查找符合参数file 的文件名, 找到后便执行该文件, 然后将第二个参数argv 传给该欲执行的文件。
        //int res = execvp(cmd, myargv);
        //printf("%d\n", res);
        execvp(cmd, myargv);
        //execl("/bin/ls", cmd, NULL);
        printf("cmd is not find\n");
        send(c, "err03", 5, 0);
        exit(0);
    }
    
    //父进程等待子进程退出
    close(fd[1]);
    wait(NULL);

    //父进程从读端读取子进程的写入结果
    char buff[1024] = {"ok#"};
    int count = read(fd[0], buff + 3, 1020);
    send(c, buff, count + 3, 0);
   
    return 0;
}
