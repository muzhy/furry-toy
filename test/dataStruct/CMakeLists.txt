find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 17)

include_directories(${GTEST_INCLUDE_DIRS})

include_directories("${PROJECT_SOURCE_DIR}/dataStruct")

# 所有cpp文件
file(GLOB dataStruct_test_src
     "*.cpp"
)

add_executable(dataStruct_test ${dataStruct_test_src})

target_link_libraries(dataStruct_test
    ${GTEST_BOTH_LIBRARIES}
    pthread 
)

gtest_discover_tests(dataStruct_test)