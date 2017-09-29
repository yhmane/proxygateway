//
//  serverfunc.c
//
//  Created by hwangyunho on 2017. 9. 18.
//  last modified by hwangyunho on 2017. 9. 26.
//  Copyright © 2017 hwangyunho. All rights reserved.
//

#include "serverfunc.h"
void readChildProc(int sig)
{
        pid_t pid;
        int status;
        pid=waitpid(-1, &status, WNOHANG);
        printf("removed proc id: %d \n", pid);
}

void recvAuthReqMsg(int clnt_sock, dgt_auth_req_msg *auth_req)                               
{
        len=recv(clnt_sock, auth_req,sizeof(*auth_req),0);
        if(len==-1) errorHandling("recv() error");
}

void setAuthResMsg(char *static_ip, dgt_auth_res_msg *auth_res)                                                
{
        auth_res->msg_type=AUTH_RES_MSG ;
        auth_res->rtn_len=strlen(static_ip);
        memcpy(auth_res->rtn_msg, static_ip, strlen(static_ip)+1);
}

void sendAuthResMsg(int clnt_sock, dgt_auth_res_msg *auth_res)                               
{   
        len=send(clnt_sock, auth_res, sizeof(*auth_res),0);
        if(len==-1) errorHandling("send() error");
}

void recvSqlReqMsg(int clnt_sock, dgt_sql_req_msg *sql_req)                                  
{
        len=recv(clnt_sock, sql_req, sizeof(*sql_req),0);
        if(len==-1) errorHandling("recv() error");
}

void setSqlResMsg(dgt_sql_req_msg *sql_req, dgt_sql_res_msg *sql_res)           
{
        char sql1[SQL_TEXT]="Result : select sql is successful.\n";
        char sql2[SQL_TEXT]="Result : delete sql is successful.\n";
        char sql3[SQL_TEXT]="Result : update sql is successful.\n";
        char sql4[SQL_TEXT]="Result : unsupported sql is executed.\n";
        sql_res->msg_type=SQL_RES_MSG;
        sql_res->sql_type=sql_req->sql_type;
        memset(sql_res->rtn_data, '\0', sizeof(sql_res->rtn_data));     //sql message reset
    
        if(sql_res->sql_type==SELECT) {
                memcpy(sql_res->rtn_data, sql1, strlen(sql1));
                sql_res->rtn_len=strlen(sql1);
        } else if(sql_res->sql_type==DELETE) {
                memcpy(sql_res->rtn_data, sql2, strlen(sql2));
                sql_res->rtn_len=strlen(sql2);
        } else if(sql_res->sql_type==UPDATE) {
                memcpy(sql_res->rtn_data, sql3, strlen(sql3));
                sql_res->rtn_len=strlen(sql3);
        } else {
                memcpy(sql_res->rtn_data, sql4, strlen(sql4));
                sql_res->rtn_len=strlen(sql4);
        }
}

void sendSqlResMsg(int clnt_sock, dgt_sql_res_msg *sql_res)                                  
{   
        len=send(clnt_sock, sql_res, sizeof(*sql_res),0);
        if(len==-1) errorHandling("send() error");
}

void recvCloseReqMsg(int clnt_sock, dgt_close_req_msg *close_req)                              
{
        len=recv(clnt_sock, close_req, sizeof(*close_req),0);
        if(len==-1) errorHandling("recv() error");
}

void setCloseResMsg(dgt_close_res_msg *close_res)                                 
{
        char final[RTN_MSG]="Server connection is closed\n";
        close_res->msg_type=CLOSE_RES_MSG;
        close_res->rtn_len=strlen(final);   
        memcpy(close_res->rtn_msg, final, strlen(final));
}

void sendCloseResMsg(int clnt_sock, dgt_close_res_msg *close_res)                              
{   
        len=send(clnt_sock, close_res, sizeof(*close_res),0);
        if(len==-1) errorHandling("send() error");
}

//
//      check id "scott", password "tiger"
//
void checkIdPwd(UB2 *msg_type, dgt_auth_req_msg auth_req)
{
        if((strcasecmp((char *)auth_req.user_id,"scott")==0) && (strcasecmp((char *)auth_req.password, "tiger")==0)) *msg_type=AUTH_RES_MSG;                                
        else *msg_type=CLOSE_RES_MSG;
}
