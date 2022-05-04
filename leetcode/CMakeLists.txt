cmake_minimum_required(VERSION 3.0.0)
project(furry_toy VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

aux_source_directory(. DIR_LEETCODE_SRCS)

add_library(leetcode ${DIR_LEETCODE_SRCS}) 