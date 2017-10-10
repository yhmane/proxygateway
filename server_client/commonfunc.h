//
//  commonfunc.h
//
//  Created by hwangyunho on 2017. 9. 18.
//  last modified by hwangyunho on 2017. 9. 29.
//  Copyright © 2017 hwangyunho. All rights reserved.
//

#ifndef __COMMONFUNC_H_
#define __COMMONFUNC_H_

#include "header.h"

void errorHandling(char *message);
void sendMsgType(int sock, UB2 msg_type);
void recvMsgType(int sock, UB2 *msg_type);			
#endif