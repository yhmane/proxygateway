//
//  clientfunc.c
//
//  Created by hwangyunho on 2017. 9. 18.
//  last modified by hwangyunho on 2017. 9. 26.
//  Copyright © 2017 hwangyunho. All rights reserved.
//

#include "clientfunc.h"
void setAuthReqMsg(dgt_auth_req_msg *auth_req)
{
	auth_req->msg_type=AUTH_REQ_MSG;
	printf("ID : ");
	scanf("%s", auth_req->user_id);
	printf("Password : ");
	scanf("%s", auth_req->password);
}
void sendAuthReqMsg(int sock, dgt_auth_req_msg *auth_req)
{
	len=send(sock, auth_req, sizeof(*auth_req),0);
	if(len==-1) errorHandling("send() error");   
}

void recvAuthResMsg(int sock, dgt_auth_res_msg *auth_res)
{
	len=recv(sock, auth_res,sizeof(*auth_res),0);
	if(len==-1) errorHandling("recv() error");
}

void setSqlReqMsg(dgt_sql_req_msg *sql_req)
{
	char check_sql[1025];
	sql_req->msg_type=SQL_REQ_MSG;
	printf("SQL > ");
	fgets((char *)sql_req->sql_text, SQL_TEXT,stdin);
	sql_req->sql_text[strlen((char *)sql_req->sql_text) - 1] = '\0';				// delete '\n'
	sql_req->sql_len = strlen((char *)sql_req->sql_text);
	
	for(len =0; len<6;len++) check_sql[len]=sql_req->sql_text[len];			//substr select, delete, update		
	if(strcasecmp(check_sql,"select") ==0) sql_req->sql_type=SELECT;
	else if(strcasecmp(check_sql,"delete") ==0) sql_req->sql_type=DELETE;
	else if(strcasecmp(check_sql,"update") ==0) sql_req->sql_type=UPDATE;
	else sql_req->sql_type=SQLERROR;
}

void sendSqlReqMsg(int sock, dgt_sql_req_msg *sql_req)
{
	len=send(sock, sql_req, sizeof(*sql_req),0);
	if(len==-1) errorHandling("send() error");
}

void recvSqlResMsg(int sock, dgt_sql_res_msg *sql_res)
{
	len=recv(sock, sql_res,sizeof(*sql_res),0);
	if(len==-1) errorHandling("recv() error");
}

void setCloseReqMsg(dgt_close_req_msg *close_req)
{
	close_req->msg_type=CLOSE_REQ_MSG;
}

void sendCloseReqMsg(int sock, dgt_close_req_msg *close_req)
{
	len=send(sock, close_req, sizeof(*close_req),0);
	if(len==-1) errorHandling("send() error");
}

void recvCloseResMsg(int sock, dgt_close_res_msg *close_res)  
{
	len=recv(sock, close_res,sizeof(*close_res),0);
	if(len==-1) errorHandling("recv() error");
}

//
// display auth_res_msg
//
void displayAuthResMsg(dgt_auth_res_msg auth_res)			//divide char *msg 127.0.0.1, 9190 => char ip 127.0.0.1  char port 9190 
{
	int ipCheck=0, portCheck=0;
	char ip[RTN_MSG], port[RTN_MSG];
	printf("=====================================\n");
	printf("Connection is Successful.\n");
	while(auth_res.rtn_msg[ipCheck]!=',')
	{
		ip[ipCheck]=auth_res.rtn_msg[ipCheck];
		ipCheck++;
	}
	for(ipCheck+=2;ipCheck<auth_res.rtn_len;ipCheck++) port[portCheck++]=auth_res.rtn_msg[ipCheck];
	printf(" Server Ip : %s\n", ip);
	printf(" Server Port : %s\n", port);
	printf("=====================================\n");
}

//
// quit client connect
//
void timeOut(int signo)
{
	alarm(60);						//session is 60s
	msg_type=CLOSE_REQ_MSG;
	sendMsgType(sock, msg_type);
	close(sock);
	exit(1);
}

//
// ctrl+c 
//
void ctrlChandler(int signo)
{
	msg_type=CLOSE_REQ_MSG;
	sendMsgType(sock, msg_type);
	close(sock);
	exit(1);
}