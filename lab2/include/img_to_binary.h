#include <opencv2/opencv.hpp>

void get_binary(const cv::Mat &input_img, cv::Mat &output_img, int threshold = 255/2)
{
    output_img = cv::Mat::zeros(input_img.size(), CV_8U);

    for ( int i = 0; i < input_img.rows; i++ )
    {
        for ( int j = 0; j < input_img.cols; j++ )
        {
            uchar pixel = input_img.at<uchar>(i, j);
            output_img.at<uchar>(i, j) = pixel < threshold ? 0 : 255;
        }
    }
}