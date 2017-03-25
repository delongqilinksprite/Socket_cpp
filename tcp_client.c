/*
 * filename : tcp_client.c
 * author : Derron
 * date : 2017.01.01
 * mail : derron2016@163.com                                                                                                    
 */

#include <stdio.h>
#include "Socket.h"

int main()
{
	int debug = 0;
	int block = 1;
	char rev_buf[4096];
	char sed_buf[] = "I'm from tcp client!";
	TCPClient client(debug,block);
	if (client.socketConnect("127.0.0.1", 1234) < 0) 
	{
		printf("socket connect error!\n");
		return -1;
	}
	if(client.transmit(sed_buf)<0)
		printf("Send msg failed!\n");
	if(client.receive(rev_buf) > 0)
		printf("rev:%s\n",rev_buf);
	client.socketDisconnect();
	return 0;
}
