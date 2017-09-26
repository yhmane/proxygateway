//
//  dbclient.h
//  tcp
//
//  Created by hwangyunho on 2017. 9. 18.
//  last modified by hwangyunho on 2017. 9. 26.
//  Copyright © 2017년 hwangyunho. All rights reserved.
//


#ifndef __HEADER_H_
#include "header.h"
#endif

#ifndef __COMMONFUNC_H_
#include "commonfunc.h"
#endif

#ifndef __SERVERFUNC_H_
#define __SERVERFUNC_H_

void read_childproc(int sig);
void recvClntmessage1(int clnt_sock, dgt_auth_req_msg *auth_req);                       //receive msgtype1 
void setServmessage2(UB2 msgtype, char *ipmsg, dgt_auth_res_msg *auth_res);             //set msgtype2                   
void sendServmessage2(int clnt_sock, dgt_auth_res_msg *auth_res);                       //send msgtype2
void recvClntmessage3(int clnt_sock, dgt_sql_req_msg *sql_req);                         //receive msgtype3
void setServmessage4(UB2 msgtype, dgt_sql_req_msg *sql_req, dgt_sql_res_msg *sql_res);  //set msgtype4
void sendServmessage4(int clnt_sock, dgt_sql_res_msg *sql_res);                         //send msgtype4
void recvClntmessage5(int clnt_sock, dgt_close_req_msg *close_req);                     //receive msgtype5
void setServmessage6(UB2 msgtype, dgt_close_res_msg *close_res);                        //set msgtype6
void sendServmessage6(int clnt_sock, dgt_close_res_msg *close_res);                     //send msgtype6
#endif
