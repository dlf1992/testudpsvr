#include <stdio.h>
#include <stdlib.h>
#include "udpserver.h"
#include "udpsvr.h"
#include "taskprocess.h"

int StartUDPService(unsigned short svrport,pudpFun Callback)
{
	UdpServer * pudpserver = UdpServer::GetInstance();
	if(NULL == pudpserver)
	{
		//printf("ptcpserver == NULL.\n");
		return -1;
	}
	if(pudpserver->init(svrport,Callback))
	{
		if(pudpserver->startpool())
		{
			pudpserver->epoll();	
		}	
	}
	else
	{
		//printf("UdpServer init failed.\n");
		UdpServer::Destroy();
		return -1;		
	}
	return 0;
}
int SendSvrdata(const char* ip,unsigned short port,const char *data,int datalen)
{
	UdpServer * pudpserver = UdpServer::GetInstance();
	if(NULL == pudpserver)
	{
		//printf("ptcpserver == NULL.\n");
		return -1;
	}
	int sendlen = pudpserver->senddata(ip,port,data,datalen);
	return sendlen;
}

