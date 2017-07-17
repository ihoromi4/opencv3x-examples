
cmake_minimum_required(VERSION 2.8)

project(LoadModifySaveImage)

find_package(OpenCV REQUIRED)

add_executable(LoadModifySaveImage main.cpp)

target_link_libraries(LoadModifySaveImage ${OpenCV_LIBS})

