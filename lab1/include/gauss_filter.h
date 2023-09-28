#include <vector>
#include <opencv2/opencv.hpp>

void gauss(const cv::Mat &input_img, cv::Mat &output_img, int aperture_size) {
    output_img = cv::Mat::zeros(input_img.size(), CV_8U);
    double k = 0; // коэффициент нормировки
    std::vector<std::vector<float>> Fk;

    int frameWidth = aperture_size/2;

    int index = 4;
    for (int i = 0; i <= frameWidth; i++)
    {
        std::vector<float> *row = new std::vector<float>;
        for (int j = 0; j <= frameWidth; j++)
        {
            row->push_back(pow(index, j+i));
        }
        for (int j = frameWidth; j > 0; j--)
        {
            row->push_back(pow(index, j+i-1));
        }
        Fk.push_back(*row);
        
    }
    for (int i = frameWidth; i > 0; i--)
    {
        std::vector<float> *row = new std::vector<float>;
        for (int j = 0; j <= frameWidth; j++)
        {
            row->push_back(pow(index, j+i-1));
        }
        for (int j = frameWidth; j > 0; j--)
        {
            row->push_back(pow(index, j+i-2));
        }
        Fk.push_back(*row);
        
    }

    std::cout << "Aperture:" << std::endl;
    for (auto i: Fk)
    {
        for (auto j: i)
        {
            k += j;
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }


    for (int i = frameWidth; i < input_img.cols - frameWidth; i++)
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
            uchar blurred = Rez / k; // осуществляем нормировку
            output_img.at<uchar>(j, i) = blurred;
        }
}
