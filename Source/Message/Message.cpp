#include "Message.h"
#include <cstring>
#include <iostream>

using namespace std;

Message::Message(unsigned char packet[PACKET_SIZE])
{

    this->is_ack = (bool)packet[0];
    this->packet_id = (uint32_t)((packet[1] << (8 * 3)) + (packet[2] << (8 * 2)) + (packet[3] << 8) + packet[4]);

    // this->source_ip = (packet[3] << (8 * 3)) + (packet[4] << (8 * 2)) + (packet[5] << 8) + packet[6];
    // this->dest_ip = (packet[7] << (8 * 3)) + (packet[8] << (8 * 2)) + (packet[9] << 8) + packet[10];
    // this->source_port = (packet[11] << 8) + packet[12];
    // this->dest_port = (packet[13] << 8) + packet[14];
    // this->w_size = (uint16_t)((packet[15] << 8) + packet[16]);
    // this->is_end = (bool)packet[17];
    // this->is_redundant = (bool)packet[18];

    this->w_size = (uint32_t)((packet[5] << (8 * 3)) + (packet[6] << (8 * 2)) + (packet[7] << 8) + packet[8]);
    
    memcpy(this->packet, packet, PACKET_SIZE);

    
    
    for (int i = 0; i < MSG_SIZE; i++)
    {
        this->msg[i] = packet[i + PACKET_SIZE - MSG_SIZE];
    }
}

Message::Message(uint32_t packet_id, uint32_t w_size, bool is_ack, char msg[MSG_SIZE] = { 0 })
{
    this->is_ack = is_ack;
    // this->source_ip = source_ip;
    // this->dest_ip = dest_ip;
    this->packet_id = packet_id;
    // this->source_port = source_port;
    // this->dest_port = dest_port;
    this->w_size = w_size;
    // this->is_end = is_end;
    // this->is_redundant = is_redundant;
    
    this->packet[0] = is_ack;

    this->packet[1] = (packet_id & 0xFF000000) >> (8 * 3);
    this->packet[2] = (packet_id & 0xFF0000) >> (8 * 2);
    this->packet[3] = (packet_id & 0xFF00) >> 8;
    this->packet[4] = (packet_id & 0xFF);

    // this->packet[3] = (source_ip & 0xFF000000) >> (8 * 3);
    // this->packet[4] = (source_ip & 0xFF0000) >> (8 * 2);
    // this->packet[5] = (source_ip & 0xFF00) >> 8;
    // this->packet[6] = (source_ip & 0xFF);

    // this->packet[7] = (dest_ip & 0xFF000000) >> (8 * 3);
    // this->packet[8] = (dest_ip & 0xFF0000) >> (8 * 2);
    // this->packet[9] = (dest_ip & 0xFF00) >> 8;
    // this->packet[10] = (dest_ip & 0xFF);

    // this->packet[11] = (source_port & 0xFF00) >> 8;
    // this->packet[12] = source_port & 0xFF;

    // this->packet[13] = (dest_port & 0xFF00) >> 8;
    // this->packet[14] = dest_port & 0xFF;

    // this->packet[15] = (w_size & 0xFF00) >> 8;
    // this->packet[16] = w_size & 0xFF;

    // this->packet[17] = is_end;

    // this->packet[18] = is_redundant;

    this->packet[5] = (w_size & 0xFF000000) >> (8 * 3);
    this->packet[6] = (w_size & 0xFF0000) >> (8 * 2);
    this->packet[7] = (w_size & 0xFF00) >> 8;
    this->packet[8] = (w_size & 0xFF);

    if (msg[0] != 0)
    {
        for (int i = 0; i < MSG_SIZE; i++)
        {
            this->packet[PACKET_SIZE - MSG_SIZE + i] = msg[i];
        }

        memcpy(this->msg, msg, MSG_SIZE);
    }
}

void Message::setSourcePort(uint16_t source_port)
{
    this->source_port = source_port;
    this->packet[11] = (source_port & 0xFF00) >> 8;
    this->packet[12] = source_port & 0xFF;
}