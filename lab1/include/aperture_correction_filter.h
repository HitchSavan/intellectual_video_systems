#include <vector>
#include <opencv2/opencv.hpp>

void aperture_correction(const cv::Mat &input_img, cv::Mat &output_img, int aperture_size, float percentage) {
    output_img = cv::Mat::zeros(input_img.size(), CV_8U);
    double k = 0; // коэффициент нормировки
    std::vector<std::vector<float>> Fk;

    int frameWidth = aperture_size/2;

    int index = int(100/percentage - 1) + 8;

    for (int i = 0; i < frameWidth; i++)
    {
        std::vector<float> *row = new std::vector<float>;
        for (int j = 0; j < aperture_size; j++)
        {
            row->push_back(-1);
        }
        Fk.push_back(*row);
    }

    std::vector<float> *row = new std::vector<float>;
    for (int i = 0; i < aperture_size; i++)
    {
        if (i == frameWidth)
            row->push_back(index);
        else
            row->push_back(-1);
    }
    Fk.push_back(*row);

    for (int i = frameWidth; i > 0; i--)
    {
        std::vector<float> *row = new std::vector<float>;
        for (int j = 0; j < aperture_size; j++)
        {
            row->push_back(-1);
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
                    uchar blurred = input_img.at<uchar>(j + jj,
                    i + ii);
                    Rez += Fk[ii + frameWidth][jj + frameWidth] * blurred;
                }
            uchar blurred = Rez / k; // осуществляем нормировку
            output_img.at<uchar>(j, i) = blurred;
        }
}
