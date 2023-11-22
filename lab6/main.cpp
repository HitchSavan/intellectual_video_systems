#include <iostream>
#include <string>
#include <unordered_map>
#include <opencv2/opencv.hpp>
#include <utils/utils.h>
#include <clusterization.hpp>

int main(int argc, char* argv[]) {

    cv::Mat src_prev;
    cv::Mat src_cur;
    cv::Mat src_prev_colour;
    cv::Mat src_cur_colour;

    if ( argc <=2 ) {
        src_prev_colour = cv::imread("p1_1.jpg");
        src_cur_colour = cv::imread("p1_2.jpg");
    } else if ( argc == 3 ) {
        src_prev_colour = cv::imread(argv[1]);
        src_cur_colour = cv::imread(argv[2]);
    }

    cv::cvtColor(src_prev_colour, src_prev, cv::COLOR_BGR2GRAY);
    cv::cvtColor(src_cur_colour, src_cur, cv::COLOR_BGR2GRAY);

    std::unordered_map<std::string, cv::Mat> output_imgs;

    output_imgs["vectors"] = cv::Mat();
    output_imgs["filteredVectors"] = cv::Mat();
    output_imgs["clusterized"] = cv::Mat();



    std::string output_folder = "output";
    std::string create_folder = "mkdir " + output_folder;
    system(create_folder.c_str());
    output_folder += "/";
    
    int confirm = 0;
    std::cout << "\nShow all? (0/1) ";
    std::cin >> confirm;

    if (confirm) {
        show_img("inputL", src_prev);
        show_img("inputR", src_cur);
        cv::waitKey(0);
    }
    std::cout << "\n";
}
