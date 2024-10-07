#include <gtest/gtest.h>
#include "MailMessageBuilder.h"
#include "MailAddress.h"
#include "MailMessage.h"
#include "MailAttachment.h"

TEST(MailMessageBuilderTest, SetFrom) {
    ISXMM::MailMessageBuilder builder;
    builder.set_from("sender@example.com", "Sender Name")
        .add_to("placeholder");
    
    ISXMM::MailMessage message = builder.Build();
    EXPECT_EQ(message.from.get_address(), "sender@example.com");
    EXPECT_EQ(message.from.get_name(), "Sender Name");
}

TEST(MailMessageBuilderTest, AddTo) {
    ISXMM::MailMessageBuilder builder;
    builder.set_from("sender@example.com", "Sender Name")
           .add_to("recipient@example.com", "Recipient Name");
    
    ISXMM::MailMessage message = builder.Build();
    EXPECT_EQ(message.to.size(), 1);
    EXPECT_EQ(message.to.at(0).get_address(), "recipient@example.com");
    EXPECT_EQ(message.to.at(0).get_name(), "Recipient Name");
}

TEST(MailMessageBuilderTest, AddCc) {
    ISXMM::MailMessageBuilder builder;
    builder.set_from("sender@example.com", "Sender Name")
           .add_cc("cc@example.com", "Cc Name")
           .add_to("placeholder");
    
    ISXMM::MailMessage message = builder.Build();
    EXPECT_EQ(message.cc.size(), 1);
    EXPECT_EQ(message.cc.at(0).get_address(), "cc@example.com");
    EXPECT_EQ(message.cc.at(0).get_name(), "Cc Name");
}

TEST(MailMessageBuilderTest, AddBcc) {
    ISXMM::MailMessageBuilder builder;
    builder.set_from("sender@example.com", "Sender Name")
           .add_bcc("bcc@example.com", "Bcc Name")
           .add_to("placeholder");
    
    ISXMM::MailMessage message = builder.Build();
    EXPECT_EQ(message.bcc.size(), 1);
    EXPECT_EQ(message.bcc.at(0).get_address(), "bcc@example.com");
    EXPECT_EQ(message.bcc.at(0).get_name(), "Bcc Name");
}

TEST(MailMessageBuilderTest, SetSubject) {
    ISXMM::MailMessageBuilder builder;
    builder.set_from("sender@example.com", "Sender Name")
           .set_subject("Test Subject")
           .add_to("placeholder");
    
    ISXMM::MailMessage message = builder.Build();
    EXPECT_EQ(message.subject, "Test Subject");
}

TEST(MailMessageBuilderTest, SetBody) {
    ISXMM::MailMessageBuilder builder;
    builder.set_from("sender@example.com", "Sender Name")
           .set_body("Test Body")
           .add_to("placeholder");
    
    ISXMM::MailMessage message = builder.Build();
    EXPECT_EQ(message.body, "Test Body");
}

TEST(MailMessageBuilderTest, AddAttachment) {
    ISXMM::MailMessageBuilder builder;
    builder.set_from("sender@example.com", "Sender Name")
           .add_attachment("/path/to/attachment.txt")
           .add_to("placeholder");
    
    ISXMM::MailMessage message = builder.Build();
    EXPECT_EQ(message.attachments.size(), 1);
    EXPECT_EQ(message.attachments.at(0).get_path(), "/path/to/attachment.txt");
}

TEST(MailMessageBuilderTest, BuildThrowsIfFromNotSet) {
    ISXMM::MailMessageBuilder builder;
    builder.add_to("recipient@example.com", "Recipient Name");
    
    EXPECT_THROW(builder.Build(), std::runtime_error);
}

TEST(MailMessageBuilderTest, BuildThrowsIfToNotSet) {
    ISXMM::MailMessageBuilder builder;
    builder.set_from("sender@example.com", "Sender Name");
    
    EXPECT_THROW(builder.Build(), std::runtime_error);
}