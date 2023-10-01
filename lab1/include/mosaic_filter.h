#include <vector>
#include <opencv2/opencv.hpp>
#include <utils/utils.h>

void mosaic(const cv::Mat &input_img, cv::Mat &output_img, int aperture_size = 3) {
    std::cout << "\nMosaic filter\n";

    output_img = cv::Mat::zeros(input_img.size(), CV_8U);
    double k = 0; // коэффициент нормировки
    std::vector<std::vector<float>> Fk;

    int frameWidth = aperture_size/2;

    int index = 4;
    float power;
    for (int i = 0; i <= frameWidth; i++)
    {
        std::vector<float> *row = new std::vector<float>;
        for (int j = 0; j <= frameWidth; j++)
        {
            power = pow(index, j+i);
            row->push_back(power);
        }
        for (int j = frameWidth; j > 0; j--)
        {
            power = pow(index, j+i-1);
            row->push_back(power);
        }
        Fk.push_back(*row);
        
    }
    for (int i = frameWidth; i > 0; i--)
    {
        std::vector<float> *row = new std::vector<float>;
        for (int j = 0; j <= frameWidth; j++)
        {
            power = pow(index, j+i-1);
            row->push_back(power);
        }
        for (int j = frameWidth; j > 0; j--)
        {
            power = pow(index, j+i-2);
            row->push_back(power);
        }
        Fk.push_back(*row);
        
    }
    
    std::cout << "Aperture:" << std::endl;
    for (auto i: Fk)
    {
        for (auto j: i)
        {
            k += j;
            std::cout << int(j) << " ";
        }
        std::cout << std::endl;
    }


    for (int i = frameWidth; i < input_img.cols - frameWidth; i += aperture_size)
    {
        for (int j = frameWidth; j < input_img.rows - frameWidth; j += aperture_size)
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
                    
            uchar blurred = Rez / k; // осуществляем нормировку

            for (int ii = -frameWidth; ii <= frameWidth; ii++)
                for (int jj = -frameWidth; jj <= frameWidth; jj++)
                {
                    output_img.at<uchar>(j + jj, i + ii) = blurred;
                }
        }
        if (( ( i + 1 ) % ( (input_img.cols - frameWidth) / 10 ) ) == 0)
            progressbar(input_img.cols - frameWidth, i);
    }
}
