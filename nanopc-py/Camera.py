import cv2
import numpy as np

class Camera :
    def __init__(self, cameraNum) :
        self.__capture = cv2.VideoCapture(cameraNum)
        self.__image = np.ndarray(1)
        self.__error = 0
        self.__isNormal = False
        self.__capture.set(cv2.CAP_PROP_FRAME_WIDTH, 320)
        self.__capture.set(cv2.CAP_PROP_FRAME_HEIGHT, 240)
    
    def __del__(self) :
        cv2.destroyAllWindows()
        self.__capture.release()
    
    def takeGrayImage(self) :
        self.__takeImage()
        self.__image = cv2.cvtColor(self.__image,
                                    cv2.COLOR_BGR2GRAY)
    
    def processImage(self) :
        self.__image = cv2.threshold(self.__image, 0, 255, 
                        cv2.THRESH_BINARY_INV|cv2.THRESH_OTSU)[1]
        sum = 1     # prevent divide by zero
        weight = 0
        pixels = self.getRowPixels(self.getImageHeight()-1)
        for i in range(self.getImageWidth()) :
            sum += pixels[i]
            weight += pixels[i] * i

        if weight != 0 :
            self.__error = weight // sum - self.getImageWidth() // 2
            self.__isNormal = True
        else :
            self.__error = 0
            self.__isNormal = False

    def isNormal(self) :
        return self.__isNormal

    def getImage(self) :
        return self.__image

    def isOpen(self) :
        return self.__capture.isOpened()

    def getRowPixels(self, row) :
        return self.__image[row]

    def getImageWidth(self) :
        return len(self.__image[0])
    
    def getImageHeight(self) :
        return len(self.__image)

    def getError(self) :
        return self.__error

    def showImage(self) :   # for debug
        cv2.imshow("Image", self.__image)
        cv2.waitKey(30)

    def drawErrorLine(self) :   # for debug
        startPoint = (self.__error + self.getImageWidth() // 2, 
                      self.getImageHeight()-1)
        endPoint = (self.__error + self.getImageWidth() // 2, 0)
        color = 120
        cv2.line(self.__image, startPoint, endPoint, color)
    
    def __takeImage(self) :
        self.__image = self.__capture.read()[1]
    