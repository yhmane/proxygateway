//
//  commonfunc.c
//
//  Created by hwangyunho on 2017. 9. 18.
//  last modified by hwangyunho on 2017. 9. 26.
//  Copyright © 2017 hwangyunho. All rights reserved.
//

#include "commonfunc.h"

void errorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void sendMsgType(int sock, UB2 msg_type)
{
	len=send(sock, &msg_type, sizeof(msg_type),0);
	if(len==-1) errorHandling("send() error");
}

void recvMsgType(int sock, UB2 *msg_type)				
{
	len=recv(sock, msg_type, sizeof(*msg_type),0);
	if(len==-1) errorHandling("recv() error");
}