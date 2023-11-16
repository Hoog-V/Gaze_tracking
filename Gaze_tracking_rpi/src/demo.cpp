#include "IrisLandmark.hpp"

#include <iostream>
#include <opencv2/highgui.hpp>

#define SHOW_FPS    (1)

#if SHOW_FPS
    #include <chrono>
#endif

#include "LibCamera.h"
using namespace cv;


int main(int argc, char* argv[]) {
 my::IrisLandmark irisLandmarker("../models");
time_t start_time = time(0);
    int frame_count = 0;
    float lens_position = 100;
    float focus_step = 50;
    LibCamera cam;
    uint32_t width = 800;
    uint32_t height = 480;
    uint32_t stride;
    char key;
    int window_width = width;
    int window_height = height;

    if (width > window_width)
    {
        cv::namedWindow("libcamera-demo", cv::WINDOW_NORMAL);
        cv::resizeWindow("libcamera-demo", window_width, window_height);
    } 

    int ret = cam.initCamera(0);
    cam.configureStill(width, height, formats::RGB888, 1, 0);
    ControlList controls_;
    int64_t frame_time = 1000000 / 30;
    // Set frame rate
        controls_.set(controls::FrameDurationLimits, libcamera::Span<const int64_t, 2>({ frame_time, frame_time }));
    // Adjust the brightness of the output images, in the range -1.0 to 1.0
    controls_.set(controls::Brightness, 0.5);
    // Adjust the contrast of the output image, where 1.0 = normal contrast
    controls_.set(controls::Contrast, 1.5);
    // Set the exposure time
    controls_.set(controls::ExposureTime, 20000);
    cam.set(controls_);
    
    #if SHOW_FPS
        float sum = 0;
        int count = 0;
    #endif
        cv::namedWindow("Face detector", cv::WINDOW_NORMAL);
    	cv::setWindowProperty("Face detector", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);
    if (!ret) {
        bool flag;
        LibcameraOutData frameData;
        cam.startCamera();
        cam.VideoStream(&width, &height, &stride);
        while (true) {
            flag = cam.readFrame(&frameData);
            if(!flag)
            continue;
            Mat frame;
            Mat rframe(height, width, CV_8UC3, frameData.imageData, stride);
  	 cv::flip(rframe, frame, 1);

	 #if SHOW_FPS
            auto start = std::chrono::high_resolution_clock::now();
        #endif

        irisLandmarker.loadImageToInput(frame);
        irisLandmarker.runInference();

	 for (auto landmark: irisLandmarker.getAllFaceLandmarks()) {
            cv::circle(frame, landmark, 2, cv::Scalar(0, 255, 0), -1);
        }

        for (auto landmark: irisLandmarker.getAllEyeLandmarks(true, true)) {
            cv::circle(frame, landmark, 2, cv::Scalar(0, 0, 255), -1);
        }

        for (auto landmark: irisLandmarker.getAllEyeLandmarks(false, true)) {
            cv::circle(frame, landmark, 2, cv::Scalar(0, 0, 255), -1);
        }

        #if SHOW_FPS
            auto stop = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            float inferenceTime = duration.count() / 1e3;
            sum += inferenceTime;
            count += 1;
            int fps = (int) 1e3/ inferenceTime;

            cv::putText(frame, std::to_string(fps), cv::Point(20, 70), cv::FONT_HERSHEY_PLAIN, 3, cv::Scalar(0, 196, 255), 2);
        #endif

      
    	cv::imshow("Face detector", frame);
    
            if (cv::waitKey(10) == 27)
            break;
        cam.returnFrameBuffer(frameData);
        }
        destroyAllWindows();
        cam.stopCamera();
    }
    cam.closeCamera();

    return 0;
}
