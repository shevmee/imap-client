#include <functional>
#include <filesystem>
#include <fstream>

#include "MailMessageFormatter.h"
#include "Base64.h"



namespace ISXMS
{

class MessageSender
{
public:
    MessageSender(const ISXMM::MailMessage& message, std::function<bool(const std::string&)> send);

    bool SendMessage();
private:
    static uint16_t inline S_FILE_CHUNK_SIZE = 72;
    ISXMM::MailMessage m_message;
    std::function<bool(const std::string&)> m_send;

    bool SendMailHeaders();
    bool SendMailBody();
    bool SendAttachments();

    bool SendFile(const std::filesystem::path& path);
};

}; // namespace ISXMS