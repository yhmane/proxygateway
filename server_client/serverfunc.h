//
//	serverfunc.h
//
//  Created by hwangyunho on 2017. 9. 18.
//  last modified by hwangyunho on 2017. 9. 29.
//  Copyright © 2017 hwangyunho. All rights reserved.
//


#ifndef __SERVERFUNC_H_
#define __SERVERFUNC_H_

#include "commonfunc.h"

void readChildProc(int sig);
void recvAuthReqMsg(int clnt_sock, dgt_auth_req_msg *auth_req);                      
void setAuthResMsg(char *static_ip, dgt_auth_res_msg *auth_res);                                
void sendAuthResMsg(int clnt_sock, dgt_auth_res_msg *auth_res);                      
void recvSqlReqMsg(int clnt_sock, dgt_sql_req_msg *sql_req);                         
void setSqlResMsg(dgt_sql_req_msg *sql_req, dgt_sql_res_msg *sql_res);  
void sendSqlResMsg(int clnt_sock, dgt_sql_res_msg *sql_res);                         
void recvCloseReqMsg(int clnt_sock, dgt_close_req_msg *close_req);                     
void setCloseResMsg(dgt_close_res_msg *close_res);                        
void sendCloseResMsg(int clnt_sock, dgt_close_res_msg *close_res);        


//
//	check id "scott" pwd "tiger"
//
void checkIdPwd(UB2 *msg_type, dgt_auth_req_msg auth_req);          
#endif
