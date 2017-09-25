//
//  dbclient.c
//  tcp
//
//  Created by hwangyunho on 2017. 9. 18.
//  Copyright © 2017년 hwangyunho. All rights reserved.
//

#include "clientfunc.h"
int main(int argc, char *argv[])
{
	int sock;
	char id[33], password[33], sql_text[1025];
	struct sockaddr_in serv_adr;
	UB2 msgtype=0;

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
		
	msgtype=auth_req_msg;
	while(1)
	{
		switch(msgtype)					//send protocal
		{
			case auth_req_msg:							//msgtype1
				printf("ID : ");
   				scanf("%s", id);
  				printf("Password : ");
    			scanf("%s", password);

    			setClntmessage1(id, password, msgtype, &auth_req);
				sendmsgtype(sock, &msgtype);
				sendClntmessage1(sock, &auth_req);
				break;
			case sql_req_msg:							//msgtype3
				printf("SQL > ");
				fgets(sql_text,1025,stdin);
				sql_text[strlen(sql_text) - 1] = '\0';
				if((strcmp(sql_text, "exit;")==0) || (strcmp(sql_text, "EXIT;")==0)) {
					msgtype=close_req_msg;
					continue;
				}
				setClntmessage3(sock, msgtype, sql_text, &sql_req);
				sendmsgtype(sock, &msgtype);
				sendClntmessage3(sock, &sql_req);
				break;
			case close_req_msg:							//msgtype5
				setClntmessage5(msgtype, &close_req);
				sendmsgtype(sock, &msgtype);
				sendClntmessage5(sock, &close_req);
				break;
		}

		recvmsgtype(sock, &msgtype);

		switch(msgtype)				//receive protocal
		{
			case auth_req_msg:
			case auth_res_msg:						//msgtype2
				recvServmessage2(sock, &auth_res);
				printf("%s", auth_res.rtn_msg);
				if(msgtype==auth_req_msg)
				{	
					close(sock);
					puts("Server Connection closed");
					exit(1);
				}
				else
					msgtype=sql_req_msg;
				getchar();
				break;
			case sql_res_msg:						//msgtype4
				recvServmessage4(sock, &sql_res);
				printf("%s", sql_res.rtn_data);
				msgtype=sql_req_msg;
				break;		
			case close_res_msg:						//msgtype6
				recvServmessage6(sock,  &close_res);
				printf("%s", close_res.rtn_msg);
				close(sock);
				return 0;
		}
	}
}
