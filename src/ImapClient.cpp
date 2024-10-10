#include "ImapClient.h"
#include "ImapResponse.h"

#include <iostream>
#include <boost/format.hpp>

namespace ISXIC
{
ImapClient::ImapClient(asio::io_context& io_context, asio::ssl::context& ssl_context)
    : m_smart_socket(std::make_unique<ISXSmartSocket::SmartSocket>(io_context, ssl_context))
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
                
                // For our server
                // m_smart_socket->AsyncWriteCoroutine("STARTTLS\r\n", yield);
                // m_smart_socket->AsyncReadCoroutineI(yield);

                m_smart_socket->AsyncUpgradeSecurityCoroutine(yield); 

                // Once SSL is established, read the server's greeting (IMAP servers send a greeting)
                ISXResponse::IMAPResponse::CheckStatus(
                    m_smart_socket->AsyncReadCoroutineI(yield), ISXResponse::StatusType::OK);
                    
                AsyncSendCapabilityCmd(yield);
                IncrementTag();
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

    m_username = username;
    m_password = password;

    asio::spawn(
        m_smart_socket->GetIoContext(),
        [this, username, password, promise = std::move(promise)](asio::yield_context yield) mutable
        {
            try
            {
                AsyncSendLoginCmd(yield, username, password);
                ISXResponse::IMAPResponse::CheckStatus(
                    m_smart_socket->AsyncReadCoroutineI(yield), ISXResponse::StatusType::OK);

                IncrementTag();
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

future<void> ImapClient::AsyncSelectFolder(const string& folder)
{
    std::promise<void> promise;
    future<void> future = promise.get_future();

    //std::string query = (boost::format("A003 SELECT %1%\r\n") % folder).str();

    asio::spawn(
        m_smart_socket->GetIoContext(),
        [this, folder, promise = std::move(promise)](asio::yield_context yield) mutable
        {
            try
            {
                AsyncSendSelectCmd(folder, yield);

                ISXResponse::IMAPResponse::CheckStatus(
                    m_smart_socket->AsyncReadCoroutineI(yield), ISXResponse::StatusType::OK);

                IncrementTag();
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

future<void> ImapClient::AsyncFetchMail(const string& mail_index, const string& arg)
{
    std::promise<void> promise;
    future<void> future = promise.get_future();

    asio::spawn(
        m_smart_socket->GetIoContext(),
        [this, arg, promise = std::move(promise), mail_index](asio::yield_context yield) mutable
        {
            try
            {
                AsyncSendFetchCmd(mail_index, arg, yield);
                ISXResponse::IMAPResponse::CheckStatus(
                    m_smart_socket->AsyncReadCoroutineI(yield), ISXResponse::StatusType::OK);

                IncrementTag();
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
                ISXResponse::IMAPResponse::CheckStatus(
                    m_smart_socket->AsyncReadCoroutineI(yield), ISXResponse::StatusType::OK);
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

future<void> ImapClient::AsyncCapability()
{
    std::promise<void> promise;
    future<void> future = promise.get_future();

    asio::spawn(
        m_smart_socket->GetIoContext(),
        [this, promise = std::move(promise)](asio::yield_context yield) mutable
        {
            try
            {
                AsyncSendCapabilityCmd(yield);
                ISXResponse::IMAPResponse::CheckStatus(
                    m_smart_socket->AsyncReadCoroutineI(yield), ISXResponse::StatusType::OK);
                    
                IncrementTag();
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

future<void> ImapClient::AsyncBye()
{
    std::promise<void> promise;
    future<void> future = promise.get_future();

    asio::spawn(
        m_smart_socket->GetIoContext(),
        [this, promise = std::move(promise)](asio::yield_context yield) mutable
        {
            try
            {
                AsyncSendByeCmd(yield);
                ISXResponse::IMAPResponse::CheckStatus(
                    m_smart_socket->AsyncReadCoroutineI(yield), ISXResponse::StatusType::OK);
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

void ImapClient::IncrementTag(){
    std::string letter_part(1, m_tag[0]);
    std::string number_part = m_tag.substr(1);

    int number = 0;
    for (char c : number_part) {
        number = number * 10 + (c - '0');
    }

    number++;

    if (number > 999) {
        number = 0;
        letter_part[0]++;
    }

    std::string new_number_part = std::to_string(number);
    while (new_number_part.length() < number_part.length()) {
        new_number_part = '0' + new_number_part;
    }

    m_tag = letter_part + new_number_part;
}

bool ImapClient::Reset()
{
    m_smart_socket = std::make_unique<ISXSmartSocket::SmartSocket>(
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

bool ImapClient::AsyncSendLoginCmd(asio::yield_context& yield, const string& username, const string& password)
{
    std::string query = (boost::format("%1% %2% %3% %4% \r\n")
        % m_tag
        % S_CMD_LOGIN
        % username
        % password).str();

    return m_smart_socket->AsyncWriteCoroutine(query, yield);
}

bool ImapClient::AsyncSendLogoutCmd(asio::yield_context& yield)
{
    std::string query = (boost::format("%1% %2%\r\n")
        % m_tag
        % S_CMD_LOGOUT).str();

    return m_smart_socket->AsyncWriteCoroutine(query, yield);
}

bool ImapClient::AsyncSendByeCmd(asio::yield_context& yield)
{
    std::string query = (boost::format("%1% %2% \r\n")
        % m_tag
        % S_CMD_BYE).str();

    return m_smart_socket->AsyncWriteCoroutine(query, yield);
}

bool ImapClient::AsyncSendCapabilityCmd(asio::yield_context& yield)
{
    std::string query = (boost::format("%1% %2% \r\n")
        % m_tag
        % S_CMD_CAPABILITY).str();
    return m_smart_socket->AsyncWriteCoroutine(query, yield);
}

bool ImapClient::AsyncSendSelectCmd(const string& folder, asio::yield_context& yield)
{
    std::string query = (boost::format("%1% %2% %3%\r\n")
        % m_tag
        % S_CMD_SELECT
        % folder).str();

    return m_smart_socket->AsyncWriteCoroutine(query, yield);
}

bool ImapClient::AsyncSendFetchCmd(const string& mail_index, const string& arg, asio::yield_context& yield)
{
    // std::string query = (boost::format("%1% %2% %3% BODY[]\r\n")
    //     % m_tag
    //     % S_CMD_FETCH
    //     % mail_index).str();

    std::string query = (boost::format("%1% %2% %3% %4%\r\n")
        % m_tag
        % S_CMD_FETCH
        % mail_index
        % arg).str();


    return m_smart_socket->AsyncWriteCoroutine(query, yield);
}

}; // namespace ISXIC  ENVELOPE
