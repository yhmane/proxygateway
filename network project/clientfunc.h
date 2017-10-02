//
//  clientfunc.h
//
//  Created by hwangyunho on 2017. 9. 18.
//  last modified by hwangyunho on 2017. 9. 26.
//  Copyright © 2017 hwangyunho. All rights reserved.
//

#ifndef __HEADER_H_
#include "header.h"
#endif

#ifndef __COMMONFUNC_H_
#include "commonfunc.h"
#endif

#ifndef __CLIENTFUNC_H_
#define __CLIENTFUNC_H_


int sock;
void setAuthReqMsg(dgt_auth_req_msg *auth_req);
void sendAuthReqMsg(int sock, dgt_auth_req_msg *auth_req);
void recvAuthResMsg(int sock, dgt_auth_res_msg *auth_res);
void setSqlReqMsg(dgt_sql_req_msg *sql_req);
void sendSqlReqMsg(int sock, dgt_sql_req_msg *sql_req);
void recvSqlResMsg(int sock, dgt_sql_res_msg *sql_res);
void setCloseReqMsg(dgt_close_req_msg *close_req);
void sendCloseReqMsg(int sock, dgt_close_req_msg *close_req);
void recvCloseResMsg(int sock, dgt_close_res_msg *close_res);

//
// display auth_res_msg
//
void displayAuthResMsg(dgt_auth_res_msg auth_res);

//
// quit client connect
//
void timeOut(int signo);

//
// ctrl+c 
//
void ctrlChandler(int signo);
#endif

