add_library(MessageSenderI
    src/MessageSender.cpp
)

target_include_directories(MessageSenderI PUBLIC include)

target_link_libraries(MessageSenderI
    MailMessageI
    Base64I
)