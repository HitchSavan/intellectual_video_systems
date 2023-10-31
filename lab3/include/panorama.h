#include <vector>
#include <utility>
#include <utils/utils.h>
#include <opencv2/opencv.hpp>

std::pair< std::vector<cv::KeyPoint>, cv::Mat > detect_keypoints(const cv::Mat &input_img, cv::Mat &output_img)
{
    cv::Ptr<cv::ORB> detector = cv::ORB::create();

    std::vector<cv::KeyPoint> keypoints;
    cv::Mat descriptor;
    
    detector->detect(input_img, keypoints);
    detector->compute(input_img, keypoints, descriptor);

    cv::drawKeypoints(input_img, keypoints, output_img);

    return std::make_pair(keypoints, descriptor);
}

void sewing(const cv::Mat &input_img, cv::Mat &output_img)
{
    for ( int i = 0; i < input_img.cols; i++ )
    {
        for ( int j = 0; j < input_img.rows; j++ )
        {
            if ((output_img.at<cv::Vec4b>(j, i)[0] == 0 && output_img.at<cv::Vec4b>(j, i)[1] == 0 && output_img.at<cv::Vec4b>(j, i)[2] == 0)
                || output_img.at<cv::Vec4b>(j, i)[3] != 255)
            {
                output_img.at<cv::Vec4b>(j, i) = input_img.at<cv::Vec4b>(j, i);
            }
        }
    }
}

cv::Mat match(const std::vector<cv::Mat> &input_imgs, cv::Mat &output_img, cv::Mat &matches_img,
    std::pair<std::vector<cv::KeyPoint>, cv::Mat> &detection_result1,
    std::pair<std::vector<cv::KeyPoint>, cv::Mat> &detection_result2)
{
    std::vector<std::vector<cv::KeyPoint>> keypoints = {detection_result1.first, detection_result2.first};
    std::vector<cv::Mat> descriptors = {detection_result1.second, detection_result2.second};

    cv::BFMatcher matcher(cv::NORM_HAMMING);
    
    std::vector<std::vector<cv::DMatch>> matches;

    matcher.knnMatch(descriptors[0], descriptors[1], matches, 2);

    std::vector<cv::KeyPoint> matched1, matched2;
    std::vector<cv::Point2f> points1, points2;
    std::vector<cv::DMatch> good_matches;

    float match_ratio = 0.5f;
    for (size_t i = 0; i < matches.size(); i++)
    {
        cv::DMatch first = matches[i][0];
        float dist1 = matches[i][0].distance;
        float dist2 = matches[i][1].distance;

        if (dist1 < match_ratio * dist2)
        {
            int new_i = static_cast<int>(matched1.size());
            matched1.push_back(keypoints[0][first.queryIdx]);
            matched2.push_back(keypoints[1][first.trainIdx]);
            good_matches.push_back(cv::DMatch(new_i, new_i, 0));

            points1.push_back(keypoints[0][first.queryIdx].pt);
            points2.push_back(keypoints[1][first.trainIdx].pt);
        }
    }

    cv::drawMatches(input_imgs[0], matched1, input_imgs[1], matched2, good_matches, matches_img);
    
    cv::Mat homography = cv::findHomography(points1, points2, cv::RANSAC);
    cv::Mat stitch_img;

    cv::warpPerspective(input_imgs[0], stitch_img, homography, cv::Size(input_imgs[0].cols*2, input_imgs[0].rows));
    output_img = stitch_img(cv::Rect(0, 0, input_imgs[1].cols*2, input_imgs[1].rows));

    sewing(input_imgs[1], output_img);

    return output_img;
}