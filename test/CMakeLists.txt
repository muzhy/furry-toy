include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

include_directories("${CMAKE_SOURCE_DIR}/base")

message("-- CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")

add_executable(helloTest helloTest.cpp)

target_link_libraries(helloTest gtest_main)

add_executable(FileWatcher_test FileWatcher_test.cpp ../base/FileWatcher.cpp)

target_link_libraries(FileWatcher_test gtest_main)

add_subdirectory(leetcode)
add_subdirectory(dataStruct)

gtest_discover_tests(helloTest dataStruct_test FileWatcher_test)