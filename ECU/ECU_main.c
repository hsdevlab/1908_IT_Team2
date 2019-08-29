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
#define MAXLINE 20
#define CLIENTCNT 3
#define MAXQUEUECNT 100

int iListenPort; // 서버포트
FILE* fp; // 로그 파일 포인터
int fdSock[CLIENTCNT]; // 0:cluster, 1:controller, 2:engine
char sLogging[100];

// send 큐
char sQueue[CLIENTCNT][MAXQUEUECNT][MAXLINE];
int iQueCurrentIdx[CLIENTCNT];
int iQueSavedIdx[CLIENTCNT];

//char recv_msg[MAXLINE], send_msg[MAXLINE];

int readLine(int fd, char* str);
int logging(char* str);
void* thRecver();
void* thSender();

/* 한 줄 읽기 */
int readLine(int fd, char* str)
{
	int n;
	do{
		n = read(fd, str, 1);
	}while(n > 0 && *str++ != NULL);

   return(n > 0);
}


int logging(char* str) // 로깅
{	
	fprintf(stderr, "logging\n");
	// 로그 파일 열기
	if((fp = fopen("ECU_log.txt","a"))==NULL)
	{
		fprintf(stderr, "file open error");
		exit(1);
	}
	fprintf(fp,"log : %s\n", str);
	fprintf(stderr, "log : %s\n", str);
	fclose(fp);
}

void* thRecver(void *arg)
{
	char recv_msg[MAXLINE];
	int thr_arg = *((int *)arg);
	fprintf(stderr, "t_recver\n");
	while(1)
	{
		if(fdSock[thr_arg] == NULL)
		{
			continue;
		}
		memset(recv_msg, 0x00, sizeof(recv_msg));
		int iLen = readLine(fdSock[thr_arg], recv_msg);
		fprintf(stderr, "rcv");
		sprintf(sLogging, "recv : %s\n", recv_msg); 		
		logging(sLogging);

		func();
		iQueSavedIdx[thr_arg]++;
		if(iQueSavedIdx[thr_arg] >= MAXQUEUECNT)
		{
			iQueSavedIdx[thr_arg] = 0;
		}
		sprintf(sQueue[thr_arg][iQueCurrentIdx[thr_arg]], "return:%s\n", recv_msg);

		if(iLen == 0)
		{
			printf("누구 접속 종료\n");
			break;
		}
	}
}

void* thSender()
{	
	fprintf(stderr, "t_sender\n");
	int iLenSend = 0;
	while(1)
	{
		for(int i=0;i<CLIENTCNT;i++)
		{
			if(fdSock[i] == NULL)
			{
				continue;
			}
			if(iQueCurrentIdx[i] != iQueSavedIdx[i])
			{
				iLenSend=write(fdSock[i], sQueue[i][iQueCurrentIdx[i]], strlen(sQueue[i][iQueCurrentIdx[i]])+1);
				sprintf(sLogging, "send : %s\n", sQueue[i][iQueCurrentIdx[i]]);
				fprintf(stderr, "sended\n");
				logging(sLogging);
				iQueCurrentIdx[i]++;
				if(iQueCurrentIdx[i] >= MAXQUEUECNT)
				{
					iQueCurrentIdx[i] = 0;
				}
			}
		}
	}
}


int main(int argc, char* argv[])
{
	if (argc != 2) {		
		fprintf(stderr, "사용법: %s <port>\n", argv[0]);		
		exit(0);	
	}

	iListenPort = atoi(argv[1]);

	// Socket 생성
	int listenfd, connfd, clientlen;
	int thr_arg;
	struct sockaddr_in serveraddr, clientaddr;	
	struct hostent *hp;	
	char *haddrp;	
	listenfd = socket(AF_INET, SOCK_STREAM, DEFAULT_PROTOCOL);
	bzero((char *) &serveraddr, sizeof(serveraddr));	
	serveraddr.sin_family = AF_INET;	
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);	
	serveraddr.sin_port = htons((unsigned short)iListenPort);

	int sockopt = 1;
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(sockopt)) == -1) {
		printf("Socket option = SO_REUSEADDR(on)\n");
		exit(EXIT_FAILURE);
	}
	struct linger solinger = { 1, 0 };
	if (setsockopt(listenfd, SOL_SOCKET, SO_LINGER, &solinger, sizeof(struct linger))
			== -1) {
		perror("setsockopt(SO_LINGER)");
	}

	bind(listenfd, &serveraddr, sizeof(serveraddr));
	// Socket 생성 완료		
	listen(listenfd, 5); // Socket 접속 대기
	
	int thr_id[2];	
	pthread_t p_thread[2];

	//pthread_create(&p_thread[0], NULL, thRecver , NULL);			
	//sleep(1);
	pthread_create(&p_thread[1], NULL, thSender , NULL);

	printf("SW ON\n");

	while (1)
	{ /* 소켓 연결 요청 수락 */
		clientlen = sizeof(clientaddr);
		connfd = accept(listenfd, &clientaddr, &clientlen);
		hp = gethostbyaddr((char *)&clientaddr.sin_addr.s_addr,	sizeof(clientaddr.sin_addr.s_addr), AF_INET);
		haddrp = inet_ntoa(clientaddr.sin_addr);	
		fprintf(stderr, "accpeted\n");

		if(!(strcmp(inmsg,"cluster")))
		{
			printf("Cluster On\n");
			fdSock[0] = connfd;
			thr_arg = 0;
		}

		else if(!(strcmp(inmsg,"controller")))
		{
			printf("Controller On\n");
			fdSock[1] = connfd;
			thr_arg = 1;
		}

		else if(!(strcmp(inmsg,"engine")))
		{
			printf("Engine On\n");
			fdSock[2] = connfd;
			thr_arg = 2;
		}
		pthread_create(&p_thread[0], NULL, thRecver , (void *)&thr_arg);

		sleep(1);
	}

	close(connfd);
	return 0;
}



























