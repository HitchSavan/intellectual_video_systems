# pragma once

#include <vector>
#include <opencv2/opencv.hpp>
#include <utils/utils.h>

void base_outline(const cv::Mat &input_img, cv::Mat &output_img, std::vector<std::vector<float>> &Fk)
{
    int frameWidth = Fk.size()/2;
    
    std::cout << "Aperture:" << std::endl;
    for (auto i: Fk)
    {
        for (auto j: i)
        {
            std::cout << int(j) << " ";
        }
        std::cout << std::endl;
    }

    for (int i = frameWidth; i < input_img.cols - frameWidth; i++)
    {
        for (int j = frameWidth; j < input_img.rows - frameWidth; j++)
        {
            uchar pix_value = input_img.at<uchar>(j, i);
            // далее производим свертку
            float Rez = 0;
            for (int ii = -frameWidth; ii <= frameWidth; ii++)
                for (int jj = -frameWidth; jj <= frameWidth; jj++)
                {
                    uchar blurred = input_img.at<uchar>(j + jj, i + ii);
                    Rez += Fk[ii + frameWidth][jj + frameWidth] * blurred;
                }
            output_img.at<uchar>(j, i) = Rez;
        }
        if (( ( i + 1 ) % ( (input_img.cols - frameWidth) / 10 ) ) == 0)
            progressbar(input_img.cols - frameWidth, i);
    }
}