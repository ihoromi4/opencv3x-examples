#include <iostream>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/videoio/videoio.hpp>
#include <opencv2/objdetect/objdetect.hpp>

using namespace std;
using namespace cv;

CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;

string window_name = "Capture - Face detection";


void detectAndDisplay(Mat frame)
{
    vector<Rect> faces;
    Mat frame_gray;

    cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

    face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30));

    for (size_t i = 0; i < faces.size(); i++) {
        Point center(faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2);
        ellipse(frame, center, Size(faces[i].width/2, faces[i].height/2), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0);

        Mat faceROI = frame_gray(faces[i]);
        vector<Rect> eyes;

        eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30));

        for (size_t j = 0; j < eyes.size(); j++) {
            Point eye_center(faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y + eyes[j].y + eyes[j].height/2);
            int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
            circle(frame, eye_center, radius, Scalar( 255, 0, 0 ), 4, 8, 0);
        }
    }
    imshow(window_name, frame);
}


int main()
{
    cout << "OpenCV version: " << CV_MAJOR_VERSION << "." << CV_MINOR_VERSION << endl;

    VideoCapture cam(0);
    Mat frame;

    if (!cam.isOpened()) {
        cout << "Error opening video capture!" << endl;
    }

    if (!face_cascade.load("../haarcascade_frontalface_alt.xml")) {
        cout << "Error loading face cascade!" << endl;
    }

    if (!eyes_cascade.load("../haarcascade_eye_tree_eyeglasses.xml")) {
        cout << "Error loading eyes cascade!" << endl;
    }

    while (cam.read(frame)) {
        if (frame.empty())
            break;

        detectAndDisplay(frame);

        if (waitKey(10) == 'q') {
            break;
        }
    }

    return 0;
}

