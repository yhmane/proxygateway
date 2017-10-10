//
//  dbclient.c
//
//  Created by hwangyunho on 2017. 9. 18.
//  last modified by hwangyunho on 2017. 9. 29.
//  Copyright © 2017 hwangyunho. All rights reserved.
//

#include "clientfunc.h"
int main(int argc, char *argv[])
{
	struct sockaddr_in serv_adr;
	
	if(argc!=3) {															//execute like	"./dbclient <proxy_ip> <proxy_port>"
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	sock=socket(PF_INET, SOCK_STREAM, 0);   					
	if(sock==-1) errorHandling("socket() error");
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));
	
	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1) errorHandling("connect() error!");		
	signal(SIGINT, ctrlChandler);									//ctrl + c signal;
	signal(SIGALRM, timeOut);										//time limit 120s signal;
	alarm(120);							

	msg_type=AUTH_REQ_MSG;
	while(1)
	{
		switch(msg_type)													//send protocal
		{
			case AUTH_REQ_MSG:
				setAuthReqMsg(&auth_req);									//scanf id, pwd
				sendMsgType(sock, msg_type);								//send id/pwd
				sendAuthReqMsg(sock, &auth_req);
				break;
			
			case SQL_REQ_MSG:							
				setSqlReqMsg(&sql_req);										//scanf sql sentence 
				if((strcasecmp((char *)sql_req.sql_text, "exit;")==0)) {	//if sql sentence=="exit" ->  connection send CLOSE_REQ
					msg_type=CLOSE_REQ_MSG;
					continue;
				}
				sendMsgType(sock, msg_type);
				sendSqlReqMsg(sock, &sql_req);								//send sql
				break;
			
			case CLOSE_REQ_MSG:							
				setCloseReqMsg(&close_req);
				sendMsgType(sock, msg_type);
				sendCloseReqMsg(sock, &close_req);							//send CloseResMsg
				break;

			default: break;
		}

		recvMsgType(sock, &msg_type);

		switch(msg_type)													//receive protocal
		{
			case AUTH_RES_MSG:							
				recvAuthResMsg(sock, &auth_res);							//receive ip, port
				displayAuthResMsg(auth_res);
				msg_type=SQL_REQ_MSG;
				getchar();													//scanf buffer drop
				break;
			
			case SQL_RES_MSG:							
				recvSqlResMsg(sock, &sql_res);								//receive sql result
				printf("%s", sql_res.rtn_data);
				msg_type=SQL_REQ_MSG;
				break;		
			
			case CLOSE_RES_MSG:						
				recvCloseResMsg(sock,  &close_res);							//receive CloseResMSg
				printf("%s", close_res.rtn_msg);
				close(sock);
				return 0;

			default: break;
		}
	}
	return 0;
}