//
// Created by victor on 16-11-23.
//

#ifndef GAZE_DIRECTION_ESTIMATOR_H
#define GAZE_DIRECTION_ESTIMATOR_H
#include "IrisLandmark.hpp"

typedef struct {double yaw; double pitch; double roll;} angle_t;

auto process(my::FaceLandmark* landmark);

angle_t get_face_angle(my::ModelLoader* model);
#endif //GAZE_DIRECTION_ESTIMATOR_H
