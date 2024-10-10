#include "ImapClient.h"
#include "ImapResponse.h"

#include <iostream>
#include <boost/format.hpp>

namespace ISXICI
{
ImapClient::ImapClient(asio::io_context& io_context, asio::ssl::context& ssl_context)
    : m_smart_socket(std::make_unique<ISXSmartSocketI::SmartSocket>(io_context, ssl_context))
    , m_timeout(S_DEFAULT_TIMEOUT)
{
    m_smart_socket->SetTimeout(S_DEFAULT_TIMEOUT);
}

ImapClient::~ImapClient()
{
    try
    {
        if (m_smart_socket->IsOpen())
        {
            AsyncLogout().get();
        }

        delete m_smart_socket.release();
    }
    catch (const std::exception& e)
    {
        std::cerr << "IMAP Exception in destructor caught while logging out, session could have been broken: " << e.what() << std::endl;
    }
}

future<void> ImapClient::AsyncConnect(const string& server, std::uint16_t port)
{
    std::promise<void> promise;
    future<void> future = promise.get_future();

    asio::spawn(
        m_smart_socket->GetIoContext(),
        [this, server, port, promise = std::move(promise)](asio::yield_context yield) mutable
        {
            try
            {
                // Connect to the server (plain socket)
                m_smart_socket->AsyncConnectCoroutine(server, port, yield);
                
                m_smart_socket->AsyncWriteCoroutine("STARTTLS\r\n", yield);
                m_smart_socket->AsyncReadCoroutineI(yield);

                // Start the SSL handshake immediately after connecting
                m_smart_socket->AsyncUpgradeSecurityCoroutine(yield); // Ensure SSL handshake here

                // Once SSL is established, read the server's greeting (IMAP servers send a greeting)
                ISXResponseI::IMAPResponse::CheckStatus(
                    m_smart_socket->AsyncReadCoroutineI(yield), ISXResponseI::StatusType::OK);
                m_smart_socket->AsyncWriteCoroutine("A001 CAPABILITY\r\n", yield);
                m_smart_socket->AsyncReadCoroutineI(yield);
                promise.set_value();
            }
            catch (...)
            {
                promise.set_exception(std::current_exception());
            }
        }
    );

    return future;
}


future<void> ImapClient::AsyncLogin(const string& username, const string& password)
{
    std::promise<void> promise;
    future<void> future = promise.get_future();
    std::string temp = "A002";

    m_username = username;
    m_password = password;

    std::string query = (boost::format("%1% %2% %3% %4% \r\n")
        % temp
        % S_CMD_LOGIN
        % username
        % password).str();

    asio::spawn(
        m_smart_socket->GetIoContext(),
        [this, query, promise = std::move(promise)](asio::yield_context yield) mutable
        {
            try
            {
                //m_smart_socket->AsyncWriteCoroutine("A001 CAPABILITY", yield);
                //m_smart_socket->AsyncReadCoroutineI(yield);
                m_smart_socket->AsyncWriteCoroutine(query, yield);
                ISXResponseI::IMAPResponse::CheckStatus(
                    m_smart_socket->AsyncReadCoroutineI(yield), ISXResponseI::StatusType::OK);
                //m_smart_socket->AsyncReadCoroutineI(yield);
                promise.set_value();
            }
            catch (...)
            {
                std::cerr<<"LOGIN ERROR"<<std::endl;
                promise.set_exception(std::current_exception());
                std::cerr<<"LOGIN ERROR2"<<std::endl;
            }
        }
    );

    return future;
}

future<void> ImapClient::AsyncSelectFolder(const string& folder)
{
    std::promise<void> promise;
    future<void> future = promise.get_future();

    std::string query = (boost::format("SELECT %1%\r\n") % folder).str();

    asio::spawn(
        m_smart_socket->GetIoContext(),
        [this, query, folder, promise = std::move(promise)](asio::yield_context yield) mutable
        {
            try
            {
                m_smart_socket->AsyncWriteCoroutine(query, yield);
                ISXResponseI::IMAPResponse::CheckStatus(
                    m_smart_socket->AsyncReadCoroutineI(yield), ISXResponseI::StatusType::OK);

                AsyncSendSelectCmd(folder, yield); // Now 'folder' is captured

                promise.set_value();
            }
            catch (...)
            {
                std::cerr<<"ERROR SELECTFOLDER"<<std::endl;
                promise.set_exception(std::current_exception());
            }
        }
    );

    return future;
}

future<void> ImapClient::AsyncFetchMail(const std::uint32_t mail_index)
{
    std::promise<void> promise;
    future<void> future = promise.get_future();

    std::string query = (boost::format("%1% %2% BODY[] \r\n")
        % S_CMD_FETCH
        % mail_index).str();

    asio::spawn(
        m_smart_socket->GetIoContext(),
        [this, query, promise = std::move(promise), mail_index](asio::yield_context yield) mutable
        {
            try
            {
                AsyncSendFetchCmd(mail_index, yield);
                ISXResponseI::IMAPResponse::CheckStatus(
                    m_smart_socket->AsyncReadCoroutineI(yield), ISXResponseI::StatusType::OK);

                promise.set_value();
            }
            catch (...)
            {
                promise.set_exception(std::current_exception());
            }
        }
    );

    return future;
}

future<void> ImapClient::AsyncSearchMail(const string& criteria)
{
    std::promise<void> promise;
    future<void> future = promise.get_future();

    std::string query = (boost::format("SEARCH %1%\r\n") % criteria).str();

    asio::spawn(
        m_smart_socket->GetIoContext(),
        [this, query, criteria, promise = std::move(promise)](asio::yield_context yield) mutable
        {
            try
            {
                m_smart_socket->AsyncWriteCoroutine(query, yield);
                ISXResponseI::SMTPResponse::CheckStatus(
                    m_smart_socket->AsyncReadCoroutine(yield), ISXResponseI::StatusType::PositiveCompletion);

                AsyncSendSearchCmd(criteria, yield); // Now 'criteria' is captured

                promise.set_value();
            }
            catch (...)
            {
                promise.set_exception(std::current_exception());
            }
        }
    );

    return future;
}


future<void> ImapClient::AsyncLogout()
{
    std::promise<void> promise;
    future<void> future = promise.get_future();

    asio::spawn(
        m_smart_socket->GetIoContext(),
        [this, promise = std::move(promise)](asio::yield_context yield) mutable
        {
            try
            {
                AsyncSendLogoutCmd(yield);
                ISXResponseI::IMAPResponse::CheckStatus(
                    m_smart_socket->AsyncReadCoroutineI(yield), ISXResponseI::StatusType::OK);
                promise.set_value();
            }
            catch (...)
            {
                promise.set_exception(std::current_exception());
            }
        }
    );

    return future;
}

bool ImapClient::Reset()
{
    m_smart_socket = std::make_unique<ISXSmartSocketI::SmartSocket>(
        m_smart_socket->GetIoContext(), m_smart_socket->GetSslContext());

    return true;
}

bool ImapClient::Dispose()
{
    return m_smart_socket->Close();
}

bool ImapClient::ConnectionIsOpen()
{
    return m_smart_socket->IsOpen();
}

bool ImapClient::SetTimeout(int timeout)
{
    return m_smart_socket->SetTimeout(timeout);
}

bool ImapClient::AsyncSendLoginCmd(asio::yield_context& yield)
{
    std::string query = (boost::format("%1% %2% %3% %4% \r\n")
        % "a001"
        % S_CMD_LOGIN
        % m_username
        % m_password).str();

    return m_smart_socket->AsyncWriteCoroutine(query, yield);
}

bool ImapClient::AsyncSendLogoutCmd(asio::yield_context& yield)
{
    return m_smart_socket->AsyncWriteCoroutine(S_CMD_LOGOUT + "\r\n", yield);
}

bool ImapClient::AsyncSendSelectCmd(const string& folder, asio::yield_context& yield)
{
    std::string query = (boost::format("%1% %2% \r\n")
        % S_CMD_SELECT
        % folder).str();

    return m_smart_socket->AsyncWriteCoroutine(query, yield);
}

bool ImapClient::AsyncSendFetchCmd(const std::uint32_t mail_index, asio::yield_context& yield)
{
    std::string query = (boost::format("%1% %2% BODY[] \r\n")
        % S_CMD_FETCH
        % mail_index).str();

    return m_smart_socket->AsyncWriteCoroutine(query, yield);
}

bool ImapClient::AsyncSendSearchCmd(const string& criteria, asio::yield_context& yield)
{
    std::string query = (boost::format("%1% %2% \r\n")
        % S_CMD_SEARCH
        % criteria).str();

    return m_smart_socket->AsyncWriteCoroutine(query, yield);
}

bool ImapClient::AsyncSendNoopCmd(asio::yield_context& yield)
{
    return m_smart_socket->AsyncWriteCoroutine(S_CMD_NOOP + "\r\n", yield);
}
}; // namespace ISXIC
