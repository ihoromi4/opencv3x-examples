#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/tracking.hpp> // need contrib

using namespace std;
using namespace cv;

Scalar COLOR_RED = Scalar(0, 0, 255); // BGR
Scalar COLOR_GREEN = Scalar(0, 255 ,0);

void drawText(Mat &frame)
{
    int dx = 10;
    int iy = 2;
    int sy = 15;

    putText(frame, "'m' - MIL Tracker", Point(dx, sy * iy++), 1, 1, Scalar(0, 0, 255));
    putText(frame, "'b' - BOOSTING Tracker", Point(dx, sy * iy++), 1, 1, Scalar(0, 0, 255));
    putText(frame, "'f' - MEDIANFLOW Tracker", Point(dx, sy * iy++), 1, 1, Scalar(0, 0, 255));
    putText(frame, "'t' - TLD Tracker", Point(dx, sy * iy++), 1, 1, Scalar(0, 0, 255));
    putText(frame, "'k' - KCF Tracker", Point(dx, sy * iy), 1, 1, Scalar(0, 0, 255));
}

void initTracker(string name, Ptr<Tracker> &tracker, Rect2d &roi, Mat &frame)
{
    tracker = Tracker::create(name);
    roi = selectROI("tracker", frame);
    //destroyWindow("tracker");

    if (roi.width == 0 || roi.height == 0) {
        return;
    }

    tracker->init(frame, roi);
}

int main()
{
    cout << "OpenCV version: " << CV_MAJOR_VERSION << "." << CV_MINOR_VERSION << endl;

    int64 start_tick, end_tick;
    double frame_time = 0;
    int fps = 0;

    Rect2d roi;
    Mat frame;

    VideoCapture capture(0);
    if (!capture.isOpened()) {
        cerr << "Unable to open camera!" << endl;
        return -1;
    }

    Ptr<Tracker> tracker;

    while (1) {
        start_tick = cv::getTickCount();

        capture >> frame;

        switch (waitKey(20)) {
            case 'm':
                initTracker("MIL", tracker, roi, frame);
                break;
            case 'b':
                initTracker("BOOSTING", tracker, roi, frame);
                break;
            case 'f':
                initTracker("MEDIANFLOW", tracker, roi, frame);
                break;
            case 't':
                initTracker("TLD", tracker, roi, frame);
                break;
            case 'k':
                initTracker("KCF", tracker, roi, frame);
                break;
            case 'q':
                return 0;
        }

        if (tracker != NULL) {
            if (tracker->update(frame, roi)) {
                rectangle(frame, roi, COLOR_GREEN, 2, 1);
            } else {
                rectangle(frame, roi, COLOR_RED, 2, 1);
            }
        }

        drawText(frame);

        putText(frame, "FPS: " + to_string(fps), Point(10, 15), 1, 1, Scalar(50, 50, 50));

        imshow("tracker", frame);

        end_tick = cv::getTickCount();
        frame_time = (double)(end_tick - start_tick) / cv::getTickFrequency();
        fps = 1.0d / frame_time;

    }

    return 0;
}
