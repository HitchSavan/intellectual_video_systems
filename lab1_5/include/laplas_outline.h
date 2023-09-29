#include <vector>
#include <opencv2/opencv.hpp>

#include <base_outline.h>

void laplas_outline(const cv::Mat &input_img, cv::Mat &output_img)
{
    output_img = cv::Mat::zeros(input_img.size(), CV_8U);
    std::vector<std::vector<float>> Fk;

    Fk = {
        {0, -1, 0},
        {-1, 4, -1},
        {0, -1, 0}
    };

    base_outline(input_img, output_img, Fk);
}