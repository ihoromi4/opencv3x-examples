#include <iostream>
#include <opencv2/highgui.hpp>
//#include <opencv2/imgproc.hpp>
#include <opencv2/video.hpp>

using namespace std;
using namespace cv;

void help()
{
    cout << "This program shows how to use background substraction methods provided by" << "OpenCV" << endl;
}

int main()
{
    cout << "OpenCV version: " << CV_MAJOR_VERSION << "." << CV_MINOR_VERSION << endl;

    bool calcBackground = 1;

    Mat frame;
    Mat fgMaskMOG2;
    Mat fgMaskKNN;
    Mat background;

    VideoCapture capture(0);
    if (!capture.isOpened()) {
        cerr << "Unable to open camera!" << endl;
    }

    Ptr<BackgroundSubtractor> pKNN = createBackgroundSubtractorKNN();
    Ptr<BackgroundSubtractor> pMOG2 = createBackgroundSubtractorMOG2();

    namedWindow("frame", 1);
    namedWindow("mask MOG2", 1);
    namedWindow("mask KNN", 1);
    namedWindow("background", 1);

    help();

    while (1) {
        capture >> frame;

        if (calcBackground) {
            pMOG2->apply(frame, fgMaskMOG2);
            pKNN->apply(frame, fgMaskKNN);
        }

        pMOG2->getBackgroundImage(background);

        imshow("frame", frame);
        imshow("mask MOG2", fgMaskMOG2);
        imshow("mask KNN", fgMaskKNN);
        imshow("background", background);

        switch (waitKey(30)) {
            case 'q':
                return 0;
        }
    }

    return 0;
}
