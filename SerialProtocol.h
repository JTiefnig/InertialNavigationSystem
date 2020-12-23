
#include "mbed.h"



enum MessageType
{
    SEND_DATA=0,
    REGISTER_ID,
};



struct SerialMessage
{
    
    MessageType mtype;
    //Head
    unsigned char dlc;

    unsigned char id;
    // data
    char* data; // todo more cpp stlye
    // Todo checksum; 
};


class SerialProtocol : public Serial
{

public:

    SerialProtocol(PinName tx, PinName rx)
        :Serial(tx, rx, 115200)
    {

    }

    void SendData(const SerialMessage& msg);


    void SendInt(const char* id, int value);
    void SendFloat(unsigned char id, float value);
    void SendDouble(unsigned char id, double value);
    void SendShort(const char* id, int16_t value);


    // Need some kind of data link structure will think of that
    // Timer update 

};