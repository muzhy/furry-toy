#ifndef __FURRY_TOY_NETWORK_STSB_SERVER__
#define __FURRY_TOY_NETWORK_STSB_SERVER__

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <exception>
#include <stdio.h>
#include <system_error>
#include <errno.h>
#include <unistd>

namespace furry_toy{

// STSB —— Single Thread Sync Blocking
template<class typeHandler>
class TStsbTCPServer
{
public:
    TStsbServer();
    ~TStsbServer();
    
    void start(int port, int max_wait_socket, int type, int addrType);

private:
    
};

template<class typeHandler>
void TStsbTCPServer<typeHandler>::start(int port, int max_wait_socket, int type, int addrType)
{
    /************************************************************************
     * int  socket(int protofamily, int type, int protocol);    //返回sockfd
     * @param protofamily 协议域，常用的协议域有：AF_INET(IPV4)、AF_INET6(IPV6)、
     *  AF_LOCAL（或称AF_UNIX，Unix域socket）、AF_ROUTE... 
     *  协议域定义了socket的地址类型，在通信中必须采用对应的地址，在socket.h中定义
     * @param type socket类型，常用类型：SOCK_STREAM、SOCK_DGRAM、SOCK_RAW、
     *  SOCK_PACKET、SOCK_SEQPACKET... 在socket_type.h中定义
     * @param protocol 协议，IPPROTO_TCP、IPPTOTO_UDP、
     *  IPPROTO_SCTP、IPPROTO_TIPC， 为0时自动选择type类型对应的默认协议
     * type 和protocol不能随意组合
     * @return sockfd socket描述符
     * 以下代码创建了一个TCP连接的sockfd
    *************************************************************************/
    int listen_fd = socket(addrType, type, IPPROTO_TCP);

    struct sockaddr_in addr;
    // IPv4
    addr.sin_family = AF_INET;
    // 转换为网络字节序
    addr.sin_port = htons(port);
    // Address to accept any incoming messages. 
    addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(listen_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
    {
        throw std::system_error(errno, strerror(errno));
    }

    if(listen(listen_fd, max_wait_socket) == -1)
    {
        throw std::system_error(errno, strerror(errno));
    }

    int conn = 0;
    char client_ip[INET6_ADDRSTRLEN] = "";
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    try
    {
        while(true)
        {
            std::cout << "listen..." << std::endl;
            conn = accept(listen_fd, (struct sockaddr*)&client_addr, &client_addr_len);
            if(conn < 0)
            {
                // 以阻塞状态运行，若accept返回-1应该抛出异常
                throw std::system_error(errno, strerror(errno))
            }

            // 不负责业务处理逻辑，仅负责读取和发送数据
            typeHandler handler;
            handler.setSockAddr(client_addr);
            char buf[256];
            char sendBuf[256];
            // 此时有两种处理方式
            // 1. 将socket指针传递给handler，由handler内部负责读写socket和处理业务逻辑
            // 2. socket的读写在这里完成, 当socket可读时,读取数据并交给handler处理, 当handler处理完成需要写数据时,返回数据在这里发送数据
            while (!handler.finished())
            {
                memset(buf, 0, sizeof(buf));
                int len = recv(conn, buf, sizeof(buf), 0);
                if(len == 0)
                {
                    handler.onClientClose();
                    break;
                }
                else if(len < 0)
                {
                    std::cerr << "socket error : " << len << " client_ip " << client_ip << ":" << ntohs(client_addr.sin_port) << std::endl;
                    handler.onSocketError();
                    break;
                }
                else{
                    handler.onReadData(buf, len);
                }

                while(handler.needSendData())
                {
                    memset(sendBuf, 0, sizeof(sendBuf));
                    int sendDataLen = handler.getSendData(sendBuf, sizeof(sendBuf));
                    if(sendDataLen > 0)
                    {
                        // TODO 发送数据
                    }
                    else
                    {
                        break;
                    }
                }
            }  

            close(conn);           
        }
    }
    catch(std::exception& e)
    {   
        close(listen_fd);
        throw e;
    }

    return ;
}

}

#endif