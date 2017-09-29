12days 
Date 17.09.18 ~ 17.09.29
*Network project*

-Project1 
Make Simple DB Client & Simple DB Server
File: header.h 
      dbserver.c dbserver.o
      dbclient.c dbclient.o
      commonfunc.c commonfunc.h commonfunc.o
      clientfunc.c clientfunc.h clientfunc.o 
      serverfunc.c serverfunc.h serverfunc.o


-Project2
project1+ proxygateway
Check the msg, print
File: header.h 
      dbserver.c dbserver.o
      dbclient.c dbclient.o
      proxygateway.c proxygateway.p
      commonfunc.c commonfunc.h commonfunc.o
      clientfunc.c clientfunc.h clientfunc.o 
      serverfunc.c serverfunc.h serverfunc.o



-Use gcc option for compile
step1) make obj file
gcc -c xxxx.c  => xxxx.o
gcc -c aaaa.c  => aaaa.o
step2) make exe file
gcc -o main xxxx.o aaaa.o => main.exe
