#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <opencv2/opencv.hpp>
#include <utils/utils.h>
#include <panorama.h>

void recursive_panoram(cv::Mat &left, cv::Mat &right, int iter) {
    
}

int main(int argc, char** argv) {

    std::string filename1 = "p1.jpg";
    std::string filename2 = "p2.jpg";
    std::string filename3 = "p3.jpg";
    std::string output_folder = "output";
    std::vector<cv::Mat> output_images;
    std::vector<cv::Mat> matches_images;

    cv::Mat image1 = cv::imread(filename1);
    cv::Mat image2 = cv::imread(filename2);
    cv::Mat image3 = cv::imread(filename3);
    std::vector<cv::Mat> src_images = {image1, image2, image3};

    for (auto &img : src_images) {
        cv::copyMakeBorder(img, img, 0, 0, img.cols, 0, cv::BORDER_CONSTANT, cv::Scalar(0));
    }

    cv::Mat keypoint_image1;
    cv::Mat keypoint_image2;
    cv::Mat keypoint_image3;
    std::vector<cv::Mat> keypoint_images = {keypoint_image1, keypoint_image2, keypoint_image3};
    std::vector<std::pair <std::vector<cv::KeyPoint>, cv::Mat>> detection_result;
    
    std::string create_folder = "mkdir " + output_folder;
    system(create_folder.c_str());
    output_folder += "/";
    
    for (size_t i = 0; i < src_images.size(); ++i)
    {
        detection_result.push_back( detect_keypoints(src_images[i], keypoint_images[i]) );
    }

    for (size_t i = 0; i < src_images.size()-1; ++i)
    {
        cv::Mat out, matc;
        output_images.push_back(out);
        matches_images.push_back(matc);

        match({src_images[i], src_images[i+1]},
            output_images[i], matches_images[i],
            detection_result[i], detection_result[i+1]);
    }

    imwrite_vector(output_folder + "keypoints", "jpg", keypoint_images);

    imwrite_vector(output_folder + "matches", "jpg", matches_images);
    imwrite_vector(output_folder + "result", "jpg", output_images);

    int confirm = 0;
    std::cout << "\nShow all? (0/1) ";
    std::cin >> confirm;

    if (confirm)
    {
        show_imgs("keypoints", keypoint_images);
        show_imgs("matches", matches_images);
        show_imgs("result", output_images, 0.5);
        show_imgs("input", src_images, 0.5);
        cv::waitKey(0);
    }
    std::cout << "\n";
}