find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

# 所有cpp文件
file(GLOB leetcode_src
     "*.cpp"
)

add_executable(leetcode_test ${leetcode_src})

target_link_libraries(leetcode_test
    leetcode 
    ${GTEST_BOTH_LIBRARIES}
    pthread
)

gtest_discover_tests(leetcode_test)