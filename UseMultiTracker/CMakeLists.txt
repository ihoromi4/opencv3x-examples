cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDARD 11)
set(TARGET UseMultiTracker)
set(SOURCE_FILES main.cpp)

project(${TARGET})

find_package(OpenCV REQUIRED)

add_executable(${TARGET} ${SOURCE_FILES})

target_link_libraries(${TARGET} ${OpenCV_LIBS})
