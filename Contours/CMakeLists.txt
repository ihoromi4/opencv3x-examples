
cmake_minimum_required(VERSION 2.8)

set(TARGET Contourus)
set(SOURCES main.cpp)

project(${TARGET})

find_package(OpenCV REQUIRED)

add_executable(${TARGET} ${SOURCES})

target_link_libraries(${TARGET} ${OpenCV_LIBS})

