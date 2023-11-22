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

double getCorrelation(const cv::Mat &previous, const cv::Mat &current)
{
    double sumMAD = 0;
    cv::Mat diff = cv::Mat(current.size(), CV_32FC1);
    cv::absdiff(previous, current, diff);

    for (int i = 0; i < diff.cols; ++i)
    {
        for (int j = 0; j < diff.rows; ++j)
        {
            sumMAD += diff.at<float>(j, i);
        }
    }

    return sumMAD / (diff.rows * diff.cols);
}

cv::Point compare_3SS(const cv::Mat &previousBlock, const cv::Mat &currentFrame, int step, int iteration, cv::Point centralBlock)
{
    std::map<double, cv::Point> blocksResults;

    for (int i = -step; i < centralBlock.x + step; i += step)
    {
        for (int j = -step; j < centralBlock.y + step; j += step)
        {
            double result = getCorrelation(previousBlock, currentFrame(cv::Rect(i, j, previousBlock.cols, previousBlock.rows)));
            blocksResults[result] = {i, j};
        }
    }

    if (blocksResults.begin()->second == centralBlock || iteration == 3) {
        return blocksResults.begin()->second;
    } else {
        return compare_3SS(previousBlock, currentFrame, step/2, iteration+1, blocksResults.begin()->second);
    }
}

void getVectorsImg(const cv::Mat &previous, const cv::Mat &current, int blockSize, std::unordered_map<std::string, cv::Mat> &output)
{
    // draw vector - arrowedLine()
    cv::Mat prevBlock;
    cv::Mat curBlock;

    std::unordered_map<cv::Point, cv::Point> vectors;
    cv::Point destination;

    for (int i = 0; i < current.cols - blockSize; i += blockSize)
    {
        for (int j = 0; j < current.rows - blockSize; j += blockSize)
        {
            prevBlock = previous(cv::Rect(i, j, blockSize, blockSize));

            destination = compare_3SS(prevBlock, current, 4*blockSize, 1, cv::Point(i, j));
        }
    }
}