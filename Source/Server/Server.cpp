#include <bits/stdc++.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> 
#include <string.h>
#include <fstream>
#include "Server.h"

using namespace std;


Server::Server()
{
    struct sockaddr_in server_addr;    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    server_addr.sin_port = htons(SERVER_PORT);     
    int opt = 1;

    if ((this->socket_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
        throw runtime_error("socket error");

    if (setsockopt(this->socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
        throw runtime_error("setsockopt error");

    if(bind(this->socket_fd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr)) < 0)
        throw runtime_error("bind error");

    if(listen(this->socket_fd, 3) < 0)
        throw runtime_error("listen error");
}

void Server::start()
{
    struct sockaddr_in router_addr;
    socklen_t length = sizeof(router_addr);
    if((this->router_fd = accept(this->socket_fd, (struct sockaddr*) &router_addr, &length)) < 0)
        throw runtime_error("accept error");

    ofstream file("new_file.dt");

    bool is_end = false;

    while (true) {
        unsigned char buff[PACKET_SIZE];
        recv(this->router_fd, buff, PACKET_SIZE, 0);
        Message *msg = new Message(buff);
        if (window.size() != msg->getWSize())
        {
            resetWindow(msg->getWSize());
        }
        window[msg->getPacketId()] = msg;
        bool window_is_full = true;
        for (auto &el : window)
        {
            if (el == 0)
            {
                window_is_full = false;
                break;
            }
        }
        if (msg->isEnd())
        {
            is_end = true;
        }
        if (window_is_full)
        {
            for (auto &el : window)
            {
                file<<el->getMsg();
            }
            Message ack(
                inet_addr("127.0.0.1"), 
                inet_addr("127.0.0.1"), 
                msg->getWSize(), 
                SERVER_PORT, 
                msg->getSourcePort(), 
                0,
                true,
                false,
                false,
                ""
                );
                cerr<<"id: "<<ack.getPacketId()<<endl;
                cerr<<"FFFFFFFFFFFFFF"<<endl;
            send(this->router_fd, ack.getPacket(), PACKET_SIZE, 0);
            if (is_end) break;
        }
    }
    file.close();
}

void Server::resetWindow(int new_size)
{
    for (auto &el : window)
    {
        delete el;
    }
    window.clear();
    window.resize(new_size, 0);
}
