#include <iostream>
#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/tracking.hpp> // no contrib

using namespace std;
using namespace cv;

int main()
{
    cout << "OpenCV version: " << CV_MAJOR_VERSION << "." << CV_MINOR_VERSION << endl;

    Rect roi;
    Mat frame;

    VideoCapture capture(0);
    if (!capture.isOpened()) {
        cerr << "Unable to open camera!" << endl;
        return -1;
    }

    //Ptr<Tracker> tracker = Tracker::create("KCF");

    while (1) {
        capture >> frame;

        imshow("frame", frame);

        switch (waitKey(20)) {
            case 'q':
                return 0;
        }
    }

    return 0;
}
