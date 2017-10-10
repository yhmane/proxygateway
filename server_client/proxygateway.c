//
//  proxygateway.c
//
//  Created by hwangyunho on 2017. 9. 18.
//  last modified by hwangyunho on 2017. 9. 29.
//  Copyright © 2017 hwangyunho. All rights reserved.
//

#include "serverfunc.h"
#include "clientfunc.h"
int main(int argc, char *argv[])
{	
	int proxy_sock, clnt_sock, state;
	struct sockaddr_in proxy_adr, clnt_adr, serv_adr;
	pid_t pid;
	struct sigaction act;
	socklen_t adr_sz;
	
	if(argc!=4) {													//execute like "./proxygateway <server_ip> <proxy_port> <server_port>"
		printf("Usage : %s <IP> <port> <port>\n", argv[0]);
		exit(1);
	}

	act.sa_handler=readChildProc;									//signal handler  => if pid is killed, display pid num
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	state=sigaction(SIGCHLD, &act, 0);
	
	proxy_sock=socket(PF_INET, SOCK_STREAM, 0);
	memset(&proxy_adr, 0, sizeof(proxy_adr));
	proxy_adr.sin_family=AF_INET;
	proxy_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	proxy_adr.sin_port=htons(atoi(argv[2]));
	
	if(bind(proxy_sock, (struct sockaddr*) &proxy_adr, sizeof(proxy_adr))==-1) errorHandling("bind() error");
	if(listen(proxy_sock, 5)==-1) errorHandling("listen() error");
	signal(SIGALRM, timeOutProxy);
    
	while(1)														//multi process structure
	{
		adr_sz=sizeof(clnt_adr);
		clnt_sock=accept(proxy_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
		if(clnt_sock==-1) continue;									
		
		serv_sock=socket(PF_INET, SOCK_STREAM, 0);   			
		if(serv_sock==-1) errorHandling("socket() error");
		memset(&serv_adr, 0, sizeof(serv_adr));
		serv_adr.sin_family=AF_INET;
		serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
		serv_adr.sin_port=htons(atoi(argv[3]));
	
		if(connect(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1) errorHandling("connect() error!");
		
		pid=fork();												
		if(pid==-1) {												//fork failure
			close(clnt_sock);
			continue;
		}
		if(pid==0) {												//fork success
			close(proxy_sock);
			alarm(180);
			while(1)
			{
				recvMsgType(clnt_sock, &msg_type);					
																	//protocal 
				switch(msg_type)									/*first receive message from Client, second send the message to Server*/ 
				{			
					case AUTH_REQ_MSG:								//receive AUTH_REQ_MSG from Client, send AUTH_REQ_MSG to Server
						recvAuthReqMsg(clnt_sock, &auth_req);
						printf("Client Request Msg -> %s, %s\n", auth_req.user_id, auth_req.password);		
						sendMsgType(serv_sock, msg_type);		
						sendAuthReqMsg(serv_sock, &auth_req);
						break;	
					
					case SQL_REQ_MSG:								//receive SQL_REQ_MSG from Client, send SQL_REQ_MSG to Server
						recvSqlReqMsg(clnt_sock, &sql_req);
						printf("Client Request Msg -> %s\n", sql_req.sql_text);								
						sendMsgType(serv_sock, msg_type);			
						sendSqlReqMsg(serv_sock, &sql_req);
						break;
					
					case CLOSE_REQ_MSG:								//receive CLOSE_REQ_MSG from CLient, send CLOSE_REQ_MSG to Server
						recvCloseReqMsg(clnt_sock, &close_req);	
						sendMsgType(serv_sock, msg_type);			
						sendCloseReqMsg(serv_sock, &close_req);
						printf("Client Request Msg -> exit;\n");
						break;
					
					default : break;
				}
				recvMsgType(serv_sock, &msg_type);
																	//protocal
				switch(msg_type)									/*first receive message from Server, second send the message to Client*/
				{					
					case AUTH_RES_MSG:								//receive AUTH_RES_MSG from Server, send AUTH_RES_MSG to Client
						recvAuthResMsg(serv_sock, &auth_res);		
						printf("Server Response Msg -> %s\n", auth_res.rtn_msg);
						sendMsgType(clnt_sock, msg_type);		
						sendAuthResMsg(clnt_sock, &auth_res);
						break;
					
					case SQL_RES_MSG:								//receive SQL_RES_MSG from Server, send SQL_RES_MSG to Client
						recvSqlResMsg(serv_sock, &sql_res);
						printf("Server Response Msg -> %s", sql_res.rtn_data);
						sendMsgType(clnt_sock, msg_type);		
						sendSqlResMsg(clnt_sock, &sql_res);
						break;		
					
					case CLOSE_RES_MSG:								//receive CLOSE_RES_MSG from Server, send CLOSE_RES_MSG to Client
						recvCloseResMsg(serv_sock,  &close_res);		
						printf("Server Response Msg -> %s", close_res.rtn_msg);
						close(serv_sock);
						sendMsgType(clnt_sock, msg_type);		
						sendCloseResMsg(clnt_sock, &close_res);
						return 0;
						break;

					default : break;
				}
			}
		}
		else
			close(clnt_sock);
	}
	close(proxy_sock);
	return 0;
}
