cmake_minimum_required(VERSION 3.1)
project(mymuduo)

# mymuduo 设置动态库路径 根目录lib文件夹下
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# 设置调试信息
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")

message(STATUS "SRC_LIST: ${SRC_LIST}")
#定义参与编译的源代码文件
aux_source_directory(. SRC_LIST)

# 编译动态库
add_library(mymuduo SHARED ${SRC_LIST})