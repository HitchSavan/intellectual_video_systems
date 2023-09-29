#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <gauss_filter.h>
#include <mosaic_filter.h>
#include <median_filter.h>
#include <aperture_correction_filter.h>

int main(int argc, char** argv) {
    std::string filename = "../kitten.png";
    std::string output_folder = "output";

    int gauss_aperture_size = 5;
    int mosaic_aperture_size = 3;
    int aper_correction_aperture_size = 3;
    int median_aperture_size = 3;
    float aper_correction_percentage = 8;

    std::cout << "Input filter id (1-4): ";

    int filterId = 1;
    std::cin >> filterId;

    cv::Mat image = cv::imread(filename, cv::IMREAD_GRAYSCALE);
    cv::imshow("input "+filename, image);

    switch (filterId)
    {
        case 1:
        {
            std::cout << "Input aperture size (3, 5, 7 ...): ";
            std::cin >> gauss_aperture_size;
            cv::Mat gauss_img;
            gauss(image, gauss_img, gauss_aperture_size);
            cv::imshow("gaussian output "+filename, gauss_img);
            break;
        }
        case 2:
        {
            cv::Mat mosaic_img;
            mosaic(image, mosaic_img, mosaic_aperture_size);
            cv::imshow("mosaic output "+filename, mosaic_img);
            break;
        }
        case 3:
        {
            std::cout << "Input percentage (20-30 max): ";
            std::cin >> aper_correction_percentage;
            cv::Mat aper_cor_img;
            aperture_correction(image, aper_cor_img, aper_correction_aperture_size, aper_correction_percentage);
            cv::imshow("mosaic output "+filename, aper_cor_img);
            break;
        }
        case 4:
        {
            cv::Mat median_img;
            median_f(image, median_img, median_aperture_size);
            cv::imshow("median output "+filename, median_img);
            break;
        }
        
        default:
            break;
    }

    int confirm = 0;
    std::cout << "Save all? (0/1) ";
    std::cin >> confirm;
    if (confirm)
    {
        std::string create_folder = "mkdir " + output_folder;
        system(create_folder.c_str());
        output_folder += "/";

        cv::Mat gauss_img;
        gauss(image, gauss_img, gauss_aperture_size);
        cv::imwrite(output_folder + "gauss_img.jpg", gauss_img);

        cv::Mat mosaic_img;
        mosaic(image, mosaic_img, mosaic_aperture_size);
        cv::imwrite(output_folder + "mosaic_img.jpg", mosaic_img);

        cv::Mat aper_cor_img;
        aperture_correction(image, aper_cor_img, aper_correction_aperture_size, aper_correction_percentage);
        cv::imwrite(output_folder + "aper_cor_img.jpg", aper_cor_img);

        cv::Mat median_img;
        median_f(image, median_img, median_aperture_size);
        cv::imwrite(output_folder + "median_img.jpg", median_img);
    }

    cv::waitKey(0);
}