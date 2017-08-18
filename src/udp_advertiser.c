/*
* filename : udp_client.c
* author : Derron
* date : 2017.01.01
* mail : derron2016@163.com                                                                     
*/

#include <stdio.h>
#include "Socket.h"

int main()
{
	UDPAdvertiser udpAdvertiser;
	char sed_buf[] = "I'm from udp client!";
	if(udpAdvertiser.socketConnect(6666) < 0)
	{
		printf("udpAdvertiser connect failed...\n");
		return -1;
	}
	while(true)
	{
		if(udpAdvertiser.transmit(sed_buf) < 0)
			printf("udpAdvertiser send msg failed!\n");
		sleep(1);
	}
	udpAdvertiser.socketDisconnect();
	return 0;
}
