#include <bits/stdc++.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> 
#include <string.h>
#include <fstream>
#include <unistd.h>
#include "Server.h"

using namespace std;


Server::Server()
{
    struct sockaddr_in router_addr;   
    memset(&router_addr, 0, sizeof(router_addr));
    router_addr.sin_family = AF_INET;
    router_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    router_addr.sin_port = htons(ROUTER_PORT); 

    if ((this->socket_fd = socket(AF_INET,SOCK_STREAM, 0)) < 0) {
        throw runtime_error("socket error");
    }

    if (inet_pton(AF_INET, "127.0.0.1", &router_addr.sin_addr) <= 0) {
        throw runtime_error("address error");
    }

    if (connect(this->socket_fd, (struct sockaddr *)&router_addr, sizeof(router_addr)) < 0)
    {
        throw runtime_error("cennect error");
    }

    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    setsockopt(this->socket_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
}

void Server::start()
{
    ofstream file("./new_file2");

    bool is_end = false;
    short window_size = 0;
    short sum_of_packets = -1;

    while (true) {
        unsigned char buff[PACKET_SIZE] = { 0 };
        auto r = recv(this->socket_fd, buff, PACKET_SIZE, 0);
        if (r <= 0)
        {
            close(socket_fd);
            break;
        }
        Message *msg = new Message(buff);

        if (window_size != msg->getWSize() || (msg->getWSize() == 1 && last_sent_packet != msg->getPacketId()))
        {
            window_size = msg->getWSize();
            resetWindow(window_size);
        }

        auto idx = msg->getPacketId() % msg->getWSize();

        if (window[idx] == NULL)
        {
            if (idx == 0)
                sum_of_packets += 1;
            else
                sum_of_packets += idx;
        }
        
        window[idx] = msg;

        saveWindow(sum_of_packets, window_size, file);
        
        if (is_end)
        {
            close(this->socket_fd);
            break;
        }
    }
    file.close();
}

void Server::saveWindow(short &sum_of_packets, short window_size, ofstream &file)
{
    if (sum_of_packets == window_size * (window_size - 1) / 2 || (window[window_size - 1] != NULL && window[window_size - 1]->getPacketId() == last_sent_packet && last_sent_window == window_size))
    {
        sum_of_packets = -1;
        if (window[window_size - 1]->getPacketId() != last_sent_packet || last_sent_window != window_size)
            for (auto &el : window)
            {
                if (el == 0) break;
                file<<el->getMsg();
            }
        this->last_sent_packet = window[window_size - 1]->getPacketId();
        last_sent_window = window_size;
        
        Message ack(
            last_sent_packet,
            window_size,
            true,
            ""
            );
        send(this->socket_fd, ack.getPacket(), PACKET_SIZE, 0);
    }
}

void Server::resetWindow(short new_size)
{
    for (auto &el : window)
    {
        delete el;
    }
    window.clear();
    window.resize(new_size, NULL);
}
