/*
 * filename : Socket.h
 * author : Derron
 * date : 2017.01.01
 * mail : derron2016@163.com                                                                                                    
 */

#ifndef __SOCKET_H
#define __SOCKET_H

#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <errno.h>  
#include <unistd.h>  
#include <sys/shm.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>
#include <fcntl.h>

#define MaxLen 4096

class TCPServer
{
public:
	TCPServer(void);
	TCPServer(int Debug,int Block);
	~TCPServer(void);
    int socketConnect(int PORT);  
	int acceptClient();
    int receive(char *data); 
	int transmit(const char *data);
    void socketDisconnect(void);  
private:
	int server_sockfd;
	int sockConn;
	int Debug;
	int Block;
};

class TCPClient
{
public:
	TCPClient(void);
	TCPClient(int Debug,int Block);
	~TCPClient(void);
    int socketConnect(const char* IP,int PORT);  
    int transmit(const char *data); 
	int receive(char *data); 
    void socketDisconnect(void);  
private:
	int sockClient;
	int Debug;
	int Block;
};

class UDPServer
{
public:
	UDPServer(void);
	UDPServer(int Debug,int Block);
	~UDPServer(void);
	int socketConnect(int PORT);
	int receive(char *data);
	int transmit(const char *data);
	void socketDisconnect(void);
private:
	int Block;
	int Debug;
	int udpFD;
	socklen_t sin_size;
	struct sockaddr_in client;
};

class UDPClient
{
public:
	UDPClient(void);
	UDPClient(int Debug,int Block);
	~UDPClient(void);
	int socketConnect(const char* IP,int PORT);  
    int transmit(const char *data);
	int receive(char *data);
	void socketDisconnect(void);
private:
	int Block;
	int udpFD;
	int Debug;
	socklen_t sin_size;
	struct sockaddr_in client;
};

#endif
