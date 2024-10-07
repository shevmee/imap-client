cmake_minimum_required(VERSION 3.0)

project(Base64 VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

# Add library target
add_library(Base64 STATIC)

# Specify source files
target_sources(Base64 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Base64.cpp
)

# Include directories for the SmartSocket target
target_include_directories(Base64 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)