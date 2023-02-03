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
        uint32_t ssthresh = 0xffffffff;
        uint32_t cwnd = 1;
        uint32_t max_cwnd = 1;
        uint32_t packet_id = 0;
        vector<Message*> window;
        void sendWindow(ofstream &file, bool is_first_call = true);

    public:
        Client();
        void start();
};

#endif
