#include "readcnf.h"

//读取serinfo.cnf文件中信息,成功０，失败-1
int read_serinfo(sock_info* ser)
{
    if(NULL == ser) return -1;

    FILE* fp = fopen("serinfo.cnf", "r");
    if(NULL == fp)
    {
        printf("serinfo.cnf is not exist\n");
        return -1;
    }

    int index = 0;

    while(1)
    {
        char readbuff[128] = {0};

        //如果文件内容为空break
        if(NULL == fgets(readbuff, 127, fp))
        {
            break;
        }

        //记录行数
        index++;

        //不对#开头或者空行处理
        if(readbuff[0] == '\n' || readbuff[0] == '#')
        {
            continue;
        }

        //有效行的末尾加上\0;
        readbuff[strlen(readbuff) - 1] = 0;

        //使用"　"分割readbuff
        char* s = strtok(readbuff, " ");

        //IPS获取
        //如果第一个子字符串s和IP相同
        if(strcmp(s, "IP") == 0)
        {
            //让s指向第二个子字符串
            s = strtok(NULL, " ");
            //如果第二个子字符串为空,IP说明有误
            if(NULL == s)
            {
                printf("IP is not set, in line %d\n", index);
                fclose(fp);
                return -1;
            }
            //IP正确,将其存储在sock_info中
            strcpy(ser->ips, s);

            //测试
            printf("IP: %s\n", ser->ips);
        }
        
        //获取端口
        if(strcmp(s, "PORT") == 0)
        {
            s = strtok(NULL," ");
            if(NULL == s)
            {
                printf("PORT is not set, in line %d\n", index);
                fclose(fp);
                return -1;
            }

            ser->port = atoi(s);

            //测试
            printf("PORT: %d\n", ser->port);      
        }

        //获取监听队列大小LISMAX
        if(strcmp(s, "LISMAX") == 0)
        {
            s = strtok(NULL, " ");
            if(NULL == s)
            {
                printf("LISMAX is not set, in line %d\n", index);
                fclose(fp);
                return -1;
            }

            ser->lismax = atoi(s);

            printf("lismax:%d\n", ser->lismax);
        }
    }

    fclose(fp);
    return 0;

}

/*
int main()
{
    sock_info ser;
    read_serinfo(&ser);
    return 0;    
}
*/