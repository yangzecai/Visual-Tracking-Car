import serial

class Car :
    def __init__(self, dev, baudRate) :
        self.__serial = serial.Serial(dev, baudRate)
        self.__kPacketSize = 6
        self.__packet = bytearray(self.__kPacketSize)
        self.__tranSpeed = 0
        self.__rotaSpeed = 0

    def __del__(self) :
        self.stop()
        self.__serial.close()

    def move(self, tranSpeed, rotaSpeed) :
        self.__tranSpeed = tranSpeed
        self.__rotaSpeed = rotaSpeed
        self.__updatePacket()
        self.__sendPacket()

    def stop(self) :
        self.move(0, 0)

    def __setPacketHead(self) :
        self.__packet[0] = 0xAA

    def __setPacketData(self) :
        self.__packet[1] = self.__tranSpeed & 0xFF
        self.__packet[2] = self.__tranSpeed >> 8 & 0xFF
        self.__packet[3] = self.__rotaSpeed & 0xFF
        self.__packet[4] = self.__rotaSpeed >> 8 & 0xFF

    def __setPacketCheck(self) :
        sum = 0
        for i in range(self.__kPacketSize-1) :
            sum += self.__packet[i]
        self.__packet[self.__kPacketSize-1] = sum & 0xFF
    
    def __updatePacket(self) :
        self.__setPacketHead()
        self.__setPacketData()
        self.__setPacketCheck()

    def __sendPacket(self) :
        self.__serial.write(self.__packet)
