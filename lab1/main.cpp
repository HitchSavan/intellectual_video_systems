#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <gauss_filter.h>
#include <mosaic_filter.h>
#include <median_filter.h>
#include <aperture_correction_filter.h>

int main(int argc, char** argv) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::string filename = "../kitten.png";
    std::string output_folder = "output";

    int gauss_aperture_size = 5;
    int mosaic_aperture_size = 3;
    int aper_correction_aperture_size = 3;
    int median_aperture_size = 3;
    float aper_correction_percentage = 20;

    std::cout << "Input filter id for showing only one pic (1-4, 0 for skip): ";
    int filterId = 1;
    std::cin >> filterId;

    cv::Mat image = cv::imread(filename, cv::IMREAD_GRAYSCALE);

    switch (filterId)
    {
        case 1:
        {
            std::cout << "Gaussian filter\n";
            std::cout << "Input aperture size (3, 5, 7 ...): ";
            std::cin >> gauss_aperture_size;
            cv::Mat gauss_img;
            gauss(image, gauss_img, gauss_aperture_size);
            show_img("gaussian output "+filename, gauss_img);
            break;
        }
        case 2:
        {
            std::cout << "Mosaic filter\n";
            cv::Mat mosaic_img;
            mosaic(image, mosaic_img, mosaic_aperture_size);
            show_img("mosaic output "+filename, mosaic_img);
            break;
        }
        case 3:
        {
            std::cout << "Aperture correction filter\n";
            std::cout << "Input percentage (20-30 max): ";
            std::cin >> aper_correction_percentage;
            cv::Mat aper_cor_img;
            aperture_correction(image, aper_cor_img, aper_correction_aperture_size, aper_correction_percentage);
            show_img("aperture correction output "+filename, aper_cor_img);
            break;
        }
        case 4:
        {
            std::cout << "Median filter\n";
            cv::Mat median_img;
            median_f(image, median_img, median_aperture_size);
            show_img("median output "+filename, median_img);
            break;
        }
        
        default:
            break;
    }

    std::cout << "\nSave all? (0/1) ";
    int confirm = 0;
    std::cin >> confirm;
    if (confirm)
    {
        std::cout << "\nShow all? (0/1) ";
        std::cin >> confirm;

        std::string create_folder = "mkdir " + output_folder;
        system(create_folder.c_str());
        output_folder += "/";

        cv::Mat gauss_img;
        gauss(image, gauss_img, 3);
        cv::imwrite(output_folder + "gauss3_img.jpg", gauss_img);

        gauss(image, gauss_img, 5);
        cv::imwrite(output_folder + "gauss5_img.jpg", gauss_img);

        cv::Mat mosaic_img;
        mosaic(image, mosaic_img, mosaic_aperture_size);
        cv::imwrite(output_folder + "mosaic_img.jpg", mosaic_img);

        cv::Mat aper_cor_img;
        aperture_correction(image, aper_cor_img, aper_correction_aperture_size, aper_correction_percentage);
        cv::imwrite(output_folder + "aper_cor_img.jpg", aper_cor_img);

        cv::Mat median_img;
        median_f(image, median_img, median_aperture_size);
        cv::imwrite(output_folder + "median_img.jpg", median_img);

        if (confirm)
        {
            show_img("gaussian output "+filename, gauss_img);
            show_img("mosaic output "+filename, mosaic_img);
            show_img("aperture correction output "+filename, aper_cor_img);
            show_img("median output "+filename, median_img);
        }
    }
    std::cout << "\n";

    show_img("input "+filename, image);
    if (confirm != 0)
        cv::waitKey(0);
}