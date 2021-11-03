#include "up.h"

//上传文件
int up_file(int sockfd, char *cmd, char *filename)
{
    if (cmd == NULL || filename == NULL)
    {
        return -1;
    }
    //获取myargv[1]文件的md5序列
    char md5[32] = {0};
    if (get_md5sum(filename, md5) == NULL)
    {
        printf("err07\n");
        return -1;
    }
    //合成如：up filename md5序列

    strcat(cmd, " ");
    int len = strlen(cmd);
    for (int i = 0; i < 32; i++)
    {
        cmd[len + i] = md5[i];
    }
    //strcat(cmd, md5);

    //得到文件大小
    struct stat filebuff;
    memset(&filebuff, 0, sizeof(filebuff));
    stat(filename, &filebuff);

    char sizebuff[16] = {0};
    sprintf(sizebuff, "%d", filebuff.st_size);

    strcat(cmd, " ");
    strcat(cmd, sizebuff);

    //printf("cmd:%s\n",cmd);

    //发送up md5序列
    send(sockfd, cmd, strlen(cmd), 0);

    char status[64] = {0};
    recv(sockfd, status, 63, 0);
    if (strncmp(status, "warn:", 5) == 0)
    {
        printf("%s\n", status);
        memset(status, 0, 64);
        fgets(status, 64, stdin);
        send(sockfd, status, strlen(status), 0);
        if (strncmp(status, "n", 1) == 0)
        {
            return -1;
        }
    }

    if (strncmp(status, "ready", 5) == 0)
    {
        send_data(sockfd, filename);
    }

    memset(status, 0, sizeof(status));
    recv(sockfd, status, 31, 0);
    printf("%s\n", status);

    return 0;
}

//打开并读文件
void send_data(int sockfd, char *filename)
{
    if (filename == NULL)
        return;
    int fdr = open(filename, O_RDONLY);
    if (fdr == -1)
        return;

    int num = 0;
    char databuff[1024] = {0};
    while ((num = read(fdr, databuff, 1024)) > 0)
    {
        send(sockfd, databuff, num, 0);
    }

    close(fdr);
}
