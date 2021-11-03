#include "cli_init.h"
#include "downfile.h"
#include "up.h"

void fun1(int sig)
{
    printf("err\n");
}
int main(int argc, char* argv[])
{
    signal(SIGSEGV,fun1);
    char* ip = "127.0.0.1";
    int port = 8000;

    int res = get_ip_port(argc, argv, &ip, &port);
    if(res == -1)
    {
        printf("连接失败，请检查服务器ip和端口\n");
        exit(0);
    }
   
    printf("connecting ...%s %d\n", ip, port);

    int sockfd = connect_ser(ip, port);
    if(sockfd == -1)
    {
        printf("连接失败，请检查服务器ip和端口\n");
        exit(0);
    }

    while(1)
    {
        char buff_cmd[128] = {0};
        printf("connect %s >>", ip);
        fflush(stdout);
        fgets(buff_cmd, 128, stdin);
        if(0 == strncmp(buff_cmd, "end", 3))
        {
            break;
        }

        //删除换行符
        buff_cmd[strlen(buff_cmd) - 1] = 0;
        //备份buff
        char buff_cmd_bak[128] = {0};
        strcpy(buff_cmd_bak, buff_cmd);

        //得到指令
        char* myargv[10] = {0};
        char* cmd = get_cmd(buff_cmd, myargv);
        if(cmd == NULL)
        {
            continue;
        }
        //下载
        if(strcmp(cmd, "get") == 0)
        {
            //发送请求
            if(send_reply(sockfd, buff_cmd_bak, myargv[1]) == -1)
            {
                continue;
            }
        }
        //上传
        else if(strcmp(cmd, "up") == 0)
        {
            if(up_file(sockfd, buff_cmd_bak, myargv[1]) == -1)
            {
                continue;
            }
            
        }
        //其他命令
        else 
        {
            char databuff[1024] = {0};
            send(sockfd, buff_cmd_bak, strlen(buff_cmd_bak), 0);
            
            //服务器关闭
            if(recv(sockfd, databuff, 1023, 0) == 0)
            {
                printf("ser was closed\n");
                break;
            }

            //服务器返回结果正确
            if(strncmp(databuff, "ok#", 3) == 0)
            {
                printf("%s\n", databuff + 3);
            }
            else
            {
                printf("命令执行失败\n");
            }
        }
       
        printf("---------------------\n");
    }

    close(sockfd);
}