#ifndef __FURRY_TOY_NETWORK_STSB_SERVER__
#define __FURRY_TOY_NETWORK_STSB_SERVER__

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <exception>
#include <stdio.h>
#include <system_error>
#include <errno.h>
#include <unistd.h>

namespace furry_toy{

// STSB —— Single Thread Sync Blocking
template<class typeHandler>
class TStsbTCPServer
{
public:
    TStsbTCPServer();
    ~TStsbTCPServer();
    
    void start(int port, int max_wait_socket, int type, int addrType);

private:
    
};

template<class typeHandler>
TStsbTCPServer<typeHandler>::TStsbTCPServer()
{

}

template<class typeHandler>
TStsbTCPServer<typeHandler>::~TStsbTCPServer()
{

}

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
        // 当一个连接的处理抛出异常时，是整个程序应该停止，还是说仅停止改连接的处理，继续处理下一个连接的请求
        while(true)
        {
            std::cout << "listen..." << std::endl;
            conn = accept(listen_fd, (struct sockaddr*)&client_addr, &client_addr_len);
            if(conn < 0)
            {
                // 以阻塞状态运行，若accept返回-1应该抛出异常
                throw std::system_error(errno, strerror(errno));
            }

            // 不负责业务处理逻辑，仅负责读取和发送数据
            typeHandler handler(client_addr);
            char buf[256];
            char sendBuf[256];
            // 此处可以有两种处理方式
            // 1. 将socket指针传递给handler，当socket状态更新时，设置状态，通知handler，但是读写还是交由handler处理，似乎这是更通用的做法（要事件驱动时才能实现）
            // 2. socket的读写在这里完成, 当socket可读时,读取数据并交给handler处理, 当handler处理完成需要写数据时,返回数据在这里发送数据
            try{
                while (!handler.isFinished())
                {
                    memset(buf, 0, sizeof(buf));
                    // flag参数可选值详见socket.h的enum MSG_ 的部分
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
                            // 发送数据
                            int sendLen = send(conn, sendBuf, sendDataLen, 0);
                            if(sendLen == -1)
                            {
                                // 发送错误
                                throw std::system_error(errno, strerror(errno));
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                } 
            }
            catch(std::exception& e)
            {
                close(conn);
                conn = 0;  
                continue;
            }                     
        }
    }
    catch(std::exception& e)
    {   
        if(conn > 0)
        {
            close(conn);
        }
        close(listen_fd);
        throw e;
    }

    return ;
}

}

#endif