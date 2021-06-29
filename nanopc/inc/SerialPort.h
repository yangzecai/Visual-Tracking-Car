#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <stdint.h>

class SerialPort {
public:
    SerialPort();
    ~SerialPort();
    bool openPort(const char * dev);
    int setup(int speed,int flow_ctrl,int databits,
              int stopbits,int parity);
    void setSpeed(int speed);
    int setParity(int databits,int stopbits,int parity);
    int readBuffer(uint8_t * buffer,int size);
    int writeBuffer(uint8_t * buffer,int size);
    uint8_t getchar();
    void closePort();
private:
    int fd_;
    static const int speedArr_[7];
    static const int nameArr_[7];
};

#endif