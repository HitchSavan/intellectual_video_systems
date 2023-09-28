#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
    auto image = cv::imread("kitten.jpg");
    std::cout << "it works< wow: ";
    cv::imshow("kitten", image);
    cv::waitKey(0);
}