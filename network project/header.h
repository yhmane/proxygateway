//
//  header.h
//
//  Created by hwangyunho on 2017. 9. 18.
//  last modified by hwangyunho on 2017. 9. 26.
//  Copyright © 2017 hwangyunho. All rights reserved.
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

//
// data type
//

#define UB2 unsigned short
#define UB4 unsigned int
#define SCHAR signed char

//
// sql type
//

#define SELECT 1
#define DELETE 2
#define UPDATE 3

//
// sql error
//	wrong sql || exit; || EXIT;

#define SQLERROR 4

//
// message type
//

#define EMPTY_MSG 0
#define AUTH_REQ_MSG 1
#define AUTH_RES_MSG 2
#define SQL_REQ_MSG 3
#define SQL_RES_MSG 4
#define CLOSE_REQ_MSG 5
#define CLOSE_RES_MSG 6

//
// message length
//

#define USERID 33
#define PASSWORD 33
#define RTN_MSG 257
#define SQL_TEXT 1025
#define RTN_DATA 1025

typedef struct _dgt_auth_req_msg
{
	UB2 msg_type;
	SCHAR user_id[USERID];
	SCHAR password[PASSWORD];
}dgt_auth_req_msg; 
 
typedef struct _dgt_auth_res_msg
{
	UB2 msg_type;
	UB2 rtn_len;
	SCHAR rtn_msg[RTN_MSG];
}dgt_auth_res_msg; 
 
typedef struct _dgt_sql_req_msg	
{
	UB2 msg_type;
	UB4 sql_len;
	SCHAR sql_text[SQL_TEXT];
	UB2 sql_type;
}dgt_sql_req_msg; 
 
typedef struct _dgt_sql_res_msg	
{
	UB2 msg_type;
	UB4 rtn_len;
	SCHAR rtn_data[RTN_DATA];
	UB2 sql_type;
}dgt_sql_res_msg;

typedef struct _dgt_close_req_msg 	
{
	UB2 msg_type;
}dgt_close_req_msg;

typedef struct _dgt_close_res_msg		
{
	UB2 msg_type;
	UB2 rtn_len;
	SCHAR rtn_msg[RTN_MSG];
}dgt_close_res_msg; 

int len;
UB2 msg_type;

dgt_auth_req_msg auth_req;
dgt_auth_res_msg auth_res;
dgt_sql_req_msg sql_req;
dgt_sql_res_msg sql_res;
dgt_close_req_msg close_req;
dgt_close_res_msg close_res; 
#endif
