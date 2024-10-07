#include "MessageSender.h"

namespace ISXMS
{

MessageSender::MessageSender(const ISXMM::MailMessage& message, std::function<bool(const std::string&)> send)
    : m_message(message), m_send(send) {}

bool MessageSender::SendMessage()
{
    return SendMailHeaders() &&
           SendMailBody() &&
           SendAttachments();
}

bool MessageSender::SendFile(const std::filesystem::path& file_path)
{
    bool result = true;

    std::ifstream file;
    std::vector<char> buffer(S_FILE_CHUNK_SIZE);
    file.open(file_path);

    if (!file) return false;
    if (std::filesystem::file_size(file_path) > ISXMM::MailAttachment::S_MAX_SIZE) return false;

    while (!file.eof())
    {
        file.read(buffer.data(), buffer.size());
        std::string encoded_buffer = ISXBase64::Base64Encode(std::string(buffer.begin(), buffer.begin() + file.gcount())) + "\r\n";
        result &= m_send(encoded_buffer);
    };
    return result;
};

bool MessageSender::SendAttachments()
    {
        bool result = true;

        if (m_message.attachments.empty())
            return true;
            
        for (auto& attachment : m_message.attachments)
        {
            m_send("--" + ISXMM::MailMessageFormatter::boundary + "\r\n");
            m_send(ISXMM::MailMessageFormatter::MailAttachmentHeaders(attachment));
            result &= SendFile(attachment.get_path());
        };
        result &= m_send("--" + ISXMM::MailMessageFormatter::boundary + "--\r\n");

        return result;
    };

bool MessageSender::SendMailHeaders()
{
     return m_send(ISXMM::MailMessageFormatter::MailHeaders(m_message));
}

bool MessageSender::SendMailBody()
{
    return m_send(ISXMM::MailMessageFormatter::MailBody(m_message));
}

}; // namespace ISXMS