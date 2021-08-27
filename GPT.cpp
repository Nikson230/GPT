#include <iostream>
#include <string>
#include <chrono>

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif
#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

int main(){
  asio::error_code ec;

  asio::io_context context;

  asio::ip::tcp::endpoint endpoint(asio::ip::make_address("76.76.21.21", ec), 80);

  asio::ip::tcp::socket socket(context);

  socket.connect(endpoint);

  if(!ec){
    std::cout << "Connected!" << std::endl;
  } else {
    std::cout << "Failed to connect to address:\n" << ec.message() << std::endl;
  }

  if(socket.is_open()){
    std::string request = "GET /index.html HTTP/1.1\r\n"
      "Host: gas.metasync.app\r\n"
      "Connection: close\r\n\r\n";

    socket.write_some(asio::buffer(request.data(), request.size()), ec);

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(20000ms);

    size_t bytes = socket.available();
    std::cout << "Bytes available: " << bytes << std::endl;

    if(bytes > 0){
      std::vector<char> buffer(bytes);
      socket.read_some(asio::buffer(buffer.data(), buffer.size()), ec);

      for(auto c : buffer)
	std::cout << c;
    }
  }

  system("PAUSE");
  return 0;
}
