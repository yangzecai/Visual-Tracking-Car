# cmake 最低版本号要求
cmake_minimum_required(VERSION 2.8)

# 项目信息
project("Visual-Tracking-Car")

# 编译参数
# SET(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -std=c++11")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -std=c++11")
add_definitions(-std=c++11)
# 设置可执行文件生成路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

find_package(OpenCV REQUIRED)

# 设置头文件路径列表
set(INC_LIST
    ${CMAKE_SOURCE_DIR}/inc
    ${OpenCV_INCLUDE_DIRS}
)

# 包含头文件
include_directories(${INC_LIST})

# 添加cmake子目录
add_subdirectory(src)

# 添加执行文件
add_executable(Visual-Tracking-Car ${PROJECT_SOURCE_DIR}/main.cpp)

# 添加链接
target_link_libraries(Visual-Tracking-Car
    source-objects
    ${OpenCV_LIBS}
)