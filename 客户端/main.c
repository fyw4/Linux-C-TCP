#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <pthread.h>
#include <errno.h>

#define LISTENQ 5
#define PORT 5000
#define ServerIP "192.168.237.131"

int main()
{
	int sendfd;
	int ret;
	int sendnum;
	char sendbuf[4096];
	pthread_t tid;
	struct sockaddr_in servaddr;

	sendfd = socket(AF_INET,SOCK_STREAM,0);

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(ServerIP);
	servaddr.sin_port = htons(PORT);

	connect(sendfd,(struct sockaddr *)&servaddr,sizeof(struct sockaddr));
	while(1)
	{
		printf("Input Mesg:");
		scanf("%s",sendbuf);
		sendnum = send(sendfd,sendbuf,strlen(sendbuf),0);
		if(sendnum < 0)
		{
			perror("send erro");
			exit(1);
		}
	}
	close(sendfd);
	exit(0);
}



