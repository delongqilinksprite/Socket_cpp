/*
 * filename : Socket.cpp
 * author : Derron
 * date : 2017.01.01
 * mail : derron2016@163.com
 */

#include "Socket.h"

/********************TCP Server**********************/

TCPServer::TCPServer(int Debug,int Block)
{
	this->Debug = Debug;
	this->Block = Block;
}

TCPServer::TCPServer(void)
{
	Debug = 0;
	Block = 1;
}

TCPServer::~TCPServer(void)
{
	
}

int TCPServer::socketConnect(int PORT)
{
	server_sockfd = socket(AF_INET,SOCK_STREAM, 0);  
    struct sockaddr_in server_sockaddr;  
    server_sockaddr.sin_family = AF_INET;  
    server_sockaddr.sin_port = htons(PORT);  
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);  
    if(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1)  
    {  
        if(Debug != 0)
			printf("bind error!\n");	
        return -1;  
    }  
  
    if(listen(server_sockfd,5) == -1)  
    {  
		if(Debug != 0)
			printf("listen error!\n"); 
        return -1;  
    }  
	if(Block == 0)
	{
		int flag;
		if (flag = fcntl(server_sockfd, F_GETFL, 0) <0)
			perror("get flag");
		flag |= O_NONBLOCK;
		if (fcntl(server_sockfd, F_SETFL, flag) < 0)
			perror("set flag");
	}
	if(Debug != 0)
		printf("connect successful!\n");  
	return 0;      
}

int TCPServer::acceptClient()
{
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);

	sockConn = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);  
    if(sockConn<0)  
    {  
        if(Debug != 0)
			printf("connect error!\n"); 
        return -1;  
    }  
	return 0;
}

int TCPServer::receive(char *data)
{
	char buffer[MaxLen];
	int leng = recv(sockConn, buffer, MaxLen, 0);
	if(leng < 0)
	{
		if(Debug != 0)
			printf("recive null!\n");
		return 0;
	}
	buffer[leng] = '\0';
	strcpy(data,buffer);
	return leng;
}

int TCPServer::transmit(const char *data)
{
	if (send(sockConn, data, strlen(data), 0) < 0)  
    {  
		if(Debug != 0)
			printf("send data error: %s(errno: %d)\n", strerror(errno), errno);  
        return -1;  
    }  
	return 0;
}

void TCPServer::socketDisconnect()
{
	close(sockConn);  
}

/********************TCP Client**********************/

TCPClient::TCPClient(void)
{
	Debug = 0;
	Block = 0;
}

TCPClient::TCPClient(int Debug,int Block)
{
	this->Debug = Debug;
	this->Block = Block;
}

TCPClient::~TCPClient(void)
{
	
}

int TCPClient::socketConnect(const char* IP,int PORT)
{
	struct sockaddr_in servaddr;  
    if ((sockClient = socket(AF_INET, SOCK_STREAM, 0)) < 0)   
    {  
		if(Debug != 0)
			printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);  
        return -1;  
    }  
    memset(&servaddr, 0, sizeof(servaddr));  
    servaddr.sin_family = AF_INET;  
    servaddr.sin_port = htons(PORT);  
    if (inet_pton(AF_INET, IP, &servaddr.sin_addr) <= 0)   
    {  
		if(Debug != 0)
			printf("inet_pton error for %s\n", IP);  
        return -1;  
    }   
    if (connect(sockClient, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)   
    {  
		if(Debug != 0)
			printf("connect error: %s(errno: %d)\n", strerror(errno), errno);  
        return -1;  
    }  
	if(Block == 0)
	{
		int flag;
		if (flag = fcntl(sockClient, F_GETFL, 0) <0)
			perror("get flag");
		flag |= O_NONBLOCK;
		if (fcntl(sockClient, F_SETFL, flag) < 0)
			perror("set flag");
	}
	if(Debug != 0)
		printf("connect successful!\n");  
	return 0;
}

int TCPClient::transmit(const char *data)
{
	if (send(sockClient, data, strlen(data), 0) < 0)  
    {  
		if(Debug != 0)
			printf("send data error: %s(errno: %d)\n", strerror(errno), errno);  
        return -1;  
    }  
	return 0;
}

int TCPClient::receive(char *data)
{
	char buffer[MaxLen];  
	int leng = recv(sockClient, buffer, MaxLen, 0);
	if(leng < 0)
	{
		if(Debug != 0)
			printf("recive null!\n");
		return 0;
	}
	buffer[leng] = '\0';
	strcpy(data,buffer);
	return leng;
}

void TCPClient::socketDisconnect(void)
{
	close(sockClient);
}


/********************UDP Server**********************/

UDPServer::UDPServer(void)
{
	Debug = 0;
	Block = 1;
}

UDPServer::UDPServer(int Debug,int Block)
{
	this->Debug = Debug;
	this->Block = Block;
}

UDPServer::~UDPServer(void)
{
}

int UDPServer::socketConnect(int PORT)
{    
    struct sockaddr_in server;    
    memset(&server,0,sizeof(server)); 
    server.sin_family=AF_INET; 
    server.sin_addr.s_addr=htonl(INADDR_ANY);  
    server.sin_port=htons(PORT);   
    if((udpFD=socket(PF_INET,SOCK_DGRAM,0))<0)  
    {    
		if(Debug == 1)
			printf("socket error");  
        return -1;  
    }  
    if (bind(udpFD,(struct sockaddr *)&server,sizeof(struct sockaddr))<0)  
    {  
		if(Debug == 1)
			printf("bind error");  
        return -1;  
    }  
	if(Block == 0)
	{
		int flag;
		if (flag = fcntl(udpFD, F_GETFL, 0) <0)
			perror("get flag");
		flag |= O_NONBLOCK;
		if (fcntl(udpFD, F_SETFL, flag) < 0)
			perror("set flag");
	}
	return 0;
}

int UDPServer::receive(char *data)
{
	int len;
	char buf[MaxLen]; 
	sin_size=sizeof(struct sockaddr_in);
	if((len=recvfrom(udpFD,buf,MaxLen,0,(struct sockaddr *)&client,&sin_size))<0)  
    { 
        return -1;  
    }  
    buf[len]='\0';  
	strcpy(data,buf);
	return len;
}

int UDPServer::transmit(const char *data)
{
	if(sendto(udpFD,data,strlen(data),0,(struct sockaddr *)&client,sin_size) < 0)  
    {  
		if(Debug != 0)
			printf("send data error: %s(errno: %d)\n", strerror(errno), errno);  
        return -1;  
    }  
	return 0;
}

void UDPServer::socketDisconnect(void)
{
	close(udpFD);
}

/********************UDP Client**********************/

UDPClient::UDPClient(void)
{
	Debug = 0;
	Block = 1;
}

UDPClient::UDPClient(int Debug,int Block)
{
	this->Debug = Debug;
	this->Block = Block;
}

UDPClient::~UDPClient(void)
{
	
}

int UDPClient::socketConnect(const char* IP,int PORT)
{    
    memset(&client,0,sizeof(client));  
    client.sin_family=AF_INET; 
    client.sin_addr.s_addr=inet_addr(IP);  
    client.sin_port=htons(PORT);  
    if((udpFD=socket(PF_INET,SOCK_DGRAM,0))<0)  
    {    
		if(Debug == 1)
			printf("socket error\n");  
        return -1;  
    }  
	if(Block == 0)
	{
		int flag;
		if (flag = fcntl(udpFD, F_GETFL, 0) <0)
			perror("get flag");
		flag |= O_NONBLOCK;
		if (fcntl(udpFD, F_SETFL, flag) < 0)
			perror("set flag");
	}
	return 0;
}

int UDPClient::transmit(const char *data)
{
	int len;  
	sin_size = sizeof(struct sockaddr_in);  
    if((len=sendto(udpFD,data,strlen(data),0,(struct sockaddr *)&client,sin_size))<0)  
    {  
		if(Debug == 1)
			printf("send data error!\n");   
        return -1;  
    }  
	return 0;
}

int UDPClient::receive(char *data)
{
	int len;
	char buf[MaxLen]; 
	if((len=recvfrom(udpFD,buf,MaxLen,0,(struct sockaddr *)&client,&sin_size))<0)  
    { 
        return -1;  
    }  
    buf[len]='\0';  
	strcpy(data,buf);
	return len;
}

void UDPClient::socketDisconnect(void)
{
	close(udpFD);
}
