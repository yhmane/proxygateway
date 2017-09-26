//
//  dbclient.h
//  tcp
//
//  Created by hwangyunho on 2017. 9. 18.
//  last modified by hwangyunho on 2017. 9. 26.
//  Copyright © 2017년 hwangyunho. All rights reserved.
//


#include "commonfunc.h"

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void sendmsgType(int sock, UB2 *msgType)				//send msgtype just UB2
{
	checklen=send(sock, msgtype, sizeof(*msgType),0);
	if(checklen==-1) 
		error_handling("send() error");
}

void recvmsgType(int sock, UB2 *msgType)				//send msgtype just UB2
{
	checklen=recv(sock, msgtype,sizeof(*msgType),0);
	if(checklen==-1)
		error_handling("recv() error");
}
