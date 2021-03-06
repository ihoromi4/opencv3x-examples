#include "opencv2/video/tracking.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

Point2f point;
bool addRemovePt = false;


static void help()
{
    cout << "Example of Lukas-Kanade optical flow" << endl;
    cout << "OpenCV version: " << CV_VERSION << endl;
    cout << "It uses camera by default, but you can provide a path to video";
    cout << "Hot keys:" << endl;
    cout << "\tESC - quit the program" << endl;
    cout << "\tr - auto-initialize tracking" << endl;
    cout << "\tc - delete all the points" << endl;
    cout << "\tn - switch the \"night\" mode on/off" << endl;
}


static void onMouse( int event, int x, int y, int /*flags*/, void* /*param*/ )
{
    if( event == EVENT_LBUTTONDOWN )
    {
        point = Point2f((float)x, (float)y);
        addRemovePt = true;
    }
}


int main( int argc, char** argv )
{
    int64 start_tick, end_tick;
    double frame_time = 0;
    double fps = 0;

    VideoCapture cap;
    TermCriteria termcrit(TermCriteria::COUNT | TermCriteria::EPS, 20, 0.03);
    Size subPixWinSize(10, 10), winSize(31, 31);

    const int MAX_COUNT = 500;
    bool needToInit = false;
    bool nightMode = false;

    cv::CommandLineParser parser(argc, argv, "{@input||}{help h||}");

    string input = parser.get<string>("@input");

    help();

    if (input.empty()) {
        cap.open(0);
    } else if (input.size() == 1 && isdigit(input[0])) {
        cap.open(input[0] - '0');
    } else {
        cap.open(input);
    }

    if (!cap.isOpened()) {
        cout << "Could not initialize capturing..." << endl;
        return 0;
    }

    namedWindow("LK Demo", 1);
    setMouseCallback("LK Demo", onMouse, 0);

    Mat gray, prevGray, image, frame;
    vector<Point2f> points[2];

    while (1) {
        start_tick = cv::getTickCount();

        cap >> frame;
        if (frame.empty()) {
            break;
        }

        frame.copyTo(image);
        cvtColor(image, gray, COLOR_BGR2GRAY);

        if (nightMode) {
            image = Scalar::all(0);
        }

        if (needToInit) {
            // automatic initialization
            goodFeaturesToTrack(gray, points[1], MAX_COUNT, 0.01, 10, Mat(), 3, 0, 0.04);
            cornerSubPix(gray, points[1], subPixWinSize, Size(-1,-1), termcrit);
            addRemovePt = false;
        } else if (!points[0].empty()) {
            vector<uchar> status;
            vector<float> err;

            if(prevGray.empty()) {
                gray.copyTo(prevGray);
            }

            calcOpticalFlowPyrLK(prevGray, gray, points[0], points[1], status, err, winSize,
                                 3, termcrit, 0, 0.001);

            size_t i, k;
            for (i = k = 0; i < points[1].size(); i++) {
                if (addRemovePt) {
                    if (norm(point - points[1][i]) <= 5) {
                        addRemovePt = false;
                        continue;
                    }
                }

                if (!status[i]) {
                    continue;
                }

                points[1][k++] = points[1][i];
                circle( image, points[1][i], 3, Scalar(0,255,0), -1, 8);
            }
            points[1].resize(k);
        }

        if (addRemovePt && points[1].size() < (size_t)MAX_COUNT) {
            vector<Point2f> tmp;
            tmp.push_back(point);
            cornerSubPix( gray, tmp, winSize, Size(-1,-1), termcrit);
            points[1].push_back(tmp[0]);
            addRemovePt = false;
        }

        needToInit = false;

        putText(image, "FPS: " + to_string(fps), Point(10, 15), 1, 1, Scalar(50, 50, 50));

        imshow("LK Demo", image);

        switch(waitKey(10)) {
            case 27:
                return 0;
            case 'q':
                return 0;
            case 'r':
                needToInit = true;
                break;
            case 'c':
                points[0].clear();
                points[1].clear();
                break;
            case 'n':
                nightMode = !nightMode;
                break;
        }

        std::swap(points[1], points[0]);
        cv::swap(prevGray, gray);

        end_tick = cv::getTickCount();
        frame_time = (double)(end_tick - start_tick) / cv::getTickFrequency();
        fps = 1.0d / frame_time;
    }

    return 0;
}

