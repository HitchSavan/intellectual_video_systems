#include <iostream>
#include <string>
#include <unordered_map>
#include <opencv2/opencv.hpp>
#include <utils/utils.h>
#include <clusterization.hpp>

int main(int argc, char* argv[]) {

    cv::Mat src_prev_colour;
    cv::Mat src_cur_colour;

    if ( argc <=2 ) {
        src_prev_colour = cv::imread("p1_1.jpg");
        src_cur_colour = cv::imread("p1_2.jpg");
    } else if ( argc == 3 ) {
        src_prev_colour = cv::imread(argv[1]);
        src_cur_colour = cv::imread(argv[2]);
    }

    std::unordered_map<std::string, cv::Mat> output_imgs;

    output_imgs["vectors"] = src_cur_colour.clone();
    output_imgs["filteredVectors"] = src_cur_colour.clone();
    output_imgs["clusterized"] = src_cur_colour.clone();

    getVectorsImg(src_prev_colour, src_cur_colour, 15, output_imgs);


    std::string output_folder = "output";
    std::string create_folder = "mkdir " + output_folder;
    system(create_folder.c_str());
    output_folder += "/";

    cv::imwrite(output_folder + "vectors.jpg", output_imgs["vectors"]);
    
    int confirm = 0;
    std::cout << "\nShow all? (0/1) ";
    std::cin >> confirm;

    if (confirm) {
        show_img("inputL", src_prev_colour);
        show_img("inputR", src_cur_colour);
        show_img("vectors", output_imgs["vectors"]);
        cv::waitKey(0);
    }
    std::cout << "\n";
}
