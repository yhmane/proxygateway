//
//  header.h
//  tcp
//
//  Created by hwangyunho on 2017. 9. 18.
//  last modified by hwangyunho on 2017. 9. 25.
//  Copyright © 2017년 hwangyunho. All rights reserved.
//

#ifndef __HEADER_H_
#define __HEADER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>
#include <sys/wait.h>

#define BUF_SIZE 1024
#define UB2 unsigned short
#define UB4 unsigned int
#define SCHAR signed char
#define select 1
#define delete 2
#define update 3
#define sqlerror 4
#define auth_req_msg 1
#define auth_res_msg 2
#define sql_req_msg 3
#define sql_res_msg 4
#define close_req_msg 5
#define close_res_msg 6

int checklen;
typedef struct _dgt_auth_req_msg		//type1
{
	UB2 msg_type;
	SCHAR user_id[33];
	SCHAR password[33];
}dgt_auth_req_msg; 
 
typedef struct _dgt_auth_res_msg		//type2
{
	UB2 msg_type;
	UB2 rtn_len;
	SCHAR rtn_msg[257];
}dgt_auth_res_msg; 
 
typedef struct _dgt_sql_req_msg			//type3
{
	UB2 msg_type;
	UB4 sql_len;
	SCHAR sql_text[1025];
	UB2 sql_type;
}dgt_sql_req_msg; 
 
typedef struct _dgt_sql_res_msg			//type4
{
	UB2 msg_type;
	UB4 rtn_len;
	SCHAR rtn_data[1025];
	UB2 sql_type;
}dgt_sql_res_msg;

typedef struct _dgt_close_req_msg 		//type5
{
	UB2 msg_type;
}dgt_close_req_msg;

typedef struct _dgt_close_res_msg		//type6
{
	UB2 msg_type;
	UB2 rtn_len;
	SCHAR rtn_msg[257];
}dgt_close_res_msg; 

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void sendmsgtype(int sock, UB2 *msgtype)				//send msgtype just UB2
{
	checklen=send(sock, msgtype, sizeof(*msgtype),0);
	if(checklen==-1) 
		error_handling("send() error");
}

void recvmsgtype(int sock, UB2 *msgtype)				//send msgtype just UB2
{
	checklen=recv(sock, msgtype,sizeof(*msgtype),0);
	if(checklen==-1)
		error_handling("recv() error");
}
#endif
