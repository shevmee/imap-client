
add_library(MailMessageI
    src/MailAddress.cpp
    src/MailAttachment.cpp
    src/MailMessageBuilder.cpp
    src/MailMessageFormatter.cpp)

target_include_directories(MailMessageI PUBLIC include)
