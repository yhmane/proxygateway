//
//  dbclient.c
//
//  Created by hwangyunho on 2017. 9. 18.
//  last modified by hwangyunho on 2017. 9. 29.
//  Copyright © 2017 hwangyunho. All rights reserved.
//

#include "serverfunc.h"
int main(int argc, char *argv[])
{	
	int serv_sock, clnt_sock, state;
	struct sockaddr_in serv_adr, clnt_adr;
	pid_t pid;
	struct sigaction act;
	char static_ip[RTN_MSG]="192.168.111.11";
	socklen_t adr_sz;
	
	if(argc!=2) {															//execute like "./dbserver <server_port>"
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	act.sa_handler=readChildProc;											//signal handler  => if pid is killed, display pid num
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	state=sigaction(SIGCHLD, &act, 0);
	
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)	errorHandling("bind() error");
	if(listen(serv_sock, 5)==-1) errorHandling("listen() error");
	
	while(1)																// multi process structure
	{
		adr_sz=sizeof(clnt_adr);
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
		if(clnt_sock==-1) continue;
		else puts("Connected............. ");
		
		pid=fork();
		if(pid==-1) {														//fork failure
			close(clnt_sock);
			continue;
		}
		if(pid==0) {														//fork success
			close(serv_sock);
			while(1)
			{
				recvMsgType(clnt_sock, &msg_type);
				switch(msg_type)											//receive protocal						
				{			
					case AUTH_REQ_MSG:										//receive id, pwd
						recvAuthReqMsg(clnt_sock, &auth_req);
						checkIdPwd(&msg_type, auth_req);					//id, pwd == (scott, tiger) ? send AuthResMsg : exit(1) 
						break;	
					
					case SQL_REQ_MSG:										//receive sql sentence
						recvSqlReqMsg(clnt_sock, &sql_req);	
						msg_type=SQL_RES_MSG;
						break;
					
					case CLOSE_REQ_MSG:										//receive Close_Req msg
						recvCloseReqMsg(clnt_sock, &close_req);
						msg_type=CLOSE_RES_MSG;
						break;
					
					default: break;
				}
				sendMsgType(clnt_sock, msg_type);
				switch(msg_type)											//send protocal
				{
					case AUTH_RES_MSG:										//send ip, port
						strcat(static_ip, ", ");
						strcat(static_ip, argv[1]);							// ex) 127.0.0.1 , 9190

						setAuthResMsg(static_ip, &auth_res);
						sendAuthResMsg(clnt_sock, &auth_res);
						break;					
					
					case SQL_RES_MSG:										//send sql result
						setSqlResMsg(&sql_req, &sql_res);
						sendSqlResMsg(clnt_sock, &sql_res);
						break;
					
					case CLOSE_RES_MSG:										//send disconnect msg
						setCloseResMsg(&close_res);
						sendCloseResMsg(clnt_sock, &close_res);
						close(clnt_sock);
						puts("Server connection is closed");
						return 0;
						break;

					default: break;
				}
			}
		}
		else close(clnt_sock);
	}
	close(serv_sock);
	return 0;
}
