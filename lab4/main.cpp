#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <utils/utils.h>
#include <lab2/include/img_to_binary.h>
#include <lab2/include/morphological_operations.h>
#include <gradient_and_strobing.h>

int main(int argc, char* argv[]) {

    // TODO: implement config loading

    cv::Mat src_left;
    cv::Mat src_right;

    if ( argc <=2 ) {
        src_left = cv::imread("p3_1.jpg", cv::IMREAD_GRAYSCALE);
        src_right = cv::imread("p3_2.jpg", cv::IMREAD_GRAYSCALE);
    } else {
        src_left = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
        src_right = cv::imread(argv[2], cv::IMREAD_GRAYSCALE);
    }

    cv::Mat out_MAD;
    cv::Mat out_MAD_binary;

    cv::Mat out_gradientR;
    cv::Mat out_gradientR_binary;
    
    cv::Mat out_concat;
    cv::Mat out_concat_binary;

    cv::Mat out_open;
    cv::Mat out_filtered;
    cv::Mat out_cropped;
    cv::Mat out_histogram;
    cv::Mat out_histogram_crop;
    cv::Mat out_masked;
    
    cv::absdiff(src_right, src_left, out_MAD);

    get_gradient(src_right, out_gradientR);

    get_binary(out_MAD, out_MAD_binary, 10);
    get_binary(out_gradientR, out_gradientR_binary, 20);

    cv::bitwise_and(out_MAD_binary, out_gradientR_binary, out_concat);
    // cv::bitwise_and(out_MAD, out_gradientR, out_concat);

    opening(out_concat, out_open, 3, CROSS);
    closing(out_open, out_filtered, 105);

    crop_border(out_filtered, out_cropped);

    int strob_threshould = 50;

    strob(out_cropped, out_histogram_crop, strob_threshould);
    std::vector<std::vector<int>> weights = strob(out_filtered, out_histogram, strob_threshould);
    mask_strob(src_right, out_masked, weights);


    std::string output_folder = "output";
    std::string create_folder = "mkdir " + output_folder;
    system(create_folder.c_str());
    output_folder += "/";

    cv::imwrite(output_folder + "MAD.jpg", out_MAD);
    cv::imwrite(output_folder + "MAD_binary.jpg", out_MAD_binary);
    cv::imwrite(output_folder + " gradientR.jpg", out_gradientR);
    cv::imwrite(output_folder + " gradientR_binary.jpg", out_gradientR_binary);
    cv::imwrite(output_folder + "concatinated.jpg", out_concat);
    cv::imwrite(output_folder + "opening.jpg", out_open);
    cv::imwrite(output_folder + "filtered.jpg", out_filtered);
    cv::imwrite(output_folder + "cropped.jpg", out_cropped);
    cv::imwrite(output_folder + "histogram.jpg", out_histogram);
    cv::imwrite(output_folder + "histogram_crop.jpg", out_histogram_crop);
    cv::imwrite(output_folder + "masked.jpg", out_masked);
    
    int confirm = 0;
    std::cout << "\nShow all? (0/1) ";
    std::cin >> confirm;

    if (confirm) {
        show_img("inputL", src_left);
        show_img("inputR", src_right);
        show_img("MAD", out_MAD);
        show_img(" gradientR", out_gradientR);
        show_img("concatinated", out_concat);
        show_img("opening", out_open);
        show_img("filtered", out_filtered);
        show_img("cropped", out_cropped);
        show_img("histogram", out_histogram);
        show_img("histogram_crop", out_histogram_crop);
        show_img("masked", out_masked);
        cv::waitKey(0);
    }
    std::cout << "\n";
}
