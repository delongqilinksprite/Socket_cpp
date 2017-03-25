/*
 * filename : udp_server.c
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
	char sed_buf[] = "I'm from udp server!";
	UDPServer server(debug,block);
	if(server.socketConnect(6666) < 0)
	{
		printf("socket connect error!\n");
		return -1;
	}
	if(server.receive(rev_buf) > 0 )
		printf("rev:%s\n",rev_buf);
	if(server.transmit(sed_buf) < 0)
		printf("Send msg failed!\n");
	server.socketDisconnect();
	return 0;
}
