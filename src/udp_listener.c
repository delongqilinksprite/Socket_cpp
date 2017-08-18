/*
 * filename : udp_listener.c
 * author : Derron
 * date : 2017.01.01
 * mail : derron2016@163.com                                                                                                    
 */

#include <stdio.h>
#include "Socket.h"

int main()
{
	char rev_buf[4096];
	UDPListener udpListener;
	if(udpListener.socketConnect(6666) < 0)
	{
		printf("udpListener connect failed...\n");
		return -1;
	}
	while(true)
	{
		if(udpListener.receive(rev_buf) < 0)
		{
			printf("udpListener get msg failed...\n");
			return -1;
		}
		printf("rev:%s\n",rev_buf);
	}
	udpListener.socketDisconnect();
	return 0;
}
