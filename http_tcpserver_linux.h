#ifndef INCLUDED_HTTP_TCPSERVER_LINUX
#define INCLUDED_HTTP_TCPSERVER_LINUX

#include <stdio.h>
#include <stdlib.h>
#include <string>

// network APIs
#include <arpa/inet.h>
#include <sys/socket.h>

namespace http {

class TCPServer {
public:
  TCPServer(std::string ip_address, int port);
  ~TCPServer();

  void startListen();

private:
  std::string m_ip_address;
  int m_port;
  int m_socket;
  int m_new_socket;
  long incoming_message;
  struct sockaddr_in m_socketAddress;
  unsigned int m_socketAddress_len;
  std::string m_server_message;

  int startServer();
  void closeServer();
  void acceptConnection(int &new_socket);
  std::string buildResponse();
  void sendResponse();
};
} // namespace http

#endif // !INCLUDED_HTTP_TCPSERVER_LINUX
