#include "Message.h"
#include <cstring>
#include <iostream>

using namespace std;

Message::Message(unsigned char packet[PACKET_SIZE])
{

    this->is_ack = (bool)packet[0];
    this->packet_id = (uint16_t)((packet[1] << 8) + packet[2]);

    // if (this->is_ack)
    // {
    //     cerr<<"id: "<<packet_id<<"  P1:  "<<(int)packet[1]<<" P2: "<<(int)packet[2]<<endl;
    // }

    this->source_ip = (packet[3] << (8 * 3)) + (packet[4] << (8 * 2)) + (packet[5] << 8) + packet[6];
    this->dest_ip = (packet[7] << (8 * 3)) + (packet[8] << (8 * 2)) + (packet[9] << 8) + packet[10];
    this->source_port = (packet[11] << 8) + packet[12];
    this->dest_port = (packet[13] << 8) + packet[14];
    this->w_size = (uint16_t)((packet[15] << 8) + packet[16]);
    this->is_end = (bool)packet[17];
    this->is_redundant = (bool)packet[18];
    
    memcpy(this->packet, packet, PACKET_SIZE);
    
    for (int i = 0; i < MSG_SIZE; i++)
    {
        this->msg[i] = packet[i + PACKET_SIZE - MSG_SIZE];
    }
}

Message::Message(uint32_t source_ip, uint32_t dest_ip, uint16_t packet_id, uint16_t source_port, uint16_t dest_port, uint16_t w_size, bool is_ack, bool is_end, bool is_redundant, char msg[MSG_SIZE])
{
    this->is_ack = is_ack;
    this->source_ip = source_ip;
    this->dest_ip = dest_ip;
    this->packet_id = packet_id;
    this->source_port = source_port;
    this->dest_port = dest_port;
    
    this->packet[0] = is_ack;

    this->packet[1] = (packet_id & 0xFF00) >> 8;
    this->packet[2] = packet_id & 0x00FF;

    this->packet[3] = (source_ip & 0xFF000000) >> (8 * 3);
    this->packet[4] = (source_ip & 0xFF0000) >> (8 * 2);
    this->packet[5] = (source_ip & 0xFF00) >> 8;
    this->packet[6] = (source_ip & 0xFF);

    this->packet[7] = (dest_ip & 0xFF000000) >> (8 * 3);
    this->packet[8] = (dest_ip & 0xFF0000) >> (8 * 2);
    this->packet[9] = (dest_ip & 0xFF00) >> 8;
    this->packet[10] = (dest_ip & 0xFF);

    this->packet[11] = (source_port & 0xFF00) >> 8;
    this->packet[12] = source_port & 0xFF;

    this->packet[13] = (dest_port & 0xFF00) >> 8;
    this->packet[14] = dest_port & 0xFF;

    this->packet[15] = (w_size & 0xFF00) >> 8;
    this->packet[16] = w_size & 0xFF;

    this->packet[17] = is_end;

    this->packet[18] = is_redundant;

    for (int i = 0; i < MSG_SIZE; i++)
    {
        this->packet[PACKET_SIZE - MSG_SIZE + i] = msg[i];
    }

    memcpy(this->msg, msg, MSG_SIZE);
}

void Message::setSourcePort(uint16_t source_port)
{
    this->source_port = source_port;
    this->packet[11] = (source_port & 0xFF00) >> 8;
    this->packet[12] = source_port & 0xFF;
}