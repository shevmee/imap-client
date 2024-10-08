#include "ImapClient.h"
#include "SmartSocket.h"

#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include <memory>
#include <thread>

using std::cout;
void finish(asio::io_context& io_context, std::thread& worker, std::unique_ptr<ISXIC::ImapClient>& imap_client);

int main()
{
    // Initialize the io_context and ssl_context
    boost::asio::io_context io_context;
    boost::asio::ssl::context ssl_context(boost::asio::ssl::context::tls_client);
    ssl_context.set_verify_mode(boost::asio::ssl::verify_none);
    
    // Run io_context in a separate thread
    std::thread worker([&io_context]() {
        asio::io_context::work work(io_context);
        io_context.run();
    });

    std::unique_ptr<ISXIC::ImapClient> imap_client = std::make_unique<ISXIC::ImapClient>(io_context, ssl_context);

    try
    {
        // Connect and authenticate to the IMAP server
        imap_client->AsyncConnect("imap.gmail.com", 993).get();
        imap_client->AsyncLogin("yehorfur@gmail.com", "\"bwhp mwfr utdv udcj\"").get();
        std::cout<<"LOL"<<std::endl;

        // Select the INBOX folder
        
        //imap_client->AsyncSelectFolder("INBOX").get();

        // // Fetch mail with index 1 (just as an example)
        // imap_client->AsyncFetchMail(1).get();

        // // Optionally, search for specific emails
        // imap_client->AsyncSearchMail("UNSEEN").get();

        finish(io_context, worker, imap_client);

    } catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        finish(io_context, worker, imap_client);
        return 1;
    };

    return 0;
}

void finish(asio::io_context& io_context, std::thread& worker, std::unique_ptr<ISXIC::ImapClient>& imap_client)
{
    delete imap_client.release();

    if (!io_context.stopped())
        io_context.stop();

    if (worker.joinable())
        worker.join();
}