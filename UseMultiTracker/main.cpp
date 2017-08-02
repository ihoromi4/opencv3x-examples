#include <iostream>

#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

int main( int argc, char** argv )
{
    cout << "OpenCV version: " << CV_MAJOR_VERSION << "." << CV_MINOR_VERSION << endl;

    int64 start_tick, end_tick;
    double frame_time = 0;
    double fps = 0;

    // set the default tracking algorithm
    std::string trackingAlg = "MIL";

    // create the tracker
    MultiTracker trackers;

    VideoCapture capture;
    if (argc > 1) {
        // set input video
        std::string video = argv[1];
        capture.open(video);
    } else {
        capture.open(0);
    }

    if (!capture.isOpened()) {
        cout << "capture not open!" << endl;
        return 0;
    }

    Mat frame;

    // get bounding box
    capture >> frame;
    vector<Rect2d> ROIs;

    while(1) {
        Rect2d roi;
        roi = selectROI("tracker", frame);
        if (!ROIs.empty() && roi == ROIs.back()) {
            break;
        }
        ROIs.push_back(roi);
    }

    trackers.add(trackingAlg, frame, ROIs);

    // do the tracking
    printf("Start the tracking process, press ESC to quit.\n");

    while (1) {
        start_tick = cv::getTickCount();

        // get frame from the video
        capture >> frame;

        // stop the program if no more images
        if (frame.rows == 0 || frame.cols == 0)
            break;

        //update the tracking result
        trackers.update(frame);

        // draw the tracked object
        for (auto rect : trackers.objects) {
            rectangle(frame, rect, Scalar(0, 255, 0));
        }

        putText(frame, "FPS: " + to_string(fps), Point(10, 15), 1, 1, Scalar(50, 50, 50));

        // show image with the tracked object
        imshow("tracker",frame);

        switch (waitKey(10)) {
            case 27:
                return 0;
            case 'q':
                return 0;
        }

        end_tick = cv::getTickCount();
        frame_time = (double)(end_tick - start_tick) / cv::getTickFrequency();
        fps = 1.0d / frame_time;
    }
}
