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
#define CLIENTCNT 4
#define MAXQUEUECNT 100

int iListenPort; // 서버포트
FILE* fp; // 로그 파일 포인터
int fdSock[CLIENTCNT]; // 0:cluster, 1:controller, 2:engine
char sLogging[100];

// send 큐
char sQueue[CLIENTCNT][MAXQUEUECNT][MAXLINE];
int iQueCurrentIdx[CLIENTCNT];
int iQueSavedIdx[CLIENTCNT];

// Global variables
int gear_state 	 = 0; // 0: P, 1: N, 2: R, 3: D
int accel_val 	 = 0; // 0, 1, 2, 3
int break_val	 = 0; // 0, 1, 2, 3
int wink_state 	 = 0; // 0: None, 1: Left, 2: right, 3: Warning
int current_speed = 0;
int current_fuel;
int current_total_distance;
double dist = 0.0;
int dist_count = 0;
FILE *fp;

int readLine(int fd, char* str);
int logging(char* str);
void* thRecver();
void* thSender();

void* naturalDeceleration();
void AddQueue(char *str, int iFdNum);

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

void* naturalDeceleration()
{
	char tmp_msg[20];
	memset(tmp_msg, 0x00, sizeof(tmp_msg));
	while(1)
	{
		printf("current speed : %d\n", current_speed);
		nonActuator(); // -2km/h per second 
		if(dist < 1){
			dist += current_speed / 3600;
		}
		if(dist >= 1){
			increaseDistance();
			dist_count++;
			dist = 0.0;
			sprintf(tmp_msg,"1 %s\n",current_total_distance);
			AddQueue(tmp_msg, 1);
		}
		if(dist_count >= FUEL_IFFICIENCY){
			decreaseFuel();
			dist_count = 0;
			sprintf(tmp_msg,"2 %s\n",current_fuel);			
			AddQueue(tmp_msg, 1);
		}
		sprintf(tmp_msg,"0 %s\n",current_speed);
		AddQueue(tmp_msg, 1);
		sleep(1);
	}
}

void* thRecver(void *arg)
{
	char recv_msg[MAXLINE], send_msg[MAXLINE];
	int thr_arg = *((int *)arg);
	fprintf(stderr, "t_recver\n");
	while(1)
	{
		if(fdSock[thr_arg] == NULL)
		{
			continue;
		}
		memset(recv_msg, 0x00, sizeof(recv_msg));
		memset(send_msg, 0x00, sizeof(send_msg));
		int iLen = readLine(fdSock[thr_arg], recv_msg);
		fprintf(stderr, "rcv");
		sprintf(sLogging, "recv : %s\n", recv_msg); 		
		logging(sLogging);

		// Preprocessing "recv_msg"
		int command, content;
		sscanf(recv_msg, "%d %d", &command, &content);

		if(thr_arg == 1)
		{
			if(command == 0){
				accel_val = content;
				accelActuator();
				printf("[Accel]----Current speed : %d----------\n", current_speed);
				printf("[Accel]----Current gear  : %d----------\n", gear_state);
			}else if(command == 1){
				break_val = content;
				breakActuator();
				printf("[Break]----Current speed : %d----------\n", current_speed);
				printf("[Break]----Current gear  : %d----------\n", gear_state);
			}else if(command == 2){
				gear_state = content;
				printf("[Gear]-----Gear state changed: %d------\n", gear_state);
			}else if(command == 3){
				wink_state = content;
				printf("wink !!!\n");
			}else if(command == 4){
				// TODO : Send music change signal.
			}else if(command == 9){

			}

			sprintf(send_msg,"3 %d\n", gear_state);
			AddQueue(send_msg, 1);
			sprintf(send_msg,"4 %d\n", wink_state);
			AddQueue(send_msg, 1);
			//sprintf(send_msg,"5 %s",itoa(current_speed)); //Music Control information
			//AddQueue(send_msg, 1);
		}
	}
}

void AddQueue(char *str, int iFdNum)
{
	sprintf(sQueue[iFdNum][iQueSavedIdx[iFdNum]], "%s\n", str);

	iQueSavedIdx[iFdNum]++;
	if(iQueSavedIdx[iFdNum] >= MAXQUEUECNT)
	{
		iQueSavedIdx[iFdNum] = 0;
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
			while(iQueCurrentIdx[i] != iQueSavedIdx[i])
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
	loadFile();
	iListenPort = atoi(argv[1]);

	// Socket 생성
	int listenfd, connfd, clientlen;
	int thr_arg;
	char onoff_msg[20];
	memset(onoff_msg, 0x00, sizeof(onoff_msg));
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
	
	int thr_id[CLIENTCNT+1];	
	pthread_t p_thread[CLIENTCNT+1];
	pthread_t nD_thread;
	// pthread_create(&p_thread[0], NULL, thRecver , NULL);			

	thr_id[3] = pthread_create(&p_thread[3], NULL, thSender , NULL);
	sleep(1);
	pthread_create(&nD_thread, NULL, naturalDeceleration , NULL);

	printf("SW ON\n");

	while (1)
	{ /* 소켓 연결 요청 수락 */
		clientlen = sizeof(clientaddr);
		connfd = accept(listenfd, &clientaddr, &clientlen);
		hp = gethostbyaddr((char *)&clientaddr.sin_addr.s_addr,	sizeof(clientaddr.sin_addr.s_addr), AF_INET);
		haddrp = inet_ntoa(clientaddr.sin_addr);	
		fprintf(stderr, "accpeted\n");

		readLine(connfd, onoff_msg);

		if(!(strcmp(onoff_msg,"cluster")))
		{
			printf("Cluster On\n");
			fdSock[0] = connfd;
			thr_arg = 0;
		}

		else if(!(strcmp(onoff_msg,"controller")))
		{
			printf("Controller On\n");
			fdSock[1] = connfd;
			thr_arg = 1;
		}

		else if(!(strcmp(onoff_msg,"engine")))
		{
			printf("Engine On\n");
			fdSock[2] = connfd;
			thr_arg = 2;
		}
		thr_id[thr_arg] = pthread_create(&p_thread[thr_arg], NULL, thRecver , (void *)&thr_arg);

		sleep(1);
	}

	close(connfd);
	return 0;
}



























