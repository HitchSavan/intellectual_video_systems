#include <vector>
#include <opencv2/opencv.hpp>

void erosion(const cv::Mat &input_img, cv::Mat &output_img, int aperture_size = 3)
{
    output_img = cv::Mat::zeros(input_img.size(), CV_8U);
    int frameWidth = aperture_size/2;

    uchar min = 255;
    
    for ( int i = frameWidth; i < input_img.rows - frameWidth; i++ )
    {
        for ( int j = frameWidth; j < input_img.cols - frameWidth; j++ )
        {
            min = 255;

            for ( int ii = -frameWidth; ii <= frameWidth; ii++ )
            {
                for ( int jj = -frameWidth; jj <= frameWidth; jj++ )
                {
                    if ( input_img.at<uchar>(i+ii, j+jj) < min )
                        min = input_img.at<uchar>(i+ii, j+jj);
                }
            }

            output_img.at<uchar>(i, j) = min;
        }
    }
}

void dilation(const cv::Mat &input_img, cv::Mat &output_img, int aperture_size = 3)
{
    output_img = cv::Mat::zeros(input_img.size(), CV_8U);
    int frameWidth = aperture_size/2;

    uchar max = 0;
    
    for ( int i = frameWidth; i < input_img.rows - frameWidth; i++ )
    {
        for ( int j = frameWidth; j < input_img.cols - frameWidth; j++ )
        {
            max = 0;

            for ( int ii = -frameWidth; ii <= frameWidth; ii++ )
            {
                for ( int jj = -frameWidth; jj <= frameWidth; jj++ )
                {
                    if ( input_img.at<uchar>(i+ii, j+jj) > max )
                        max = input_img.at<uchar>(i+ii, j+jj);
                }
            }

            output_img.at<uchar>(i, j) = max;
        }
    }
}


void opening(const cv::Mat &input_img, cv::Mat &output_img, int aperture_size = 3)
{
    cv::Mat tmp_img;
    
    erosion(input_img, tmp_img, aperture_size);
    dilation(tmp_img, output_img, aperture_size);
}

void closing(const cv::Mat &input_img, cv::Mat &output_img, int aperture_size = 3)
{
    cv::Mat tmp_img;
    
    dilation(input_img, tmp_img, aperture_size);
    erosion(tmp_img, output_img, aperture_size);
}

void outline(const cv::Mat &input_img, cv::Mat &output_img, int aperture_size = 3)
{
    cv::Mat tmp_img1;
    cv::Mat tmp_img2;
    
    dilation(input_img, tmp_img1, aperture_size);
    erosion(input_img, tmp_img2, aperture_size);

    cv::absdiff(tmp_img1, tmp_img2, output_img);
}

void gradient(const cv::Mat &input_img, cv::Mat &output_img)
{
    cv::Mat tmp_img1;
    cv::Mat tmp_img2;
    cv::Mat tmp_img3;
    cv::Mat tmp_img1_c;
    cv::Mat tmp_img2_c;
    cv::Mat tmp_img3_c;
    cv::Mat output_img_c;
    
    outline(input_img, tmp_img1, 3);
    outline(input_img, tmp_img2, 5);
    outline(input_img, tmp_img3, 7);

    tmp_img1.convertTo(tmp_img1_c, CV_32F);
    tmp_img2.convertTo(tmp_img2_c, CV_32F);
    tmp_img3.convertTo(tmp_img3_c, CV_32F);

    output_img_c = (tmp_img1_c + tmp_img2_c + tmp_img3_c) / 3;

    output_img_c.convertTo(output_img, CV_8U);
}