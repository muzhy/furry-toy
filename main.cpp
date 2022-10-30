#include <iostream>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

int main(int, char**) {

    std::cout << "start server ..." << std::endl;
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
    int listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listen_fd == -1)
    {
        std::cerr << "Error: init socket failed! err[" << errno << "]: " << strerror(errno) << std::endl;
        return -1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    // 转换为网络字节序
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(listen_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
    {
        std::cerr << "Error: bind socket failed! err[" << errno << "]" << strerror(errno) << std::endl;
        return -1;
    }

    int max_wait_socket = 10;
    if(listen(listen_fd, max_wait_socket) == -1)
    {
        std::cerr << "Error: listen failed! err[" << errno << "]" << strerror(errno) << std::endl;
        return -1;
    }

    int conn = 0;
    char client_ip[INET_ADDRSTRLEN] = "";
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    while (true)
    {
        std::cout << "listen..." << std::endl;
        conn = accept(listen_fd, (struct sockaddr*)&client_addr, &client_addr_len);
        if(conn < 0)
        {
            std::cerr << "accept error" << std::endl;
            continue;
        }
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        std::cout << "... connect " << client_ip << ":" << ntohs(client_addr.sin_port) << std::endl;

        char buf[256];
        while (true)
        {
            memset(buf, 0, sizeof(buf));
            int len = recv(conn, buf, sizeof(buf), 0);
            if(len == 0)
            {
                std::cout << "client close connect" << std::endl;
                break;
            }
            else if(len < 0)
            {
                std::cerr << "socket error : " << len << " client_ip " << client_ip << ":" << ntohs(client_addr.sin_port) << std::endl;
                break;
            }
            else
            {
                if(strcmp(buf, "quit\n") == 0 || strcmp(buf, "quit\r\n") == 0)
                {
                    std::cout << "close connect, " << " client_ip " << client_ip << ":" << ntohs(client_addr.sin_port) << std::endl;
                    break;
                }
                else
                {
                    std::cout << buf << std::endl;
                }
            }
        }

        close(conn);        
    }
    
    close(listen_fd);

    return 0;
}
