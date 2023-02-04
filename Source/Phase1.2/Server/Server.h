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
        short last_sent_packet = -1, last_sent_window = 0;
        vector<Message*> window;
        void resetWindow(short new_size);
        void saveWindow(short &sum_of_packets, short window_size, ofstream &file);

    public:
        Server();
        void start();
};

#endif
