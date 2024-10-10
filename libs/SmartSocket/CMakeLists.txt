cmake_minimum_required(VERSION 3.0)

project(SmartSocketI VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

add_library(SmartSocketI STATIC)

target_sources(SmartSocketI PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SmartSocket.cpp
)

target_include_directories(SmartSocketI PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(SmartSocketI PUBLIC
    ImapResponseI
)
if(WIN32 AND MINGW)
    message("Windows and mingw detected")
    find_package(OpenSSL REQUIRED)

    target_link_libraries(SmartSocketI PUBLIC 
        OpenSSL::SSL
        OpenSSL::Crypto
        ws2_32
    )
endif()