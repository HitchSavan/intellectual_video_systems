#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <utils/utils.h>
#include <img_to_binary.h>
#include <morphological_operations.h>

int main(int argc, char** argv) {

    std::string filename = "../kitten.png";
    std::string output_folder = "output";

    cv::Mat image = cv::imread(filename, cv::IMREAD_GRAYSCALE);
    cv::Mat binary_img;
    cv::Mat binary_erosion_img;
    cv::Mat binary_dilation_img;
    cv::Mat binary_opening_img;
    cv::Mat binary_closing_img;

    cv::Mat grayscale_erosion_img;
    cv::Mat grayscale_dilation_img;
    cv::Mat grayscale_opening_img;
    cv::Mat grayscale_closing_img;

    cv::Mat outline_img;
    cv::Mat gradient_img;
    
    std::cout << "\nSave all? (0/1) ";
    int confirm = 0;
    std::cin >> confirm;
    if (confirm)
    {
        std::string create_folder = "mkdir " + output_folder;
        system(create_folder.c_str());
        output_folder += "/";

        cv::imwrite(output_folder + "grayscale.jpg", image);
        
        get_binary(image, binary_img);
        cv::imwrite(output_folder + "binary_img.jpg", binary_img);


        erosion(binary_img, binary_erosion_img);
        cv::imwrite(output_folder + "binary_erosion_img.jpg", binary_erosion_img);

        dilation(binary_img, binary_dilation_img);
        cv::imwrite(output_folder + "binary_dilation_img.jpg", binary_dilation_img);

        opening(binary_img, binary_opening_img);
        cv::imwrite(output_folder + "binary_opening_img.jpg", binary_opening_img);

        closing(binary_img, binary_closing_img);
        cv::imwrite(output_folder + "binary_closing_img.jpg", binary_closing_img);


        erosion(image, grayscale_erosion_img);
        cv::imwrite(output_folder + "grayscale_erosion_img.jpg", grayscale_erosion_img);

        dilation(image, grayscale_dilation_img);
        cv::imwrite(output_folder + "grayscale_dilation_img.jpg", grayscale_dilation_img);

        opening(image, grayscale_opening_img);
        cv::imwrite(output_folder + "grayscale_opening_img.jpg", grayscale_opening_img);

        closing(image, grayscale_closing_img);
        cv::imwrite(output_folder + "grayscale_closing_img.jpg", grayscale_closing_img);
        

        outline(image, outline_img);
        cv::imwrite(output_folder + "outline_img.jpg", outline_img);

        gradient(image, gradient_img);
        cv::imwrite(output_folder + "gradient_img.jpg", gradient_img);


        std::cout << "\nShow all? (0/1) ";
        std::cin >> confirm;

        if (confirm)
        {
            show_img("binary "+filename, binary_img);
            show_img("binary erosion "+filename, binary_erosion_img);
            show_img("binary dilation "+filename, binary_dilation_img);
            show_img("binary opening "+filename, binary_opening_img);
            show_img("binary closing "+filename, binary_closing_img);

            show_img("grayscale erosion "+filename, grayscale_erosion_img);
            show_img("grayscale dilation "+filename, grayscale_dilation_img);
            show_img("grayscale opening "+filename, grayscale_opening_img);
            show_img("grayscale closing "+filename, grayscale_closing_img);
            
            show_img("outline "+filename, outline_img);
            show_img("gradient "+filename, gradient_img);
        }
    }

    std::cout << "\n";

    show_img("input "+filename, image);
    if (confirm != 0)
        cv::waitKey(0);
}