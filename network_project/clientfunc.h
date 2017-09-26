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

#ifndef __CLIENTFUNC_H_
#define __CLIENTFUNC_H_

void setClntmessage1(char *id, char *password, UB2 msgType, dgt_auth_req_msg *auth_req);		//set msgtype1
void sendClntmessage1(int sock, dgt_auth_req_msg *auth_req);					                      //send msgtype1
void recvServmessage2(int sock, dgt_auth_res_msg *auth_res);					                      //receive msgtype2
void setClntmessage3(UB2 msgType, UB4 sql_len, char *sql_text, dgt_sql_req_msg *sql_req); 	//set msgtype3
void sendClntmessage3(int sock, dgt_sql_req_msg *sql_req);					                        //send msgtype3
void recvServmessage4(int sock, dgt_sql_res_msg *sql_res);					                        //receive msgtype4
void setClntmessage5(UB2 msgType, dgt_close_req_msg *close_req);					                  //set msgtype5
void sendClntmessage5(int sock, dgt_close_req_msg *close_req);					                    //send msgtype5
void recvServmessage6(int sock, dgt_close_res_msg *close_res);					                    //receive msgtype6    
#endif
