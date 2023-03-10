#ifndef ROUTER
#define ROUTER

#include <vector>
#include <map>
#include <mutex>
#include <bits/stdc++.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <thread>
#include "../config.h"
#include "../Message/Message.h"
#include <sys/time.h>

using namespace std;

class Router
{
    private:
        int server_fd, client_fd, socket_fd, max_sd;
        fd_set readfds;
        mutex m_messages;
        vector<Message*> messages;
        struct sockaddr_in client_addr;

        void sendPacket();
        void receivePacket();


    public:
        Router();
        void start();
};

#endif
