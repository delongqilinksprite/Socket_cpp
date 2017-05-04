/*
 * filename : sysmsg.c
 * author : Derron
 * date : 2017.01.01
 * mail : derron2016@163.com                                                                                                    
 */

#include <stdio.h>
#include "Socket.h"

int main()
{
	SysMsg sysmsg;
	const char *test_domain = "www.baidu.com";
	const char *test_eth = "eth0";
	string domain_ip = sysmsg.get_ip_by_domain(test_domain);
	string local_mac = sysmsg.get_local_mac(test_eth);
	string local_ip = sysmsg.get_local_ip(test_eth);
	string sys_time = sysmsg.get_system_time();
	cout << "baidu ip:" << domain_ip << endl;	
	cout << "local mac:" << local_mac << endl;
	cout << "local ip:" << local_ip << endl;
	cout << "time:" << sys_time << endl;
	return 0;
}
