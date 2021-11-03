#include "thread.h"
#include "sock_init.h"


int main()
{
	int sockfd = create_socket();
	if(-1 == sockfd)
	{
		//正规将其写入服务器日志文件中
		printf("create_socket error!\n");
		return -1;
	}
	//printf("%d\n", sockfd);

	
	//接受客户端的连接,从sockfd套接字中接收客户端的连接
	while(1)
	{
		//成功返回接受的套接字的文件描述符,失败返回-1
		
		int c = accept_client(sockfd);
		
		if(-1 == c)
		{
			continue;
		}
		printf("c = %d, 已连接\n", c);

		//启动一个线程去处理客户端的c文件描述符
		start_thread(c);
	}
}

