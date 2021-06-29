#include "Car.h"

Car::Car()
    : serial_()
    , packet_()
    , tranSpeed_(0)
    , rotaSpeed_(0)
{
}

Car::~Car()
{
    move(0, 0);
}

void Car::init()
{
    serial_.openPort("/dev/ttySAC2");
    serial_.setup(115200, 0, 8, 1, 'N');
}

void Car::move(Speed tranSpeed, Speed rotaSpeed)
{
    tranSpeed_ = tranSpeed;
    rotaSpeed_ = rotaSpeed;
    updatePacket();
}

void Car::updatePacket()
{
    setPacketHead();
    setPacketData();
    setPacketCheck();
}

void Car::setPacketHead()
{
    packet_[0] = 0xAA;
}

void Car::setPacketData()
{
    packet_[1] = tranSpeed_ & 0xFF;
    packet_[2] = tranSpeed_ >> 8;
    packet_[3] = rotaSpeed_ & 0xFF;
    packet_[4] = rotaSpeed_ >> 8;
}

void Car::setPacketCheck()
{
    uint8_t sum = 0;
    for(int i = 0; i < kPacketSize_; ++i) {
        sum += packet_[i];
    }
    packet_[kPacketSize_ - 1] = sum;
}
