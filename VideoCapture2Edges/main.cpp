#include <iostream>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main()
{
    cout << "OpenCV version: " << CV_MAJOR_VERSION << "." << CV_MINOR_VERSION << endl;

    VideoCapture capture(0);

    if (!capture.isOpened()) {
        return -1;
    }

    Mat frame, edges;

    namedWindow("edges", 1);

    while(1) {
        capture >> frame;

        cvtColor(frame, edges, COLOR_BGR2GRAY);
        GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
        Canny(edges, edges, 0, 30, 3);

        imshow("edges", edges);

        switch (waitKey(30)) {
            case 'q':
                return 0;
        }
    }

    return 0;
}

