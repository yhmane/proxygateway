//
//  clientfunc.c
//
//  Created by hwangyunho on 2017. 9. 18.
//  last modified by hwangyunho on 2017. 9. 29.
//  Copyright © 2017 hwangyunho. All rights reserved.
//

#include "clientfunc.h"
void setAuthReqMsg(dgt_auth_req_msg *auth_req)				//set msg_type, scanf id & pwd
{
	auth_req->msg_type=AUTH_REQ_MSG;				
	printf("ID : ");
	scanf("%s", auth_req->user_id);
	printf("Password : ");
	scanf("%s", auth_req->password);
}
void sendAuthReqMsg(int sock, dgt_auth_req_msg *auth_req)	//send AuthReqMsg to Server
{
	check_byte=write(sock, auth_req, sizeof(*auth_req));
	if(check_byte==-1) errorHandling("send() error");   
}

void recvAuthResMsg(int sock, dgt_auth_res_msg *auth_res)	//receive AuthResMsg from Server
{
	len=0, check_byte=0;
	while(sizeof(*auth_res)>len)							//receive all the bytes
	{
		check_byte=read(sock, auth_res,sizeof(*auth_res));
		if(check_byte==-1) errorHandling("recv() error");
		len+=check_byte;
	}
	if(auth_res->rtn_len!=strlen((char *)auth_res->rtn_msg)) errorHandling("recv() error!"); //sigend -> char * casting to use strlen()
}

void setSqlReqMsg(dgt_sql_req_msg *sql_req)				//set msg_type, scanf sql sentence
{
	char check_sql[SQL_TEXT];
	sql_req->msg_type=SQL_REQ_MSG;
	printf("SQL > ");
	fgets((char *)sql_req->sql_text, SQL_TEXT,stdin);
	sql_req->sql_text[strlen((char *)sql_req->sql_text) - 1] = '\0';		// delete '\n' 
	sql_req->sql_len = strlen((char *)sql_req->sql_text);					/* (char *) casting signed to char*  */
	
	for(len =0; len<6;len++) check_sql[len]=sql_req->sql_text[len];			//substr(0,6) to check senetence includes select, delete, update		
	if(strcasecmp(check_sql,"select") ==0) sql_req->sql_type=SELECT;
	else if(strcasecmp(check_sql,"delete") ==0) sql_req->sql_type=DELETE;
	else if(strcasecmp(check_sql,"update") ==0) sql_req->sql_type=UPDATE;
	else sql_req->sql_type=SQLERROR;
}

void sendSqlReqMsg(int sock, dgt_sql_req_msg *sql_req)		//send SqlReqMsg to Server
{
	check_byte=write(sock, sql_req, sizeof(*sql_req));
	if(check_byte==-1) errorHandling("send() error");
}

void recvSqlResMsg(int sock, dgt_sql_res_msg *sql_res)		//receive SqlResMsg from Server
{
	len=0, check_byte=0;
	while(sizeof(*sql_res)>len)								//receive all the bytes
	{
		check_byte=read(sock, sql_res,sizeof(*sql_res));
		if(check_byte==-1) errorHandling("recv() error");
		len+=check_byte;
	}
	if(sql_res->rtn_len!=strlen((char *)sql_res->rtn_data)) errorHandling("recv() error!"); //sigend -> char * casting to use strlen()
}

void setCloseReqMsg(dgt_close_req_msg *close_req)			//set msg_type CloseReq
{
	close_req->msg_type=CLOSE_REQ_MSG;
}

void sendCloseReqMsg(int sock, dgt_close_req_msg *close_req)	//send CloseReqMsg to Server
{
	check_byte=write(sock, close_req, sizeof(*close_req));
	if(check_byte==-1) errorHandling("send() error");
}

void recvCloseResMsg(int sock, dgt_close_res_msg *close_res)  	//receive CloseResMsg from Server
{
	len=0, check_byte=0;
	while(sizeof(*close_res)>len)								//receive all the bytes
	{
		check_byte=read(sock, close_res,sizeof(*close_res));
		if(check_byte==-1) errorHandling("recv() error");
		len+=check_byte;
	}
	if(close_res->rtn_len!=strlen((char *)close_res->rtn_msg)) errorHandling("recv() error!"); //sigend -> char * casting to use strlen()
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
	while(auth_res.rtn_msg[ipCheck]!=',')					//find the index=','  ip=x.x.x.x
	{
		ip[ipCheck]=auth_res.rtn_msg[ipCheck];
		ipCheck++;
	}
	for(ipCheck+=2;ipCheck<auth_res.rtn_len;ipCheck++) port[portCheck++]=auth_res.rtn_msg[ipCheck]; //port=x
	printf(" Server Ip : %s\n", ip);
	printf(" Server Port : %s\n", port);
	printf("=====================================\n");
}

//
// time limit signal
//
void timeOut(int signo)
{
	msg_type=CLOSE_REQ_MSG;					
	sendMsgType(sock, msg_type);			//send CloseReq to Server
	sendCloseReqMsg(sock, &close_req);
	close(sock);							//disconnect
	exit(1);
}

//
// time limit signal
//
void timeOutProxy(int signo)
{
	msg_type=CLOSE_REQ_MSG;					
	sendMsgType(serv_sock, msg_type);			//send CloseReq to Server
	sendCloseReqMsg(serv_sock, &close_req);
	close(serv_sock);							//disconnect
	exit(1);
}

//
// ctrl+c signal
//
void ctrlChandler(int signo)
{
	msg_type=CLOSE_REQ_MSG;
	sendMsgType(sock, msg_type);			//send CloseReq to Server
	sendCloseReqMsg(sock, &close_req);		
	close(sock);							//disconnect
	exit(1);
}