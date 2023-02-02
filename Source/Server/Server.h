#ifndef SERVER
#define SERVER

#include "../config.h"
#include "../Message/Message.h"
#include <vector>

using namespace std;

class Server
{
    private:
        int socket_fd, router_fd;
        vector<Message*> window;
        void resetWindow(int new_size);
    public:
        Server();
        void start();
};

#endif
