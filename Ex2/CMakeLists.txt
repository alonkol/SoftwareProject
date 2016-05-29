cmake_minimum_required(VERSION 3.3)
project(hw2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

###
# Where to find CMake modules and OpenCV
set(OpenCV_DIR  "C:/opencv/build" "C:/opencv/opencv_contrib-master")
set(OpenCV_VERSION 3.0)
set(CMAKE_MODULE_PATH "C:/Users/alon/Desktop/softwareProj/hw2/cmake")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories("C:/opencv/build/include")
include_directories("C:/opencv/opencv_contrib-master/modules/xfeatures2d/include")
include_directories("C:/opencv/opencv_contrib-master/modules/ximgproc/include")
include_directories("C:/opencv/opencv_contrib-master/modules/xobjdetect/include")



#set(SOURCE_FILES main.cpp sp_image_proc_util.cpp)
add_executable(hw2 ${SOURCE_FILES} main.cpp)

# add libs you need
# FIND HOW TO ADD THE LIBS I NEED!!!!
#set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)
#set(OpenCV_LIBS libopencv_core310 libopencv_imgcodecs310 libopencv_imgproc310 libopencv_xfeatures2d310 libopencv_features2d310 libopencv_highgui310)
#set(OpenCV_LIBS opencv_xfeatures2d)

# linking
target_link_libraries(hw2 ${OpenCV_LIBS})

####





