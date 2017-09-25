//
//  serverfunc.h
//  tcp
//
//  Created by hwangyunho on 2017. 9. 18.
//  last modified by hwangyunho on 2017. 9. 25.
//  Copyright © 2017년 hwangyunho. All rights reserved.
//

#ifndef __HEADER_H_
#include "header.h"
#endif

#ifndef __SERVERFUNC_H_
#define __SERVERFUNC_H_

void read_childproc(int sig)
{
        pid_t pid;
        int status;
        pid=waitpid(-1, &status, WNOHANG);
        printf("removed proc id: %d \n", pid);
}
void recvClntmessage1(int clnt_sock, dgt_auth_req_msg *auth_req)                               //receive msgtype1 
{
        checklen=recv(clnt_sock, auth_req,sizeof(*auth_req),0);
        if(checklen==-1)
                error_handling("recv() error");
}

void setServmessage2(UB2 msgtype, dgt_auth_res_msg *auth_res)                                   //set msgtype2                   
{
        char r2_msg[257]="================================\nConnection is Successful.\nServer IP :192.168.10.111\nServer Port : 1521\n================================\n";
        auth_res->msg_type=auth_res_msg ;
        auth_res->rtn_len=strlen(r2_msg);
        memcpy(auth_res->rtn_msg, r2_msg, strlen(r2_msg)+1);
}
void sendServmessage2(int clnt_sock, dgt_auth_res_msg *auth_res)                                //send msgtype2
{   
        checklen=send(clnt_sock, auth_res, sizeof(*auth_res),0);
        if(checklen==-1)
                error_handling("send() error");
}

void recvClntmessage3(int clnt_sock, dgt_sql_req_msg *sql_req)                                  //receive msgtype3
{
        checklen=recv(clnt_sock, sql_req, sizeof(*sql_req),0);
        if(checklen==-1)
                error_handling("recv() error");
}

void setServmessage4(UB2 msgtype, dgt_sql_req_msg *sql_req, dgt_sql_res_msg *sql_res)           //set msgtype4
{
        char sql1[]="Result : select sql is successful.\n";
        char sql2[]="Result : delete sql is successful.\n";
        char sql3[]="Result : update sql is successful.\n";
        char sql4[]="Result : unsupported sql is executed.\n";
        sql_res->msg_type=sql_res_msg;
        sql_res->sql_type=sql_req->sql_type;
        memset(sql_res->rtn_data, '\0', sizeof(sql_res->rtn_data));
    
        if(sql_res->sql_type==select) {
                memcpy(sql_res->rtn_data, sql1, strlen(sql1));
                sql_res->rtn_len=strlen(sql1);
        }
        else if(sql_res->sql_type==delete) {
                memcpy(sql_res->rtn_data, sql2, strlen(sql2));
                sql_res->rtn_len=strlen(sql2);
        }
        else if(sql_res->sql_type==update) {
                memcpy(sql_res->rtn_data, sql3, strlen(sql3));
                sql_res->rtn_len=strlen(sql3);
        }
        else {
                memcpy(sql_res->rtn_data, sql4, strlen(sql4));
                sql_res->rtn_len=strlen(sql4);
        }
}

void sendServmessage4(int clnt_sock, dgt_sql_res_msg *sql_res)                                  //send msgtype4
{   
        checklen=send(clnt_sock, sql_res, sizeof(*sql_res),0);
        if(checklen==-1)
                error_handling("send() error");
}

void recvClntmessage5(int clnt_sock, dgt_close_req_msg *close_req)                              //receive msgtype5
{
        checklen=recv(clnt_sock, close_req, sizeof(*close_req),0);
        if(checklen==-1)
                error_handling("recv() error");
}

void setServmessage6(UB2 msgtype, dgt_close_res_msg *close_res)                                 //set msgtype6
{
        char final[]="Server connection is closed\n";
        close_res->msg_type=close_res_msg;
        close_res->rtn_len=strlen(final);   
        memcpy(close_res->rtn_msg, final, strlen(final));
}

void sendServmessage6(int clnt_sock, dgt_close_res_msg *close_res)                              //send msgtype6
{   
        checklen=send(clnt_sock, close_res, sizeof(*close_res),0);
        if(checklen==-1)
                error_handling("send() error");
}
#endif
