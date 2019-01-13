cmake_minimum_required(VERSION 2.8)

project(capture)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(cv_capture capture.cpp)

SET(GCC_ADDITIONAL_FLAGS "-std=c++11 -g")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_ADDITIONAL_FLAGS}" )

target_link_libraries(cv_capture ${OpenCV_LIBS})
