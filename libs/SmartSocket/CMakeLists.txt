cmake_minimum_required(VERSION 3.0)

project(SmartSocket VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

add_library(SmartSocket STATIC)

target_sources(SmartSocket PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SmartSocket.cpp
)

target_include_directories(SmartSocket PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(SmartSocket PUBLIC
    SMTPResponse
    ImapResponse
)