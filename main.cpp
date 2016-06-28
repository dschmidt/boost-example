#include "httpclient.h"

#include <boost/asio/io_service.hpp>

#include <iostream>


int main(int argc, char* argv[])
{
  try
  {
    if (argc != 3)
    {
      std::cout << "Usage: async_client <server> <path>\n";
      std::cout << "Example:\n";
      std::cout << "  async_client www.boost.org /LICENSE_1_0.txt\n";
      return 1;
    }

    const std::string& server = argv[1];
    const std::string& path = argv[2];

    boost::asio::io_service io_service;

    HttpClient client(io_service, server, path);

    io_service.run();
  }
  catch (std::exception& e)
  {
    std::cout << "Exception: " << e.what() << "\n";
  }

  return 0;
}
