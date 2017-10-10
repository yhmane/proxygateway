//
//  serverfunc.c
//
//  Created by hwangyunho on 2017. 9. 18.
//  last modified by hwangyunho on 2017. 9. 29.
//  Copyright © 2017 hwangyunho. All rights reserved.
//

#include "serverfunc.h"
void readChildProc(int sig)                                                      //display killed process id
{
        pid_t pid;
        int status;
        pid=waitpid(-1, &status, WNOHANG);
        printf("removed proc id: %d \n", pid);
}

void recvAuthReqMsg(int clnt_sock, dgt_auth_req_msg *auth_req)                  //receive AuthResMsg from Client    
{
        len=0, check_byte=0;
        while(sizeof(*auth_req)>len)                            
        {
                check_byte=read(clnt_sock, auth_req,sizeof(*auth_req));         //receive all the bytes
                if(check_byte==-1) errorHandling("recv() error");
                len+=check_byte;
        }
}

void setAuthResMsg(char *static_ip, dgt_auth_res_msg *auth_res)                 //set char msg[] ="ip, port"                                   
{
        auth_res->msg_type=AUTH_RES_MSG ;
        auth_res->rtn_len=strlen(static_ip);
        memcpy(auth_res->rtn_msg, static_ip, strlen(static_ip)+1);
}

void sendAuthResMsg(int clnt_sock, dgt_auth_res_msg *auth_res)                  //send AuthResMsg to Client                     
{   
        check_byte=write(clnt_sock, auth_res, sizeof(*auth_res));
        if(check_byte==-1) errorHandling("send() error");
}

void recvSqlReqMsg(int clnt_sock, dgt_sql_req_msg *sql_req)                     //receive SqlResMsg from Client             
{
        len=0, check_byte=0;
        while(sizeof(*sql_req)>len)                                             //receive all the bytes
        {
                check_byte=read(clnt_sock, sql_req, sizeof(*sql_req));
                if(check_byte==-1) errorHandling("recv() error");
                len+=check_byte;
        }
        if(sql_req->sql_len!=strlen((char *)sql_req->sql_text)) errorHandling("recv() error!"); //sigend -> char * casting to use strlen()
}

void setSqlResMsg(dgt_sql_req_msg *sql_req, dgt_sql_res_msg *sql_res)           //store sql result in sql_res->rtn_data     
{
    char *sql[5]={"",
                  "Result : select sql is successful.\n",
                  "Result : delete sql is successful.\n",
                  "Result : update sql is successful.\n",
                  "Result : unsupported sql is executed.\n"};
        sql_res->msg_type=SQL_RES_MSG;
        sql_res->sql_type=sql_req->sql_type;                                    
        memset(sql_res->rtn_data, '\0', sizeof(sql_res->rtn_data));             //sql message reset
    
        if(sql_res->sql_type==SELECT) {
                memcpy(sql_res->rtn_data, sql[SELECT], strlen(sql[SELECT])+1);
                sql_res->rtn_len=strlen(sql[SELECT]);
        } else if(sql_res->sql_type==DELETE) {
                memcpy(sql_res->rtn_data, sql[DELETE], strlen(sql[DELETE])+1);
                sql_res->rtn_len=strlen(sql[DELETE]);
        } else if(sql_res->sql_type==UPDATE) {
                memcpy(sql_res->rtn_data, sql[UPDATE], strlen(sql[UPDATE])+1);
                sql_res->rtn_len=strlen(sql[UPDATE]);
        } else {
                memcpy(sql_res->rtn_data, sql[SQLERROR], strlen(sql[SQLERROR])+1);
                sql_res->rtn_len=strlen(sql[SQLERROR]);
        }
}

void sendSqlResMsg(int clnt_sock, dgt_sql_res_msg *sql_res)                     //send SqlResMsg to Client                               
{   
        check_byte=write(clnt_sock, sql_res, sizeof(*sql_res));
        if(check_byte==-1) errorHandling("send() error");
}

void recvCloseReqMsg(int clnt_sock, dgt_close_req_msg *close_req)               //receive CloseReqMsg from Client                   
{
        len=0, check_byte=0;
        while(sizeof(*close_req)>len)                                           //receive all the bytes
        {
                check_byte=read(clnt_sock, close_req, sizeof(*close_req));
                if(check_byte==-1) errorHandling("recv() error");
                len+=check_byte;
        }
}

void setCloseResMsg(dgt_close_res_msg *close_res)                              //char msg[]="Server connection is closed"
{
        char final[RTN_MSG]="Server connection is closed\n";
        close_res->msg_type=CLOSE_RES_MSG;
        close_res->rtn_len=strlen(final);   
        memcpy(close_res->rtn_msg, final, strlen(final)+1);
}

void sendCloseResMsg(int clnt_sock, dgt_close_res_msg *close_res)               //send CloseReqMsg to Client                   
{   
        check_byte=write(clnt_sock, close_res, sizeof(*close_res));
        if(check_byte==-1) errorHandling("send() error");
}

//
//      check id "scott", password "tiger"
//
void checkIdPwd(UB2 *msg_type, dgt_auth_req_msg auth_req)                       //check id,pwd == (scott, tiger)? send AuthResMsg : exit(1)
{
        if((strcasecmp((char *)auth_req.user_id,"scott")==0) && (strcasecmp((char *)auth_req.password, "tiger")==0)) *msg_type=AUTH_RES_MSG;                                
        else *msg_type=CLOSE_RES_MSG;
}
