
all:publishRoute

publishRoute:publishRoute.cpp
	g++ -g -o publishRoute publishRoute.cpp -I/project/public -L/project/public /project/public/_public.cpp /routing/public/_ftp.cpp -lftp -lm -lc
	cp publishRoute ../bin/.
clean:
	rm publishRoute
