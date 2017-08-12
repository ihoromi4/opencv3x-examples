#include <iostream>
#include <pthread.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


class VideoThread
{
public:
    cv::Mat frame;
    pthread_mutex_t waitFrame;
    pthread_mutex_t waitRead;
    pthread_t thread;
    VideoCapture &capture;

    VideoThread(VideoCapture &cap);
//    CameraThread(int id);
//    CameraThread(string filename);
    ~VideoThread();

    static void* readCamera(void *context);
    VideoThread& operator>>(Mat &frame);

private:
    void init();
};

void VideoThread::init()
{
    int code;
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
//    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);

    code = pthread_mutex_init(&waitFrame, &attr);

    if (code != 0) {
        cerr << "pthread_mutex_init() exit code: " << code << endl;
    }

    pthread_mutex_lock(&waitFrame);

    code = pthread_mutex_init(&waitRead, &attr);

    if (code != 0) {
        cerr << "pthread_mutex_init() exit code: " << code << endl;
    }

    pthread_create(&thread, NULL, VideoThread::readCamera, (void*)this);
    pthread_detach(thread);
}

VideoThread::VideoThread(VideoCapture &cap) :
    capture(cap)
{
    init();
}

VideoThread::~VideoThread()
{
    int code = pthread_mutex_destroy(&waitFrame);

    if (code != 0) {
        cerr << "pthread_mutex_destroy() exit code: " << code << endl;
    }
}

void* VideoThread::readCamera(void *context)
{
    VideoThread* self = (VideoThread*)context;
    Mat newFrame;

    while (1) {
        pthread_mutex_lock(&(self->waitRead));
        self->capture >> newFrame;
        if (newFrame.empty()) {
            cerr << "Capture closed" << endl;
            break;
        }

        self->frame = newFrame;

        pthread_mutex_unlock(&(self->waitFrame));
    }
}

VideoThread& VideoThread::operator>>(Mat &frame)
{
    pthread_mutex_lock(&waitFrame);
    frame = this->frame;
    pthread_mutex_unlock(&waitRead);
//    this->frame.copyTo(frame);

    return *this;
}


int main() {
    int64 start_tick, end_tick;
    double frame_time = 0;
    double fps = 0;

    VideoCapture capture(0);
    capture.set(CAP_PROP_FPS, 30);
    VideoThread video(capture);
    Mat frame;

    while (1) {
        start_tick = cv::getTickCount();

//        capture >> frame;
        video >> frame;

        putText(frame, "FPS: " + to_string(fps), Point(10, 15), 1, 1, Scalar(50, 50, 50));

        imshow("frame", frame);

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
