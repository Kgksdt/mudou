cmake_minimum_required(VERSION 3.10)
project(test_project)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 14)

# 添加包含目录，指向项目的 include 文件夹
include_directories(../) # 使编译器能够找到 Timestamp.h

# 添加库的路径
link_directories(../lib)

# 查找源文件
set(SOURCES testTimestamp.cc)

# 创建可执行文件
add_executable(test_project ${SOURCES})

# 链接共享库
target_link_libraries(test_project mymuduo)
