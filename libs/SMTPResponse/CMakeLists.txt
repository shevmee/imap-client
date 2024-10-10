
add_library(SMTPResponseI STATIC
    src/SMTPResponse.cpp
)

target_include_directories(SMTPResponseI PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)