#include "httpclient.h"

#include <boost/asio/io_service.hpp>
#include <boost/asio/write.hpp>

//WIP
#include <boost/asio.hpp>

#include <iostream>
#include <istream>
#include <ostream>
#include <string>

HttpClient::HttpClient(boost::asio::io_service& io_service, const std::string& server, const std::string& path)
    :   resolver_(io_service)
    ,   socket_(io_service)
{
    std::ostream request_stream(&request_);
    request_stream << "GET " << path << " HTTP/1.0\r\n";
    request_stream << "Host: " << server << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";

    boost::asio::ip::tcp::resolver::query query(server, "http");

    resolver_.async_resolve(query, [this](const boost::system::error_code& err, boost::asio::ip::tcp::resolver::iterator it) {
        handleResolve(err, it);
    });
}

void HttpClient::handleReadContent(const boost::system::error_code& err)
{
  if (!err)
  {
    // Write all of the data that has been read so far.
    std::cout << &response_;

    // Continue reading remaining data until EOF.
    boost::asio::async_read(socket_, response_,
        boost::asio::transfer_at_least(1),
        [this](const boost::system::error_code& err, std::size_t bytes_transferred) {
            handleReadContent(err);
        }
    );
  }
  else if (err != boost::asio::error::eof)
  {
    std::cout << "Error: " << err << "\n";
  }
}

void HttpClient::handleReadStatusLine(const boost::system::error_code& err)
 {
   if (!err)
   {
     // Check that response is OK.
     std::istream response_stream(&response_);
     std::string http_version;
     response_stream >> http_version;
     unsigned int status_code;
     response_stream >> status_code;
     std::string status_message;
     std::getline(response_stream, status_message);
     if (!response_stream || http_version.substr(0, 5) != "HTTP/")
     {
       std::cout << "Invalid response\n";
       return;
     }
     if (status_code != 200)
     {
       std::cout << "Response returned with status code ";
       std::cout << status_code << "\n";
       return;
     }

     // Read the response headers, which are terminated by a blank line.
     boost::asio::async_read_until(socket_, response_, "\r\n\r\n", [this](const boost::system::error_code& err, std::size_t bytes_transferred) {
        handleReadContent(err);
     });
   }
   else
   {
     std::cout << "Error: " << err << "\n";
   }
}


void HttpClient::handleWriteRequest(const boost::system::error_code& err)
{
    if (!err)
    {
      boost::asio::async_read_until(socket_, response_, "\r\n", [this](const boost::system::error_code& err, std::size_t bytes_transferred) {
            handleReadStatusLine(err);
      });
    }
    else
    {
      std::cout << "Error: " << err.message() << "\n";
    }
}

void HttpClient::handleConnect(const boost::system::error_code& err)
{
    if (!err)
    {
      // The connection was successful. Send the request.
      boost::asio::async_write(socket_, request_, [this](const boost::system::error_code& err, std::size_t bytes_transferred) {
            handleWriteRequest(err);
      });
    }
    else
    {
      std::cout << "Error: " << err.message() << "\n";
    }
}

void HttpClient::handleResolve(const boost::system::error_code& err, boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
{
    if (!err)
    {
      boost::asio::async_connect(socket_, endpoint_iterator, [this](const boost::system::error_code& err, boost::asio::ip::tcp::resolver::iterator iterator) {
        handleConnect(err);
      });
    }
    else
    {
      std::cout << "Error: " << err.message() << "\n";
    }
}
