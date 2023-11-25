#include <opencv2/opencv.hpp>
// checklist
    // 1. разбиение изображения на блоки NхN.
    // 2. совмещение блоков текущего кадра с блоками предыдущего, оценка степени соответствия.
        // 2.1. совмещение алгоритмом 3SS:
            // I. вычисл. целевой функции в 9 блоках, образ. квадрат, на расстоянии d друг от друга.
            // II. выч. в 8 блоках, вокруг наибольшей корреляции, на расстоянии d/2. 
            // III. повтор II. на расстоянии d/4, наибольшая корреляция - конец вектора.
        // 2.2. минимизация целевой функции MAD - сумма абс. разности блоков попиксельно, деленная на площадь блока.
    // 3. Медианная фильтрация (мб для каждого пикселя хранить коорд начала и конца вектора, сравнивать направление, усреднять).
    // 4. Алг кластеризации.
        // 4.1. Последовательный перебор блоков, метка ненулевых векторов, если не помечены.
        // 4.2. Проверка 8 его соседей.
        // 4.3. Если вектора в какой-то степени сонаправленные - метим их одинаково.

// ready
    // 1.
    // 2.1.
    // 2.2.
    // vectors

namespace lab6
{
    class Point : public cv::Point {

        using cv::Point::Point;

        friend bool operator<(const Point& l, const Point& r)
        {
            return l.x < r.x ? true : (l.x == r.x ? l.y < r.y : false);
        }

        friend bool operator>(const Point& l, const Point& r)
        {
            return r < l;
        }
    };
    struct MoveVector {
        MoveVector(cv::Point _start, cv::Point _end) {
            start = _start;
            end = _end;
            coords = _end - _start;
        }

        // codirectional vectors
        friend bool operator==(const MoveVector& l, const MoveVector& r)
        {
            if (l.coords.x / r.coords.x == l.coords.y / r.coords.y)
            {
                return 0 < (l.coords.x * r.coords.x + l.coords.y * r.coords.y);
            }
            return false;
        }

        double distance(const MoveVector& r)
        {
            return std::sqrt(std::pow(start.x - r.start.x, 2) + std::pow(start.y - r.start.y, 2));
        }
        
        cv::Point start;
        cv::Point end;
        cv::Point coords;
    };
}

void fiterVectors(int stepSize, std::map<lab6::Point, lab6::MoveVector> &vectors)
{
    int border = vectors.begin()->first.x;

    for (int i = border + stepSize; i < vectors.rbegin()->first.x; i += stepSize*2)
    {
        for (int j = border + stepSize; j < vectors.rbegin()->first.y; j += stepSize*2)
        {

            double min = INT_MAX;
            cv::Point minCoords;
            

            for (int ii = -stepSize; ii <= stepSize; ii += stepSize)
            {
                for (int jj = -stepSize; jj <= stepSize; jj += stepSize)
                {
                    if (jj == 00 && ii == 00)
                        continue;
                    
                }
            }
        }
    }
}

void drawVectors(const cv::Mat &input_img, cv::Mat &output_img, std::map<lab6::Point, lab6::MoveVector> &vectors)
{
    output_img = input_img.clone();

    for (auto &vector : vectors)
    {
        cv::arrowedLine(output_img, vector.second.start, vector.second.end, cv::Scalar(255, 0, 0, 255));
    }
}

double getCorrelation(const cv::Mat &previous, const cv::Mat &current)
{
    double sumMAD = 0;
    cv::Mat diff; // = cv::Mat(current.size(), CV_32FC1);
    cv::absdiff(previous, current, diff);

    for (int i = 0; i < diff.cols; ++i)
    {
        for (int j = 0; j < diff.rows; ++j)
        {
            sumMAD += diff.at<uchar>(j, i);
        }
    }

    return sumMAD / (diff.rows * diff.cols);
}

cv::Point compare_3SS(const cv::Mat &previousBlock, const cv::Mat &currentFrame, int step, int iteration, cv::Point centralBlock)
{
    std::map<double, cv::Point> blocksResults;
    double central_result = 0;

    for (int i = centralBlock.x - step; i <= centralBlock.x + step; i += step)
    {
        for (int j = centralBlock.y - step; j <= centralBlock.y + step; j += step)
        {
            double result = getCorrelation(previousBlock, currentFrame(cv::Rect(i, j, previousBlock.cols, previousBlock.rows)));
            blocksResults[result] = {i, j};

            if (i == centralBlock.x && j == centralBlock.y)
            {
                central_result = result;
            }
        }
    }

    if (blocksResults.begin()->second == centralBlock || blocksResults.begin()->first == central_result || iteration == 3) {
        return centralBlock;
    } else {
        return compare_3SS(previousBlock, currentFrame, step/2, iteration+1, blocksResults.begin()->second);
    }
}

void getVectorsImg(const cv::Mat &previous_img, const cv::Mat &current_img, int blockSize, std::unordered_map<std::string, cv::Mat> &output_imgs)
{
    cv::Mat src_prev;
    cv::Mat src_cur;

    cv::cvtColor(previous_img, src_prev, cv::COLOR_BGR2GRAY);
    cv::cvtColor(current_img, src_cur, cv::COLOR_BGR2GRAY);

    // draw vector - arrowedLine()
    cv::Mat prevBlock;
    cv::Mat curBlock;

    std::map<lab6::Point, lab6::MoveVector> vectors;
    cv::Point destination;

    for (int i = blockSize*6; i < src_cur.cols - blockSize*6; i += blockSize)
    {
        for (int j = blockSize*6; j < src_cur.rows - blockSize*6; j += blockSize)
        {
            prevBlock = src_prev(cv::Rect(i, j, blockSize, blockSize));

            vectors[lab6::Point(i, j)] = lab6::MoveVector(cv::Point(i, j), compare_3SS(prevBlock, src_cur, 4*blockSize, 1, cv::Point(i, j)));
        }
    }

    drawVectors(current_img, output_imgs["vectors"], vectors);
}