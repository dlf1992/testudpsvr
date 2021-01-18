/******************************************************************
* Copyright(c) 2020-2028 ZHENGZHOU Tiamaes LTD.
* All right reserved. See COPYRIGHT for detailed Information.
*
* @fileName: udpserver.cpp
* @brief: UDP通信服务端实现
* @author: dinglf
* @date: 2021-01-15
* @history:
*******************************************************************/
#include "udpserver.h"
UdpServer* UdpServer::m_pUdpServer = NULL;
bool UdpServer::init(unsigned short svrport,pudpFun callback)
{
	m_port = svrport;
	Task::task_callback = callback;
	bzero(&ServerAddr, sizeof(ServerAddr));
	//配置ServerAddr
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(m_port);
    ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    //创建Socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0)
    {
        //printf("UdpServer socket init error\n");
        return false;
    }
    int opt = 1;
    // sockfd为需要端口复用的套接字
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt));
	
    int ret = bind(sockfd, (struct sockaddr *)&ServerAddr, sizeof(ServerAddr));
    if(ret < 0)
    {
        //printf("UdpServer bind init error\n");
        return false;
    }
    //create Epoll
    epollfd = epoll_create(1024);
    if(epollfd < 0)
    {
        //printf("UdpServer epoll_create init error\n");
        return false;
    }
	//printf("UdpServer init success.\n");
	return true;
    //pool = new threadpool<BaseTask>(threadnum);  //创建线程池	
}
bool UdpServer::startpool()
{
    pool = new threadpool<BaseTask>(threadnum);  //创建线程池
	if(NULL == pool)
	{
		//printf("pool=NULL.\n");
		return false;
	}
    pool->start();   //线程池启动
	return true;
}
void UdpServer::epoll()
{
    //pool->start();   //线程池启动
    //char tmp[32];
    addfd(epollfd, sockfd, false);
    while(!is_stop)
    {//调用epoll_wait
        int ret = epoll_wait(epollfd, events, MAX_EVENT, -1);

        if(ret < 0)  //出错处理
        {
            //被信号中断
            if (errno == EINTR)
            {
                //printf("errno == EINTR\n");
                continue;
            }
            else
			{
                //printf("epoll_wait error\n");
                break;
            }
        }
        for(int i = 0; i < ret; ++i)
        {
            int fd = events[i].data.fd;
            if(fd == sockfd)  //数据到来
            {
                char buffer[MAX_BUFFER];
                readagain:	memset(buffer, 0, sizeof(buffer));
                struct sockaddr_in client_address;
                socklen_t client_addrlength = sizeof( client_address );
                int ret = recvfrom(fd,buffer,MAX_BUFFER-1, 0, (struct sockaddr*)&client_address, &client_addrlength);
                if(ret < 0)
                {
                    if(errno == EAGAIN)
                    {
                        //printf("read error! errno = EAGAIN,continue.\n");
                        continue;
                    }
					else if(errno == EINTR)
					{
                        //printf("read error! errno = EINTR,readagain.\n");
                        goto readagain;						
					}
					else 
					{
						//printf("read error! errno = %d,break.\n",errno);
						break;
					}
                }
				else if(ret > 0)
				{
                    //printf("received data,fd = %d,datalen = %d\n",fd,ret);
                    char *ip;
					unsigned short port = 0;
					ip = inet_ntoa(client_address.sin_addr);
					port = client_address.sin_port;
					BaseTask *task = NULL;
                    task = new Task(buffer,ret,ip,port);
					if(NULL == task)
					{
						//printf("task=NULL.\n");
						continue;
					}
					if(NULL == pool)
					{
						//printf("pool=NULL.\n");
						break;
					}
					////printf("epoll task = %p\n",task);
                    pool->append_task(task);				
				}
				else
				{
					continue;
				}
            }
        }
    }
    close(sockfd);//结束。

    //pool->stop();
}
void UdpServer::stoppool()
{
	if(NULL == pool)
	{
		//printf("pool=NULL.\n");
		return;
	}
    pool->stop();   //线程池停止	
}
int  UdpServer::senddata(const char* ip,unsigned short port,const char *data,int datalen)
{
	if(sockfd == -1)
	{
		return -1;
	}
    struct sockaddr_in client_address;
    socklen_t client_addrlength = sizeof(client_address);	
	bzero(&client_address,sizeof(client_address));
	client_address.sin_family = AF_INET;
	client_address.sin_port = htons(port);
	client_address.sin_addr.s_addr = inet_addr(ip);
	int sendlen = sendto(sockfd,data,datalen,0,(struct sockaddr*)&client_address, client_addrlength);
	if(sendlen < 0)
	{
		printf("send error\n");
	}
	return sendlen;
}