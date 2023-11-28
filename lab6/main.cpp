#include <iostream>
#include <string>
#include <unordered_map>
#include <opencv2/opencv.hpp>
#include <utils/utils.h>
#include <clusterization.hpp>

int main(int argc, char* argv[]) {

    int blockSize = 5;

    cv::Mat src_prev_colour;
    cv::Mat src_cur_colour;

    for (int iShift = 0; iShift < 360; ++iShift)
    {    
        std::cout << "Shift " << iShift << std::endl;
        for (int i = 45; i < 360; i += 45)
        {
            lab6::MoveVector vect1(cv::Point(0, 0), cv::Point(1 + 100 * cos((i + iShift) * 3.14159265 / 180), 1 + 100 * sin((i + iShift) * 3.14159265 / 180)));
            lab6::MoveVector vect2(cv::Point(0, 0), cv::Point(1 + 100 * cos((i - 45 + iShift) * 3.14159265 / 180), 1 + 100 * sin((i - 45 + iShift) * 3.14159265 / 180)));
            if (vect2.coords.x != 0 && vect2.coords.y != 0)
                std::cout << vect1.coords.x / vect2.coords.x << "\t" << vect1.coords.y / vect2.coords.y << std::endl;
        }
    }

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

    getVectorsImg(src_cur_colour, src_prev_colour, blockSize, output_imgs);

    std::cout << std::endl;
    std::string output_folder = "output";
    std::string create_folder = "mkdir " + output_folder;
    system(create_folder.c_str());
    output_folder += "/";

    cv::imwrite(output_folder + "vectors.jpg", output_imgs["vectors"]);
    cv::imwrite(output_folder + "filteredVectors.jpg", output_imgs["filteredVectors"]);
    cv::imwrite(output_folder + "clusterized.jpg", output_imgs["clusterized"]);
    
    int confirm = 0;
    std::cout << "\nShow all? (0/1) ";
    std::cin >> confirm;

    if (confirm) {
        show_img("inputPrev", src_prev_colour);
        show_img("inputCur", src_cur_colour);
        show_img("vectors", output_imgs["vectors"]);
        show_img("filteredVectors", output_imgs["filteredVectors"]);
        show_img("clusterized", output_imgs["clusterized"]);
        cv::waitKey(0);
    }
    std::cout << "\n";
}
