cmake_minimum_required(VERSION 3.1)

project(dip_anno
        LANGUAGES    CXX
        VERSION     0.0.1
)

set(CMAKE_MAKE_PROGRAM make)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(OpenCV REQUIRED)

message(STATUS "OpenCV library status:")
message(STATUS "    config: ${OpenCV_DIR}")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

INCLUDE_DIRECTORIES( /usr/include/opencv4 )

add_executable(dip_anno main.cpp)

target_link_libraries(dip_anno
    ${OpenCV_LIBS}
)
