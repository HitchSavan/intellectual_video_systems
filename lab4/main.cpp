#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <utils/utils.h>
#include <lab2/include/img_to_binary.h>
#include <gradient_and_strobing.h>

int main(int argc, char* argv[]) {

    // TODO: implement config loading

    cv::Mat src_left;
    cv::Mat src_right;
    cv::Mat src_left_colour;
    cv::Mat src_right_colour;

    if ( argc <=2 ) {
        src_left_colour = cv::imread("p3_1.jpg");
        src_right_colour = cv::imread("p3_2.jpg");
    } else {
        src_left_colour = cv::imread(argv[1]);
        src_right_colour = cv::imread(argv[2]);
    }

    cv::cvtColor(src_left_colour, src_left, cv::COLOR_BGR2GRAY);
    cv::cvtColor(src_right_colour, src_right, cv::COLOR_BGR2GRAY);

    cv::Mat out_MAD;
    cv::Mat out_MAD_binary;

    cv::Mat out_gradientR;
    cv::Mat out_gradientR_binary;
    
    cv::Mat out_concat;
    cv::Mat out_concat_binary;

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

    int closing_mask_size = 105;
    filter(out_concat, out_filtered, closing_mask_size);

    crop_border(out_filtered, out_cropped);

    int strob_threshould = 20;

    strob(out_cropped, out_histogram_crop, strob_threshould);
    std::vector<std::vector<int>> weights = strob(out_filtered, out_histogram, strob_threshould);
    mask_strob_object(src_right_colour, out_masked, weights);


    std::string output_folder = "output";
    std::string create_folder = "mkdir " + output_folder;
    system(create_folder.c_str());
    output_folder += "/";

    cv::imwrite(output_folder + "MAD.jpg", 255-out_MAD);
    cv::imwrite(output_folder + "MAD_binary.jpg", 255-out_MAD_binary);
    cv::imwrite(output_folder + " gradientR.jpg", 255-out_gradientR);
    cv::imwrite(output_folder + " gradientR_binary.jpg", 255-out_gradientR_binary);
    cv::imwrite(output_folder + "concatinated.jpg", 255-out_concat);
    cv::imwrite(output_folder + "filtered.jpg", 255-out_filtered);
    cv::imwrite(output_folder + "cropped.jpg", 255-out_cropped);
    cv::imwrite(output_folder + "histogram.jpg", 255-out_histogram);
    cv::imwrite(output_folder + "histogram_crop.jpg", 255-out_histogram_crop);
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
        show_img("filtered", out_filtered);
        show_img("cropped", out_cropped);
        show_img("histogram", out_histogram);
        show_img("histogram_crop", out_histogram_crop);
        show_img("masked", out_masked);
        cv::waitKey(0);
    }
    std::cout << "\n";
}
