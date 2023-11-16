cmake_minimum_required(VERSION 3.18..3.21)

# Set app name
set(APP_NAME FaceMeshCpp)
set(CMAKE_CXX_STANDARD 17)
# Set project
project(${APP_NAME})

# Make executable app.
add_executable(${APP_NAME}
        src/gaze_direction_estimator.cpp
        src/gaze_direction_estimator.h)

# Add source file
add_subdirectory(src)

# Find opengl libraries
find_package(OpenCV REQUIRED)

# Add include path
target_include_directories(${APP_NAME}
        PRIVATE ${OpenCV_INCLUDE_DIRS})



add_subdirectory(
        "../tensorflow/tensorflow/lite"
        "${CMAKE_CURRENT_BINARY_DIR}/tensorflow-lite" EXCLUDE_FROM_ALL)

# Link libraries to app.
target_link_libraries(${APP_NAME}
        PRIVATE ${OpenCV_LIBS}
        PRIVATE tensorflow-lite
)
