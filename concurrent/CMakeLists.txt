cmake_minimum_required(VERSION 3.0.0)

aux_source_directory(. DIR_CONCURRENT_SRCS)

add_library(concurrent ${DIR_CONCURRENT_SRCS}) 