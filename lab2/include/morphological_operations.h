#include <vector>
#include <opencv2/opencv.hpp>
#include <utils/utils.h>

enum Shape {
    SQUARE,
    CROSS,
    CIRCLE
};

void apply_shape(cv::Mat &mask, int aperture_size, Shape shape)
{
    if (!(aperture_size % 2))
        aperture_size++;

    mask = cv::Mat::zeros(cv::Size(aperture_size, aperture_size), CV_8U);

    switch(shape)
    {
        case CROSS:
            for (int i = 0; i < aperture_size; i++)
            {
                mask.at<uchar>(i, aperture_size/2) = 255;
                mask.at<uchar>(aperture_size/2, i) = 255;
            }
            break;

        case CIRCLE:
            cv::circle(mask, cv::Point(aperture_size/2, aperture_size/2), aperture_size/2, cv::Scalar(255), cv::FILLED);
            break;

        default:
            mask = cv::Mat::ones(cv::Size(aperture_size, aperture_size), CV_8U);
    }
}

void erosion(const cv::Mat &input_img, cv::Mat &output_img, int aperture_size = 3, Shape shape = SQUARE)
{
    std::cout << std::endl << "\tErosion" << std::endl;
    output_img = cv::Mat::zeros(input_img.size(), CV_8U);
    int frameWidth = aperture_size/2;

    cv::Mat mask;
    apply_shape(mask, aperture_size, shape);

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
                    if ( input_img.at<uchar>(i+ii, j+jj) < min && mask.at<uchar>(ii+frameWidth, jj+frameWidth) != 0 )
                        min = input_img.at<uchar>(i+ii, j+jj);
                }
            }

            output_img.at<uchar>(i, j) = min;
        }
        if (( ( i + 1 ) % ( (input_img.rows - frameWidth) / 10 ) ) == 0)
            progressbar(input_img.rows - frameWidth, i);
    }
}

void dilation(const cv::Mat &input_img, cv::Mat &output_img, int aperture_size = 3, Shape shape = SQUARE)
{
    std::cout << std::endl << "\tDilation" << std::endl;

    output_img = cv::Mat::zeros(input_img.size(), CV_8U);
    int frameWidth = aperture_size/2;

    cv::Mat mask;
    apply_shape(mask, aperture_size, shape);

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
                    if ( input_img.at<uchar>(i+ii, j+jj) > max && mask.at<uchar>(ii+frameWidth, jj+frameWidth) != 0 )
                        max = input_img.at<uchar>(i+ii, j+jj);
                }
            }

            output_img.at<uchar>(i, j) = max;
        }
        if (( ( i + 1 ) % ( (input_img.rows - frameWidth) / 10 ) ) == 0)
            progressbar(input_img.rows - frameWidth, i);
    }
}


void opening(const cv::Mat &input_img, cv::Mat &output_img, int aperture_size = 3, Shape shape = SQUARE)
{
    std::cout << std::endl << "Opening";
    std::cout << "\t mask size " << aperture_size;
    cv::Mat tmp_img;
    
    erosion(input_img, tmp_img, aperture_size, shape);
    dilation(tmp_img, output_img, aperture_size, shape);
}

void closing(const cv::Mat &input_img, cv::Mat &output_img, int aperture_size = 3, Shape shape = SQUARE)
{
    std::cout << std::endl << "Closing";
    std::cout << "\t mask size " << aperture_size;
    cv::Mat tmp_img;
    
    dilation(input_img, tmp_img, aperture_size, shape);
    erosion(tmp_img, output_img, aperture_size, shape);
}

void outline(const cv::Mat &input_img, cv::Mat &output_img, int aperture_size = 3, Shape shape = SQUARE)
{
    cv::Mat tmp_img1;
    cv::Mat tmp_img2;
    
    dilation(input_img, tmp_img1, aperture_size, shape);
    erosion(input_img, tmp_img2, aperture_size, shape);

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