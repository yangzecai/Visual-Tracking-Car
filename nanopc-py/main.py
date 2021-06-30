import signal
from Camera import *
from Car import *

stopSignal = False

def stopHandler(signum, frame) :
    global stopSignal
    stopSignal = True

if __name__ == "__main__" :
    
    signal.signal(signal.SIGINT, stopHandler)
    signal.signal(signal.SIGTERM, stopHandler)

    camera = Camera(0)
    car = Car("/dev/ttySAC2", 115200)
    while camera.isOpen() and not stopSignal :
        camera.takeGrayImage()
        camera.processImage()
        # camera.drawErrorLine()    # for debug
        # camera.showImage()        # for debug

        if camera.isNormal() :
            tranSpeed = 100
            rotaSpeed = round(camera.getError() * 0.5)
            car.move(tranSpeed, rotaSpeed)
        else :
            car.stop()
