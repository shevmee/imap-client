#include "SmtpClient.h"
#include "SMTPResponse.h"
#include "MessageSender.h"

#include <iostream>

#include <boost/format.hpp>

namespace ISXSC
{
SmtpClient::SmtpClient(asio::io_context& io_context, asio::ssl::context& ssl_context)
    : m_smart_socket(std::make_unique<ISXSmartSocket::SmartSocket>(io_context, ssl_context))
    , m_timeout(S_DEFAULT_TIMEOUT)
{
    m_smart_socket->SetTimeout(S_DEFAULT_TIMEOUT);
};

SmtpClient::~SmtpClient()
{
    try
    {
        if (m_smart_socket->IsOpen())
        {
            AsyncQuit().get();
        };

        delete m_smart_socket.release();
    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception in destructor catched while quitting, session could have been broken: " << e.what() << std::endl;
    }
};

future<void> SmtpClient::AsyncConnect(const string& server, std::uint16_t port)
{
    std::promise<void> promise;
    future<void> future = promise.get_future();

    asio::spawn(
        m_smart_socket->GetIoContext()
        , [this, server, port, promise = std::move(promise)](asio::yield_context yield)
        mutable
        {
            try
            {
                m_smart_socket->AsyncConnectCoroutine(server, port, yield);
                ISXResponse::SMTPResponse::CheckStatus(
                    m_smart_socket->AsyncReadCoroutine(yield), ISXResponse::StatusType::PositiveCompletion);
            
                AsyncSendEhloCmd(yield);
                ISXResponse::SMTPResponse::CheckStatus(
                    m_smart_socket->AsyncReadCoroutine(yield), ISXResponse::StatusType::PositiveCompletion);
            
                AsyncSendStartTlsCmd(yield);
                ISXResponse::SMTPResponse::CheckStatus(
                    m_smart_socket->AsyncReadCoroutine(yield), ISXResponse::StatusType::PositiveCompletion);
            
                AsyncUpgradeSecurity(yield);

                promise.set_value();
            } catch (...)
            {
                promise.set_exception(std::current_exception());
            };
        }
    );
    
    return future;
};

future<void> SmtpClient::AsyncRegister(const string& username, const string& password)
{
    std::promise<void> promise;
    future<void> future = promise.get_future();

    std::string register_string = '\0' + username + '\0' + password;
    std::string encoded_register_string = ISXBase64::Base64Encode(register_string);

    std::string query = (format("%1% %2% \r\n")
        % S_CMD_REGISTER
        % encoded_register_string).str();

    asio::spawn(
        m_smart_socket->GetIoContext()
        , [this, username, password, query, promise = std::move(promise)](asio::yield_context yield)
        mutable
        {
            try
            {
                m_smart_socket->AsyncWriteCoroutine(query, yield);
                ISXResponse::SMTPResponse::CheckStatus(
                    m_smart_socket->AsyncReadCoroutine(yield), ISXResponse::StatusType::PositiveCompletion);

                promise.set_value();
            }
            catch(...)
            {
                promise.set_exception(std::current_exception());
            };
        }
    );

    return future;
};

future<void> SmtpClient::AsyncAuthenticate(const string& username, const string& password)
{
    std::promise<void> promise;
    future<void> future = promise.get_future();

    m_username = username;
    m_password = password;
    std::string auth_string = '\0' + username + '\0' + password;
    std::string encoded_auth_string = ISXBase64::Base64Encode(auth_string);

    std::string query = (format("%1% %2% \r\n")
        % S_CMD_AUTH_PLAIN
        % encoded_auth_string).str();

    asio::spawn(
        m_smart_socket->GetIoContext()
        , [this, username, password, query, promise = std::move(promise)](asio::yield_context yield)
        mutable
        {
            try
            {
                m_smart_socket->AsyncWriteCoroutine(query, yield);
                ISXResponse::SMTPResponse::CheckStatus(
                    m_smart_socket->AsyncReadCoroutine(yield), ISXResponse::StatusType::PositiveCompletion);

                promise.set_value();
            }
            catch(...)
            {
                promise.set_exception(std::current_exception());
            };
        }
    );

    return future;
};

future<void> SmtpClient::AsyncSendMail(const ISXMM::MailMessage& mail_message)
{
    std::promise<void> promise;
    future<void> future = promise.get_future();
    asio::spawn(
        m_smart_socket->GetIoContext()
        , [this, mail_message, promise = std::move(promise)](asio::yield_context yield)
        mutable
        {   
            try
            {
                AsyncSendMailFromCmd(mail_message.from, yield);
                ISXResponse::SMTPResponse::CheckStatus(
                    m_smart_socket->AsyncReadCoroutine(yield), ISXResponse::StatusType::PositiveCompletion);

                for (auto &group : {mail_message.to, mail_message.cc, mail_message.bcc})
                {
                    for (auto& to : group)
                    {
                        AsyncSendRcptToCmd(to, yield);
                        ISXResponse::SMTPResponse::CheckStatus(
                            m_smart_socket->AsyncReadCoroutine(yield), ISXResponse::StatusType::PositiveCompletion);
                    }
                }

                AsyncSendDataCmd(yield);
                ISXResponse::SMTPResponse::CheckStatus(
                    m_smart_socket->AsyncReadCoroutine(yield), ISXResponse::StatusType::PositiveIntermediate);

                ISXMS::MessageSender message_sender(mail_message, [&](const string& query)
                {
                    return m_smart_socket->AsyncWriteCoroutine(query, yield);
                });
                message_sender.SendMess();

                m_smart_socket->AsyncWriteCoroutine("\r\n.\r\n", yield);
                ISXResponse::SMTPResponse::CheckStatus(
                    m_smart_socket->AsyncReadCoroutine(yield), ISXResponse::StatusType::PositiveCompletion);
                
                promise.set_value();
            } 
            catch(...)
            {
                promise.set_exception(std::current_exception());
            };
        }
    );
    
    return future;
};

future<void> SmtpClient::AsyncQuit()
{
    std::promise<void> promise;
    future<void> future = promise.get_future();

    asio::spawn(
        m_smart_socket->GetIoContext()
        , [this, promise = std::move(promise)](asio::yield_context yield)
        mutable
        {
            try
            {
                AsyncSendQuitCmd(yield);
                ISXResponse::SMTPResponse::CheckStatus(
                    m_smart_socket->AsyncReadCoroutine(yield), ISXResponse::StatusType::PositiveCompletion);
                promise.set_value();
            }
            catch(...)
            {
                promise.set_exception(std::current_exception());
            };
        }
    );

    return future;
};

bool SmtpClient::Reset()
{
    m_smart_socket = std::make_unique<ISXSmartSocket::SmartSocket>(
        m_smart_socket->GetIoContext(), m_smart_socket->GetSslContext());

    return true;
};

bool SmtpClient::Dispose()
{
    return m_smart_socket->Close();
};

bool SmtpClient::ConnectionIsOpen()
{
    return m_smart_socket->IsOpen();
};

bool SmtpClient::SetTimeout(int timeout)
{
    return m_smart_socket->SetTimeout(timeout);
};

bool SmtpClient::AsyncSendEhloCmd(asio::yield_context& yield)
{
    string query = (format("%1% %2%:%3% \r\n")
        % S_CMD_EHLO
        % m_smart_socket->GetLocalname()
        % m_smart_socket->GetLocalPort()).str();

    return m_smart_socket->AsyncWriteCoroutine(query, yield);   
};

bool SmtpClient::AsyncSendStartTlsCmd(asio::yield_context& yield)
{
    return m_smart_socket->AsyncWriteCoroutine(S_CMD_STARTTLS + "\r\n", yield);
};

bool SmtpClient::AsyncUpgradeSecurity(asio::yield_context& yield)
{
    return m_smart_socket->AsyncUpgradeSecurityCoroutine(yield);
};

bool SmtpClient::AsyncSendMailFromCmd(const ISXMM::MailAddress& mail_address, asio::yield_context& yield)
{
    string query = (format("%1%: <%2%> \r\n")
        % S_CMD_MAIL_FROM
        % mail_address.get_address()).str();

    return m_smart_socket->AsyncWriteCoroutine(query, yield);
};

bool SmtpClient::AsyncSendRcptToCmd(const ISXMM::MailAddress& mail_address, asio::yield_context& yield)
{

    string query = (format("%1%: <%2%> \r\n")
        % S_CMD_RCPT_TO
        % mail_address.get_address()).str();
            

    return m_smart_socket->AsyncWriteCoroutine(query, yield);
}

bool SmtpClient::AsyncSendDataCmd(asio::yield_context& yield)
{
    return m_smart_socket->AsyncWriteCoroutine(S_CMD_DATA + "\r\n", yield);
};

bool SmtpClient::AsyncSendQuitCmd(asio::yield_context& yield)
{
    return m_smart_socket->AsyncWriteCoroutine(S_CMD_QUIT + "\r\n", yield);
};
}; // namespace ISXSC