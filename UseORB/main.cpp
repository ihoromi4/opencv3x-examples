#include <iostream>

#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>

using namespace std;
using namespace cv;

int main()
{
    cout << "OpenCV version: " << CV_MAJOR_VERSION << "." << CV_MINOR_VERSION << endl;

    VideoCapture capture(0);
    if (!capture.isOpened()) {
        cerr << "Couldn't open default camera" << endl;
    }
    Mat frameA, frameB;
    Mat descriptorsA, descriptorsB;
    vector<KeyPoint> keypointsA, keypointsB;

    // ORB Settings
    int nfeatures = 100;
    float scaleFactor = 1.2f;
    int nlevels = 8;
    int edgeThreshold = 31;
    int firstLevel = 0;
    int WTA_K = 2;
    int scoreType = ORB::HARRIS_SCORE;
    int patchSize = 31;

    Ptr<FeatureDetector> detector = ORB::create(
            nfeatures,
            scaleFactor,
            nlevels,
            edgeThreshold,
            firstLevel,
            WTA_K,
            scoreType,
            patchSize
    );

    //BFMatcher matcher(cv::NORM_HAMMING);
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");

    capture >> frameA;
    detector->detect(frameA, keypointsA);
    detector->compute(frameA, keypointsA, descriptorsA);

    while(1) {
        frameA.copyTo(frameB);
        capture >> frameA;

        // Сохраняем ключевые точки из предыдущего кадра
        keypointsB.swap(keypointsA);

        // Сохраняем дескрипторы из предыдущего кадра
        descriptorsA.copyTo(descriptorsB);

        // Находим ключевые точки (фичи)
        detector->detect(frameA, keypointsA);
        // Вычисляем дескрипторы
        detector->compute(frameA, keypointsA, descriptorsA);

        cout << "Found keypoints: " << keypointsA.size() << endl;

        vector<DMatch> matches;

        if (!descriptorsA.empty() && !descriptorsB.empty()) {
            matcher->match(descriptorsA, descriptorsB, matches);
        }

        vector<DMatch> best_matches;

        for (auto &match : matches) {
            if (match.distance < 20) {
                best_matches.push_back(match);
            }
        }

        cout << "Found matches: " << best_matches.size() << endl;

        Mat out;
        drawMatches(
                frameA, keypointsA,
                frameB, keypointsB,
                best_matches, out
        );

        //drawKeypoints(frame, keypointsA, out, Scalar::all(255));

        imshow("frame", out);

        switch (waitKey(20)) {
            case 'q':
                return 0;
        }
    }

    return 0;
}

