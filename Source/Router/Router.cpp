#include "Router.h"

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

    if(listen(this->socket_fd, 2) < 0)
    {
        throw runtime_error("listen error");
    }
}

void Router::start()
{
    this->connectToServer();
    thread t1(&Router::receiveClientPacket, this);
    thread t2(&Router::receiveServerPacket, this);
    thread t3(&Router::sendPacket, this);

    t1.join();
    t2.join();
    t3.join();
}

void Router::connectToServer() 
{
    struct sockaddr_in server_addr;   
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(SERVER_PORT); 

    if ((this->server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        throw runtime_error("socket error");
    }

    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        throw runtime_error("address error");
    }

    if (connect(this->server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        throw runtime_error("cennect error");
    }
}

void Router::receiveClientPacket()
{
    socklen_t length = sizeof(this->client_addr);
    if ((this->client_fd = accept(this->socket_fd, (struct sockaddr*)&this->client_addr, &length)) < 0)
        throw runtime_error("accept error");

    while (true)
    {
        unsigned char buff[PACKET_SIZE] = { 0 };
        recv(this->client_fd, buff, PACKET_SIZE, 0);
        Message msg(buff);
        if (msg.getSourcePort() == 0)
        {
            msg.setSourcePort(ntohs(this->client_addr.sin_port));
        }
        cerr<<"CLIENT MSG: "<<"   ID: "<<msg.getPacketId()<<"   isAck:  "<<msg.isAck()<<endl;
        m_messages.lock();
        messages.push_back(msg);
        m_messages.unlock();
    }
}

void Router::receiveServerPacket()
{
    while (true)
    {
        unsigned char buff[PACKET_SIZE] = { 0 };
        recv(this->server_fd, buff, PACKET_SIZE, 0);
        Message msg(buff);
        cerr<<"SERVER MSG: "<<"   ID: "<<msg.getPacketId()<<"   isAck:  "<<msg.isAck()<<endl;
        if (msg.isAck())
        {
            m_messages.lock();
            messages.push_back(msg);
            m_messages.unlock();
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
            if (messages[0].isAck())
                send(this->client_fd, messages[0].getPacket(), PACKET_SIZE, 0);  
            else
                send(this->server_fd, messages[0].getPacket(), PACKET_SIZE, 0);  
            messages.erase(messages.begin()); 
        }
        m_messages.unlock();
    }
}
