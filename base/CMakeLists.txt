cmake_minimum_required(VERSION 3.0.0)

# 查找MongoDB C++驱动（可选）
find_package(mongocxx QUIET)

# 设置源文件
set(BASE_SOURCES
    FileWatcher.cpp
    LocalFileReader.cpp
)

if(mongocxx_FOUND)
    list(APPEND BASE_SOURCES MongoDBFileReader.cpp)
endif()

# 创建库
add_library(base ${BASE_SOURCES})

# 设置包含目录
target_include_directories(base PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# 链接MongoDB库（如果找到）
if(mongocxx_FOUND)
    target_link_libraries(base mongocxx::mongocxx)
endif()