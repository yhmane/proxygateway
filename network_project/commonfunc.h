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
#define __COMMONFUNC_H_

void error_handling(char *message);
void sendmsgType(int sock, UB2 *msgType);				//send msgtype just UB2
void recvmsgType(int sock, UB2 *msgType);				//send msgtype just UB2
#endif
