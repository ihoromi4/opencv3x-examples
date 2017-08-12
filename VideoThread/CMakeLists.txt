cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 11)

set(TARGET VideoThread)
set(SOURCES main.cpp)

project(${TARGET})

find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED)

add_executable(${TARGET} ${SOURCES})

target_link_libraries(${TARGET} ${OpenCV_LIBS})
target_link_libraries(${TARGET} Threads::Threads)
