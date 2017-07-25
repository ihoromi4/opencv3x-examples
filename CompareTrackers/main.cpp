#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/tracking.hpp> // need contrib

using namespace std;
using namespace cv;

Scalar COLOR_RED = Scalar(0, 0, 255); // BGR
Scalar COLOR_GREEN = Scalar(0, 255 ,0);

void drawText(Mat &frame)
{
    putText(frame, "'m' - MIL Tracker", Point(10, 15), 1, 1, Scalar(0, 0, 255));
    putText(frame, "'b' - BOOSTING Tracker", Point(10, 30), 1, 1, Scalar(0, 0, 255));
    putText(frame, "'f' - MEDIANFLOW Tracker", Point(10, 45), 1, 1, Scalar(0, 0, 255));
    putText(frame, "'t' - TLD Tracker", Point(10, 60), 1, 1, Scalar(0, 0, 255));
    putText(frame, "'k' - KCF Tracker", Point(10, 75), 1, 1, Scalar(0, 0, 255));
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

    Rect2d roi;
    Mat frame;

    VideoCapture capture(0);
    if (!capture.isOpened()) {
        cerr << "Unable to open camera!" << endl;
        return -1;
    }

    Ptr<Tracker> tracker;

    while (1) {
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

        imshow("tracker", frame);

    }

    return 0;
}
