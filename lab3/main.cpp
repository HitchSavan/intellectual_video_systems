#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <utils/utils.h>
#include <panorama.h>

void panorama(cv::Mat &left, cv::Mat &right, cv::Mat &output, int iter = 0) {
    cv::Mat keypoint_imageL, keypoint_imageR;
    std::pair <std::vector<cv::KeyPoint>, cv::Mat> detection_resultL = detect_keypoints(left, keypoint_imageL);
    std::pair <std::vector<cv::KeyPoint>, cv::Mat> detection_resultR = detect_keypoints(right, keypoint_imageR);

    cv::Mat matchimg;
    match({left, right},
            output, matchimg,
            detection_resultL, detection_resultR);
}

void rec_panorama(std::vector<cv::Mat> &input, std::vector<cv::Mat> &output, int iter = 0) {
    if (input.size() != output.size()) {
        std::cout << "IO vectors mismatch";
        return;
    }
    if (iter != output.size()-1) {
        panorama(output[iter], input[iter+1], output[iter+1]);
        rec_panorama(input, output, iter+1);
    }
}

void recursive_panorama_sewing(std::vector<cv::Mat> &input, std::vector<cv::Mat> &output) {
    for (auto &img : input)
        cv::copyMakeBorder(img, img, 0 + img.rows, 0 + img.rows, img.cols + img.cols, 0, cv::BORDER_CONSTANT, cv::Scalar(0));

    if (output.empty()) {
        for (size_t i = 0; i < input.size()-1; ++i) {
            cv::Mat out;
            output.push_back( out );
        }
    }
    output.insert(output.begin(), input[0]);

    rec_panorama(input, output);

    output.erase(output.begin());
    for (auto &img : output)
        img = img(cv::Rect(0, 0, img.cols/2, img.rows));
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

    cv::Mat keypoint_image1;
    cv::Mat keypoint_image2;
    cv::Mat keypoint_image3;
    std::vector<cv::Mat> keypoint_images = {keypoint_image1, keypoint_image2, keypoint_image3};
    std::vector<std::pair <std::vector<cv::KeyPoint>, cv::Mat>> detection_result;
    
    std::string create_folder = "mkdir " + output_folder;
    system(create_folder.c_str());
    output_folder += "/";
    
    for (size_t i = 0; i < src_images.size(); ++i) {
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

    imwrite_vector(output_folder + "recursive", "jpg", rec_output);

    for (auto &img : output_images)
        img = img(cv::Rect(0, 0, img.cols/2, img.rows));
    imwrite_vector(output_folder + "keypoints", "jpg", keypoint_images);
    imwrite_vector(output_folder + "matches", "jpg", matches_images);
    imwrite_vector(output_folder + "result", "jpg", output_images);

    int confirm = 0;
    std::cout << "\nShow all? (0/1) ";
    std::cin >> confirm;

    if (confirm) {
        show_imgs("keypoints", keypoint_images);
        show_imgs("matches", matches_images);
        show_imgs("result", output_images, 0.5);
        show_imgs("input", src_images, 0.5);
        show_imgs("recursive", rec_output, 0.5);
        cv::waitKey(0);
    }
    std::cout << "\n";
}