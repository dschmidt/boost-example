#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/streambuf.hpp>

typedef void (*ReadContentCallback)(boost::asio::streambuf& response);

class HttpClient
{
public:
    HttpClient(boost::asio::io_service& io_service, const std::string& server, const std::string& path, ReadContentCallback);

private:
    void handleReadContent(const boost::system::error_code& err);
    void handleReadStatusLine(const boost::system::error_code& err);
    void handleWriteRequest(const boost::system::error_code& err);
    void handleConnect(const boost::system::error_code& err);
    void handleResolve(const boost::system::error_code& err, boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

    boost::asio::ip::tcp::resolver resolver_;
    boost::asio::ip::tcp::socket socket_;
    boost::asio::streambuf request_;
    boost::asio::streambuf response_;

    ReadContentCallback readContentCallback_;
};

#endif // HTTPCLIENT_H
