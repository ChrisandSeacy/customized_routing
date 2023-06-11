MYSQLINCL= -I/usr/include/mysql

MYSQLLIB= -L/usr/lib64/mysql

PUBLICL= -I/project/public/db/mysql

PUBCPP= /project/public/db/mysql/_mysql.cpp

MYSQLLIBS= -lmysqlclient

all:info_advertisement TPRM C-CVN

info_advertisement:info_advertisement.cpp _mysql.cpp
	g++ -g -o info_advertisement info_advertisement.cpp $(PUBLICL) $(PUBCPP) $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS) -I/project/public /project/public/_public.cpp -lm -lc
	cp info_advertisement ../bin/.
TPRM:TPRM.cpp _mysql.cpp
	g++ -g -o TPRM TPRM.cpp $(PUBLICL) $(PUBCPP) $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS) -I/project/public /project/public/_public.cpp -lm -lc
	cp TPRM ../bin/.       
C-CVN:C-CVN.cpp _mysql.cpp
	g++ -g -o C-CVN C-CVN.cpp $(PUBLICL) $(PUBCPP) $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS) -I/project/public /project/public/_public.cpp -lm -lc
	cp C-CVN ../bin/.
clean:
	rm info_advertisement TPRM C-CVN
