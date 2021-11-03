#include "sock_init.h"


int create_socket()
{
    sock_info ser;
    int res = read_serinfo(&ser);
	//printf("res = %d\n", res);
    if(-1 == res) 
    {
        printf("read_serinfo is fail\n");
        return -1;
    }

	//printf("%d\n", ser.port);
	//printf("%s\n", ser.ips);

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	//printf("sockfd = %d\n", sockfd);
	if(-1 == sockfd)
	{
		return -1;
	}

	//绑定ip端口
	struct sockaddr_in saddr;
    memset(&saddr,0,sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(ser.port);
	saddr.sin_addr.s_addr = inet_addr(ser.ips);

	res = bind(sockfd, (const struct sockaddr*)&saddr, sizeof(saddr));
	if(-1 == res)
	{
		return -1;
	}

	//监听套接字
	res = listen(sockfd, ser.lismax);
	if(-1 == res)
	{
		printf("listen error\n");
		return -1;
	}
	//printf("%d\n", res);
	return sockfd;
}

int accept_client(int sockfd)
{
	if(-1 == sockfd) return -1;

	//保存客户端的信息
	struct sockaddr_in caddr;
	int lenc = sizeof(caddr);
	int c = accept(sockfd, (struct sockaddr*)&caddr, &lenc);
	
	if(-1 == c) return -1;

	return c;
}