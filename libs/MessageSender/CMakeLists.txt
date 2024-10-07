add_library(MessageSender
    src/MessageSender.cpp
)

target_include_directories(MessageSender PUBLIC include)

target_link_libraries(MessageSender
    MailMessage
    Base64
)