#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/tracking.hpp> // need contrib

using namespace std;
using namespace cv;

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

    Ptr<Tracker> tracker = Tracker::create("KCF");

    capture >> frame;
    roi = selectROI("tracker", frame);

    if (roi.width == 0 || roi.height == 0) {
        return 0;
    }

    tracker->init(frame, roi);

    while (1) {
        capture >> frame;

        tracker->update(frame, roi);

        rectangle(frame, roi, Scalar(255, 0, 0), 2, 1);

        imshow("frame", frame);

        switch (waitKey(20)) {
            case 'q':
                return 0;
        }
    }

    return 0;
}
