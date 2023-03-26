cmake_minimum_required(VERSION 3.24)
project(untitled)

set(CMAKE_CXX_STANDARD 17)

find_package(PCL 1.7 REQUIRED)
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})


find_package(CUDA REQUIRED)

set(CUDA_NVCC_FLAGS "-arch=sm_86")

cuda_add_executable(example  kernel.cu)

target_link_libraries(example ${CUDA_LIBRARIES})

add_executable(pcd_write_test main.cpp)
target_link_libraries(pcd_write_test ${PCL_LIBRARIES})
