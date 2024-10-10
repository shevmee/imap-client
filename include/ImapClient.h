#pragma once

#include "SmartSocket.h"
#include "MailMessage.h"

#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <memory>
#include <string>
#include <future>

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
    future<void> AsyncFetchMail(const std::uint32_t mail_index);
    future<void> AsyncSearchMail(const string& criteria);
    future<void> AsyncLogout();
    std::string test() { return "IMAP server";}

    bool Reset();
    bool Dispose();
    bool ConnectionIsOpen();
    bool SetTimeout(int timeout);

private:
    unique_ptr<ISXSmartSocketI::SmartSocket> m_smart_socket;

    string m_username;
    string m_password;
    string m_current_folder;

    int m_timeout;

    bool AsyncSendLoginCmd(asio::yield_context& yield);
    bool AsyncSendLogoutCmd(asio::yield_context& yield);
    bool AsyncSendSelectCmd(const string& folder, asio::yield_context& yield);
    bool AsyncSendFetchCmd(const std::uint32_t mail_index, asio::yield_context& yield);
    bool AsyncSendSearchCmd(const string& criteria, asio::yield_context& yield);
    bool AsyncSendNoopCmd(asio::yield_context& yield);
};
}; // namespace ISXIC
