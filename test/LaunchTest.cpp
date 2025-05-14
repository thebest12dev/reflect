#include "../core/Main.h"
#include <Windows.h>
#include <gtest/gtest.h>

class LaunchTestFixture : public ::testing::Test {
protected:
  uint8_t argc;
  std::vector<std::string> argv;

  void SetUp() override {
    argc = 0;
    argv.clear();
  }

  void initializeArgs(const std::vector<std::string> &args) {
    argc = static_cast<uint8_t>(args.size());
    argv = args;
  }
};

TEST_F(LaunchTestFixture, Launch1ArgTest) {
  initializeArgs({"foo"});
  EXPECT_EQ(reflect::cliMain(argc, argv),
            REFLECT_ERROR_NO_FILES_SPECIFIED);
}

TEST_F(LaunchTestFixture, Launch2ArgTest) {
  initializeArgs({"foo", "bar"});
  EXPECT_EQ(reflect::cliMain(argc, argv), REFLECT_ERROR_FILE_NOT_FOUND);
}

TEST_F(LaunchTestFixture, LaunchNoArgsTest) {
  initializeArgs({});
  EXPECT_EQ(reflect::cliMain(argc, argv),
            REFLECT_ERROR_NO_FILES_SPECIFIED);
}
