#include "Router.h"
#include <fstream>

using namespace std;


Router::Router()
{
    struct sockaddr_in router_addr;
    router_addr.sin_family = AF_INET;
    router_addr.sin_addr.s_addr = INADDR_ANY;
    router_addr.sin_port = htons(ROUTER_PORT);      
    int opt = 1;

    if((this->socket_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {  
        throw runtime_error("socket error");
    }

    if (setsockopt(this->socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        throw runtime_error("setsockopt error");
    }

    
    if(bind(this->socket_fd, (struct sockaddr *)&router_addr, sizeof(struct sockaddr)) < 0)
    {
        throw runtime_error("bind error");
    }

    if(listen(this->socket_fd, 3) < 0)
    {
        throw runtime_error("listen error");
    }
}

void Router::start()
{
    socklen_t length = sizeof(this->client_addr);
    if ((this->server_fd = accept(this->socket_fd, (struct sockaddr*)&this->client_addr, &length)) < 0)
        throw runtime_error("accept error");

    if ((this->client_fd = accept(this->socket_fd, (struct sockaddr*)&this->client_addr, &length)) < 0)
        throw runtime_error("accept error");

    max_sd = server_fd;
    if (client_fd > max_sd)
        max_sd = client_fd;

    thread t1(&Router::receivePacket, this);
    thread t3(&Router::sendPacket, this);

    t1.join();
    t3.join();
}

void Router::receivePacket()
{
    while (true)
    {
        FD_ZERO(&readfds);  
     
        FD_SET(client_fd, &readfds);
        // FD_SET(server_fd, &readfds);

        int activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);  
       
        if ((activity < 0) && (errno != EINTR))  
            throw runtime_error("select error");  

        

        // if (FD_ISSET(server_fd , &readfds))
        // {
        //     r = recv(this->server_fd, buff, PACKET_SIZE, 0);
        //     Message msg(buff);
        //     cerr<<"SERVER MSG: "<<"   ID: "<<msg.getPacketId()<<"   isAck:  "<<msg.isAck()<<endl;

        //     if (r < 0)
        //         throw runtime_error("read error");
        //     m_messages.lock();
        //     messages.push_back(msg);
        //     m_messages.unlock();
        // }
        // else
         if (FD_ISSET(client_fd , &readfds))
        {
            ssize_t total_r = 0, r;
            unsigned char prev_buff[PACKET_SIZE] = { 0 }, curr_buff[PACKET_SIZE];
            while (true)
            {
                unsigned char buff[PACKET_SIZE] = { 0 };
                r = recv(this->client_fd, buff, PACKET_SIZE, 0);
                // if (recv(this->client_fd, NULL, 1, MSG_PEEK | MSG_DONTWAIT) == 0)
                // {
                //     break;
                // }

                // cerr<<"RRRRR: "<<r<<endl;
                
                if (total_r + r == PACKET_SIZE)
                {
                    memcpy(curr_buff, prev_buff, PACKET_SIZE);
                    for (auto i = total_r; i < PACKET_SIZE; i++)
                    {
                        curr_buff[i] = buff[i - total_r];
                    }
                    Message msg(curr_buff);
        // cerr<<"MSG: "<<msg.getMsg()<<" ID: "<<msg.getPacketId()<<endl;
        //             if (strlen((char*)msg.getMsg()) == 0 && msg.getPacketId() == 0)
        //             {
        //                 cerr<<"TOTALR: "<<total_r<<" R: "<<r<<endl;
        //                 cerr<<"CURR"<<endl;
        //                 for (int i = 0; i < PACKET_SIZE; i++)
        //                 {
        //                     cerr<<"P"<<i<<": "<<(int)curr_buff[i]<<endl;
        //                 }
        //                 cerr<<"BUFF"<<endl;
        //                 for (int i = 0; i < PACKET_SIZE; i++)
        //                 {
        //                     cerr<<"P"<<i<<": "<<(int)buff[i]<<endl;
        //                 }
        //                 cerr<<"PREV"<<endl;
        //                 for (int i = 0; i < PACKET_SIZE; i++)
        //                 {
        //                     cerr<<"P"<<i<<": "<<(int)prev_buff[i]<<endl;
        //                 }
        //             }
                    cerr<<"IIIIDDDD: "<<msg.getPacketId()<<endl;
        m_messages.lock();
            messages.push_back(msg);
            m_messages.unlock();
                    // cerr<<"E CLIENT MSG: "<<msg.getMsg()<<"   ID: "<<msg.getPacketId()<<"   isAck:  "<<msg.isAck()<<" SIZE: "<<msg.getWSize()<<endl;
                    break;
                }
                else if (total_r + r > PACKET_SIZE)
                {
                    memcpy(curr_buff, prev_buff, PACKET_SIZE);
                    for (auto i = total_r; i < PACKET_SIZE; i++)
                    {
                        curr_buff[i] = buff[i - total_r];
                    }
                    Message msg(curr_buff);
                    cerr<<"IIIIDDDD: "<<msg.getPacketId()<<endl;
        // cerr<<"MSG: "<<msg.getMsg()<<" ID: "<<msg.getPacketId()<<" SSS"<<endl;
        //             if (strlen((char*)msg.getMsg()) == 0 && msg.getPacketId() == 0)
        //             {
        //                 cerr<<"TOTALR: "<<total_r<<" R: "<<r<<endl;
        //                 cerr<<"CURR"<<endl;
        //                 for (int i = 0; i < PACKET_SIZE; i++)
        //                 {
        //                     cerr<<"P"<<i<<": "<<(int)curr_buff[i]<<endl;
        //                 }
        //                 cerr<<"BUFF"<<endl;
        //                 for (int i = 0; i < PACKET_SIZE; i++)
        //                 {
        //                     cerr<<"P"<<i<<": "<<(int)buff[i]<<endl;
        //                 }
        //                 cerr<<"PREV"<<endl;
        //                 for (int i = 0; i < PACKET_SIZE; i++)
        //                 {
        //                     cerr<<"P"<<i<<": "<<(int)prev_buff[i]<<endl;
        //                 }
        //             }

        m_messages.lock();
            messages.push_back(msg);
            m_messages.unlock();
                    // cerr<<"CLIENT MSG: "<<msg.getMsg()<<"   ID: "<<msg.getPacketId()<<"   isAck:  "<<msg.isAck()<<" SIZE: "<<msg.getWSize()<<endl;
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
            // cerr<<"LAST: "<<(int)buff[PACKET_SIZE - 1]<<endl;
            
            // cerr<<"RRRRR: "<<r<<endl;
            if (recv(this->client_fd, NULL, 1, MSG_PEEK | MSG_DONTWAIT) == 0)
            {
                close(this->client_fd);
                close(this->server_fd);
                shutdown(this->socket_fd, SHUT_RDWR);
                break;
            }
        }
            
    }
}

void Router::sendPacket()
{
    while (true)
    {
        m_messages.lock();
        if (!messages.empty())
        {
            // cerr<<"SENDING: "<<messages.size()<<" ID: "<<messages[0].getPacketId()<< " size: "<<messages[0].getWSize()<<endl;
            // if (messages[0].isAck())
            //     send(this->client_fd, messages[0].getPacket(), PACKET_SIZE, 0);  
            // else
                send(this->server_fd, messages[0].getPacket(), PACKET_SIZE, 0); 
            cerr<<"ID: "<<messages[0].getPacketId()<<" SIZE: "<<messages[0].getWSize()<<" remain: "<<messages.size() - 1<<" MSG: "<<messages[0].getMsg()<<endl;
            messages.erase(messages.begin()); 
        }
        m_messages.unlock();
    }
}
