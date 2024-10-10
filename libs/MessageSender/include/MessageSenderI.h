#include <functional>
#include <filesystem>
#include <fstream>

#include "MailMessageFormatterI.h"
#include "Base64I.h"



namespace ISXMSI
{

class MessageSender
{
public:
    MessageSender(const ISXMMI::MailMessage& message, std::function<bool(const std::string&)> send);

    bool SendMess();
private:
    static uint16_t inline S_FILE_CHUNK_SIZE = 72;
    ISXMMI::MailMessage m_message;
    std::function<bool(const std::string&)> m_send;

    bool SendMailHeaders();
    bool SendMailBody();
    bool SendAttachments();

    bool SendFile(const std::filesystem::path& path);
};

}; // namespace ISXMS