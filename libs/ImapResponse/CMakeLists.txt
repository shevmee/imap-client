add_library(ImapResponse STATIC
    src/ImapResponse.cpp
)

target_include_directories(ImapResponse PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
