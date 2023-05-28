#include "http_tcpserver_linux.h"

#include <iostream>
#include <sstream>
#include <unistd.h>

const int BUFFER_SIZE = 30720;
namespace logger {

void log(const std::string &message) { std::cout << message << std::endl; }

void exitWithError(const std::string &error_message) {
  logger::log("ERROR: " + error_message);
  exit(1);
}
} // namespace logger

namespace http {

TCPServer::TCPServer(std::string ip_address, int port)
    : m_ip_address(ip_address), m_port(port), m_socket(), incoming_message(),
      m_new_socket(), m_socketAddress(),
      m_socketAddress_len(sizeof(m_socketAddress)),
      m_server_message(buildResponse()) {

  m_socketAddress.sin_family = AF_INET;
  m_socketAddress.sin_port = htons(m_port);
  m_socketAddress.sin_addr.s_addr = inet_addr(m_ip_address.c_str());

  if (startServer() != 0) {
    std::ostringstream ss;
    ss << "Failed to start server with PORT: "
       << ntohs(m_socketAddress.sin_port);
    logger::log(ss.str());
  }
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

void TCPServer::startListen() {
  if (listen(m_socket, 20) < 0) {
    logger::exitWithError("Socket listen failed");
  }

  std::ostringstream ss;
  ss << "\n*** Listening on ADDRESS: " << inet_ntoa(m_socketAddress.sin_addr)
     << " PORT: " << ntohs(m_socketAddress.sin_port) << " ***\n\n";
  logger::log(ss.str());

  int bytesReceived;

  while (true) {
    logger::log("====== Waiting for a new connection ======\n\n\n");
    acceptConnection(m_new_socket);

    char buffer[BUFFER_SIZE] = {0};
    bytesReceived = read(m_new_socket, buffer, BUFFER_SIZE);
    if (bytesReceived < 0) {
      logger::exitWithError(
          "Failed to read bytes from client socket connection");
    }

    std::ostringstream ss;
    ss << "------ Received Request from client ------\n\n";
    logger::log(ss.str());

    sendResponse();

    close(m_new_socket);
  }
}

void TCPServer::acceptConnection(int &new_socket) {
  new_socket =
      accept(m_socket, (sockaddr *)&m_socketAddress, &m_socketAddress_len);
  if (new_socket < 0) {
    std::ostringstream ss;
    ss << "Server failed to accept incoming connection from ADDRESS: "
       << inet_ntoa(m_socketAddress.sin_addr)
       << "; PORT: " << ntohs(m_socketAddress.sin_port);
    logger::exitWithError(ss.str());
  }
}

std::string TCPServer::buildResponse() {
  std::string htmlFile =
      "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from "
      "your Server :) </p></body></html>";
  std::ostringstream ss;
  ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: "
     << htmlFile.size() << "\n\n"
     << htmlFile;

  return ss.str();
}

void TCPServer::sendResponse() {
  long bytesSent;

  bytesSent =
      write(m_new_socket, m_server_message.c_str(), m_server_message.size());

  if (bytesSent == m_server_message.size()) {
    logger::log("------ Server Response sent to client ------\n\n");
  } else {
    logger::log("Error sending response to client");
  }
}

} // namespace http
