//
//  dbclient.h
//  tcp
//
//  Created by hwangyunho on 2017. 9. 18.
//  last modified by hwangyunho on 2017. 9. 26.
//  Copyright © 2017년 hwangyunho. All rights reserved.
//



#include "clientfunc.h"
void setClntmessage1(char *id, char *password, UB2 msgType, dgt_auth_req_msg *auth_req)		//set msgtype1
{
	auth_req->msg_type=AUTH_REQ_MSG;
	memcpy(auth_req->user_id, id, strlen(id)+1);
	memcpy(auth_req->password, password, strlen(id)+1);
}
void sendClntmessage1(int sock, dgt_auth_req_msg *auth_req)					//send msgtype1
{
	checklen=send(sock, auth_req, sizeof(*auth_req),0);
	if(checklen==-1)
		error_handling("send() error");   
}

void recvServmessage2(int sock, dgt_auth_res_msg *auth_res)					//receive msgtype2
{
	checklen=recv(sock, auth_res,sizeof(*auth_res),0);
	if(checklen==-1)
		error_handling("recv() error");
}

void setClntmessage3(UB2 msgType, UB4 sql_len, char *sql_text, dgt_sql_req_msg *sql_req)	//set msgtype3
{
	int i;
	char t[SQL_TEXT];
	sql_req->msg_type=msgType;
	sql_req->sql_len = strlen(sql_text);
	memcpy(sql_req->sql_text, sql_text, strlen(sql_text)+1);
    
	for(i =0; i<6;i++)
		t[i]=sql_req->sql_text[i];
	if((strcmp(t,"select") ==0) || (strcmp(t,"SELECT") ==0))
		sql_req->sql_type=SELECT;
	else if((strcmp(t,"delete") ==0) || (strcmp(t,"DELETE") ==0))
		sql_req->sql_type=DELETE;
	else if((strcmp(t,"update") ==0) || (strcmp(t,"UPDATE") ==0))
		sql_req->sql_type=UPDATE;
	else
		sql_req->sql_type=SQLERROR;
}
void sendClntmessage3(int sock, dgt_sql_req_msg *sql_req)					//send msgtype3
{
	checklen=send(sock, sql_req, sizeof(*sql_req),0);
	if(checklen==-1)
		error_handling("send() error");
}

void recvServmessage4(int sock, dgt_sql_res_msg *sql_res)					//receive msgtype4
{
	checklen=recv(sock, sql_res,sizeof(*sql_res),0);
	if(checklen==-1)
		error_handling("recv() error");
}

void setClntmessage5(UB2 msgType, dgt_close_req_msg *close_req)					//set msgtype5
{
	close_req->msg_type=CLOSE_REQ_MSG;
}
void sendClntmessage5(int sock, dgt_close_req_msg *close_req)					//send msgtype5
{
	checklen=send(sock, close_req, sizeof(*close_req),0);
	if(checklen==-1)
		error_handling("send() error");
}

void recvServmessage6(int sock, dgt_close_res_msg *close_res)					//receive msgtype6    
{
	checklen=recv(sock, close_res,sizeof(*close_res),0);
	if(checklen==-1)
		error_handling("recv() error");
}