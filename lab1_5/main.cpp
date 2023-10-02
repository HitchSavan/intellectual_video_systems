#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <sobel_outline.h>
#include <laplas_outline.h>
#include <lab1/include/gauss_filter.h>


int main(int argc, char** argv) {
    std::string filename = "../kitten.png";
    std::string output_folder = "output";

    std::cout << "Input outliner id for showing only one pic (1-10, 0 for skip): ";

    int outlineId = 1;
    std::cin >> outlineId;

    cv::Mat image = cv::imread(filename, cv::IMREAD_GRAYSCALE);

    cv::Mat gauss3_img;
    cv::Mat gauss5_img;
    cv::Mat gauss_diff;
    gauss(image, gauss3_img, 3);
    gauss(image, gauss5_img, 5);
    cv::absdiff(gauss3_img, gauss5_img, gauss_diff);

    cv::Mat canny_img;
    float thr1 = 50;
    float thr2 = 200;
    cv::Canny(image, canny_img, thr1, thr2);

    switch (outlineId)
    {
        case 1:
        {
            cv::Mat sobel_horizontal_img;
            sobel_horizontal(image, sobel_horizontal_img);
            show_img("horizontal sobel output "+filename, sobel_horizontal_img);
            break;
        }
        case 2:
        {
            cv::Mat sobel_vertical_img;
            sobel_vertical(image, sobel_vertical_img);
            show_img("vertical sobel output "+filename, sobel_vertical_img);
            break;
        }
        case 3:
        {
            cv::Mat sobel_diagonal_img;
            sobel_diagonal(image, sobel_diagonal_img);
            show_img("diagonal sobel output "+filename, sobel_diagonal_img);
            break;
        }
        case 4:
        {
            cv::Mat sobel_general_img;
            sobel_general(image, sobel_general_img);
            show_img("general sobel output "+filename, sobel_general_img);
            break;
        }
        case 5:
        {
            break;
        }
        case 6:
        {
            break;
        }
        case 7:
        {
            break;
        }
        case 8:
        {
            break;
        }
        case 9:
        {
            break;
        }
        case 10:
        {
            cv::Mat laplas_img;
            laplas_outline(image, laplas_img);
            show_img("laplas outline output "+filename, laplas_img);
            break;
        }
        
        default:
            break;
    }

    int confirm = 0;
    std::cout << "\nSave all? (0/1) ";
    std::cin >> confirm;

    if (confirm)
    {
        std::cout << "\nShow all? (0/1) ";
        std::cin >> confirm;

        std::string create_folder = "mkdir " + output_folder;
        system(create_folder.c_str());
        output_folder += "/";

        cv::imwrite(output_folder + "gauss_diff.jpg", gauss_diff*7);

        cv::imwrite(output_folder + "canny_img.jpg", canny_img);
        
        cv::Mat sobel_horizontal_img;
        sobel_horizontal(image, sobel_horizontal_img);
        cv::imwrite(output_folder + "sobel_horizontal_img.jpg", sobel_horizontal_img);

        cv::Mat sobel_vertical_img;
        sobel_vertical(image, sobel_vertical_img);
        cv::imwrite(output_folder + "sobel_vertical_img.jpg", sobel_vertical_img);

        cv::Mat sobel_diagonal_img;
        sobel_diagonal(image, sobel_diagonal_img);
        cv::imwrite(output_folder + "sobel_diagonal_img.jpg", sobel_diagonal_img);

        cv::Mat sobel_general_img;
        sobel_general(image, sobel_general_img);
        cv::imwrite(output_folder + "sobel_general_img.jpg", sobel_general_img);

        cv::Mat laplas_img;
        laplas_outline(image, laplas_img);
        cv::imwrite(output_folder + "laplas_img.jpg", laplas_img);
        if (confirm)
        {
            show_img("gauss dif "+filename, gauss_diff);
            show_img("canny  "+filename, canny_img);
            show_img("horizontal sobel output "+filename, sobel_horizontal_img);
            show_img("vertical sobel output "+filename, sobel_vertical_img);
            show_img("diagonal sobel output "+filename, sobel_diagonal_img);
            show_img("general sobel output "+filename, sobel_general_img);
            show_img("laplas outline output "+filename, laplas_img);
        }
    }
    std::cout << "\n";

    show_img("input "+filename, image);
    if (confirm != 0)
        cv::waitKey(0);
}