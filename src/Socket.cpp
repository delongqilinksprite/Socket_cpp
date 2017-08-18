/*
 * filename : Socket.cpp
 * author : Derron
 * date : 2017.01.01
 * mail : derron2016@163.com
 */

#include "Socket.h"

/********************SysMsg**********************/

SysMsg::SysMsg(void)
{
	//
}


SysMsg::~SysMsg(void)
{
	//
}

string SysMsg::get_ip_by_domain(const char *domain)
{
	char ip[128];
	char **pptr;  
	struct hostent *hptr;  
	hptr = gethostbyname(domain);  
	if(NULL == hptr)  
	{  
		printf("gethostbyname error for host:%s/n", domain);  
		return NULL;  
	}  
	for(pptr = hptr->h_addr_list ; *pptr != NULL; pptr++)  
	{  
		if (NULL != inet_ntop(hptr->h_addrtype, *pptr, ip, IP_SIZE) )  
		{  
			return string(ip);
		}  
	}  
	return NULL;  
}

string SysMsg::get_local_mac(const char *eth_inf)
{
	char mac[128];
	struct ifreq ifr;  
	int sd;  
	bzero(&ifr, sizeof(struct ifreq));  
	if( (sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)  
	{  
		printf("get %s mac address socket creat error\n", eth_inf);  
		return NULL;  
	}  
	strncpy(ifr.ifr_name, eth_inf, sizeof(ifr.ifr_name) - 1);  
	if(ioctl(sd, SIOCGIFHWADDR, &ifr) < 0)  
	{  
		printf("get %s mac address error\n", eth_inf);  
		close(sd);  
		return NULL;  
	}  
	snprintf(mac, MAC_SIZE, "%02x:%02x:%02x:%02x:%02x:%02x",  
	(unsigned char)ifr.ifr_hwaddr.sa_data[0],   
	(unsigned char)ifr.ifr_hwaddr.sa_data[1],  
	(unsigned char)ifr.ifr_hwaddr.sa_data[2],   
	(unsigned char)ifr.ifr_hwaddr.sa_data[3],  
	(unsigned char)ifr.ifr_hwaddr.sa_data[4],  
	(unsigned char)ifr.ifr_hwaddr.sa_data[5]);  
	close(sd);  
	return string(mac);  
}

string SysMsg::get_local_ip(const char *eth_inf)
{
	char ip[128];
	int sd;
	struct sockaddr_in sin;  
	struct ifreq ifr;  
	sd = socket(AF_INET, SOCK_DGRAM, 0);  
	if (-1 == sd)  
	{  
		printf("socket error: %s\n", strerror(errno));  
		return NULL;        
	}  
	strncpy(ifr.ifr_name, eth_inf, IFNAMSIZ);  
	ifr.ifr_name[IFNAMSIZ - 1] = 0;   
	if (ioctl(sd, SIOCGIFADDR, &ifr) < 0)  
	{  
		printf("ioctl error: %s\n", strerror(errno));  
		close(sd);  
		return NULL;  
	}  
	memcpy(&sin, &ifr.ifr_addr, sizeof(sin));  
	snprintf(ip, IP_SIZE, "%s", inet_ntoa(sin.sin_addr));  
	close(sd);  
	return string(ip);  
}

string SysMsg::get_system_time(void)
{
	char buf[128];
    struct timeval tv;
    struct timezone tz;
    struct tm *p;
    gettimeofday(&tv, &tz);
    p = localtime(&tv.tv_sec);
	sprintf(buf,"%d-%d-%d-%d:%d:%d-%3ld", 1900+p->tm_year, 1+p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, tv.tv_usec);
    return string(buf);
}

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
	//
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

/********************UDP Advertiser**********************/

UDPAdvertiser::UDPAdvertiser(void)
{
	Debug = 0;
	Block = 1;
}

UDPAdvertiser::UDPAdvertiser(int Debug,int Block)
{
	this->Debug = Debug;
	this->Block = Block;
}

int UDPAdvertiser::socketConnect(int PORT)
{
	setvbuf(stdout, NULL, _IONBF, 0);   
	fflush(stdout);   
	if ((udpFD = socket(AF_INET, SOCK_DGRAM, 0)) == -1)   
	{   
		if(Debug == 1)
			cout<<"socket error"<<endl;   
		return -1;  
	}  
	const int opt = 1;  
	int nb = 0;  
	nb = setsockopt(udpFD, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt));  
	if(nb == -1)  
	{  
		if(Debug == 1)
			cout<<"set socket error..."<<endl;  
		return -1;  
	}   
	bzero(&addrto, sizeof(struct sockaddr_in));  
	addrto.sin_family=AF_INET;  
	addrto.sin_addr.s_addr=htonl(INADDR_BROADCAST);  
	addrto.sin_port=htons(PORT);  
	nlen=sizeof(addrto);
	return 0;
}

int UDPAdvertiser::transmit(const char *data)
{
	int ret=sendto(udpFD, data, strlen(data), 0, (sockaddr*)&addrto, nlen);  
	return ret;
}

void UDPAdvertiser::socketDisconnect(void)
{
	close(udpFD);
}

UDPAdvertiser::~UDPAdvertiser(void)
{
	//
}

/********************UDP Listener**********************/
UDPListener::UDPListener()
{
	Debug = 0;
	Block = 1;
}

UDPListener::UDPListener(int Debug,int Block)
{
	this->Debug = Debug;
	this->Block = Block;
}

UDPListener::~UDPListener(void)
{
	//
}

int UDPListener::socketConnect(int PORT)
{
	setvbuf(stdout, NULL, _IONBF, 0);   
	fflush(stdout);  
	struct sockaddr_in addrto;  
	bzero(&addrto, sizeof(struct sockaddr_in));  
	addrto.sin_family = AF_INET;  
	addrto.sin_addr.s_addr = htonl(INADDR_ANY);  
	addrto.sin_port = htons(PORT);   
	bzero(&from, sizeof(struct sockaddr_in));  
	from.sin_family = AF_INET;  
	from.sin_addr.s_addr = htonl(INADDR_ANY);  
	from.sin_port = htons(PORT);  
	if ((udpFD = socket(AF_INET, SOCK_DGRAM, 0)) == -1)   
	{     
		if(Debug == 1)
			cout<<"socket error"<<endl;   
		return -1;  
	}     
	const int opt = 1;  
	int nb = 0;  
	nb = setsockopt(udpFD, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt));  
	if(nb == -1)  
	{  
		if(Debug == 1)
			cout<<"set socket error..."<<endl;  
		return -1;  
	}  
	if(bind(udpFD,(struct sockaddr *)&(addrto), sizeof(struct sockaddr_in)) == -1)   
	{     
		if(Debug == 1)
			cout<<"bind error..."<<endl;  
		return -1;  
	}  
	len = sizeof(sockaddr_in);  
}

int UDPListener::receive(char *data)
{
	char buf[MaxLen];
	int ret = -1;
	if(ret=recvfrom(udpFD, buf, MaxLen, 0, (struct sockaddr*)&from,(socklen_t*)&len) < 0)
	{
		return -1;
	}
	//buf[ret]='\0';  
	strcpy(data,buf);
	return ret;
}

void UDPListener::socketDisconnect(void)
{
	close(udpFD);
}


