#include "TypeDefinitions.h"
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <string>

namespace CinnamonToast {
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
  CTOAST_API HttpRequest(const std::string &url,
                         HttpRequestMethod method = HTTP_GET,
                         const std::string &headers = "",
                         const std::string &body = "");
  CTOAST_API void setUrl(const std::string &newUrl);
  CTOAST_API void setMethod(HttpRequestMethod newMethod);
  CTOAST_API void setHeaders(const std::string &newHeaders);
  CTOAST_API void setBody(const std::string body);
  CTOAST_API std::string getUrl() const;
  CTOAST_API HttpRequestMethod getMethod() const;
  CTOAST_API std::string getHeaders() const;
  CTOAST_API std::string getBody() const;
  CTOAST_API void initiateRequest();
  CTOAST_API std::string getResponse();
  CTOAST_API std::string getErrorMessage() const;
  CTOAST_API int getStatusCode() const;
  CTOAST_API bool isSuccess() const;
  CTOAST_API void setTimeout(int seconds);
  CTOAST_API void setProxy(const std::string &proxyUrl);
  CTOAST_API void setProxyPort(int port);
  CTOAST_API void setProxyCredentials(const std::string &username,
                                      const std::string &password);
  CTOAST_API void setFollowRedirects(bool follow);
  CTOAST_API void setUserAgent(const std::string &userAgent);
  CTOAST_API void setAccept(const std::string &accept);
  CTOAST_API void setContentType(const std::string &contentType);
  CTOAST_API void setAcceptEncoding(const std::string &acceptEncoding);
  CTOAST_API void setAcceptLanguage(const std::string &acceptLanguage);
  CTOAST_API void setKeepAlive(bool keepAlive);
};
} // namespace CinnamonToast