#/*
# * filename : Makefile
# * author : Derron
# * date : 2017.01.01
# * mail : derron2016@163.com                                                                                                   # */

all:tcp_server tcp_client udp_server udp_client

tcp_server:tcp_server.o Socket.o
	g++ tcp_server.o Socket.o -o tcp_server

tcp_server.o:tcp_server.c
	g++ -c tcp_server.c

Socket.o:Socket.cpp
	g++ -c Socket.cpp

tcp_client:tcp_client.o Socket.o
	g++ tcp_client.o Socket.o -o tcp_client

tcp_client.o:tcp_client.c
	g++ -c tcp_client.c

udp_server:udp_server.o Socket.o
	g++ udp_server.o Socket.o -o udp_server

udp_server.o:udp_server.c
	g++ -c udp_server.c

udp_client:udp_client.o Socket.o
	g++ udp_client.o Socket.o -o udp_client

udp_client.o:udp_client.c
	g++ -c udp_client.c

clean:
	rm *.o tcp_server tcp_client udp_server udp_client
