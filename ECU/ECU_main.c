#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <unistd.h>

#include "Engine.h"
#include "FileManager.h"

#define DEFAULT_PROTOCOL 0
#define MAXLINE 100

int readLine(int fd, char* str);
void cluster_thread();
void controller_thread();
void engine_thread();

void cluster_thread()
{
	pid_t pid;            // process id
    pthread_t tid;        // thread id
 
    pid = getpid();
    tid = pthread_self();
 
    //char* thread_name = (char*)data;
    int i = 0;
 
    while (i<3)   // 0,1,2 까지만 loop 돌립니다.
    {
        // 넘겨받은 쓰레드 이름과 
        // 현재 process id 와 thread id 를 함께 출력
        printf("pid:%u, tid:%x --- %d\n", (unsigned int)pid, (unsigned int)tid, i);
		//printf("[%s] pid:%u, tid:%x --- %d\n", thread_name, (unsigned int)pid, (unsigned int)tid, i);
        i++;
        sleep(1);  // 1초간 대기
    }
}

void controller_thread(int fd)
{
	pid_t pid;            // process id
    pthread_t tid;        // thread id
 
    pid = getpid();
    tid = pthread_self();

    char ctrl_Msg[20] = "[Accel] 0";
    char ctrl_mode[10];
    memset(ctrl_mode, 0x00, sizeof(ctrl_mode));
    char ctrl_level[2];
    memset(&ctrl_level, 0x00, sizeof(ctrl_level));

    
  
    //while (1)
    //{
    	//readLine(fd,ctrl_Msg);
    	char *ptr = strtok(ctrl_Msg, " ");

    	printf("%s\n", ptr);
    	memcpy(ctrl_mode, ptr, strlen(ptr) + 1);
    	printf("%s\n", ctrl_mode);

    	ptr = strtok(NULL," ");

    	printf("%s\n", ptr);
    	memcpy(ctrl_level, ptr, strlen(ptr) + 1);
    	printf("%s\n", ctrl_level);

        //sleep(1);  // 1초간 대기
    //}
}

void engine_thread()
{
	pid_t pid;            // process id
    pthread_t tid;        // thread id
 
    pid = getpid();
    tid = pthread_self();
 
    //char* thread_name = (char*)data;
    int i = 0;
 
    while (i<3)   // 0,1,2 까지만 loop 돌립니다.
    {
        // 넘겨받은 쓰레드 이름과 
        // 현재 process id 와 thread id 를 함께 출력
		printf("pid:%u, tid:%x --- %d\n", (unsigned int)pid, (unsigned int)tid, i);
        //printf("[%s] pid:%u, tid:%x --- %d\n", thread_name, (unsigned int)pid, (unsigned int)tid, i);
        i++;
        sleep(1);  // 1초간 대기
    }
}

/* 한 줄 읽기 */
int readLine(int fd, char* str)
{
	int n;
	do{
		n = read(fd, str, 1);
	}while(n > 0 && *str++ != NULL);

   return(n > 0);
}


int main(int argc, char* argv[])
{
	int listenfd, connfd, port, clientlen;
	FILE *fp;
	char inmsg[MAXLINE], outmsg[MAXLINE];
	struct sockaddr_in serveraddr, clientaddr;
    struct hostent *hp;
    char *haddrp;

	memset(inmsg, 0x00, sizeof(inmsg));
	memset(outmsg, 0x00, sizeof(outmsg));

	sprintf(outmsg, "Go\n"); 

    signal(SIGCHLD, SIG_IGN);

	int  cluster_tid;
    pthread_t pthread_cluster;

	int  controller_tid;
	pthread_t pthread_controller;

	int  engine_tid;
	pthread_t pthread_engine;

	int recv_data, send_data;

	int status;

	port = atoi(argv[1]);

    if((listenfd = socket(AF_INET, SOCK_STREAM, DEFAULT_PROTOCOL)) == -1)
    {// 소켓 생성
        printf("Server : Can't open stream socket\n");
        exit(0);
    }else printf("socket() pass\n");

	bzero((char *) &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons((unsigned short)port);
    //serveraddr 셋팅
 
    if(bind(listenfd, &serveraddr, sizeof(serveraddr)) <0)
    {//bind() 호출
        printf("Server : Can't bind local address.\n");
        exit(0);
    }else printf("bind() pass\n");
 
    if(listen(listenfd, 5) < 0)
    {//소켓을 수동 대기모드로 설정
        printf("Server : Can't listening connect.\n");
        exit(0);
    }else printf("listen() pass\n");

    while(1)
    {
		clientlen = sizeof(clientaddr);
		connfd = accept(listenfd, &clientaddr, &clientlen); 

        if(connfd < 0)
        {
            printf("Server: accept failed.\n");
            exit(0);
        }else printf("Accept() pass\n");

		/* 클라이언트의 도메인 이름과 IP 주소 결정 */
		hp = gethostbyaddr((char *)&clientaddr.sin_addr.s_addr, sizeof(clientaddr.sin_addr.s_addr), AF_INET);
		haddrp = inet_ntoa(clientaddr.sin_addr);
//		printf("서버: %s (%s) %d에 연결됨\n", hp->h_name, haddrp, clientaddr.sin_port);

		readLine(connfd, inmsg);
		printf("%s\n",inmsg);

		write(connfd, outmsg, strlen(outmsg)+1);

		//readLine(connfd, inmsg);
		//printf("%s\n",inmsg);
		

		if(!(strcmp(inmsg,"cluster")))
		{
			printf("Cluster On\n");
			cluster_tid=pthread_create(&pthread_cluster,NULL,cluster_thread,&connfd);
			//cluster_tid=pthread_create(&pthread_cluster,NULL,cluster_thread,(void*)&a);
			if(cluster_tid<0)
			{
				perror("Cluster thread create error:");
				exit(0);
			}
		}
		else if(!(strcmp(inmsg,"controller")))
		{
			printf("Controller On\n");
			controller_tid=pthread_create(&pthread_controller,NULL,controller_thread,&connfd);
			//controller_tid=pthread_create(&pthread_controller,NULL,controller_thread,(void*)&a);
			if(controller_tid<0)
			{
				perror("Controller thread create error:");
				exit(0);
			}
		}
		else if(!(strcmp(inmsg,"engine")))
		{
			printf("Engine On\n");
			engine_tid=pthread_create(&pthread_engine,NULL,engine_thread,NULL);
			//engine_tid=pthread_create(&pthread_engine,NULL,engine_thread,(void*)&a);
			if(engine_tid<0)
			{
				perror("Engine thread create error:");
				exit(0);
			}
		}

	}

	close(connfd);
	return 0;
}



























