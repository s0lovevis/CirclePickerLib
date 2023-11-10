#include <iostream>
#include <circle_picker.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>

int main() {
    cv::Mat img = cv::imread("..\\test_images\\case6.png");
    circlePicker picker(img);
    picker.pickMode();
    cv::imshow("window", picker.getCanvas());
    cv::waitKey(0);
}