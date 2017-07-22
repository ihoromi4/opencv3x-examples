#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace cv;

int main(int, char* [])
{
    // Создание видеопотока с камеры
    VideoCapture video(0);
    Mat frame, curr, prev, curr64f, prev64f, hann;
    Point2d offset;
    int key = 0;

    while (1) {
        video >> frame; // Очередной фрейм
        cvtColor(frame, curr, CV_RGB2GRAY); // Перевод в градации серого

        if (prev.empty()) {
            prev = curr.clone(); // клонирование изображения
            createHanningWindow(hann, curr.size(), CV_64F); // Создание окна Ханна
        }

        prev.convertTo(prev64f, CV_64F);
        curr.convertTo(curr64f, CV_64F);

        Point2d shift = phaseCorrelate(prev64f, curr64f, hann); // Фазовая корреляция
        double radius = cv::sqrt(shift.x * shift.x + shift.y * shift.y); // Вычисление радиуса отклонения

        if (radius > 3) {
            // вывод на экран окружности и направления смещения
            offset.x += shift.x;
            offset.y += shift.y;
        }

        Point center(curr.cols >> 1, curr.rows >> 1);
        cv::circle(frame, center, (int) radius, cv::Scalar(0, 255, 0), 3, CV_AA);
        cv::line(frame, center, Point(center.x + (int) offset.x, center.y + (int) offset.y), cv::Scalar(0, 255, 0), 3,
                 CV_AA);

        imshow("phase shift", frame);

        prev = curr.clone();

        switch (waitKey(20)) {
            case 'q':
                return 0;
        }
    }

    return 0;
}
