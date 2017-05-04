/*
 * filename : Socket.h
 * author : Derron
 * date : 2017.01.01
 * mail : derron2016@163.com                                                                                                    
 */

#ifndef __SOCKET_H
#define __SOCKET_H

#include <stdio.h> 
#include <iostream>
#include <string> 
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
#include <netdb.h>  
#include <net/if.h>   
#include <sys/ioctl.h>   
#include <sys/stat.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>   

#define MAC_SIZE    18  
#define IP_SIZE     16 
#define MaxLen      4096

using namespace std;

class SysMsg
{
public:
	SysMsg(void);
	~SysMsg(void);
	string get_ip_by_domain(const char *domain);   
	string get_local_mac(const char *eth_inf);  
	string get_local_ip(const char *eth_inf);
	string get_system_time(void);
};

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
