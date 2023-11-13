#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <vector>
#include <lab1_5/include/sobel_outline.h>

void crop_border(const cv::Mat &input_img, cv::Mat &output_img)
{
    cv::Mat bounds;
    cv::findNonZero(input_img, bounds);
    output_img = input_img(cv::boundingRect(bounds));
}

void mask_strob(const cv::Mat &input_img, cv::Mat &output_img, std::vector<std::vector<int>> &hist) {
    output_img = input_img.clone();

    for (int i = 0; i < input_img.cols; ++i)
    {
        for (int j = 0; j < input_img.rows; ++j)
        {
            if (hist[0][j] && hist[1][i])
            {
                uchar pixel_val = output_img.at<uchar>(j, i);
                output_img.at<uchar>(j, i) = pixel_val + 100 < 255 ? pixel_val + 100 : 255;
            }
        }
    }
}

void get_gradient(const cv::Mat &input_img, cv::Mat &output_img)
{
    cv::Mat outlineH;
    cv::Mat outlineV;

    cv::Mat outlineH_c;
    cv::Mat outlineV_c;
    cv::Mat output_img_c;

    // sobel_horizontal(input_img, outlineH);
    // sobel_vertical(input_img, outlineV);

    // outlineH.convertTo(outlineH_c, CV_32F);
    // outlineV.convertTo(outlineV_c, CV_32F);

    // cv::pow(outlineH_c, 2, outlineH_c);
    // cv::pow(outlineV_c, 2, outlineV_c);

    // cv::sqrt(outlineH_c + outlineV_c, output_img_c);

    // output_img_c.convertTo(output_img, CV_8U);

    sobel_general(input_img, output_img);
}

std::vector<std::vector<int>> strob(const cv::Mat &input_img, cv::Mat &strob_img, int threshold = 10) {

    std::cout << std::endl << "Strobing" << std::endl;

    std::vector<int> projectionH(input_img.rows, 0);
    std::vector<int> projectionV(input_img.cols, 0);
    
    // strob_img = input_img.clone();
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
    std::cout << std::endl;
    return {projectionH, projectionV};
}