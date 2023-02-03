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
}

void Server::start()
{
    ofstream file("new_file.dt");

    bool is_end = false;
    uint32_t window_size = 0;
    uint64_t sum_of_packets = 0;

    while (true) {
        ssize_t total_r = 0, r;
        unsigned char prev_buff[PACKET_SIZE] = { 0 }, curr_buff[PACKET_SIZE];
        Message *msg;
        while (true)
        {
            unsigned char buff[PACKET_SIZE] = { 0 };
            r = recv(this->socket_fd, buff, PACKET_SIZE, 0);
            std::cerr<<"RRRR: "<<r<<endl;
            if (r == 0)
            {
                cerr<<"TTT: "<<total_r<<endl;
                is_end = true;
                break;
            }
            
            if (total_r + r == PACKET_SIZE)
            {
                memcpy(curr_buff, prev_buff, PACKET_SIZE);
                for (auto i = total_r; i < PACKET_SIZE; i++)
                {
                    curr_buff[i] = buff[i - total_r];
                }
                msg = new Message(curr_buff);
                if (window_size != msg->getWSize())
                {
                    if (window_size != 0) 
                    {
                        saveWindow(sum_of_packets, window_size, file, is_end);
                        last_sent_size = window_size;
                    }
                    window_size = msg->getWSize();
                    resetWindow(window_size);
                }
                window[msg->getPacketId()] = msg;
                sum_of_packets += msg->getPacketId();
                std::cerr<<"ID: "<<msg->getPacketId()<<endl;

                break;
            }
            else if (total_r + r > PACKET_SIZE)
            {
                memcpy(curr_buff, prev_buff, PACKET_SIZE);
                for (auto i = total_r; i < PACKET_SIZE; i++)
                {
                    curr_buff[i] = buff[i - total_r];
                }

                msg = new Message(curr_buff);
                if (window_size != msg->getWSize())
                {
                    if (window_size != 0) 
                    {
                        saveWindow(sum_of_packets, window_size, file, is_end);
                        last_sent_size = window_size;
                    }
                    window_size = msg->getWSize();
                    resetWindow(window_size);
                }
                window[msg->getPacketId()] = msg;
                sum_of_packets += msg->getPacketId();
                std::cerr<<"IIIIIDD: "<<msg->getPacketId()<<endl;
                
                memset(prev_buff, 0, PACKET_SIZE);
                for (auto i = PACKET_SIZE - total_r; i < PACKET_SIZE; i++)
                {
                    prev_buff[i - (PACKET_SIZE - total_r)] = buff[i];
                }
                total_r = total_r + r - PACKET_SIZE;
            }
            else
            {
                for (auto i = total_r; i < total_r + r; i++)
                {
                    prev_buff[i] = buff[i - total_r];
                }
                total_r += r;
            }
        }
        saveWindow(sum_of_packets, window_size, file, is_end);










        
        
        
        

    
        
        if (is_end)
        {
            close(this->socket_fd);
            break;
        }
    }
    std::cerr<<"QQQQQQQQQQQQ33"<<endl;
    file.close();
}

void Server::saveWindow(uint64_t &sum_of_packets, uint32_t window_size, ofstream &file, bool is_end)
{
    std::cerr<<" SUM: "<<sum_of_packets<<" SIZE: "<<window_size<<endl;
    if ((sum_of_packets == window_size * (window_size - 1) / 2 || (is_end && sum_of_packets != 0)) && window_size != last_sent_size)
        {
            this->last_sent_size = window_size;
            sum_of_packets = 0;
            for (auto &el : window)
            {
                if (el == 0) break;
                std::cerr<<"ID: "<<el->getPacketId()<<" SIZE: "<<el->getWSize()<<" remain: "<<window.size() - 1<<" MSG: "<<el->getMsg()<<endl;
                file<<el->getMsg();
            }
            // Message ack(
            //     inet_addr("127.0.0.1"), 
            //     inet_addr("127.0.0.1"), 
            //     msg->getWSize(), 
            //     SERVER_PORT, 
            //     msg->getSourcePort(), 
            //     1,
            //     true,
            //     false,
            //     false,
            //     ""
            //     );

            
            // if (strlen((char*)window[window.size() - 1]->getMsg()) == 0) break;
            // Message ack(
            //     msg->getWSize(),
            //     1,
            //     true,
            //     ""
            //     );
            // send(this->socket_fd, ack.getPacket(), PACKET_SIZE, 0);
        }
}

void Server::resetWindow(uint32_t new_size)
{
    for (auto &el : window)
    {
        delete el;
    }
    window.clear();
    window.resize(new_size, 0);
}
