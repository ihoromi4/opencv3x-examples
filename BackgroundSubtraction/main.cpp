#include <iostream>

#include <opencv2/highgui.hpp>
//#include <opencv2/imgproc.hpp>
#include <opencv2/video.hpp>
#include <opencv2/bgsegm.hpp>

using namespace std;
using namespace cv;
using namespace cv::bgsegm;

void help()
{
    cout << "This program shows how to use background substraction methods provided by" << "OpenCV" << endl;
}

int main()
{
    cout << "OpenCV version: " << CV_MAJOR_VERSION << "." << CV_MINOR_VERSION << endl;

    int64 start_tick, end_tick;
    double frame_time = 0;
    double fps = 0;

    bool showBackground = 1;

    Mat frame;
    Mat fgMask;
    Mat background;

    VideoCapture capture(0);
    if (!capture.isOpened()) {
        cerr << "Unable to open camera!" << endl;
        return -1;
    }

    double cam_width = capture.get(CV_CAP_PROP_FRAME_WIDTH);
    double cam_height = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
    cout << "Camera resolution: " << cam_width << " x " << cam_height << endl;

    Ptr<BackgroundSubtractor> bgsubtractor = createBackgroundSubtractorMOG2();

    namedWindow("frame", 1);
    namedWindow("mask", 1);
    namedWindow("background", 1);

    help();

    while (1) {
        start_tick = cv::getTickCount();

        capture >> frame;

        bgsubtractor->apply(frame, fgMask);

        if (showBackground) {
            bgsubtractor->getBackgroundImage(background);
        }

        putText(frame, "FPS: " + to_string(fps), Point(10, 15), 1, 1, Scalar(50, 50, 50));

        imshow("frame", frame);
        imshow("mask", fgMask);

        if (showBackground) {
            imshow("background", background);
        } else {
            destroyWindow("background");
        }

        switch (waitKey(10)) {
            case 'q':
                return 0;
            case 'm':
                cout << "Use MOG" << endl;
                showBackground = 0;
                bgsubtractor = createBackgroundSubtractorMOG();
                break;
            case 'g':
                cout << "Use GMG" << endl;
                showBackground = 0;
                bgsubtractor = createBackgroundSubtractorGMG();
                break;
            case 'k':
                cout << "Use KNN" << endl;
                showBackground = 1;
                bgsubtractor = createBackgroundSubtractorKNN();
                break;
            case 'o':
                cout << "Use MOG2" << endl;
                showBackground = 1;
                bgsubtractor = createBackgroundSubtractorMOG2();
                break;
        }

        end_tick = cv::getTickCount();
        frame_time = (double)(end_tick - start_tick) / cv::getTickFrequency();
        fps = 1.0d / frame_time;
    }

    return 0;
}
