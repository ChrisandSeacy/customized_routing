MYSQLINCL= -I/usr/include/mysql

MYSQLLIB= -L/usr/lib64/mysql

PUBLICL= -I/project/public/db/mysql

PUBCPP= /project/public/db/mysql/_mysql.cpp

MYSQLLIBS= -lmysqlclient

CFLAGS=-g

all:client login_server getRouting Route_server

client:client.cpp
	g++ -g -o client client.cpp -lm -lc

login_server:login_server.cpp _mysql.cpp
	g++ -g -o login_server login_server.cpp $(PUBLICL) $(PUBCPP) $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS) -lm -lc

getRouting:getRouting.cpp
	g++ -g -o getRouting getRouting.cpp -lm -lc

Route_server:Route_server.cpp
	g++ -g -o Route_server Route_server.cpp $(PUBLICL) $(PUBCPP) $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS) -I/project/public /project/public/_public.cpp -lm -lc

clean:
	rm login_server client getRouting Route_server
