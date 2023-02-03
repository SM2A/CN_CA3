#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <memory>
#include <fstream>
#include <unistd.h>
#include "Client.h"
#include "../Message/Message.h"


using namespace std;

Client::Client()
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
    timeout.tv_sec = 0;
    timeout.tv_usec = TIMEOUT;
    // setsockopt(this->socket_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
}

void Client::start()
{
    ifstream file("./Source/Client/test.txt");
    ofstream file2("file.dt");
    
    if (!file)
        throw runtime_error("file error");
    
    char ch;
    while (!file.eof())
    {
        char file_sector[MSG_SIZE] = { 0 };
        for (int i = 0; i < MSG_SIZE; i++)
        {
            file >> noskipws >> ch;
            if (file.eof())
                break;
            file_sector[i] = ch;
        }
        // Message *msg = new Message(
        //     inet_addr("127.0.0.1"), 
        //     inet_addr("127.0.0.1"), 
        //     this->packet_id++, 
        //     0, 
        //     SERVER_PORT, 
        //     this->cwnd,
        //     false, 
        //     file_sector[MSG_SIZE - 1] == 0,
        //     false,
        //     file_sector
        //     );

            Message *msg = new Message(
            this->packet_id++, 
            this->cwnd,
            false, 
            file_sector
            );

        this->window.push_back(msg);
        if (this->window.size() == this->cwnd)
        {
            this->packet_id = 0;
            sendWindow();
        }
    }
    while (this->window.size() > 0 && this->window.size() != this->cwnd)
    {
        // Message *msg = new Message(
        //     inet_addr("127.0.0.1"), 
        //     inet_addr("127.0.0.1"), 
        //     this->packet_id++, 
        //     0, 
        //     SERVER_PORT, 
        //     this->cwnd,
        //     false,
        //     true,
        //     true,
        //     ""
        //     );
        Message *msg = new Message(
            this->packet_id++, 
            this->cwnd,
            false,
            ""
            );
        this->window.push_back(msg);
    }
    if (this->window.size() > 0)
    {
        sendWindow();
    }
    close(this->socket_fd);
    file.close();
    cout<<"MAX: "<<max_cwnd<<endl;
}

void Client::sendWindow(bool is_first_call)
{
    // cerr<<"WINDOW: "<<window.size()<<endl;
    for (auto &el : window)
    {
        // cerr<<"MSG: "<<el->getMsg()<<endl<<"ID: "<<el->getPacketId()<<endl<<"SIZE: "<<el->getWSize()<<endl;
        cerr<<"MSG: "<<el->getMsg()<<" ID: "<<el->getPacketId()<<endl;
        send(socket_fd, el->getPacket(), PACKET_SIZE, 0);
    }

    unsigned char buff[PACKET_SIZE] = { 0 };
    auto r = recv(this->socket_fd, buff, PACKET_SIZE, 0);
    if (r < 0 || Message(buff).getPacketId() != cwnd)
    {
        cerr<<"FFFFFFFFFFFFFFFF"<<endl;
        cerr<<"R: "<<r<<" cwnd: "<<cwnd<<" id: "<<Message(buff).getPacketId()<<endl;
        if (is_first_call)
        {
            ssthresh = cwnd / 2;
            cwnd = 1;
        }
        this->sendWindow(false);
    }
    else 
    {
        if (is_first_call)
        {
            if (cwnd < ssthresh)
                cwnd *= 2;
            else
                cwnd++;
            if (max_cwnd < cwnd)
            {
                max_cwnd = cwnd;
            }
        }

        for (auto &el : window)
        {
            delete el;
        }
        window.clear();
    }
}