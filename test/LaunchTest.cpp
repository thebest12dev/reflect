#include <Windows.h>
#include <gtest/gtest.h>
#include "../core/Main.h"

// Sample test case
TEST(LaunchTest, Launch1ArgTest) {
    EXPECT_EQ(CinnamonToast::cliMain(1, {"foo"}), CTOAST_ERROR_NO_FILES_SPECIFIED);
   
}
TEST(LaunchTest, Launch2ArgTest) {
    EXPECT_EQ(CinnamonToast::cliMain(2, { "foo", "bar" }), CTOAST_ERROR_FILE_NOT_FOUND);
}




// You can write more tests similarly.
