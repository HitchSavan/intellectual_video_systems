#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <vector>
#include <random>
#include <cstdlib>
#include <lab1_5/include/sobel_outline.h>
#include <lab2/include/morphological_operations.h>

void crop_border(const cv::Mat &input_img, cv::Mat &output_img)
{
    cv::Mat bounds;
    cv::findNonZero(input_img, bounds);
    output_img = input_img(cv::boundingRect(bounds));
}

void mask_strob_object(const cv::Mat &input_img, cv::Mat &output_img, std::vector<std::vector<int>> &hist) {
    output_img = input_img.clone();
    std::vector<int> color;
    for (int i = 0; i < 3; ++i)
    {
        color.push_back(55 + std::rand()%200);
    }

    bool top_left = false;
    std::pair<int, int> p1 = {0, 0};
    std::pair<int, int> p2 = {0, 0};

    int diagonal_pos = 0;

    for (int i = 1; i < input_img.cols; ++i)
    {
        for (int j = 1; j < input_img.rows; ++j)
        {
            diagonal_pos = input_img.cols + j - i;
            
            if (hist[0][j] && hist[1][i])
            {
                if (!top_left)
                {
                    p1 = {i, j};
                    top_left = true;
                }

                if (input_img.type() == CV_8UC3)
                {
                    cv::Vec3b pixel_val = input_img.at<cv::Vec3b>(j, i);
                    for (int k = 0; k < pixel_val.channels; ++k)
                        output_img.at<cv::Vec3b>(j, i)[k] = pixel_val[k] + color[k] < 255 ? pixel_val[k] + color[k] : 255;
                }
                else
                {
                    uchar pixel_val = input_img.at<uchar>(j, i);
                    output_img.at<uchar>(j, i) = pixel_val + color[0] < 255 ? pixel_val + color[0] : 255;
                }
                p2 = {i, j};
            }

            if (((hist[0][j-1] && !hist[0][j]) && (hist[1][i-1] && !hist[1][i])))
            {
                cv::rectangle(output_img, cv::Point(p1.first, p1.second), cv::Point(p2.first, p2.second), cv::Scalar(color[0], color[1], color[2]), 2);
                
                for (int i = 0; i < 3; ++i)
                {
                    color[i] = 55 + std::rand()%200;
                }
                top_left = false;
            }
        }
    }
}

void get_gradient(const cv::Mat &input_img, cv::Mat &output_img)
{
    sobel_general(input_img, output_img);
}

std::vector<std::vector<int>> strob(const cv::Mat &input_img, cv::Mat &strob_img, int threshold = 10) {

    std::cout << std::endl << "Strobing" << std::endl;

    std::vector<int> projectionH(input_img.rows, 0);
    std::vector<int> projectionV(input_img.cols, 0);

    int diagonal_axis = input_img.cols + input_img.rows + 1;
    std::vector<int> projectionD(diagonal_axis, 0);

    cv::copyMakeBorder(input_img, strob_img, input_img.rows*1.1, input_img.rows*1.1, input_img.cols*1.1, input_img.cols*1.1, cv::BORDER_CONSTANT, cv::Scalar(0));

    int hist_row = strob_img.rows - 1;
    int hist_col = strob_img.cols - 1;

    for (int i = 0; i < input_img.cols; ++i)
    {
        hist_row = strob_img.rows - 1;

        for (int j = 0; j < input_img.rows; ++j)
        {
            uchar pix_value = input_img.at<uchar>(j, i);

            if (pix_value)
            {
                strob_img.at<uchar>(hist_row--, i + strob_img.cols/2 - input_img.cols/2) = 255;

                hist_col = strob_img.cols - 1;
                while(strob_img.at<uchar>(j + strob_img.rows/2 - input_img.rows/2, hist_col))
                {
                    hist_col--;
                }

                strob_img.at<uchar>(j + strob_img.rows/2 - input_img.rows/2, hist_col--) = 255;
                
                ++projectionH[j];
                ++projectionV[i];
                ++projectionD[input_img.cols + j - i];
            }
        }
        if (( ( i + 1 ) % ( (input_img.cols) / 10 ) ) == 0)
            progressbar(input_img.cols, i);
    }

    for (int i = 0; i < strob_img.cols; ++i)
    {
        for (int j = 0; j < strob_img.rows; ++j)
        {
            strob_img.at<uchar>(strob_img.rows - 1 - threshold, i) = 200;
            strob_img.at<uchar>(j, strob_img.cols - 1 - threshold) = 200;
        }
    }

    for (int i = 0; i < projectionH.size(); ++i)
    {
        if (projectionH[i] < threshold)
            projectionH[i] = 0;
    }
    for (int i = 0; i < projectionV.size(); ++i)
    {
        if (projectionV[i] < threshold)
            projectionV[i] = 0;
    }
    for (int i = 1; i < projectionD.size() - 1; ++i)
    {
        if (projectionD[i] < threshold)
            projectionD[i] = 0;
    }
    std::cout << std::endl;
    return {projectionH, projectionV, projectionD};
}

void filter(const cv::Mat &input_img, cv::Mat &out_image, int closing_mask_size = 105)
{
    cv::Mat input_border_img;
    cv::Mat out_open;

    cv::copyMakeBorder(input_img, input_border_img, closing_mask_size, closing_mask_size, closing_mask_size, closing_mask_size, cv::BORDER_CONSTANT, cv::Scalar(0));

    opening(input_border_img, out_open, 3, CROSS);
    closing(out_open, out_image, closing_mask_size);

    out_image = out_image(cv::Range(closing_mask_size, out_image.rows - closing_mask_size), cv::Range(closing_mask_size, out_image.cols - closing_mask_size));
}