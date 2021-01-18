/******************************************************************
* Copyright(c) 2020-2028 ZHENGZHOU Tiamaes LTD.
* All right reserved. See COPYRIGHT for detailed Information.
*
* @fileName: taskprocess.h
* @brief: UDP通信服务端数据任务处理
* @author: dinglf
* @date: 2021-01-15
* @history:
*******************************************************************/
#ifndef _TASK_PROCESS_
#define _TASK_PROCESS_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include "locker.h"

typedef  int (*pudpFun)(const char *,int);

using namespace std;
#define MAX_BUFFER 2048  //Buffer的最大字节

class BaseTask
{
public:
	BaseTask(){}
	virtual ~BaseTask(){}
	virtual void doit() = 0;
};
 
class Task : public BaseTask
{
private:
	char sip[20];
	unsigned short sport;
	char order[MAX_BUFFER];
	int orderlen;
public:
	static pudpFun task_callback;
public:
	Task(char *str,int readlen,const char* srcip, unsigned short srcport)
	{
		memset(sip,0,sizeof(sip));
		memcpy(sip,srcip,strlen(srcip));
		sport = srcport;
		memset(order, 0, MAX_BUFFER);
		memcpy(order,str,readlen);
		orderlen = readlen;
	}
	void doit()  //任务的执行函数
	{
		//do something of the order
		processtask(order,orderlen,sip,sport);
	}
	void processtask(char *buffer,int buflen,const char* srcip, unsigned short srcport);
};
#endif