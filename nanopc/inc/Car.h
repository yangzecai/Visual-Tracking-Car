#ifndef CAR_H
#define CAR_H

#include "SerialPort.h"

class Car {
public:
    using Speed = short;

    Car();
    ~Car();
    void init();
    void move(Speed tranSpeed, Speed rotaSpeed);
    
private:
    SerialPort serial_;
    static const int kPacketSize_ = 6;
    uint8_t packet_[kPacketSize_];
    Speed tranSpeed_;
    Speed rotaSpeed_;

    void updatePacket();
    void setPacketHead();
    void setPacketCheck();
    void setPacketData();
};

#endif