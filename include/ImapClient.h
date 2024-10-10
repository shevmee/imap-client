#pragma once

#include "SmartSocket.h"
#include "MailMessage.h"

#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <memory>
#include <string>
#include <future>
#include <vector>

using namespace boost;
using std::string;
using std::unique_ptr;
using std::future;

using asio::ip::tcp;

namespace ISXICI
{
class ImapClient
{
public:
    inline static const string S_CMD_LOGIN = "LOGIN";
    inline static const string S_CMD_LOGOUT = "LOGOUT";
    inline static const string S_CMD_CAPABILITY = "CAPABILITY";
    inline static const string S_CMD_BYE = "BYE";
    inline static const string S_CMD_SELECT = "SELECT";
    inline static const string S_CMD_FETCH = "FETCH";
    inline static const string S_CMD_STORE = "STORE";
    inline static const string S_CMD_SEARCH = "SEARCH";
    inline static const string S_CMD_LIST = "LIST";
    inline static const string S_CMD_NOOP = "NOOP";

    inline static const std::uint8_t S_DEFAULT_TIMEOUT = 15;

    ImapClient(asio::io_context& io_context, asio::ssl::context& ssl_context);
    ~ImapClient();

    future<void> AsyncConnect(const string& server, std::uint16_t port);
    future<void> AsyncLogin(const string& username, const string& password);
    future<void> AsyncSelectFolder(const string& folder);
    future<void> AsyncFetchMail(const string& mail_index, const string& arg);
    future<void> AsyncLogout();
    future<void> AsyncBye();
    future<void> AsyncCapability();

    std::string test() { return "imap";}

    bool Reset();
    bool Dispose();
    bool ConnectionIsOpen();
    bool SetTimeout(int timeout);

private:
    unique_ptr<ISXSmartSocketI::SmartSocket> m_smart_socket;

    string m_username;
    string m_password;
    string m_current_folder;
    string m_tag = "A000";

    void IncrementTag();

    int m_timeout;

    bool AsyncSendLoginCmd(asio::yield_context& yield, const string& username, const string& password);
    bool AsyncSendLogoutCmd(asio::yield_context& yield);
    bool AsyncSendByeCmd(asio::yield_context& yield);
    bool AsyncSendCapabilityCmd(asio::yield_context& yield);
    bool AsyncSendSelectCmd(const string& folder, asio::yield_context& yield);
    bool AsyncSendFetchCmd(const string& mail_index, const string& arg, asio::yield_context& yield);
    std::vector<string> getInbox();
};
}; // namespace ISXIC
