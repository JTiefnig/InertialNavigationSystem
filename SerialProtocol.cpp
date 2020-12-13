#include "SerialProtocol.h"



void SerialProtocol::SendData(const SerialMessage& msg)
{
    
    putc('>');
    putc(msg.mtype);
    putc(msg.dlc+1);

    putc(msg.id);


    //printf(">%c%c%c", msg.mtype, msg.dlc+1, msg.id);

    write(msg.data, msg.dlc);

}



void SerialProtocol::SendInt(const char* id, int value)
{
    // int siz =sizeof(int);
    // SerialMessage msg;
    // msg.id = id;
    // msg.typeRep = 'i';
    // msg.datalenght = siz;
    // msg.data = (char*)(&value); // littlebit crazy type punning

    // SendData(msg);
}



void SerialProtocol::SendFloat(const char* id, float value)
{
    // int siz =sizeof(float);
    // SerialMessage msg;
    // msg.id = id;
    // msg.typeRep = 'f';
    // msg.datalenght = siz;
    // msg.data = (char*)(&value); // littlebit crazy type punning

    // SendData(msg);
}



void SerialProtocol::SendDouble(unsigned char id, double value)
{
    int siz =sizeof(double);
    SerialMessage msg;


    msg.mtype = MessageType::SEND_DATA;

    msg.dlc = siz;
    msg.id = id;
    msg.data = (char*)(&value); // littlebit crazy type punning

    SendData(msg);
}



void SerialProtocol::SendShort(const char* id, int16_t value)
{
    // int siz =sizeof(int16_t);
    // SerialMessage msg;
    // msg.id = id;
    // msg.typeRep = 's';
    // msg.datalenght = siz;
    // msg.data = (char*)(&value); // littlebit crazy type punning

    // SendData(msg);
}