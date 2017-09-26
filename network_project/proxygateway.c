//
//  dbclient.h
//  tcp
//
//  Created by hwangyunho on 2017. 9. 18.
//  last modified by hwangyunho on 2017. 9. 26.
//  Copyright © 2017년 hwangyunho. All rights reserved.
//



#include "serverfunc.h"
#include "clientfunc.h"
int main(int argc, char *argv[])
{	
	char id[USERID], password[PASSWORD];
	int proxy_sock, clnt_sock, sock, state;
	struct sockaddr_in proxy_adr, clnt_adr, serv_adr;
	pid_t pid;
	struct sigaction act;
	socklen_t adr_sz;
	UB2 msgType=EMPTY_MSG;

	dgt_auth_req_msg auth_req;
	dgt_auth_res_msg auth_res;
	dgt_sql_req_msg sql_req;
	dgt_sql_res_msg sql_res;
	dgt_close_req_msg close_req;
	dgt_close_res_msg close_res; 
	
	act.sa_handler=read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	state=sigaction(SIGCHLD, &act, 0);
	
	proxy_sock=socket(PF_INET, SOCK_STREAM, 0);
	memset(&proxy_adr, 0, sizeof(proxy_adr));
	proxy_adr.sin_family=AF_INET;
	proxy_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	proxy_adr.sin_port=htons(atoi(argv[2]));
	
	if(bind(proxy_sock, (struct sockaddr*) &proxy_adr, sizeof(proxy_adr))==-1)
		error_handling("bind() error");
	if(listen(proxy_sock, 5)==-1)
		error_handling("listen() error");
	
	while(1)
	{
		adr_sz=sizeof(clnt_adr);
		clnt_sock=accept(proxy_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
		if(clnt_sock==-1)
			continue;
		
		sock=socket(PF_INET, SOCK_STREAM, 0);   			
		if(sock==-1)
			error_handling("socket() error");
		memset(&serv_adr, 0, sizeof(serv_adr));
		serv_adr.sin_family=AF_INET;
		serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
		serv_adr.sin_port=htons(atoi(argv[3]));
	
		if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
			error_handling("connect() error!");
		
		pid=fork();
		if(pid==-1) {
			close(clnt_sock);
			continue;
		}
		if(pid==0) {
			close(proxy_sock);
			
			while(1)
			{
				recvmsgType(clnt_sock, &msgType);
				if(msgType==EMPTY_MSG) {
					close(clnt_sock);
					sendmsgType(sock, &msgType);				//send message type1 to server
					sendClntmessage1(sock, &auth_req);
					exit(1);
				}
				if(msgType==AUTH_RES_MSG) {
					close(clnt_sock);
					sendmsgType(sock, &msgType);				//send message type1 to server
					sendClntmessage3(sock, &sql_req);
					exit(1);
				}
				switch(msgType)							//receive message from client
				{			
					case AUTH_REQ_MSG:					//receive message type1 from client
						recvClntmessage1(clnt_sock, &auth_req);
						printf("Client Request Msg -> %s, %s\n", auth_req.user_id, auth_req.password);
						sendmsgType(sock, &msgType);			//send message type1 to server
						sendClntmessage1(sock, &auth_req);
						break;	
					case SQL_REQ_MSG:					//receive message type3 from client
						recvClntmessage3(clnt_sock, &sql_req);
						printf("Client Request Msg -> %s\n", sql_req.sql_text);
						sendmsgType(sock, &msgType);			//send message type3 to server
						sendClntmessage3(sock, &sql_req);
						break;
					case CLOSE_REQ_MSG:					//receive message type5 from client
						recvClntmessage5(clnt_sock, &close_req);
						sendmsgType(sock, &msgType);			//send message type3 to server
						sendClntmessage5(sock, &close_req);
						break;
				}
				recvmsgType(sock, &msgType);
				if(msgType==AUTH_REQ_MSG || msgType==SQL_REQ_MSG)
					return 0;
				switch(msgType)							//receive message from server
				{
					case AUTH_REQ_MSG:					
					case AUTH_RES_MSG:							
						recvServmessage2(sock, &auth_res);		//receive message type2 from server
						printf("Server Response Msg -> %s\n", auth_res.rtn_msg);
						sendmsgType(clnt_sock, &msgType);		//send message type2 to client
						sendServmessage2(clnt_sock, &auth_res);
						if(msgType==AUTH_REQ_MSG) {	
						close(sock);
						puts("Server Response Msg -> Server Connection closed");
						exit(1);
						}
						break;
					case SQL_RES_MSG:							
						recvServmessage4(sock, &sql_res);		//receive message type4 from server
						printf("Server Response Msg -> %s", sql_res.rtn_data);
						sendmsgType(clnt_sock, &msgType);		//send message type4 to client
						sendServmessage4(clnt_sock, &sql_res);
						break;		
					case CLOSE_RES_MSG:							
						recvServmessage6(sock,  &close_res);		//receive message type6 from server
						printf("Client Request Msg -> exit;\n");
						printf("Server Response Msg -> %s", close_res.rtn_msg);
						close(sock);
						sendmsgType(clnt_sock, &msgType);		//send message type6 to client
						sendServmessage6(clnt_sock, &close_res);
						return 0;
						break;
				}
			}
		}
		else
			close(clnt_sock);
	}
	close(proxy_sock);
	return 0;
}
