#include <iostream>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdio.h>

#include "error/error.h"
#include "network/simpleServer.h"
#include "TSQueue.hpp"

furry_toy::ErrorCodeEnum startTcpServer(int port, int max_wait_socket, std::string& err)
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
    int listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listen_fd == -1)
    {
        std::cerr << "Error: init socket failed! err[" << errno << "]: " << strerror(errno) << std::endl;
        return furry_toy::NET_SOCKET_INIT_ERR;
    }

    struct sockaddr_in addr;
    // IPv4
    addr.sin_family = AF_INET;
    // 转换为网络字节序
    addr.sin_port = htons(port);
    // Address to accept any incoming messages. 
    addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(listen_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
    {
        std::cerr << "Error: bind socket failed! err[" << errno << "]" << strerror(errno) << std::endl;
        return furry_toy::NET_SOCKET_BIND_ERR;
    }

    if(listen(listen_fd, max_wait_socket) == -1)
    {
        std::cerr << "Error: listen failed! err[" << errno << "]" << strerror(errno) << std::endl;
        return furry_toy::NET_SOCKET_LISTEN_ERR;
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
                if(strcmp(buf, "quit") == 0 || strcmp(buf, "quit\n") == 0 || strcmp(buf, "quit\r\n") == 0)
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
        shutdown(conn, SHUT_RDWR);
        close(conn);        
    }
    
    close(listen_fd);

    return furry_toy::SUCCESS;
}

int main(int, char**) {
    std::string err;
    // startTcpServer(8080, 10, err);
    furry_toy::startSimpleTcpServer(8080, 10, err);
    return 0;
}

/************************************************************
 * 监控文件夹
#include <iostream>

#include "trecallagentapp.h"

#include <sys/inotify.h>
#include <sys/stat.h>
#include <stdio.h>
#include <map>

int main(int argc, char *argv[]) {
    int fd = inotify_init();
    if(fd < 0)
    {
        std::cout << "init inotify failed!" << std::endl;
        return -1;
    }
    std::string rootFile = "/home/zhychen/tmp";
    std::map<int, std::string> watchToFileName;
    int watch_d = inotify_add_watch(fd, rootFile.c_str(), IN_CLOSE_WRITE | IN_CREATE | IN_DELETE);
    watchToFileName.insert(std::make_pair(watch_d, rootFile));
    
    char buf[1024];
    while(true)
    {
        int len = read(fd, buf, sizeof(buf) - 1);
        struct inotify_event *event = NULL;
        int nread = 0;
        while(len > 0)
        {
            event = (struct inotify_event *)&buf[nread];
            if(event->mask & IN_CLOSE_WRITE)
            {
                std::cout << event->name << " in close write" << std::endl;
            }
            else if(event->mask & IN_CREATE)
            {
                std::cout << event->name << " in create" << std::endl;
                struct stat s_buf;
                auto it = watchToFileName.find(event->wd);
                assert(it != watchToFileName.end());
                std::string parentFileName = it->second;
                std::string filename = parentFileName + "/" + event->name;
                std::cout << "full file path " << filename << std::endl;
                stat(filename.c_str(), &s_buf);
                if(S_ISDIR(s_buf.st_mode))
                {
                    std::cout << "create new sub dir : " << event->name << std::endl;
                    int wd = inotify_add_watch(fd, filename.c_str(), IN_CLOSE_WRITE | IN_CREATE | IN_DELETE );
                    watchToFileName.insert(std::make_pair(wd, filename));
                }
                else
                {
                    std::cout << "create file : " << event->name << std::endl;
                }
            }
            else if(event->mask & IN_DELETE)
            {
                std::cout << event->name << " in delete" << std::endl;
            }
            else if(event->mask & (IN_DELETE_SELF | IN_MOVE_SELF | IN_IGNORED))
            {
                std::cout <<event->name << " deleted move of ignored" << std::endl;
                watchToFileName.erase(event->wd);
                inotify_rm_watch(fd, event->wd);
            }
            len = len - sizeof(struct inotify_event) - event->len;
            nread = nread + sizeof(struct inotify_event) + event->len;
        }
    }
    close(fd);
    // std::auto_ptr<NAMESPACE_NAME::TRecallAgentApp> _au(new NAMESPACE_NAME::TRecallAgentApp);
    // return _au->run(argc, argv).errorNo();
}
*********************************************************************************/
