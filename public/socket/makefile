MYSQLINCL= -I/usr/include/mysql

MYSQLLIB= -L/usr/lib64/mysql

PUBLICL= -I/project/public/db/mysql

PUBCPP= /project/public/db/mysql/_mysql.cpp

MYSQLLIBS= -lmysqlclient

CFLAGS=-g

all:demo01 demo02 demo03 demo04 demo05 demo06 demo07 demo08 demo10 demo11 demo12\
    demo13 demo14 demo31 demo32 demo33 demo20 demo26 demo27 server client login_server

demo01:demo01.cpp
	g++ -g -o demo01 demo01.cpp -lm -lc

demo02:demo02.cpp
	g++ -g -o demo02 demo02.cpp -lm -lc

client:client.cpp
	g++ -g -o client client.cpp -lm -lc

server:server.cpp _mysql.cpp
	g++ -g -o server server.cpp $(PUBLICL) $(PUBCPP) $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS) -lm -lc

login_server:login_server.cpp _mysql.cpp
	g++ -g -o login_server login_server.cpp $(PUBLICL) $(PUBCPP) $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS) -lm -lc

demo03:demo03.cpp
	g++ -g -o demo03 demo03.cpp -lm -lc

demo04:demo04.cpp
	g++ -g -o demo04 demo04.cpp -lm -lc

demo05:demo05.cpp
	g++ -g -o demo05 demo05.cpp ../_public.cpp -lm -lc

demo06:demo06.cpp
	g++ -g -o demo06 demo06.cpp ../_public.cpp -lm -lc

demo07:demo07.cpp
	g++ -g -o demo07 demo07.cpp ../_public.cpp -lm -lc

demo08:demo08.cpp
	g++ -g -o demo08 demo08.cpp ../_public.cpp -lm -lc

demo10:demo10.cpp
	g++ -g -o demo10 demo10.cpp ../_public.cpp -lm -lc

demo11:demo11.cpp
	g++ -g -o demo11 demo11.cpp ../_public.cpp -lm -lc

demo12:demo12.cpp
	g++ -g -o demo12 demo12.cpp ../_public.cpp -lm -lc

demo13:demo13.cpp
	g++ -g -o demo13 demo13.cpp ../_public.cpp -lm -lc

demo14:demo14.cpp
	g++ -g -o demo14 demo14.cpp ../_public.cpp -lm -lc

demo31:demo31.cpp
	g++ -g -o demo31 demo31.cpp ../_public.cpp -lm -lc

demo32:demo32.cpp
	g++ -g -o demo32 demo32.cpp ../_public.cpp -lm -lc

demo33:demo33.cpp
	g++ -g -o demo33 demo33.cpp ../_public.cpp -lm -lc

demo20:demo20.cpp
	g++ -g -o demo20 demo20.cpp ../_public.cpp -lpthread -lm -lc

demo26:demo26.cpp
	g++ -g -o demo26 demo26.cpp ../_public.cpp -lm -lc

demo27:demo27.cpp
	g++ -g -o demo27 demo27.cpp ../_public.cpp -lm -lc

clean:
	rm demo01 demo02 demo03 demo04 demo05 demo06 demo07 demo08 demo10 demo11 demo12
	rm demo13 demo14 demo31 demo32 demo33 demo20 demo26 demo27
