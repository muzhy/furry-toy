find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

add_executable(helloTest helloTest.cpp)

target_link_libraries(helloTest
    ${GTEST_BOTH_LIBRARIES}
    pthread
)

add_subdirectory(leetcode)
add_subdirectory(dataStruct)

gtest_discover_tests(helloTest dataStruct_test)