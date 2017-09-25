//
//  dbclient.c
//  tcp
//
//  Created by hwangyunho on 2017. 9. 18.
//  last modified by hwangyunho on 2017. 9. 25.
//  Copyright © 2017년 hwangyunho. All rights reserved.
//

#include "clientfunc.h"
int main(int argc, char *argv[])
{
	int sock, comma=0, porti=0;
	char id[USERID], password[PASSWORD], sql_text[SQL_TEXT], ip[RTN_MSG], port[RTN_MSG];
	struct sockaddr_in serv_adr;
	UB2 msgtype=EMPTY_MSG;

	dgt_auth_req_msg auth_req;
	dgt_auth_res_msg auth_res;
	dgt_sql_req_msg sql_req;
	dgt_sql_res_msg sql_res;
	dgt_close_req_msg close_req;
	dgt_close_res_msg close_res; 
	
	if(argc!=3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	sock=socket(PF_INET, SOCK_STREAM, 0);   			
	if(sock==-1)
		error_handling("socket() error");
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));
	
	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("connect() error!");
		
	msgtype=AUTH_REQ_MSG;
	while(1)
	{
		switch(msgtype)									//send protocal
		{
			case AUTH_REQ_MSG:							//msgtype1
				printf("ID : ");
   				scanf("%s", id);
  				printf("Password : ");
				scanf("%s", password);
				
				setClntmessage1(id, password, msgtype, &auth_req);
				sendmsgtype(sock, &msgtype);
				sendClntmessage1(sock, &auth_req);
				break;
			case SQL_REQ_MSG:							//msgtype3
				printf("SQL > ");
				fgets(sql_text,SQL_TEXT,stdin);
				sql_text[strlen(sql_text) - 1] = '\0';
				if((strcmp(sql_text, "exit;")==0) || (strcmp(sql_text, "EXIT;")==0)) {
					msgtype=CLOSE_REQ_MSG;
					continue;
				}
				setClntmessage3(sock, msgtype, sql_text, &sql_req);
				sendmsgtype(sock, &msgtype);
				sendClntmessage3(sock, &sql_req);
				break;
			case CLOSE_REQ_MSG:							//msgtype5
				setClntmessage5(msgtype, &close_req);
				sendmsgtype(sock, &msgtype);
				sendClntmessage5(sock, &close_req);
				break;
		}

		recvmsgtype(sock, &msgtype);

		switch(msgtype)									//receive protocal
		{
			case AUTH_REQ_MSG:
			case AUTH_RES_MSG:							//msgtype2
				recvServmessage2(sock, &auth_res);
				
				if(msgtype==AUTH_REQ_MSG)
				{	
					close(sock);
					puts("Server Connection closed");
					exit(1);
				}
				else
				{
					msgtype=SQL_REQ_MSG;
					printf("=====================================\n");
					printf("Connection is Successful.\n");
					while(auth_res.rtn_msg[comma]!=',')
					{
						ip[comma]=auth_res.rtn_msg[comma];
						comma++;
					}
					for(comma+=2;comma<auth_res.rtn_len;comma++)
						port[porti++]=auth_res.rtn_msg[comma];
					port[porti]='\0';
					printf(" Server Ip : %s\n", ip);
					printf(" Server Port : %s\n", port);
					printf("=====================================\n");

				}
				getchar();
				break;
			case SQL_RES_MSG:							//msgtype4
				recvServmessage4(sock, &sql_res);
				printf("%s", sql_res.rtn_data);
				msgtype=SQL_REQ_MSG;
				break;		
			case CLOSE_RES_MSG:							//msgtype6
				recvServmessage6(sock,  &close_res);
				printf("%s", close_res.rtn_msg);
				close(sock);
				return 0;
		}
	}
}
