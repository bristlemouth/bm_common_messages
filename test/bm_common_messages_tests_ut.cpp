#include "aanderaa_data_msg.h"
#include "bm_common_pub_sub.h"
#include "bm_common_structs.h"
#include "bm_rbr_data_msg.h"
#include "bm_soft_data_msg.h"
#include "config_cbor_map_srv_reply_msg.h"
#include "config_cbor_map_srv_request_msg.h"
#include "sensor_header_msg.h"
#include "sys_info_svc_reply_msg.h"
#include "gtest/gtest.h"
#include "device_test_svc_reply_msg.h"
#include <cmath>
#include <math.h>

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
  d.header.version = BmSoftDataMsg::VERSION;
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
  EXPECT_EQ(decode.header.version, BmSoftDataMsg::VERSION);
  EXPECT_EQ(decode.header.reading_time_utc_ms, 123456789);
  EXPECT_EQ(decode.header.reading_uptime_millis, 987654321);
  EXPECT_EQ(decode.header.sensor_reading_time_ms, 0xdeadc0de);
  EXPECT_EQ(decode.temperature_deg_c, 18.93);
}

TEST_F(BmCommonTest, bmRbrTestTemp) {
  BmRbrDataMsg::Data d;
  d.header.version = BmRbrDataMsg::VERSION;
  d.header.reading_time_utc_ms = 123456789;
  d.header.reading_uptime_millis = 987654321;
  d.header.sensor_reading_time_ms = 0xdeadc0de;

  // Temperature
  d.sensor_type = BmRbrDataMsg::TEMPERATURE;
  d.temperature_deg_c = 18.93;

  uint8_t cbor_buffer[1024];
  size_t len = 0;
  BmRbrDataMsg::encode(d, cbor_buffer, sizeof(cbor_buffer), &len);
  EXPECT_EQ(len, 157);

  BmRbrDataMsg::Data decode;
  BmRbrDataMsg::decode(decode, cbor_buffer, len);
  EXPECT_EQ(decode.header.version, BmRbrDataMsg::VERSION);
  EXPECT_EQ(decode.header.reading_time_utc_ms, 123456789);
  EXPECT_EQ(decode.header.reading_uptime_millis, 987654321);
  EXPECT_EQ(decode.header.sensor_reading_time_ms, 0xdeadc0de);
  EXPECT_EQ(decode.sensor_type, BmRbrDataMsg::TEMPERATURE);
  EXPECT_EQ(decode.temperature_deg_c, 18.93);
  EXPECT_TRUE(isnan(d.pressure_deci_bar));

  // Pressure
  d.sensor_type = BmRbrDataMsg::PRESSURE;
  d.pressure_deci_bar = 1013.25;
  len = 0;
  BmRbrDataMsg::encode(d, cbor_buffer, sizeof(cbor_buffer), &len);
  EXPECT_EQ(len, 157);
  BmRbrDataMsg::decode(decode, cbor_buffer, len);
  EXPECT_EQ(decode.sensor_type, BmRbrDataMsg::PRESSURE);
  EXPECT_EQ(decode.pressure_deci_bar, 1013.25);
  EXPECT_TRUE(isnan(d.temperature_deg_c));

  // Pressure and Temperature
  d.sensor_type = BmRbrDataMsg::PRESSURE_AND_TEMPERATURE;
  len = 0;
  d.pressure_deci_bar = 444.25;
  d.temperature_deg_c = 25.93;
  BmRbrDataMsg::encode(d, cbor_buffer, sizeof(cbor_buffer), &len);
  EXPECT_EQ(len, 157);
  BmRbrDataMsg::decode(decode, cbor_buffer, len);
  EXPECT_EQ(decode.sensor_type, BmRbrDataMsg::PRESSURE_AND_TEMPERATURE);
  EXPECT_EQ(decode.pressure_deci_bar, 444.25);
  EXPECT_EQ(decode.temperature_deg_c, 25.93);

  // Unknown
  d.sensor_type = BmRbrDataMsg::UNKNOWN;
  len = 0;
  BmRbrDataMsg::encode(d, cbor_buffer, sizeof(cbor_buffer), &len);
  EXPECT_EQ(len, 157);
  BmRbrDataMsg::decode(decode, cbor_buffer, len);
  EXPECT_EQ(decode.sensor_type, BmRbrDataMsg::UNKNOWN);
  EXPECT_TRUE(isnan(decode.pressure_deci_bar));
  EXPECT_TRUE(isnan(decode.temperature_deg_c));
}

TEST_F(BmCommonTest, DeviceTestSvcReplyMsgTestData) {
  DeviceTestSvcReplyMsg::Data d;
  d.success = true;
  d.data_len = 4;
  d.data = (uint8_t *)malloc(d.data_len);
  d.data[0] = 0x01;
  d.data[1] = 0x02;
  d.data[2] = 0x03;
  d.data[3] = 0x04;

  uint8_t cbor_buffer[1024];
  size_t len = 0;
  EXPECT_EQ(DeviceTestSvcReplyMsg::encode(d, cbor_buffer, sizeof(cbor_buffer), &len), CborNoError);
  EXPECT_EQ(len, 30);

  DeviceTestSvcReplyMsg::Data decode;
  EXPECT_EQ(DeviceTestSvcReplyMsg::decode(decode, cbor_buffer, len), CborNoError);
  EXPECT_EQ(decode.success, true);
  EXPECT_EQ(decode.data_len, 4);
  EXPECT_EQ(decode.data[0], 0x01);
  EXPECT_EQ(decode.data[1], 0x02);
  EXPECT_EQ(decode.data[2], 0x03);
  EXPECT_EQ(decode.data[3], 0x04);
  free(d.data);
}

TEST_F(BmCommonTest, DeviceTestSvcReplyMsgTestZeroData) {
  DeviceTestSvcReplyMsg::Data d;
  d.success = true;
  d.data_len = 0;
  d.data = NULL;

  uint8_t cbor_buffer[1024];
  size_t len = 0;
  EXPECT_EQ(DeviceTestSvcReplyMsg::encode(d, cbor_buffer, sizeof(cbor_buffer), &len), CborNoError);
  EXPECT_EQ(len, 26);

  DeviceTestSvcReplyMsg::Data decode;
  EXPECT_EQ(DeviceTestSvcReplyMsg::decode(decode, cbor_buffer, len), CborNoError);
  EXPECT_EQ(decode.success, true);
  EXPECT_EQ(decode.data_len, 0);
  EXPECT_TRUE(decode.data == NULL);
  free(d.data);
}
