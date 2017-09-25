//
//  dbserver.c
//  tcp
//
//  Created by hwangyunho on 2017. 9. 18.
//  last modified by hwangyunho on 2017. 9. 25.
//  Copyright © 2017년 hwangyunho. All rights reserved.
//

#include "serverfunc.h"
int main(int argc, char *argv[])
{	
	char id[USERID], password[PASSWORD];
	int serv_sock, clnt_sock, state;
	struct sockaddr_in serv_adr, clnt_adr;
	pid_t pid;
	struct sigaction act;
	char staticip[]="127.0.0.1";
	char ipmsg[RTN_MSG];
	socklen_t adr_sz;
	UB2 msgtype=EMPTY_MSG;

	dgt_auth_req_msg auth_req;
	dgt_auth_res_msg auth_res;
	dgt_sql_req_msg sql_req;
	dgt_sql_res_msg sql_res;
	dgt_close_req_msg close_req;
	dgt_close_res_msg close_res; 
	
	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	act.sa_handler=read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	state=sigaction(SIGCHLD, &act, 0);
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");
	
	while(1)
	{
		adr_sz=sizeof(clnt_adr);
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
		if(clnt_sock==-1)
			continue;
		else
			puts("Connected............. ");
		
		pid=fork();
		if(pid==-1) {
			close(clnt_sock);
			continue;
		}
		if(pid==0) {
			close(serv_sock);
			while(1)
			{
				recvmsgtype(clnt_sock, &msgtype);
				if(msgtype==EMPTY_MSG) {
					close(clnt_sock);
					exit(1);
				}
				if(msgtype==AUTH_RES_MSG) {
					close(clnt_sock);
					exit(1);
				}
				switch(msgtype)									//receive message
				{			
					case AUTH_REQ_MSG:							//receive message type1
						recvClntmessage1(clnt_sock, &auth_req);
						strcpy(id, auth_req.user_id);
  					 	strcpy(password, auth_req.password);

   						if((strcmp(id,"scott")==0) && (strcmp(password, "tiger")==0))
     					 	msgtype=AUTH_RES_MSG;
						else {
							error_handling("access() error");
							close(clnt_sock);
							exit(1);
						}
						break;	
					case SQL_REQ_MSG:							//receive message type3
						recvClntmessage3(clnt_sock, &sql_req);	
						msgtype=SQL_RES_MSG;
						break;
					case CLOSE_REQ_MSG:							//receive message type5
						recvClntmessage5(clnt_sock, &close_req);
						msgtype=CLOSE_RES_MSG;
						break;
				}
				sendmsgtype(clnt_sock, &msgtype);
				switch(msgtype)									//send message
				{
					case AUTH_RES_MSG:							//send message type1
						strcat(ipmsg, staticip);
						strcat(ipmsg, ", ");
						strcat(ipmsg, argv[1]);

						setServmessage2(msgtype, ipmsg, &auth_res);
						sendServmessage2(clnt_sock, &auth_res);
						break;					
					case SQL_RES_MSG:							//send message tyoe3
						setServmessage4(msgtype, &sql_req, &sql_res);
						sendServmessage4(clnt_sock, &sql_res);
						break;
					case CLOSE_RES_MSG:							//send message type5
						setServmessage6(msgtype, &close_res);
						sendServmessage6(clnt_sock, &close_res);
						close(clnt_sock);
						puts("Server connection is closed");
						return 0;
						break;
				}
			}
		}
		else
			close(clnt_sock);
	}
	close(serv_sock);
	return 0;
}
