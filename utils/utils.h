#pragma once
#include <string>
#include <iostream>
#include <iomanip>
#include <opencv2/opencv.hpp>

int progressbar(float total_items, int iter, int bar_len = 40)
{
    int filled_len = int(float(bar_len * iter) / total_items);
    float percents = 100.0 * float(iter) / total_items;
    std::string bar = "";
    int i = 0;
    while ( i++ < filled_len)
    {
        bar += "=";
    }
    i = 0;
    while ( i++ < (bar_len - filled_len))
    {
        bar += "-";
    }
    std::cout << std::fixed << std::setprecision(3);
    std::cout << '[' << bar << "]\t" << percents << '\r';
    return (iter+1);
}

void show_img(std::string window_name, cv::Mat img)
{
    cv::namedWindow(window_name, cv::WINDOW_NORMAL);
    cv::resizeWindow(window_name, img.cols > 1000 ? img.cols/2 : img.cols, img.rows > 1000 ? img.rows/2 : img.rows);
    cv::imshow(window_name, img);
}