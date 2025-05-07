#include "../net/HttpRequest.h"
#include <Windows.h>
#include <gtest/gtest.h>

class HttpTestFixture : public ::testing::Test {
protected:
  cinnamontoast::HttpRequest *httpRequest;

  void SetUp() override {
    // Common setup logic for all tests
    httpRequest = nullptr;
  }

  void TearDown() override {
    // Clean up resources
    delete httpRequest;
  }

  void initializeRequest(const std::string &url) {
    httpRequest = new cinnamontoast::HttpRequest(url);
  }
};

TEST_F(HttpTestFixture, HttpGetTest) {
  initializeRequest("http://example.com");
  httpRequest->initiateRequest();
  EXPECT_TRUE(httpRequest->isSuccess());
}

TEST_F(HttpTestFixture, HttpsGetTest) {
  initializeRequest("https://example.com");
  httpRequest->initiateRequest();
  EXPECT_TRUE(httpRequest->isSuccess());
}

TEST_F(HttpTestFixture, InvalidUrlTest) {
  initializeRequest("invalid-url");
  httpRequest->initiateRequest();
  EXPECT_FALSE(httpRequest->isSuccess());
}
