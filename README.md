## C++ Socket

使用C++ 封装的linux Socket，包含TCP，UDP通信，阻塞和程序log信息简单设置即可，操作简单易用！


##### tcp server:

```
/*
 * filename : tcp_server.c
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
	char sed_buf[] = "I'm from tcp server!";
	TCPServer server(debug,block);
	if(server.socketConnect(1234) < 0)
	{
		printf("socket connect error!\n");
		return -1;
	}
	if(server.acceptClient() < 0)
	{
		printf("socket accept error!\n");
		return -1;
	}
	if(server.receive(rev_buf) > 0 )
		printf("rev:%s\n",rev_buf);
	if(server.transmit(sed_buf) < 0)
		printf("Send msg failed!\n");
	server.socketDisconnect();
	return 0;
}
```

#### tcp client

```
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

```

#### udp server

```
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
```


#### udp client

```
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
	int debug = 0;
	int block = 1;
	char rev_buf[4096];
	char sed_buf[] = "I'm from udp client!";
	UDPClient client(debug,block);
	if (client.socketConnect("127.0.0.1", 6666) < 0) 
	{
		printf("socket connect error!\n");
		return -1;
	}
	if(client.transmit(sed_buf) < 0)
		printf("Send msg failed!\n");
	if(client.receive(rev_buf) > 0)
		printf("rev:%s\n",rev_buf);
	client.socketDisconnect();
	return 0;
}
```
