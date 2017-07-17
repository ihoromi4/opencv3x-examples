
cmake_minimum_required(VERSION 2.8)

set(TARGET VideoCapture2Edges)
set(SOURCES)

project(${TARGET})

find_package(OpenCV COMPONENTS imgproc highgui REQUIRED)

add_executable(${TARGET} ${SOURCES})

target_link_libraries(${TARGET} ${OpenCV_LIBS})

