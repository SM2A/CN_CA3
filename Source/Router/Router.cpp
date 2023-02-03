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
        FD_SET(server_fd, &readfds);

        int activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);  
       
        if ((activity < 0) && (errno != EINTR))  
            throw runtime_error("select error");  

        
        ssize_t total_r = 0, r;
        unsigned char prev_buff[PACKET_SIZE] = { 0 }, curr_buff[PACKET_SIZE];

        if (FD_ISSET(server_fd , &readfds))
        {
            while (true)
            {
                unsigned char buff[PACKET_SIZE] = { 0 };
                r = recv(this->server_fd, buff, PACKET_SIZE, 0);
                
                if (total_r + r == PACKET_SIZE)
                {
                    memcpy(curr_buff, prev_buff, PACKET_SIZE);
                    for (auto i = total_r; i < PACKET_SIZE; i++)
                    {
                        curr_buff[i] = buff[i - total_r];
                    }
                    Message msg(curr_buff);
                    m_messages.lock();
                    messages.push_back(msg);
                    m_messages.unlock();
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

                    m_messages.lock();
                    messages.push_back(msg);
                    m_messages.unlock();
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
        }
        else if (FD_ISSET(client_fd , &readfds))
        {
            while (true)
            {
                unsigned char buff[PACKET_SIZE] = { 0 };
                r = recv(this->client_fd, buff, PACKET_SIZE, 0);
                
                if (total_r + r == PACKET_SIZE)
                {
                    memcpy(curr_buff, prev_buff, PACKET_SIZE);
                    for (auto i = total_r; i < PACKET_SIZE; i++)
                    {
                        curr_buff[i] = buff[i - total_r];
                    }
                    Message msg(curr_buff);
                    m_messages.lock();
                    messages.push_back(msg);
                    m_messages.unlock();
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

                    m_messages.lock();
                    messages.push_back(msg);
                    m_messages.unlock();
                    
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
        }
            
        if (recv(this->client_fd, NULL, 1, MSG_PEEK | MSG_DONTWAIT) == 0)
        {
            close(this->client_fd);
            close(this->server_fd);
            shutdown(this->socket_fd, SHUT_RDWR);
            break;
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
            if (messages[0].isAck())
                send(this->client_fd, messages[0].getPacket(), PACKET_SIZE, 0);  
            else
                send(this->server_fd, messages[0].getPacket(), PACKET_SIZE, 0); 
            // cerr<<"ID: "<<messages[0].getPacketId()<<" SIZE: "<<messages[0].getWSize()<<" remain: "<<messages.size() - 1<<" MSG: "<<messages[0].getMsg()<<endl;
            messages.erase(messages.begin()); 
        }
        m_messages.unlock();
    }
}
