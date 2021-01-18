/*
 * 	tcpsvr.h
 *
 */
 
/*条件编译*/
#ifndef UDP_SVR_H_
#define UDP_SVR_H_
//#include <iostream>
//#include <map>
//using namespace std;
 
#ifdef __cplusplus
extern "C"  //C++
{
#endif
typedef  int (*pudpFun)(const char *,int);
int StartUDPService(unsigned short svrport,pudpFun Callback);
int SendSvrdata(const char* ip,unsigned short port,const char *data,int datalen);
#ifdef __cplusplus
}
#endif
 
#endif /* UDP_SVR_H_ */