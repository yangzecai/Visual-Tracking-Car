#include "SerialPort.h"

#include <string.h>
#include <sys/ioctl.h>

const int SerialPort::speedArr_[7] = {B115200, B38400, B19200,
                                       B9600, B4800, B2400, B1200};
const int SerialPort::nameArr_[7] = {115200, 38400, 19200,
                                      9600, 4800, 2400, 1200};

SerialPort::SerialPort()
    : fd_(-1)
{
}

SerialPort::~SerialPort()
{
    if (fd_ != -1) {
        closePort();
    }
}

bool SerialPort::openPort(const char * dev)
{
    char* _dev = new char[32];
    strcpy(_dev, dev);
    fd_ = open(_dev, O_RDWR);
    if (fd_ == -1) {  
        perror("Can't Open Serial Port");
        return false;
    }
    int DTR_flag;
    DTR_flag = TIOCM_DTR;
    ioctl(fd_, TIOCMBIS, &DTR_flag);  // Set RTS pin
    return true;
}

int SerialPort::setup(int speed, int flow_ctrl, int databits,
                      int stopbits, int parity)
{
	int i;
	int status;
    struct termios options;
	/* tcgetattr(fd_,&options)得到与fd_指向对象的相关参数，并将它
    们保存于options,该函数还可以测试配置是否正确，该串口是否可用等。
    若调用成功，函数返回值为0，若调用失败，函数返回值为1. */
	if (tcgetattr(fd_, &options) != 0) {
		perror("SetupSerial 1");
		return false;
	}

    // 设置串口输入波特率和输出波特率  
	for (i= 0; i < sizeof(speedArr_) / sizeof(int); ++i) {
		if  (speed == nameArr_[i]) {
			cfsetispeed(&options, speedArr_[i]);
			cfsetospeed(&options, speedArr_[i]);
		}
	}
    
    // 修改控制模式，保证程序不会占用串口
    options.c_cflag |= CLOCAL;
    // 修改控制模式，使得能够从串口中读取输入数据
    options.c_cflag |= CREAD;

    // 设置数据流控制
    switch (flow_ctrl) {
		case 0 :// 不使用流控制
            options.c_cflag &= ~CRTSCTS;
            break;
		case 1 :// 使用硬件流控制
            options.c_cflag |= CRTSCTS;
            break;
		case 2 :// 使用软件流控制
            options.c_cflag |= IXON | IXOFF | IXANY;
            break;
    }
    // 设置数据位
    // 屏蔽其他标志位
    options.c_cflag &= ~CSIZE;
    switch (databits) {
		case 5 :
            options.c_cflag |= CS5;
            break;
		case 6 :
            options.c_cflag |= CS6;
            break;
		case 7 :
            options.c_cflag |= CS7;
            break;
		case 8 :
            options.c_cflag |= CS8;
            break;
		default:
            fprintf(stderr,"Unsupported data size\n");
            return false;
    }
    // 设置校验位
    switch (parity) {
		case 'n':
		case 'N': // 无奇偶校验位
            options.c_cflag &= ~PARENB;
            options.c_iflag &= ~INPCK;
            break;
		case 'o':
		case 'O':// 设置为奇校验
            options.c_cflag |= (PARODD | PARENB);
            options.c_iflag |= INPCK;
            break;
		case 'e':
		case 'E':// 设置为偶校验
            options.c_cflag |= PARENB;
            options.c_cflag &= ~PARODD;
            options.c_iflag |= INPCK;
            break;
		case 's':
		case 'S': // 设置为空格
            options.c_cflag &= ~PARENB;
            options.c_cflag &= ~CSTOPB;
            break;
        default:
            fprintf(stderr, "Unsupported parity\n");
            return false;
    }
    // 设置停止位
    switch (stopbits) {
		case 1:
            options.c_cflag &= ~CSTOPB; 
            break;
		case 2:
            options.c_cflag |= CSTOPB; 
            break;
		default:
            fprintf(stderr, "Unsupported stop bits\n");
            return false;
    }
    
	// 修改输出模式，原始数据输出
	options.c_oflag &= ~OPOST;
    
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	// options.c_lflag &= ~(ISIG | ICANON);
    
    // 设置等待时间和最小接收字符
    options.c_cc[VTIME] = 1; /* 读取一个字符等待1*(1/10)s */
    options.c_cc[VMIN] = 1; /* 读取字符的最少个数为1 */
    // 如果发生数据溢出，接收数据，但是不再读取 刷新收到的数据但是不读
    tcflush(fd_, TCIFLUSH);
    
    // 激活配置 (将修改后的termios数据设置到串口中）  
    if (tcsetattr(fd_, TCSANOW,&options) != 0) {
		perror("com set error!\n");
		return false;
	}
    return true;
}

int SerialPort::readBuffer(uint8_t * buffer, int size) {
	return read(fd_, buffer, size);
}

int SerialPort::writeBuffer(uint8_t * buffer, int size) {
	 return write(fd_, buffer, size);
}

uint8_t SerialPort::getchar() {
	uint8_t t;
	read(fd_, &t, 1);
	return t;
}

void SerialPort::closePort()
{
	close(fd_);
    fd_ = -1;
}
