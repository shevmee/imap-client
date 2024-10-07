#include <gtest/gtest.h>
#include "MailAttachment.h"
#include <filesystem>

TEST(MailAttachmentTest, GetPath) {
    std::filesystem::path test_path = "/path/to/attachment.txt";
    ISXMM::MailAttachment attachment(test_path);
    
    EXPECT_EQ(attachment.get_path(), test_path);
}

TEST(MailAttachmentTest, GetName) {
    std::filesystem::path test_path = "/path/to/attachment.txt";
    ISXMM::MailAttachment attachment(test_path);
    
    EXPECT_EQ(attachment.get_name(), "attachment.txt");
}

TEST(MailAttachmentTest, GetNameNoExtension) {
    std::filesystem::path test_path = "/path/to/attachment";
    ISXMM::MailAttachment attachment(test_path);
    
    EXPECT_EQ(attachment.get_name(), "attachment");
}

TEST(MailAttachmentTest, GetNameWithSpecialCharacters) {
    std::filesystem::path test_path = "/path/to/@ttachment!.txt";
    ISXMM::MailAttachment attachment(test_path);
    
    EXPECT_EQ(attachment.get_name(), "@ttachment!.txt");
}
