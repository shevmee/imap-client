add_library(ImapResponseI STATIC
    src/ImapResponse.cpp
)

target_include_directories(ImapResponseI PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
