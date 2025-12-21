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

# 检查MongoDB
find_package(mongocxx QUIET)

add_executable(FileWatcher_test FileWatcher_test.cpp)

target_link_libraries(FileWatcher_test gtest_main)
target_link_libraries(FileWatcher_test base)

add_executable(LocalFileReader_test LocalFileReader_test.cpp)

target_link_libraries(LocalFileReader_test gtest_main)
target_link_libraries(LocalFileReader_test base)

gtest_discover_tests(FileWatcher_test)
gtest_discover_tests(LocalFileReader_test)