#include "http_tcpServer_linux.h"

#include <iostream>
#include <unistd.h>

namespace logger {

void log(const std::string &message) { std::cout << message << std::endl; }

void exitWithError(const std::string &error_message) {
  logger::log("ERROR: " + error_message);
  exit(1);
}
} // namespace logger

namespace http {

TCPServer::TCPServer(std::string ip_address, int port)
    : ip_address(ip_address), port(port), m_socket(), incoming_message(),
      m_new_socket(), m_sockaddr(), m_sockaddr_len(sizeof(m_sockaddr)),
      m_server_message() {
  startServer();
}

TCPServer::~TCPServer() { closeServer(); }

int TCPServer::startServer() {

  // @ref: https://man7.org/linux/man-pages/man2/socket.2.html
  m_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (m_socket < 0) {
    logger::exitWithError("Cannot create socket!");
    return 1;
  }
  return 0;
}

void TCPServer::closeServer() {
  close(m_socket);
  close(m_new_socket);
  exit(0);
}

} // namespace http
