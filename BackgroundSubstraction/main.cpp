#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <highgui.h>
#include <cvaux.h>

using namespace std;
using namespace cv;

void help()
{
    cout << "This program shows how to use background substraction methods provided by" << endl;
    cout << "OpenCV" << endl;
}

int main()
{
    bool calcBackground = 1;

    Mat frame;
    Mat fgMaskMOG2;
    Mat background;

    VideoCapture capture(0);
    if (!capture.isOpened()) {
        cerr << "Unable to open camera!" << endl;
    }

    BackgroundSubtractorMOG2 bsMOG2;

    namedWindow("frame", 1);
    namedWindow("mask MOG2", 1);
    namedWindow("background", 1);

    help();

    while (1) {
        capture >> frame;

        if (calcBackground) {
            bsMOG2.operator()(frame, fgMaskMOG2);
        }

        bsMOG2.getBackgroundImage(background);

        imshow("frame", frame);
        imshow("mask MOG2", fgMaskMOG2);
        imshow("background", background);

        switch (waitKey(30)) {
            case 'q':
                return 0;
        }
    }

    return 0;
}
