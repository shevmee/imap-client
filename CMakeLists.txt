
cmake_minimum_required(VERSION 3.28)

project(SmtpImapClient VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

# Find OpenSSL package
find_package(OpenSSL REQUIRED)

# Find Boost package with all requirements
find_package(Boost 1.70 REQUIRED COMPONENTS system coroutine)

# Add executable target
add_library(client_imap)

# Specify source files
target_sources(client_imap PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ImapClient.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

# Include directories for the client_imap target
target_include_directories(client_imap PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIRS}
)

# # Ad subdirectory for the ErrorHandlers library
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/Handlers)

# Add subdirectory for the Base64 library
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/Base64)

# Add subdirectory for the SmartSocket library
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/SmartSocket)

# Add subdirectory for the ImapResponse library
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/ImapResponse)

# Add subdirectory for the MailMessage library
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/MailMessage)

# Add subdirectory for the MessageSender library
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/MessageSender)

# Link libraries to the client_imap target
target_link_libraries(client_imap PRIVATE
    OpenSSL::SSL
    OpenSSL::Crypto
    SmartSocketI
    Base64I
    ${Boost_LIBRARIES}
    ImapResponseI
    MailMessageI
    MessageSenderI
)


