#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

const int MIN_OBJECT_AREA = 20 * 20;
const int MAX_OBJECT_AREA = 400 * 400;

int H_MIN = 0;
int H_MAX = 255;
int S_MIN = 0;
int S_MAX = 255;
int V_MIN = 0;
int V_MAX = 255;

void on_trackbar(int, void*){}

void createTrackbars()
{
    namedWindow("trackbars", 0);

    createTrackbar("H_MIN", "trackbars", &H_MIN, H_MAX, on_trackbar);
    createTrackbar("H_MAX", "trackbars", &H_MAX, H_MAX, on_trackbar);
    createTrackbar("S_MIN", "trackbars", &S_MIN, S_MAX, on_trackbar);
    createTrackbar("S_MAX", "trackbars", &S_MAX, H_MAX, on_trackbar);
    createTrackbar("V_MIN", "trackbars", &V_MIN, V_MAX, on_trackbar);
    createTrackbar("V_MAX", "trackbars", &V_MAX, V_MAX, on_trackbar);
}

void drawObject(cv::Rect rect, Mat &frame)
{
    //circle(frame, Point(x, y), 20, Scalar(0, 255, 0), 2);
    //rectangle(frame, Rect(x-20, y-20, 40, 40), Scalar(0, 255, 0));
    rectangle(frame, rect, Scalar(0, 255, 0));
}

void flattenImage(Mat &thresh)
{
    Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3, 3));
    Mat dilateElement = getStructuringElement(MORPH_RECT, Size(8, 8));

    erode(thresh, thresh, erodeElement);
    erode(thresh, thresh, erodeElement);

    dilate(thresh, thresh, dilateElement);
    dilate(thresh, thresh, dilateElement);
}

void trackObject(int &x, int &y, Mat threshold, Mat &frame)
{
    Mat temp;
    threshold.copyTo(temp);
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;

    findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

    if (hierarchy.size() <= 0) {
        cout << "No objects found" << endl;
        return;
    }

    int objects_number = 0;

    for (int index = 0; index >= 0; index = hierarchy[index][0]) {
        Moments moment = moments((cv::Mat)contours[index]);
        double area = fabs(contourArea(contours[index])); //moment.m00;

        if (area > MIN_OBJECT_AREA && area < MAX_OBJECT_AREA) {
            x = moment.m10 / area;
            y = moment.m01 / area;

            cv::Rect rect = boundingRect(contours[index]);

            // object founded
            drawObject(rect, frame);

            objects_number++;
        }
    }

    cout << "Finded objects: " << objects_number << endl;
}

int main()
{
    cout << "OpenCV version: " << CV_MAJOR_VERSION << "." << CV_MINOR_VERSION << endl;

    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cout << "Defaul camera is not opened" << endl;
        return -1;
    }

    Mat frame;
    Mat frame_HSV;
    Mat frame_threshold;

    createTrackbars();

    namedWindow("frame", 1);
    namedWindow("threshold", 1);

    while(1) {
        int x=0, y=0;

        cap >> frame;

        // convert frame -> hsv -> threshold
        cvtColor(frame, frame_HSV, COLOR_BGR2HSV);
        inRange(frame_HSV, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), frame_threshold);

        // Сглаживаем изображение
        flattenImage(frame_threshold);

        trackObject(x, y, frame_threshold, frame);

        imshow("frame", frame);
        imshow("threshold", frame_threshold);

        switch (waitKey(30)) {
            case 'q':
                cout << "Exit" << endl;
                return 0;
        }
    }

    cap.release();

    return 0;
}

