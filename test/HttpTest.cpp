#include "../net/HttpRequest.h"
#include <Windows.h>
#include <gtest/gtest.h>

TEST(HttpTest, HttpGetTest) {
  CinnamonToast::HttpRequest req("http://example.com");
  req.initiateRequest();
  EXPECT_EQ(req.isSuccess(), true);
}

// You can write more tests similarly.
TEST(HttpTest, HttpsGetTest) {
  CinnamonToast::HttpRequest req("https://example.com");
  req.initiateRequest();
  EXPECT_EQ(req.isSuccess(), true);
}