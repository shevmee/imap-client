#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/spawn.hpp>

#include <string>
#include <sstream>
#include <regex>
#include <future>
#include <iostream>

#include "SMTPResponse.h"
#include "ImapResponse.h"

using namespace boost;

using std::string;
using asio::ip::tcp;
using std::function;

namespace ISXSmartSocketI
{
//class MethodsHandlers;

class SmartSocket
{
public:
    SmartSocket(asio::io_context& io_context, asio::ssl::context& ssl_context);
    ~SmartSocket();
    
    bool IsOpen() const;
    string GetLocalname() const;
    int GetLocalPort() const;
    string GetServername() const;
    int GetServerPort() const;

    bool SetTimeout(int timeout);

    asio::io_context& GetIoContext();
    asio::ssl::context& GetSslContext();

    bool AsyncConnectCoroutine(const string& server, int port, asio::yield_context& yield);
    bool AsyncWriteCoroutine(const string& data, asio::yield_context& yield);
    ISXResponseI::SMTPResponse AsyncReadCoroutine(asio::yield_context& yield);
    ISXResponseI::IMAPResponse AsyncReadCoroutineI(asio::yield_context& yield);
    bool AsyncUpgradeSecurityCoroutine(asio::yield_context& yield);
    bool Close();

    friend class MethodsHandlers;
    
private:
    std::unique_ptr<asio::steady_timer> StartTimer(
        int seconds, asio::yield_context& yield, system::error_code& ec);
        
    string m_server;
    int m_port;

    bool m_ssl_enabled;

    int m_timeout = 10;

    tcp::resolver m_resolver;
    asio::io_context& m_io_context;
    asio::ssl::context& m_ssl_context;
    asio::ssl::stream<tcp::socket> m_socket;

};

class MethodsHandlers
{
public:
    static bool HandleConnection(
        const string& server, const int port
        , const boost::system::error_code& error_code);

    static bool HandleRemoteEndpointOp(
        const boost::system::error_code& error_code);

    static bool HandleWrite(
        const string& data
        , const boost::system::error_code& error_code);

    static ISXResponseI::SMTPResponse HandleRead(
        boost::asio::streambuf& buffer
        , const boost::system::error_code& error_code);

    static ISXResponseI::IMAPResponse HandleReadI(
        boost::asio::streambuf& buffer
        , const boost::system::error_code& error_code);

    static bool HandleClose(
        const boost::system::error_code& error_code
        , bool* ssl_toggle);

    static bool HandleUpgradeSecurity(
        const boost::system::error_code& error_code
        , bool* ssl_toggle);
private:
    static inline void HandleError(
        const string& prefix, const boost::system::error_code& error_code);
    
    static bool LogIfTimeout(
        const boost::system::error_code& error_code);
        
    MethodsHandlers() = delete;
    ~MethodsHandlers() = delete;

    MethodsHandlers(const MethodsHandlers&) = delete;
    MethodsHandlers& operator=(const MethodsHandlers&) = delete;

    MethodsHandlers(MethodsHandlers&&) = delete;
    MethodsHandlers& operator=(MethodsHandlers&&) = delete;

    static inline std::ostream* s_log_stream = &std::clog;
};
}; // namespace ISXSmartSocket