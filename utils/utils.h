#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "wtypes.h"
#include <opencv2/opencv.hpp>

int progressbar(float total_items, int iter, int bar_len = 40)
{
    int filled_len = int(float(bar_len * iter) / total_items);
    double percents = 100.0 * float(iter) / total_items;
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
    std::cout << '[' << bar << "]\t" << percents << "%\r";
    return (iter+1);
}

void show_img(std::string window_name, cv::Mat img, double resize_koeff = 1)
{
    RECT desktop;

    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    
    int maxw = desktop.right;
    int maxh = desktop.bottom;

    cv::namedWindow(window_name, cv::WINDOW_NORMAL);
    if ( resize_koeff == 1 ) {
        resize_koeff = (img.cols > maxw || img.rows > maxh) ? min((double)maxw / img.cols, (double)maxh / img.rows) : resize_koeff;
    }
    cv::resizeWindow(window_name, img.cols * resize_koeff, img.rows * resize_koeff);
    cv::imshow(window_name, img);
}

void show_imgs(std::string window_name, std::vector<cv::Mat> imgs, double resize_koeff = 1)
{
    for (size_t i = 0; i < imgs.size(); ++i)
    {
        show_img(window_name + std::to_string(i), imgs[i], resize_koeff);
    }
}

void imwrite_vector(std::string path_to_file, std::string extension, const std::vector<cv::Mat> &input_img)
{
    for (size_t i = 0; i < input_img.size(); ++i)
    {
        cv::imwrite(path_to_file + std::to_string(i) + "." + extension, input_img[i]);
    }
}