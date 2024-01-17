#include "aanderaa_data_msg.h"
#include "bm_common_pub_sub.h"
#include "bm_common_structs.h"
#include "bm_soft_data_msg.h"
#include "config_cbor_map_srv_reply_msg.h"
#include "config_cbor_map_srv_request_msg.h"
#include "sensor_header_msg.h"
#include "sys_info_svc_reply_msg.h"
#include "gtest/gtest.h"

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

TEST_F(BmCommonTest, bmSoftTest) {
  BmSoftDataMsg::Data d;
  d.header.version = 1;
  d.header.reading_time_utc_ms = 123456789;
  d.header.reading_uptime_millis = 987654321;
  d.header.sensor_reading_time_ms = 0xdeadc0de;
  d.temperature_deg_c = 18.93;

  uint8_t cbor_buffer[1024];
  size_t len = 0;
  BmSoftDataMsg::encode(d, cbor_buffer, sizeof(cbor_buffer), &len);
  EXPECT_EQ(len, 117);

  BmSoftDataMsg::Data decode;
  BmSoftDataMsg::decode(decode, cbor_buffer, len);
  EXPECT_EQ(decode.header.version, 1);
  EXPECT_EQ(decode.header.reading_time_utc_ms, 123456789);
  EXPECT_EQ(decode.header.reading_uptime_millis, 987654321);
  EXPECT_EQ(decode.header.sensor_reading_time_ms, 0xdeadc0de);
  EXPECT_EQ(decode.temperature_deg_c, 18.93);
}
