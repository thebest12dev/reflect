#include "HttpRequest.h"
#include "Console.h"
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <regex>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
/**
 * @brief Internal namespace, used for private functions and variables.
 */
namespace {
/**
 * @brief Extracts the host and port from a URL.
 * @param url The URL to extract from.
 * @return A tuple containing the host and port.
 */
std::tuple<std::string, int> extractHostAndPort(const std::string &url) {
  // Regular expression to match the URL structure
  std::regex urlRegex(R"(^(https?)://([^:/]+)(?::(\d+))?(/.*)?$)");
  std::smatch matches;

  if (std::regex_match(url, matches, urlRegex)) {
    // Extract protocol (http or https)
    std::string protocol = matches[1];
    // Extract hostname
    std::string host = matches[2];
    // Extract port if available, otherwise use default port
    int port = (matches[3].length() > 0) ? std::stoi(matches[3])
                                         : (protocol == "https" ? 443 : 80);
    return std::make_tuple(host, port);
  } else {
    // If the URL doesn't have the protocol, treat it as "http"
    return std::make_tuple(url, 80);
  }
}
SSL_CTX *ctx;
SSL *ssl;

/**
 * @brief Initializes the socket and SSL library.
 */
class SocketInitializer {
public:
  SocketInitializer() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
      CinnamonToast::Console::error("failed to initialize winsock",
                                    "SocketInitializer");
      exit(1);
    }
    SSL_library_init();
    ctx = SSL_CTX_new(TLS_client_method());
    ssl = SSL_new(ctx);
  }
  ~SocketInitializer() {
    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    WSACleanup();
  }
};
// Initialize the socket library and SSL context
SocketInitializer internalInitializer;
} // namespace
namespace CinnamonToast {
HttpRequest::HttpRequest(const std::string &url, HttpRequestMethod method,
                         const std::string &headers, const std::string &body)
    : url(url), method(method), headers(headers), body(body), success(false) {}
void HttpRequest::setUrl(const std::string &newUrl) { url = newUrl; }
void HttpRequest::setMethod(HttpRequestMethod newMethod) { method = newMethod; }
void HttpRequest::setHeaders(const std::string &newHeaders) {
  headers = newHeaders;
}
bool HttpRequest::isSuccess() const { return success; }
void HttpRequest::setBody(const std::string newBody) { body = newBody; }
std::string HttpRequest::getUrl() const { return url; }
HttpRequestMethod HttpRequest::getMethod() const { return method; }
std::string HttpRequest::getHeaders() const { return headers; }
std::string HttpRequest::getBody() const { return body; }

std::string HttpRequest::getResponse() { return response; }
void HttpRequest::initiateRequest() {
  success = true;
  SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock == INVALID_SOCKET) {
    success = false;
    return;
  }

  std::tuple<std::string, uint16_t> values = extractHostAndPort(url);
  std::string host = std::get<0>(values);
  uint16_t port = std::get<1>(values);

  struct addrinfo hints, *result = nullptr;
  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  int iResult =
      getaddrinfo(host.c_str(), std::to_string(port).c_str(), &hints, &result);
  if (iResult != 0) {
    closesocket(sock);
    success = false;
    return;
  }

  // Attempt to connect to an address until one succeeds
  for (struct addrinfo *ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
    iResult = connect(sock, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
      closesocket(sock);
      sock = INVALID_SOCKET;
      continue;
    }
    break;
  }

  freeaddrinfo(result);

  if (sock == INVALID_SOCKET) {
    std::cerr << "Unable to connect to server." << std::endl;
    WSACleanup();
    return;
  }

  std::stringstream request;
  // Prepare the GET request
  request << "GET " +
                 std::regex_replace(url, std::regex("^[a-zA-Z]+://[^/]+"), "") +
                 " HTTP/1.1\r\n"
          << "Host: " + host + "\r\n"
          << "Connection: close\r\n"
          << "\r\n";
  if (url.rfind("https://", 0) == 0) {
    // If the URL starts with "https://", use SSL
    if (SSL_set_fd(ssl, sock) == 0) {
      Console::error("cannot set socket with SSL!", "initiateRequest");
      success = false;
      closesocket(sock);
      return;
    }
    if (SSL_connect(ssl) <= 0) {
      Console::error("SSL connection failed!", "initiateRequest");
      success = false;
      closesocket(sock);
      return;
    }
    SSL_write(ssl, request.str().c_str(), strlen(request.str().c_str()));
    char buffer[1024];
    int bytesReceived;
    std::string data;
    while ((bytesReceived = SSL_read(ssl, buffer, sizeof(buffer) - 1)) > 0) {
      buffer[bytesReceived] = '\0'; // Null-terminate the string
      data.append(buffer);
      std::memset(buffer, 0, sizeof(buffer));
    }
    this->response = data;
    closesocket(sock);
    this->response = data;
  } else {

    // Send the GET request
    if (send(sock, request.str().c_str(), request.str().length(), 0) ==
        SOCKET_ERROR) {
      success = false;
      closesocket(sock);
      WSACleanup();
      return;
    }

    int bytesReceived;
    char buffer[1024];
    std::string data;
    // Receive the response
    while ((bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
      buffer[bytesReceived] = '\0'; // Null-terminate the string
      data.append(buffer);
      std::memset(buffer, 0, sizeof(buffer));
    }

    if (bytesReceived == SOCKET_ERROR) {
      success = false;
    }

    // Close the socket
    closesocket(sock);
    this->response = data;
  }
}
// void HttpRequest::initiateRequest() {
//   SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//   struct sockaddr_in serverAddr;
//   struct hostent *host;
//   if (sock == INVALID_SOCKET) {
//     std::cerr << "Socket creation failed." << std::endl;
//     WSACleanup();
//     return;
//   }
//   std::tuple<std::string,uint16_t> values = extractHostAndPort(url);
//   // Resolve the server address and port
//   host = gethostbyname(std::get<0>(values).c_str());
//
//   if (host == NULL) {
//     std::cerr << "Host not found." << std::endl;
//     closesocket(sock);
//     WSACleanup();
//     return;
//   }
//
//   // Fill in server address
//   serverAddr.sin_family = AF_INET;
//   serverAddr.sin_port = htons(std::get<1>(values)); // HTTP port
//   serverAddr.sin_addr.s_addr = *(unsigned long *)host->h_addr_list[0];
//
//   // Connect to the server
//   if (connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) ==
//       SOCKET_ERROR) {
//     std::cerr << "Connection failed." << std::endl;
//     closesocket(sock);
//     WSACleanup();
//     return;
//   }
//   std::stringstream request;
//   // Prepare the GET request
//   request << "GET " + std::regex_replace(url,
//   std::regex("^[a-zA-Z]+://[^/]+"), "") + " HTTP/1.1\r\n"
//           << "Host: " + std::get<0>(values) + "\r\n"
//           << "Connection: close\r\n"
//           << "\r\n";
//
//   // Send the GET request
//   if (send(sock, request.str().c_str(), request.str().length(), 0) ==
//       SOCKET_ERROR) {
//     std::cerr << "Send failed." << std::endl;
//     closesocket(sock);
//     WSACleanup();
//     return;
//   }
//   int bytesReceived;
//   char buffer[1024];
//   std::string data;
//   // Receive the response
//   while ((bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
//     buffer[bytesReceived] = '\0'; // Null-terminate the string
//     data.append(buffer);
//     std::memset(buffer, 0, sizeof(buffer));
//   }
//
//   if (bytesReceived == SOCKET_ERROR) {
//     std::cerr << "Receive failed." << std::endl;
//   }
//
//   // Close the socket
//   closesocket(sock);
//   this->response = data;
// } // namespace CinnamonToast
} // namespace CinnamonToast