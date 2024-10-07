
add_library(SMTPResponse STATIC
    src/SMTPResponse.cpp
)

target_include_directories(SMTPResponse PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)