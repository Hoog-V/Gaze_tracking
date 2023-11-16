//
// Created by victor on 16-11-23.
//
#include "gaze_direction_estimator.h"


#define KWADRANT(x) ((x)*(x))


using Vector = std::vector<double>;
// Function to normalize a vector.


auto process(my::FaceLandmark* landmark) {
    cv::Point landmark33 = landmark->getFaceLandmarkAt(33);
    cv::Point landmark133 = landmark->getFaceLandmarkAt(133);
    cv::Point landmark362 = landmark->getFaceLandmarkAt(362);
    cv::Point landmark263 = landmark->getFaceLandmarkAt(263);

    double left_palpebral_fissure_width = sqrtf64(KWADRANT((landmark33.x - landmark133.x))
                                                        -KWADRANT((landmark33.y-landmark133.y)));

    double right_palpebral_fissure_width = sqrtf64(KWADRANT((landmark362.x - landmark263.x))
                                                     -KWADRANT((landmark362.y-landmark263.y)));

}


angle_t get_face_angle(my::ModelLoader* model) {

}