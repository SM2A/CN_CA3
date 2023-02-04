#include "Message.h"
#include <cstring>
#include <iostream>

using namespace std;

Message::Message(unsigned char packet[PACKET_SIZE])
{

    this->is_ack = (bool)(packet[0] & 0x01);
    this->packet_id = (short)packet[1];
    this->w_size = ((short)(packet[0] & 0b11111110)) >> 1;
    
    memcpy(this->packet, packet, PACKET_SIZE);

    
    
    for (int i = 0; i < MSG_SIZE; i++)
    {
        this->msg[i] = packet[i + PACKET_SIZE - MSG_SIZE];
    }
}

Message::Message(short packet_id, short w_size, bool is_ack, char msg[MSG_SIZE] = { 0 })
{
    this->packet_id = packet_id;
    this->w_size = w_size;
    this->is_ack = is_ack;

    this->packet[0] = (unsigned char)((w_size << 1) + is_ack);
    this->packet[1] = (unsigned char)packet_id;
    
    if (msg[0] != 0)
    {
        for (int i = 0; i < MSG_SIZE; i++)
        {
            this->packet[PACKET_SIZE - MSG_SIZE + i] = msg[i];
        }

        memcpy(this->msg, msg, MSG_SIZE);
    }
}