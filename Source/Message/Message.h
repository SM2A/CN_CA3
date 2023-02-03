#ifndef MESSAGE
#define MESSAGE

#include <stdint.h>
#include "../config.h"

using namespace std;

class Message
{
    private:
        uint32_t source_ip, dest_ip,  w_size, packet_id;
        uint16_t  source_port, dest_port;
        bool is_ack, is_end, is_redundant;
        unsigned char msg[MSG_SIZE] = { 0 }, packet[PACKET_SIZE] = { 0 };

    public:
        Message(unsigned char packet[PACKET_SIZE]);
        // Message(uint32_t source_ip, uint32_t dest_ip, uint16_t packet_id, uint16_t source_port, uint16_t dest_port, uint16_t w_size, bool is_ack, bool is_end, bool is_redundant, char msg[MSG_SIZE]);
        Message(uint32_t packet_id, uint32_t w_size, bool is_ack, char msg[MSG_SIZE]);
        unsigned char* getMsg() { return msg; }
        unsigned char* getPacket() { return packet; }
        bool isAck() { return is_ack; }
        uint32_t getPacketId() { return packet_id; }
        uint16_t getSourcePort() { return source_port; }
        uint16_t getDestPort() { return dest_port; }
        uint32_t getSourceIp() { return source_ip; }
        uint32_t getDestIp() { return dest_ip; }
        uint32_t getWSize() { return w_size; }
        bool isEnd() { return is_end; }
        bool isRedundant() { return is_redundant; }
        void setSourcePort(uint16_t source_port);
};

#endif