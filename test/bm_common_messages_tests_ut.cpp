#include "gtest/gtest.h"
#include "bm_common_pub_sub.h"
#include "bm_common_structs.h"

#include <string.h>

// The fixture for testing class Foo.
class BmCommonTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  BmCommonTest() {
     // You can do set-up work for each test here.
  }

  ~BmCommonTest() override {
     // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }

  // Objects declared here can be used by all tests in the test suite for Foo.
};

TEST_F(BmCommonTest, HeadersBuildTest) {
  EXPECT_TRUE(true); // Dummy test to make sure the headers build.
}
