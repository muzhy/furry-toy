#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <unistd.h>

#include "error.h"


#ifndef __FURRY_TOY_NETWORK_SIMPLE_SERVER__
#define __FURRY_TOY_NETWORK_SIMPLE_SERVER__

namespace furry_toy
{
typedef int SOCKET_TYPE;

// 启动简单的TCP服务，阻塞的
ErrorCodeEnum startSimpleTcpServer(int port, int max_wait_socket, std::string& err);
// 启动非阻塞的TCP服务
ErrorCodeEnum startNoblockingTcpServer(int port, int max_wait_sowkct, std::string& err);

// 创建一个TCPSocket，并在指定的port上进行监听
SOCKET_TYPE createAndListenTcpSocket(int port, int max_wait_socket);

}

#endif