#ifndef MESSAGE
#define MESSAGE

#include <stdint.h>
#include "../config.h"

using namespace std;

class Message
{
    private:
        short w_size, packet_id;
        bool is_ack;
        unsigned char msg[MSG_SIZE + 1] = { 0 }, packet[PACKET_SIZE] = { 0 };

    public:
        Message(unsigned char packet[PACKET_SIZE]);
        Message(short packet_id, short w_size, bool is_ack, char msg[MSG_SIZE]);
        unsigned char* getMsg() { return msg; }
        unsigned char* getPacket() { return packet; }
        bool isAck() { return is_ack; }
        short getPacketId() { return packet_id; }
        short getWSize() { return w_size; }
};

#endif