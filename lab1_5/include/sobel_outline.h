#include <vector>
#include <opencv2/opencv.hpp>

#include <base_outline.h>

void sobel_horizontal(const cv::Mat &input_img, cv::Mat &output_img)
{
    output_img = cv::Mat::zeros(input_img.size(), CV_8U);
    std::vector<std::vector<float>> Fk;

    Fk = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    base_outline(input_img, output_img, Fk);
}

void sobel_vertical(const cv::Mat &input_img, cv::Mat &output_img)
{
    output_img = cv::Mat::zeros(input_img.size(), CV_8U);
    std::vector<std::vector<float>> Fk;

    Fk = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    base_outline(input_img, output_img, Fk);
}

void sobel_diagonal(const cv::Mat &input_img, cv::Mat &output_img)
{
    output_img = cv::Mat::zeros(input_img.size(), CV_8U);
    std::vector<std::vector<float>> Fk;

    Fk = {
        {-2, -1, 0},
        {-1, 0, 1},
        {0, 1, 2}
    };

    base_outline(input_img, output_img, Fk);
}

void sobel_general(const cv::Mat &input_img, cv::Mat &output_img)
{
    output_img = cv::Mat::zeros(input_img.size(), CV_8U);

    int frameWidth = 3/2;
    
    for (int i = frameWidth; i < input_img.cols - frameWidth; i++)
    {
        for (int j = frameWidth; j < input_img.rows - frameWidth; j++)
        {
            uchar pix_value = input_img.at<uchar>(j, i);
            // далее производим свертку
            float Rez = abs(input_img.at<uchar>(j-1, i-1) + 2*input_img.at<uchar>(j-1, i) + input_img.at<uchar>(j-1, i+1) -
                (input_img.at<uchar>(j+1, i-1) + 2*input_img.at<uchar>(j+1, i) + input_img.at<uchar>(j, i))) +
                abs(input_img.at<uchar>(j-1, i-1) + 2*input_img.at<uchar>(j, i-1) + input_img.at<uchar>(j+1, i-1) -
                (input_img.at<uchar>(j-1, i+1) + 2*input_img.at<uchar>(j, i+1) + input_img.at<uchar>(j+1, i+1)));
            
            
            output_img.at<uchar>(j, i) = Rez;
        }
    }
}
