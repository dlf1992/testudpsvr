/******************************************************************
* Copyright(c) 2020-2028 ZHENGZHOU Tiamaes LTD.
* All right reserved. See COPYRIGHT for detailed Information.
*
* @fileName: taskprocess.cpp
* @brief: UDP通信服务端数据任务处理实现
* @author: dinglf
* @date: 2021-01-15
* @history:
*******************************************************************/
#include "taskprocess.h"

//初始化静态成员变量
pudpFun Task::task_callback = NULL;

void Task::processtask(char *buffer,int buflen,const char* srcip, unsigned short srcport)
{
	printf("client_%s:%d,buflen = %d\n",srcip,srcport,buflen);
	/*for(int i=0;i<buflen;i++)
	{
		 printf("0x%02x ",*(buffer+i));
	}
	printf("\n");*/
	//具体协议处理
	if(task_callback != NULL)
	{
		task_callback(buffer,buflen);
	}
	else
	{
		printf("task_callback == NULL\n");
	}
}