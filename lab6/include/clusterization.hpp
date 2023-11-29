#include <opencv2/opencv.hpp>
#include <utils/utils.h>
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
    // 3 ??

namespace lab6
{
    struct Point : public cv::Point {

        using cv::Point::Point;

        Point(cv::Point &point)
        {
            x = point.x;
            y = point.y;
        }

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

        MoveVector(){}

        MoveVector(cv::Point _start, cv::Point _end) {
            start = _start;
            end = _end;
            coords = _end - _start;
        }

        // codirectional vectors
        friend bool operator==(const MoveVector& l, const MoveVector& r)
        {
            if ((l.coords.x / r.coords.x) == (l.coords.y / r.coords.y))
            {
                return 0 < (l.coords.x * r.coords.x + l.coords.y * r.coords.y);
            }
            return false;
        }

        double distance(const MoveVector& r)
        {
            return std::sqrt(std::pow(coords.x - r.coords.x, 2) + std::pow(coords.y - r.coords.y, 2));
        }

        double length()
        {
            return std::sqrt(std::pow(coords.x, 2) + std::pow(coords.y, 2));
        }

        // if positive --> <90 degrees
        double getScalarMult(const MoveVector& r)
        {
            return (coords.x * r.coords.x) + (coords.y * r.coords.y);
        }

        // if cos > 0.7 --> <45 degrees
        double getCos(MoveVector& r)
        {
            return (getScalarMult(r) / (length() * r.length()));
        }

        void update(const cv::Point& newCoords)
        {
            coords = newCoords;
            end = start + newCoords;
        }
        
        cv::Point start;
        cv::Point end;
        cv::Point coords;
    };
}

// not-so-recursive
void recursiveVectorFilter(int stepSize, std::map<lab6::Point, lab6::MoveVector> &vectors, std::map<lab6::Point, lab6::MoveVector> &vectorsOut)
{
    vectorsOut = vectors;
    int border = vectorsOut.begin()->first.x;

    std::cout << std::endl << "Recursive filtering vectors... " << std::endl;

    for (int i = border + stepSize; i < vectorsOut.rbegin()->first.x; i += stepSize)
    {
        for (int j = border + stepSize; j < vectorsOut.rbegin()->first.y; j += stepSize)
        {
            if(vectorsOut[lab6::Point(i, j)].coords == cv::Point(0, 0))
                continue;
                
            int emptyAmount = 0;
            double minDist = INT_MAX;
            cv::Point minCoords = cv::Point(0, 0);
            
            for (int vX = -stepSize; vX <= stepSize; vX += stepSize)
            {
                for (int vY = -stepSize; vY <= stepSize; vY += stepSize)
                {
                    double curDist = 0;

                    if(vectorsOut[lab6::Point(i + vX, j + vY)].coords == cv::Point(0, 0))
                    {
                        continue;
                    }

                    for (int ii = -stepSize; ii <= stepSize; ii += stepSize)
                    {
                        for (int jj = -stepSize; jj <= stepSize; jj += stepSize)
                        {
                            if ((ii == vX && jj == vY) || vectorsOut[lab6::Point(i + ii, j + jj)].coords == cv::Point(0, 0))
                                continue;

                            curDist += vectorsOut[lab6::Point(i + vX, j + vY)].distance( vectorsOut[lab6::Point(i + ii, j + jj)] );
                        }
                    }

                    if (curDist < minDist)
                    {
                        minDist = curDist;
                        minCoords = vectorsOut[lab6::Point(i + vX, j + vY)].coords;
                    }
                }
            }
                
            vectorsOut[lab6::Point(i, j)] = lab6::MoveVector(cv::Point(i, j), cv::Point(i, j) + minCoords);
        }

        progressbar((float)(vectorsOut.rbegin()->first.x - stepSize), i, 10);
    }
}

void filterVectors(int stepSize, std::map<lab6::Point, lab6::MoveVector> &vectors, std::map<lab6::Point, lab6::MoveVector> &vectorsOut)
{
    int border = vectors.begin()->first.x;

    std::cout << std::endl << "Filtering vectors... " << std::endl;

    for (int i = border + stepSize; i < vectors.rbegin()->first.x; i += stepSize)
    {
        for (int j = border + stepSize; j < vectors.rbegin()->first.y; j += stepSize)
        {
            if(vectors[lab6::Point(i, j)].coords == cv::Point(0, 0))
                continue;

            double minDist = INT_MAX;
            cv::Point minCoords = cv::Point(0, 0);
            
            for (int vX = -stepSize; vX <= stepSize; vX += stepSize)
            {
                for (int vY = -stepSize; vY <= stepSize; vY += stepSize)
                {
                    if(vectors[lab6::Point(i + vX, j + vY)].coords == cv::Point(0, 0))
                        continue;

                    double curDist = 0;

                    for (int ii = -stepSize; ii <= stepSize; ii += stepSize)
                    {
                        for (int jj = -stepSize; jj <= stepSize; jj += stepSize)
                        {
                            if ((ii == vX && jj == vY) || vectors[lab6::Point(i + ii, j + jj)].coords == cv::Point(0, 0))
                                continue;

                            curDist += vectors[lab6::Point(i + vX, j + vY)].distance( vectors[lab6::Point(i + ii, j + jj)] );
                        }
                    }

                    if (curDist < minDist)
                    {
                        minDist = curDist;
                        minCoords = vectors[lab6::Point(i + vX, j + vY)].coords;
                    }
                }
            }
            
            vectorsOut[lab6::Point(i, j)] = lab6::MoveVector(cv::Point(i, j), cv::Point(i, j) + minCoords);
        }

        progressbar((float)(vectors.rbegin()->first.x - stepSize), i, 10);
    }
}

void drawVectors(const cv::Mat &input_img, cv::Mat &output_img, std::map<lab6::Point, lab6::MoveVector> &vectors)
{
    output_img = input_img.clone();

    std::cout << std::endl << "Drawing silly little arrows... " << std::endl;
    int cur = 0;
    for (auto &vector : vectors)
    {
        cv::arrowedLine(output_img, vector.second.start, vector.second.end, cv::Scalar(255, 0, 0, 255));
        progressbar((float)(vectors.size()), ++cur, 5);
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

    for (int i = centralBlock.x - step; i < centralBlock.x + step; i += step)
    {
        for (int j = centralBlock.y - step; j < centralBlock.y + step; j += step)
        {
            double result = getCorrelation(previousBlock, currentFrame(cv::Rect(i, j, previousBlock.cols, previousBlock.rows)));
            blocksResults[result] = cv::Point(i, j);

            if (i == centralBlock.x && j == centralBlock.y)
            {
                central_result = result;
            }
        }
    }

    if (iteration == 3) {
        return blocksResults.begin()->second;
    } else {
        return compare_3SS(previousBlock, currentFrame, step/2, iteration+1, blocksResults.begin()->second);
    }
}

void fillArea(const cv::Mat &input_img, cv::Mat &output_img, const lab6::Point &topLeftCoords, std::vector<int> &color, int blockSize)
{
    for (int i = 0; i < blockSize; ++i)
    {
        for (int j = 0; j < blockSize; ++j)
        {
            cv::Vec3b pixel_val = input_img.at<cv::Vec3b>(topLeftCoords.y + j, topLeftCoords.x + i);
            for (int k = 0; k < pixel_val.channels; ++k)
                output_img.at<cv::Vec3b>(topLeftCoords.y + j, topLeftCoords.x + i)[k]
                    = pixel_val[k] + color[k] < 255 ? pixel_val[k] + color[k] : 255;
        }
    }
}

void classify(const cv::Mat &input_img, cv::Mat &output_img, std::map<lab6::Point, lab6::MoveVector> &vectors, int blockSize)
{
    output_img = input_img.clone();
    cv::Mat mask = cv::Mat::zeros(input_img.size(), CV_8U);

    std::vector<int> color;

    std::map<lab6::Point, std::vector<int>> colorizedVectors;

    std::cout << std::endl << "Adding some colours... " << std::endl;
    int cur = 0;
    for (auto &vector : vectors)
    {
        if (vector.second.coords == cv::Point(0, 0))
            continue;

        if (mask.at<uchar>(lab6::Point(vector.first)) == 0)
        {
            mask.at<uchar>(lab6::Point(vector.first)) = std::rand()%255;
            for (int k = 0; k < 3; ++k)
                colorizedVectors[lab6::Point(vector.first)].push_back(std::rand()%255);
        }

        for (int i = -blockSize; i <= blockSize; i += blockSize)
        {
            for (int j = -blockSize; j <= blockSize; j += blockSize)
            {
                lab6::Point neighbourVector(vector.first.x + i, vector.first.y + j);
                if (vectors.count(neighbourVector) != 0 && vectors[neighbourVector].coords != cv::Point(0, 0))
                {
                    
                    if (mask.at<uchar>(neighbourVector) != 0 && mask.at<uchar>(neighbourVector) != mask.at<uchar>(lab6::Point(vector.first)) && vector.second.getCos(vectors[neighbourVector]) >= 0)
                    {
                        std::cout << "\nColor mismatch " << mask.at<uchar>(lab6::Point(vector.first)) << " ("
                            << lab6::Point(vector.first) << "), but already has " << mask.at<uchar>(neighbourVector) << "(" << neighbourVector << ")";
                    }

                    if (vector.second.getCos(vectors[neighbourVector]) >= 0)
                    {
                        mask.at<uchar>(neighbourVector) = mask.at<uchar>(lab6::Point(vector.first));
                        colorizedVectors[neighbourVector] = colorizedVectors[lab6::Point(vector.first)];
                    }
                }
            }
        }

        fillArea(input_img, output_img, vector.first, colorizedVectors[vector.first], blockSize);

        progressbar((float)(vectors.size()), ++cur, 5);
    }

    cv::imwrite("output/mask.jpg", mask);
}

void getVectorsImg(const cv::Mat &previous_img, const cv::Mat &current_img, int blockSize, std::unordered_map<std::string, cv::Mat> &output_imgs)
{
    cv::Mat src_prev;
    cv::Mat src_cur;

    cv::cvtColor(previous_img, src_prev, cv::COLOR_BGR2GRAY);
    cv::cvtColor(current_img, src_cur, cv::COLOR_BGR2GRAY);
    cv::copyMakeBorder(src_prev, src_prev, blockSize*7, blockSize*7, blockSize*7, blockSize*7, cv::BORDER_CONSTANT, cv::Scalar(0));
    cv::copyMakeBorder(src_cur, src_cur, blockSize*7, blockSize*7, blockSize*7, blockSize*7, cv::BORDER_CONSTANT, cv::Scalar(0));

    cv::Mat prevBlock;
    cv::Mat curBlock;

    std::map<lab6::Point, lab6::MoveVector> vectors;
    std::map<lab6::Point, lab6::MoveVector> vectorsFiltered;
    cv::Point destination;

    std::cout << "Recursive 3SS..." << std::endl;
    for (int i = blockSize*7; i < src_cur.cols - blockSize*6; i += blockSize)
    {
        for (int j = blockSize*7; j < src_cur.rows - blockSize*6; j += blockSize)
        {
            prevBlock = src_prev(cv::Rect(i, j, blockSize, blockSize));

            vectors[lab6::Point(i, j)] = lab6::MoveVector(cv::Point(i, j), compare_3SS(prevBlock, src_cur, 4*blockSize, 1, cv::Point(i, j)));
        }
        progressbar((float)(src_cur.cols - blockSize*7), i, 5);
    }

    cv::Mat src_bordered;
    cv::copyMakeBorder(previous_img, src_bordered, blockSize*7, blockSize*7, blockSize*7, blockSize*7, cv::BORDER_CONSTANT, cv::Scalar(0));

    drawVectors(src_bordered, output_imgs["vectors"], vectors);

    // recursiveVectorFilter(blockSize, vectors, vectorsFiltered);
    filterVectors(blockSize, vectors, vectorsFiltered);
    
    drawVectors(src_bordered, output_imgs["filteredVectors"], vectorsFiltered);

    classify(src_bordered, output_imgs["clusterized"], vectorsFiltered, blockSize);
    
    for (auto &imgs: output_imgs)
    {
        imgs.second = imgs.second(cv::Rect(blockSize*7, blockSize*7, previous_img.cols, previous_img.rows));
    }
}