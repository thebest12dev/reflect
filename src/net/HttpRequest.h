#pragma once
#include "TypeDefinitions.h"
#ifdef _WIN32
#include <WS2tcpip.h>
#include <WinSock2.h>
#endif
#include <string>

namespace reflect {
enum HttpRequestMethod { HTTP_GET, HTTP_POST, HTTP_PUT, HTTP_DELETE };
class HttpRequest {
private:
  std::string url;
  HttpRequestMethod method;
  std::string headers;
  std::string body;
  bool success;
  std::string response;

public:
  REFLECT_API
  HttpRequest(const std::string &url,
              HttpRequestMethod method = HttpRequestMethod::HTTP_GET,
              const std::string &headers = "", const std::string &body = "");
  REFLECT_API void setUrl(const std::string &newUrl);
  REFLECT_API void setMethod(HttpRequestMethod newMethod);
  REFLECT_API void setHeaders(const std::string &newHeaders);
  REFLECT_API void setBody(const std::string body);
  REFLECT_API std::string getUrl() const;
  REFLECT_API HttpRequestMethod getMethod() const;
  REFLECT_API std::string getHeaders() const;
  REFLECT_API std::string getBody() const;
  REFLECT_API void initiateRequest();
  REFLECT_API std::string getResponse();
  REFLECT_API std::string getErrorMessage() const;
  REFLECT_API int getStatusCode() const;
  REFLECT_API bool isSuccess() const;
  REFLECT_API void setTimeout(int seconds);
  REFLECT_API void setProxy(const std::string &proxyUrl);
  REFLECT_API void setProxyPort(int port);
  REFLECT_API void setProxyCredentials(const std::string &username,
                                       const std::string &password);
  REFLECT_API void setFollowRedirects(bool follow);
  REFLECT_API void setUserAgent(const std::string &userAgent);
  REFLECT_API void setAccept(const std::string &accept);
  REFLECT_API void setContentType(const std::string &contentType);
  REFLECT_API void setAcceptEncoding(const std::string &acceptEncoding);
  REFLECT_API void setAcceptLanguage(const std::string &acceptLanguage);
  REFLECT_API void setKeepAlive(bool keepAlive);
};
} // namespace reflect