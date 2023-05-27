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

private:
  std::string ip_address;
  int port;
  int m_socket;
  int m_new_socket;
  long incoming_message;
  struct sockaddr_in m_sockaddr;
  unsigned int m_sockaddr_len;
  std::string m_server_message;

  int startServer();
  void closeServer();
};
} // namespace http

#endif // !INCLUDED_HTTP_TCPSERVER_LINUX
