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

void *ReadMesg(void *arg)
{
    int fd = *((int *)arg);
    int nread = 0;
    char buffer[1024];
    while((nread = read(fd,buffer,sizeof(buffer))) > 0)
    {
        buffer[nread] = '\0';
        printf("get client message: %s\n",buffer);
        memset(buffer,0,sizeof(buffer));
    }

	return NULL;
}

int main()
{
	int listenfd,connfd;
	int ret;
	pid_t childpid;
	pthread_t tid;
	socklen_t clilen;
	struct sockaddr_in cliaddr,servaddr;

	clilen = sizeof(cliaddr);

	listenfd = socket(AF_INET,SOCK_STREAM,0);

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
	listen(listenfd,LISTENQ);

	while(1)
	{
		connfd = accept(listenfd,(struct sockaddr*)&cliaddr,&clilen);
		if(connfd < 0)
		{
			perror("accept failed!");
		}

		ret = pthread_create(&tid,NULL,(void*)&ReadMesg,(void*)&connfd);
		if(0 != ret)
		{
			perror("create thread erro!");
		}
	}
	close(listenfd);
	exit(0);
}



