#include <opencv2/opencv.hpp>

void gauss3(const cv::Mat &input_img, cv::Mat &output_img) {
    output_img = cv::Mat::zeros(input_img.size(), CV_8U);
    float k = 36; // коэффициент нормировки
    float Fk[3][3] = {
        {1,4,1},
        {4,16,4},
        {1,4,1}
    }; // маска фильтра
    for (int i = 1; i < input_img.cols - 1; i++)
        for (int j = 1; j < input_img.rows - 1; j++) {
            uchar pix_value = input_img.at<uchar>(j, i);
            // далее производим свертку
            float Rez = 0;
            for (int ii = -1; ii <= 1; ii++)
                for (int jj = -1; jj <= 1; jj++) {
                    uchar blurred = input_img.at<uchar>(j + jj,
                    i + ii);
                    Rez += Fk[ii + 1][jj + 1] * blurred;
                }
            uchar blurred = Rez / k; // осуществляем нормировку
            output_img.at<uchar>(j, i) = blurred;
        }
}
