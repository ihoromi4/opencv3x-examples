#include <iostream>

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;


int main()
{
    cout << "OpenCV version: " << CV_MAJOR_VERSION << "." << CV_MINOR_VERSION << endl;

    int64 start_tick, end_tick;
    double frame_time = 0;
    double fps = 0;

    Mat frame, gray, threshold;

    VideoCapture capture(0);
    if (!capture.isOpened()) {
        cerr << "Camera not open!" << endl;
        return -1;
    }

	// Setup SimpleBlobDetector parameters.
	SimpleBlobDetector::Params params;

	// Change thresholds
//	params.minThreshold = 10;
//	params.maxThreshold = 255;

	// Filter by Area.
	params.filterByArea = true;
	params.minArea = 500;

	// Filter by Circularity
	params.filterByCircularity = 0;
//	params.minCircularity = 0.1;

	// Filter by Convexity
	params.filterByConvexity = 0;
//	params.minConvexity = 0.87;

	// Filter by Inertia
//	params.filterByInertia = true;
//	params.minInertiaRatio = 0.01;


	// Storage for blobs
	vector<KeyPoint> keypoints;

    Mat mask;
    Ptr<BackgroundSubtractor> bgsubtractor = createBackgroundSubtractorKNN();

	// Set up detector with params
	Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

    while (1) {
        start_tick = cv::getTickCount();

        capture >> frame;

        cvtColor(frame, gray, COLOR_BGR2GRAY);

        bgsubtractor->apply(gray, mask);

        cv::threshold(mask, threshold, 150, 255, THRESH_BINARY_INV);

        Mat element5x5 = getStructuringElement(MORPH_RECT, Size(5, 5));
        erode(threshold, threshold, element5x5);
        dilate(threshold, threshold, element5x5);

        // Detect blobs
        detector->detect(threshold, keypoints);

        // Draw detected blobs as red circles.
        // DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures
        // the size of the circle corresponds to the size of blob

        Mat im_with_keypoints;
        drawKeypoints(frame, keypoints, im_with_keypoints, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

        putText(im_with_keypoints, "FPS: " + to_string(fps), Point(10, 15), 1, 1, Scalar(50, 50, 50));

        // Show blobs
        imshow("threshold", threshold);
        imshow("keypoints", im_with_keypoints);

        switch (waitKey(10)) {
            case 'q':
                return 0;
        }

        end_tick = cv::getTickCount();
        frame_time = (double)(end_tick - start_tick) / cv::getTickFrequency();
        fps = 1.0d / frame_time;
    }

    return 0;
}