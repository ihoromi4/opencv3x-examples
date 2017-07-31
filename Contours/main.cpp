#include <iostream>
#include <math.h>

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

const int w = 500;
int levels = 3;

vector<vector<Point> > contours;
vector<Vec4i> hierarchy;

static void on_trackbar(int, void*)
{
}

int main(int argc, char** argv)
{
    cout << "OpenCV version: " << CV_MAJOR_VERSION << "." << CV_MINOR_VERSION << endl;

    VideoCapture capture(0);

    Mat frame, frame_contours;

    vector<vector<Point> > contours0;

    namedWindow("image", 1);
    namedWindow("contours", 1);
    createTrackbar("levels+3", "contours", &levels, 7, on_trackbar);

    on_trackbar(0, 0);

    while (1) {
        capture >> frame;

        cvtColor(frame, frame, COLOR_BGR2GRAY);
        blur( frame, frame, Size(3,3) );
        findContours(frame, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
//        findContours(frame, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

//        for (size_t k = 0; k < contours.size(); k++)
//            approxPolyDP(Mat(contours[k]), contours[k], 3, true);

        frame_contours = Mat::zeros(frame.size(), CV_8UC3);
        int _levels = levels - 3;
        drawContours(frame_contours, contours, _levels <= 0 ? 3 : -1,  Scalar(128, 255), 3, 16, hierarchy, std::abs(_levels) );

        imshow("image", frame);
        imshow("contours", frame_contours);

        switch (waitKey(30)) {
            case 'q':
                return 0;
        }
    }

    return 0;
}

