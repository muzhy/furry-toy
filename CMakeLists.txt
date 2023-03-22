cmake_minimum_required(VERSION 3.0.0)
project(furry_toy VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

# 添加源文件夹路径
aux_source_directory(. DIRSRCS)
# 包含头文件目录
include_directories("${PROJECT_SOURCE_DIR}/leetcode")
include_directories("${PROJECT_SOURCE_DIR}/base")
include_directories("${PROJECT_SOURCE_DIR}/error")

# 子文件夹
add_subdirectory(leetcode)
add_subdirectory(test)
add_subdirectory(base)

include(CTest)
enable_testing()

add_executable(furry_toy main.cpp)
add_executable(tcp_client tcp_client.cpp)

# 添加链接库
target_link_libraries(furry_toy base leetcode)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
