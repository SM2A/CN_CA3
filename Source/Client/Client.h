#ifndef CLIENT
#define CLIENT

#include "../config.h"
#include "../Message/Message.h"
#include <vector>
#include <climits>

using namespace std;

class Client
{
    private:
        int socket_fd;
        int ssthresh = INT_MAX;
        int cwnd = 1;
        int max_cwnd = 1;
        uint16_t packet_id = 0;
        vector<Message*> window;
        void sendWindow(bool is_first_call = true);

    public:
        Client();
        void start();
};

#endif
