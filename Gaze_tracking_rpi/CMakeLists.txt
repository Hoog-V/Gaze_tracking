cmake_minimum_required(VERSION 3.18..3.21)

# Set app name
set(APP_NAME FaceMeshCpp)
set(CMAKE_CXX_STANDARD 17)
# Set project
project(${APP_NAME})

# Make executable app.
add_executable(${APP_NAME})

# Add source file
add_subdirectory(src)

set(LIBCAMERA_INCLUDE_DIRS "/usr/include/libcamera/")
set(LIBCAMERA_LIBRARIES "/usr/lib/aarch64-linux-gnu/libcamera.so" "/usr/lib/aarch64-linux-gnu/libcamera-base.so")  # Replace with the actual library file


# Find opengl libraries
find_package(OpenCV REQUIRED)
#find_package(libcamera REQUIRED)

# Add include path
target_include_directories(${APP_NAME}
        PRIVATE ${OpenCV_INCLUDE_DIRS}
		${LIBCAMERA_INCLUDE_DIRS})

add_subdirectory(
        "../tensorflow/tensorflow/lite"
        "${CMAKE_CURRENT_BINARY_DIR}/tensorflow-lite" EXCLUDE_FROM_ALL)

# Link libraries to app.
target_link_libraries(${APP_NAME}
        PRIVATE ${OpenCV_LIBS}
        PRIVATE tensorflow-lite
	PRIVATE ${LIBCAMERA_LIBRARIES}
)
