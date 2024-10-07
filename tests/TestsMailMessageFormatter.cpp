#include <gtest/gtest.h>
#include "MailMessageFormatter.h"

TEST(MailMessageFormatter, FormatMessage)
{
    ISXMM::MailMessage message;
    message.from = { "tom.doe@gmail.com", "Tom Doe" };
    std::string expected = "From: Tom Doe <tom.doe@gmail.com>\r\n";

    EXPECT_EQ(ISXMM::MailMessageFormatter::MailFrom(message.from), expected);
}

TEST(MailMessageFormatter, FormatMessageWithMultipleRecipients)
{
    ISXMM::MailMessage message;
    message.to = { { "john.doe@gmail.com", "John Doe" }, { "tom.doe@gmail.com" }, { "alice.doe@gmail.com", "Alice Doe" } };
    std::string expected = "To: John Doe <john.doe@gmail.com>, tom.doe@gmail.com, Alice Doe <alice.doe@gmail.com>\r\n";

    EXPECT_EQ(ISXMM::MailMessageFormatter::MailTo(message.to), expected);
}

TEST(MailMessageFormatter, FormatMessageWithCc)
{
    ISXMM::MailMessage message;
    message.cc = { { "john.doe@gmail.com", "John Doe" }, { "tom.doe@gmail.com" }, { "alice.doe@gmail.com", "Alice Doe" } };
    std::string expected = "Cc: John Doe <john.doe@gmail.com>, tom.doe@gmail.com, Alice Doe <alice.doe@gmail.com>\r\n";

    EXPECT_EQ(ISXMM::MailMessageFormatter::MailCc(message.cc), expected);
}

TEST(MailMessageFormatter, FormatMessageHeaders)
{
    ISXMM::MailMessage message;
    message.attachments = { { "file1.txt" }, { "file2.txt" } };

    std::string expect_first = "Content-Type: application/octet-stream; name=\"file1.txt\"\r\n"
                               "Content-Transfer-Encoding: base64\r\n"
                               "Content-Disposition: attachment; filename=\"file1.txt\"\r\n" "\r\n";
                            

    std::string expect_second = "Content-Type: text/plain; name=\"file2.txt\"\r\n"
                                "Content-Transfer-Encoding: base64\r\n"
                                "Content-Disposition: attachment; filename=\"file2.txt\"\r\n" "\r\n";
                            


    EXPECT_EQ(ISXMM::MailMessageFormatter::MailAttachmentHeaders(message.attachments[0]), expect_first);
    EXPECT_EQ(ISXMM::MailMessageFormatter::MailAttachmentHeaders(message.attachments[1], "text/plain"), expect_second);
}



