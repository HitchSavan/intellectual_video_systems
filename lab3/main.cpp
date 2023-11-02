#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <utils/utils.h>
#include <panorama.h>

int main(int argc, char* argv[]) {

    if (argc == 2)
    {
        std::cout << "parsed too few arguments, at least 2 pictures is needed ";
        return 1;
    }

    std::vector<cv::Mat> src_images;
    std::vector<std::pair<std::vector<cv::KeyPoint>, cv::Mat>> detection_result;
    std::vector<cv::Mat> keypoint_images;
    std::vector<cv::Mat> matches_images;
    std::vector<cv::Mat> output_images;

    for (int i = 1; i < argc; i++)
    {
        std::string filename = argv[i];
        src_images.push_back(cv::imread(filename));

        cv::Mat keypoint_img;
        keypoint_images.push_back(keypoint_img);
    }

    std::string output_folder = "output";
    
    std::string create_folder = "mkdir " + output_folder;
    system(create_folder.c_str());
    output_folder += "/";
    
    for (size_t i = 0; i < src_images.size(); ++i) {
        cv::cvtColor(src_images[i], src_images[i], cv::COLOR_BGR2BGRA);
        detection_result.push_back( detect_keypoints(src_images[i], keypoint_images[i]) );
    }

    for (size_t i = 0; i < src_images.size()-1; ++i) {
        cv::Mat out, matc;
        output_images.push_back(out);
        matches_images.push_back(matc);

        match({src_images[i], src_images[i+1]},
            output_images[i], matches_images[i],
            detection_result[i], detection_result[i+1]);
    }

    std::vector<cv::Mat> rec_output;
    recursive_panorama_sewing(src_images, rec_output);
    imwrite_vector(output_folder + "recursive", "png", rec_output);

    for (auto &img : output_images)
        crop_border(img);
    imwrite_vector(output_folder + "keypoints", "jpg", keypoint_images);
    imwrite_vector(output_folder + "matches", "jpg", matches_images);
    imwrite_vector(output_folder + "result", "jpg", output_images);

    int confirm = 0;
    std::cout << "\nShow all? (0/1) ";
    std::cin >> confirm;

    if (confirm) {
        show_imgs("input", src_images);
        show_imgs("keypoints", keypoint_images);
        show_imgs("matches", matches_images);
        show_imgs("result", output_images);
        show_imgs("recursive", rec_output);
        cv::waitKey(0);
    }
    std::cout << "\n";
}