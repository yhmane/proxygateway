//
//  dbclient.h
//  tcp
//
//  Created by hwangyunho on 2017. 9. 18.
//  last modified by hwangyunho on 2017. 9. 25.
//  Copyright © 2017년 hwangyunho. All rights reserved.
//

#ifndef __HEADER_H_
#include "header.h"
#endif

#ifndef __CLIENTFUNC_H_
#define __CLIENTFUNC_H_

void setClntmessage1(char *id, char *password, UB2 msgtype, dgt_auth_req_msg *auth_req)		//set msgtype1
{
	auth_req->msg_type=auth_req_msg;
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

void setClntmessage3(UB2 msgtype, UB4 sql_len, char *sql_text, dgt_sql_req_msg *sql_req)	//set msgtype3
{
	int i;
	char t[1025];
	sql_req->msg_type=msgtype;
	sql_req->sql_len = strlen(sql_text);
	memcpy(sql_req->sql_text, sql_text, strlen(sql_text)+1);
    
	for(i =0; i<6;i++)
		t[i]=sql_req->sql_text[i];
	if((strcmp(t,"select") ==0) || (strcmp(t,"SELECT") ==0))
		sql_req->sql_type=select;
	else if((strcmp(t,"delete") ==0) || (strcmp(t,"DELETE") ==0))
		sql_req->sql_type=delete;
	else if((strcmp(t,"update") ==0) || (strcmp(t,"UPDATE") ==0))
		sql_req->sql_type=update;
	else
		sql_req->sql_type=sqlerror;
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

void setClntmessage5(UB2 msgtype, dgt_close_req_msg *close_req)					//set msgtype5
{
	close_req->msg_type=close_req_msg;
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

#endif
