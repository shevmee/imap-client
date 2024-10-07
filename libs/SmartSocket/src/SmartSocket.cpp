#include "SmartSocket.h"

#include <iostream>
#include <boost/asio/steady_timer.hpp>
#include <memory>

namespace ISXSmartSocket
{
SmartSocket::SmartSocket(asio::io_context& io_context, asio::ssl::context& ssl_context)
    : m_io_context(io_context)
    , m_ssl_context(ssl_context)
    , m_resolver(io_context)
    , m_socket(io_context, ssl_context)
    , m_ssl_enabled(false) {};

SmartSocket::~SmartSocket()
{
    try
    {
        Close();
    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception in destructor called" << std::endl;
    }
};

bool SmartSocket::IsOpen() const
{
    return m_socket.next_layer().is_open();
};

string SmartSocket::GetLocalname() const
{
    return m_socket.next_layer().local_endpoint().address().to_string();
};

int SmartSocket::GetLocalPort() const
{
    return m_socket.next_layer().local_endpoint().port();
};

string SmartSocket::GetServername() const
{
    system::error_code ec;
    string server_name = m_socket.next_layer().remote_endpoint(ec).address().to_string();
    MethodsHandlers::HandleRemoteEndpointOp(ec);
    return server_name;
};

int SmartSocket::GetServerPort() const
{
    system::error_code ec;
    int server_port = m_socket.next_layer().remote_endpoint(ec).port();
    MethodsHandlers::HandleRemoteEndpointOp(ec);
    return server_port;
};

asio::io_context& SmartSocket::GetIoContext()
{
    return m_io_context;
};

asio::ssl::context& SmartSocket::GetSslContext()
{
    return m_ssl_context;
};

bool SmartSocket::SetTimeout(int timeout)
{
    m_timeout = timeout;
    return true;
};

bool SmartSocket::AsyncConnectCoroutine(const string& server, int port, asio::yield_context& yield)
{
    m_server = server;
    m_port = port;

    system::error_code ec;

    auto timer = StartTimer(m_timeout, yield, ec);

    tcp::resolver::query query(m_server, std::to_string(m_port));
    tcp::resolver::results_type results = m_resolver.async_resolve(query, yield[ec]);
    asio::async_connect(m_socket.next_layer(), results.begin(), results.end(), yield[ec]);
    
    timer->cancel();
    
    return MethodsHandlers::HandleConnection(GetServername(), GetServerPort(), ec);
};

bool SmartSocket::AsyncWriteCoroutine(const string& data, asio::yield_context& yield)
{
    system::error_code ec;
    
    auto timer = StartTimer(m_timeout, yield, ec);

    if (!m_ssl_enabled)
    {
    std::cout<<"KORUTINA1"<<std::endl;
        asio::async_write(m_socket.next_layer(), asio::buffer(data), yield[ec]);
    } else
    {
    std::cout<<"KORUTINA2"<<std::endl;
        asio::async_write(m_socket, asio::buffer(data), yield[ec]);
    };

    timer->cancel();
    std::cout<<"KORUTINA"<<std::endl;
    return MethodsHandlers::HandleWrite(data, ec);
};

ISXResponse::SMTPResponse SmartSocket::AsyncReadCoroutine(asio::yield_context& yield)
{
    system::error_code ec;
    asio::streambuf buffer;

    auto timer = StartTimer(m_timeout, yield, ec);

    if (!m_ssl_enabled)
    {
        asio::async_read_until(m_socket.next_layer(), buffer, "\r\n", yield[ec]);   
    } else
    {
        asio::async_read_until(m_socket, buffer, "\r\n", yield[ec]);   
    };

    timer->cancel();
    
    return MethodsHandlers::HandleRead(buffer, ec);
};

ISXResponse::IMAPResponse SmartSocket::AsyncReadCoroutineI(asio::yield_context& yield)
{
    system::error_code ec;
    asio::streambuf buffer;

    auto timer = StartTimer(m_timeout, yield, ec);

    if (!m_ssl_enabled)
    {
        asio::async_read_until(m_socket.next_layer(), buffer, "\r\n", yield[ec]);   
    } else
    {
        asio::async_read_until(m_socket, buffer, "\r\n", yield[ec]);   
    };

    timer->cancel();

    std::cout<<"QWEQWE2"<<std::endl;
    
    return MethodsHandlers::HandleReadI(buffer, ec);
};

bool SmartSocket::AsyncUpgradeSecurityCoroutine(asio::yield_context& yield)
{
    system::error_code ec;
    m_socket.async_handshake(boost::asio::ssl::stream_base::handshake_type::client, yield[ec]);
    m_ssl_enabled = true;
    return MethodsHandlers::HandleUpgradeSecurity(ec, &m_ssl_enabled);
};

bool SmartSocket::Close()
{
    system::error_code ec;
    m_socket.lowest_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
    m_socket.lowest_layer().cancel(ec);
    m_socket.lowest_layer().close(ec);
    return MethodsHandlers::HandleClose(ec, &m_ssl_enabled);
};

std::unique_ptr<asio::steady_timer> SmartSocket::StartTimer(
    int seconds, asio::yield_context& yield, system::error_code& ec)
{
    std::unique_ptr<asio::steady_timer> timer = std::make_unique<asio::steady_timer>(yield.get_executor());
    timer->expires_after(std::chrono::seconds(seconds));
    timer->async_wait([&](const system::error_code& error) {
        if (!error) {
            if (!m_ssl_enabled) {
                m_socket.next_layer().cancel();
            } else {
                m_socket.lowest_layer().cancel();
            };
        }
    });

    return timer;
};

void MethodsHandlers::HandleError(
    const string& prefix, const boost::system::error_code& error_code)
{
    *s_log_stream << prefix << ": " << error_code.message() << std::endl;
    throw std::runtime_error(error_code.message());
};

bool MethodsHandlers::LogIfTimeout(const boost::system::error_code& error_code)
{
    if (error_code == boost::asio::error::operation_aborted)
    {
        *s_log_stream << "Log: Timeout maybe reached" << std::endl;
        return true;
    }
    else
    {
        *s_log_stream << "Log: Unhandled error - " << error_code.message() << std::endl;
        return false;
    }
}

bool MethodsHandlers::HandleConnection(
    const string& server, const int port
    , const boost::system::error_code& error_code)
{
    if (!error_code)
    {
        *s_log_stream << "Log: " << "Successfully connected to " << server << ":" << port << std::endl;
        return true;
    }

    HandleError("Connection error", error_code);
    return false;
};

bool MethodsHandlers::HandleRemoteEndpointOp(
    const boost::system::error_code& error_code)
{
    if (!error_code)
    {
        return true;
    }

    HandleError("RemoteEndpoint error", error_code);
    return false;
};


bool MethodsHandlers::HandleWrite(
    const string& data
    , const boost::system::error_code& error_code)
{
    if (!error_code)
    {
        *s_log_stream << "C: " << data;
        return true;
    }

    LogIfTimeout(error_code);
    HandleError("Write error", error_code);
    return false;
};

ISXResponse::SMTPResponse MethodsHandlers::HandleRead(
    boost::asio::streambuf& buffer
    , const boost::system::error_code& error_code)
{
    if (error_code && error_code != boost::asio::error::operation_aborted)
    {
        HandleError("Reading error 1", error_code);
    } else if (error_code == boost::asio::error::operation_aborted)
    {
        HandleError("Reading error 12", boost::asio::error::timed_out);
    };

    if(!error_code)
    {
        std::stringstream response;
        std::copy(
            boost::asio::buffers_begin(buffer.data()),
            boost::asio::buffers_end(buffer.data()),
            std::ostream_iterator<char>(response)
        );
        
        ISXResponse::SMTPResponse smtp_response(response.str());
        *s_log_stream << smtp_response.get_formated_response();

        return smtp_response;
    };

    LogIfTimeout(error_code);
    HandleError("Reading error", error_code);
};

ISXResponse::IMAPResponse MethodsHandlers::HandleReadI(
    boost::asio::streambuf& buffer
    , const boost::system::error_code& error_code)
{
    std::cout<<"ZXCZXC"<<std::endl;
    std::istream is(&buffer);
    std::stringstream response;
    response<<is.rdbuf();
    std::cout<<response.str()<<std::endl;
    if (error_code && error_code != boost::asio::error::operation_aborted)
    {
        HandleError("Reading error", error_code);
    } else if (error_code == boost::asio::error::operation_aborted)
    {
        HandleError("Reading error", boost::asio::error::timed_out);
    };
    if(!error_code)
    {
        ISXResponse::IMAPResponse imap_response(response.str());
        *s_log_stream << imap_response.get_formatted_response();
        std::cout<<"OKOKOK1 "<<imap_response.get_formatted_response()<<std::endl;
        return imap_response;
    };

    LogIfTimeout(error_code);
    HandleError("Reading error", error_code);
};

bool MethodsHandlers::HandleClose(
    const boost::system::error_code& error_code
    , bool* ssl_toggle)
{
    if (!error_code)
    {
        *s_log_stream << "Log: " << "Connection closed" << std::endl;
        *ssl_toggle = false;
        return true;
    }

    HandleError("Close error", error_code);
    return false;
};

bool MethodsHandlers::HandleUpgradeSecurity(
    const boost::system::error_code& error_code
    , bool* ssl_toggle)
{
    if (!error_code)
    {
        *s_log_stream << "Log: " << "Handshake successful. Connection upgraded to TLS" << std::endl;
        *ssl_toggle = true;
        return true;
    }

    *ssl_toggle = false;
    HandleError("Update security error", error_code);
    return false;
};
}; // namespace ISXSmartSocket