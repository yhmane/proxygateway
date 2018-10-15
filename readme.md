# proxygateway
**It is about proxygateway at Client, Server (just sample project)**

### Project1 
>Make Simple DB Client & Simple DB Server

File: 
      header.h
      dbserver.c dbserver.o
      dbclient.c dbclient.o
      commonfunc.c commonfunc.h commonfunc.o
      clientfunc.c clientfunc.h clientfunc.o
      serverfunc.c serverfunc.h serv-erfunc.o


# Project2
**project1+ oxygateway**
Check the msg, print

File: header.h 

      dbserver.c dbserver.o
      
      dbclient.c dbclient.o
      
      proxygateway.c proxygateway.p
      
      commonfunc.c commonfunc.h commonfunc.o
      
      clientfunc.c clientfunc.h clientfunc.o 
      
      serverfunc.c serverfunc.h serverfunc.o



**Use gcc option for compile**

_step1) make obj file_

`gcc -c xxxx.c  => xxxx.o`

`gcc -c aaaa.c  => aaaa.o`

_step2) make exe file_

`gcc -o main xxxx.o aaaa.o => main.exe`

