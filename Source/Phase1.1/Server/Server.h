#ifndef SERVER
#define SERVER

#include "../config.h"
#include "../Message/Message.h"
#include <vector>

using namespace std;

class Server
{
    private:
        int socket_fd;
        uint32_t last_sent_size = 0;
        vector<Message*> window;
        void resetWindow(uint32_t new_size);
        void saveWindow(uint64_t &sum_of_packets, uint32_t window_size, ofstream &file, bool is_end);

    public:
        Server();
        void start();
};

#endif
