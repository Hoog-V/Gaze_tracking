#include "IrisLandmark.hpp"

#include <iostream>
#include <opencv2/highgui.hpp>

#define SHOW_FPS    (1)
#define NUM_OF_THREADS 16


#if SHOW_FPS
    #include <chrono>
#endif

pthread_mutex_t cameraLocker;
pthread_mutex_t frameLocker;
cv::Mat  outputframe;

void *worker(void *data)
{
    cv::VideoCapture *cap = (cv::VideoCapture*) data;
    cv::Mat rframe, frame;
    my::IrisLandmark irisLandmarker("../models");

    for (;;)
    {
        pthread_mutex_lock(&cameraLocker);
        bool success = cap->read(rframe); // read a new frame from video
        if (success == false)
            break;
        pthread_mutex_unlock(&cameraLocker);
        cv::flip(rframe, frame, 1);
        irisLandmarker.loadImageToInput(frame);
        irisLandmarker.runInference();
        auto landmarks = irisLandmarker.getAllFaceLandmarks();

        for (auto landmark: landmarks) {
            cv::circle(frame, landmark, 2, cv::Scalar(0, 255, 0), -1);
        }
        for (auto landmark: irisLandmarker.getAllEyeLandmarks(true, true)) {
            cv::circle(frame, landmark, 2, cv::Scalar(0, 0, 255), -1);
        }

        for (auto landmark: irisLandmarker.getAllEyeLandmarks(false, true)) {
            cv::circle(frame, landmark, 2, cv::Scalar(0, 0, 255), -1);
        }

        pthread_mutex_lock(&frameLocker);
        frame.copyTo(outputframe);
        pthread_mutex_unlock(&frameLocker);
    }

    return NULL;
}

int main(int argc, char* argv[]) {


    cv::VideoCapture cap(0);
    // cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    // cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    cap.set(cv::CAP_PROP_FPS,30);

    bool success = cap.isOpened();
    if (success == false)
    {
        std::cerr << "Cannot open the camera." << std::endl;
        return 1;
    }

    #if SHOW_FPS
        float sum = 0;
        int count = 0;
    #endif

    pthread_mutex_init(&cameraLocker,NULL);
    pthread_mutex_init(&frameLocker,NULL);
    pthread_t th[NUM_OF_THREADS];
    for(uint8_t i =0; i < NUM_OF_THREADS; i++) {
        pthread_create(&th[i], NULL, worker, (void*)&cap);
    }


    sleep(1);
    while (success)
    {

        pthread_mutex_lock(&frameLocker);
        cv::imshow("Face detector", outputframe);
        pthread_mutex_unlock(&frameLocker);
        if (cv::waitKey(10) == 27)
            break;
    }

    #if SHOW_FPS
        std::cout << "Average inference time: " << sum / count << "ms " << std::endl;
    #endif

    cap.release();
    cv::destroyAllWindows();
    return 0;
}