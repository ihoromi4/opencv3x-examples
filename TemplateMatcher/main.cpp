#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int match_method = cv::TM_SQDIFF;
cv::Point mclickA, mclickB;
bool rectDone = 0;

void onMouseCallback(int event, int x, int y, int flags, void* param)
{
    switch(event) {
        case CV_EVENT_LBUTTONDOWN:
            cout << "LB down" << endl;
            mclickA = cv::Point(x, y);
            break;
        case CV_EVENT_LBUTTONUP:
            cout << "LB up" << endl;
            mclickB = cv::Point(x, y);
            rectDone = 1;
            break;
    }
}

cv::Rect MatchingMethod(Mat frame, Mat templ)
{
    Mat matchresult;

    int rescols = frame.cols - templ.cols + 1;
    int resrows = frame.rows - templ.rows + 1;

    matchresult.create(rescols, resrows, CV_32FC1);

    matchTemplate(frame, templ, matchresult, match_method);
    normalize(matchresult, matchresult, 0, 1, cv::NORM_MINMAX, -1, Mat());

    imshow("result", matchresult);

    double minVal, maxVal;
    cv::Point minLoc, maxLoc, matchLoc;

    minMaxLoc(matchresult, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

    if (match_method == cv::TM_SQDIFF || match_method == cv::TM_SQDIFF_NORMED) {
        matchLoc = minLoc;
    } else {
        matchLoc = maxLoc;
    }

    cv::Rect rect = Rect(matchLoc, cv::Size(templ.cols, templ.rows));

    return rect;
}

int main(int argc, char** argv)
{
    VideoCapture capture(0);

    Mat frame;
    Mat templ;

    bool isTempl = 0;

    namedWindow("Display Image", cv::WINDOW_AUTOSIZE);

    setMouseCallback("Display Image", onMouseCallback, NULL);

    while(1) {
        capture >> frame;

        if (rectDone) {
            templ = frame(cv::Rect(mclickA, mclickB));
            templ = templ.clone();
            rectDone = 0;
            isTempl = 1;
            cout << "Crop template" << endl;
        }

        if (isTempl) {
            cout << "Find matching" << endl;
            cv::Rect rect = MatchingMethod(frame, templ);

            rectangle(frame, rect, Scalar(0, 255, 0), 2);

            imshow("template", templ);
        }

        imshow("Display Image", frame);

        switch(waitKey(30)) {
        case 'q':
            return 0;
        }
    }

    return 0;
}

