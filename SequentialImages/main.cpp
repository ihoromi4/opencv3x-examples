#include <iostream> 
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;
 
//our sensitivity value to be used in the absdiff() function
const static int SENSITIVITY_VALUE = 20;

//size of blur used to smooth the intensity image output from absdiff() function
const static int BLUR_SIZE = 10;

//we'll have just one object to search for
//and keep track of its position.
int theObject[2] = {0,0};

//bounding rectangle of the object, we will use the center of this as its position.
Rect objectBoundingRectangle = Rect(0,0,0,0);
 
 
string intToString(int number)
{
    std::stringstream ss;
    ss << number;
    return ss.str();
}
 
void searchForMovement(Mat thresholdImage, Mat &cameraFeed)
{
    bool objectDetected = false;

    Mat temp;

    thresholdImage.copyTo(temp);

    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(temp, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );// retrieves external contours
 
    //if contours vector is not empty, we have found some objects
    if (contours.size() > 0) {
        objectDetected=true;
    } else {
        objectDetected = false;
    }
 
    if(objectDetected) {
        //the largest contour is found at the end of the contours vector
        //we will simply assume that the biggest contour is the object we are looking for.
        vector< vector<Point> > largestContourVec;
        largestContourVec.push_back(contours.at(contours.size()-1));
        //make a bounding rectangle around the largest contour then find its centroid
        //this will be the object's final estimated position.
        objectBoundingRectangle = boundingRect(largestContourVec.at(0));
        int xpos = objectBoundingRectangle.x+objectBoundingRectangle.width/2;
        int ypos = objectBoundingRectangle.y+objectBoundingRectangle.height/2;
 
        //update the objects positions by changing the 'theObject' array values
        theObject[0] = xpos , theObject[1] = ypos;
    }
    //make some temp x and y variables so we dont have to type out so much
    int x = theObject[0];
    int y = theObject[1];
     
    //draw some crosshairs around the object
    //circle(cameraFeed, Point(x, y), 20, Scalar(0, 255, 0), 2);
    rectangle(cameraFeed, objectBoundingRectangle, Scalar(0, 255, 0), 2);
 
    //write the position of the object to the screen
    putText(cameraFeed, "Tracking object at (" + intToString(x) + "," + intToString(y) + ")", Point(x, y), 1, 1, Scalar(255, 0, 0), 2);
}

int main()
{
    cout << "OpenCV version: " << CV_MAJOR_VERSION << "." << CV_MINOR_VERSION << endl;

    bool objectDetected = false;
    bool debugMode = false; // d
    bool trackingEnabled = false; // t
    bool pause = false;

    Mat frame1, frame2;

    // their grayscale images (needed for absdiff() function)
    Mat grayImage1, grayImage2;

    // resulting difference image
    Mat differenceImage;

    // thresholded difference image (for use in findContours() function)
    Mat thresholdImage;

    VideoCapture capture(0);
    if(!capture.isOpened()){
        cout << "ERROR ACQUIRING VIDEO FEED" << endl;
        return -1;
    } 

    capture >> frame2;

    while(1) {
        frame2.copyTo(frame1);
        capture >> frame2;

        cv::cvtColor(frame1, grayImage1, COLOR_BGR2GRAY);
        cv::cvtColor(frame2, grayImage2, COLOR_BGR2GRAY);

        //perform frame differencing with the sequential images. This will output an "intensity image"
        //do not confuse this with a threshold image, we will need to perform thresholding afterwards.
        cv::absdiff(grayImage1, grayImage2, differenceImage);

        //threshold intensity image at a given sensitivity value
        cv::threshold(differenceImage, thresholdImage, SENSITIVITY_VALUE, 255, THRESH_BINARY);

        if (debugMode == true) {
            cv::imshow("Difference Image", differenceImage);
            cv::imshow("Threshold Image", thresholdImage);
        } else {
            cv::destroyWindow("Difference Image");
            cv::destroyWindow("Threshold Image");
        }

        //blur the image to get rid of the noise. This will output an intensity image
        cv::blur(thresholdImage, thresholdImage, cv::Size(BLUR_SIZE, BLUR_SIZE));
        //threshold again to obtain binary image from blur output
        cv::threshold(thresholdImage, thresholdImage, SENSITIVITY_VALUE, 255, THRESH_BINARY);

        // use morphology operation Erode
        Mat element = getStructuringElement( MORPH_RECT, Size(7, 7));
        cv::morphologyEx(thresholdImage, thresholdImage, CV_MOP_ERODE, element);

        if (debugMode == true) {
            imshow("Final Threshold Image",thresholdImage);
        } else {
            cv::destroyWindow("Final Threshold Image");
        }

        //if tracking enabled, search for contours in our thresholded image
        if (trackingEnabled) {
            searchForMovement(thresholdImage, frame1);
        }

        imshow("Frame", frame1);

        switch(waitKey(10)) {
            case 'q':
                return 0;
            case 't': //'t' has been pressed. this will toggle tracking
                trackingEnabled = !trackingEnabled;
                cout << "Tracking mode: " << trackingEnabled << endl;
                break;
            case 'd': //'d' has been pressed. this will debug mode
                debugMode = !debugMode;
                cout << "Debug mode: " << debugMode << endl;
                break;
        }
    }

    return 0;
}

