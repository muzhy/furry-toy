cmake_minimum_required(VERSION 3.0.0)

aux_source_directory(. DIR_NETWORK_SRCS)

add_library(network ${DIR_NETWORK_SRCS}) 

target_include_directories(network
    PUBLIC ${PROJECT_SOURCE_DIR}
    PUBLIC ${PROJECT_SOURCE_DIR}/error
)