#include <iostream>
#include <opencv2/opencv.hpp>

int main() 
{
    cv::VideoCapture captrue(0);

    cv::Mat frame;
    captrue >> frame;
    cv::imshow("test", frame);

    return 0;
}