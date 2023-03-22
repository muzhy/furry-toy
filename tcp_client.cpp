#include <iostream>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdio.h>

#include "error/error.h"

int main(int, char**) {
    std::cout << "start tcp client ..." << std::endl;

    int client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in addr;
    // IPv4
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(client_socket, (struct sockaddr*)&addr, sizeof(addr)) != 0)
    {
        std::cerr << "connet server failed!" << std::endl;
        return -1;
    }

    std::string str;
    while(true)
    {
        std::cin >> str;

        int len = send(client_socket, str.c_str(), str.size(), 0);
        if(str == "quit")
        {
            break;
        }
    }

    close(client_socket);

    return 0;
}