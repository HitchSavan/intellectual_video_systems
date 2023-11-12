#include <vector>
#include <numeric>
#include <opencv2/opencv.hpp>
#include <utils/utils.h>

void median_f(const cv::Mat &input_img, cv::Mat &output_img, int aperture_size = 3) {
    std::cout << "\nMedian filter\n";

    output_img = cv::Mat::zeros(input_img.size(), CV_8U);
    std::vector<float> Fk;
    
    int frameWidth = aperture_size/2;

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
                    Fk.push_back(input_img.at<uchar>(j + jj, i + ii));
                }
            std::sort(Fk.begin(), Fk.end());
            uchar blurred = Fk[aperture_size*aperture_size/2]; // осуществляем нормировку
            Fk.clear();
            output_img.at<uchar>(j, i) = blurred;
        }
        if (( ( i + 1 ) % ( (input_img.cols - frameWidth) / 10 ) ) == 0)
            progressbar(input_img.cols - frameWidth, i);
    }
}
