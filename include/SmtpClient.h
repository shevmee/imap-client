#pragma once

#include "MailMessage.h"
#include "SmartSocket.h"

#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>

#include <memory>

using namespace boost;
using std::string;
using std::unique_ptr;
using std::future;

using asio::ip::tcp;

namespace ISXSC
{
class SmtpClient
{
public:
    inline static const string S_CMD_EHLO = "EHLO";
    inline static const string S_CMD_STARTTLS = "STARTTLS";
    inline static const string S_CMD_AUTH_PLAIN = "AUTH PLAIN";
    inline static const string S_CMD_REGISTER = "REGISTER";
    inline static const string S_CMD_MAIL_FROM = "MAIL FROM";
    inline static const string S_CMD_RCPT_TO = "RCPT TO";
    inline static const string S_CMD_DATA = "DATA";
    inline static const string S_CMD_QUIT = "QUIT";

    inline static const std::uint8_t S_DEFAULT_TIMEOUT = 5;

    SmtpClient(asio::io_context& io_context, asio::ssl::context& ssl_context);
    ~SmtpClient();

    future<void> AsyncConnect(const string& server, std::uint16_t port);
    future<void> AsyncRegister(const string& username, const string& password);
    future<void> AsyncAuthenticate(const string& username, const string& password);
    future<void> AsyncSendMail(const ISXMM::MailMessage& mail_message);
    future<void> AsyncQuit();

    bool Reset();

    bool Dispose();
    bool ConnectionIsOpen();
    bool SetTimeout(int timeout);

private:

    std::unique_ptr<ISXSmartSocket::SmartSocket> m_smart_socket;

    string m_username;
    string m_password;

    int m_timeout;

    bool AsyncSendMailFromCmd(const ISXMM::MailAddress& mail_message, asio::yield_context& yield);
    bool AsyncSendRcptToCmd(const ISXMM::MailAddress& mail_address, asio::yield_context& yield);
    bool AsyncSendDataCmd(asio::yield_context& yield);
    bool AsyncSendQuitCmd(asio::yield_context& yield);

    bool AsyncSendEhloCmd(asio::yield_context& yield);
    bool AsyncSendStartTlsCmd(asio::yield_context& yield);
    bool AsyncUpgradeSecurity(asio::yield_context& yield);
};
}; // namespace ISXSC