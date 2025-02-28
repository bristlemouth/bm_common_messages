#include "power_info_reply_msg.h"
#include "gtest/gtest.h"

// The fixture for testing class
class PowerInfo : public ::testing::Test {
protected:
  PowerInfo() {}
  ~PowerInfo() override {}
  void SetUp() override {}

  void TearDown() override {}
};

TEST_F(PowerInfo, PowerInfoReply) {
  PowerInfoReplyData encode;

  encode.total_on_s = UINT32_MAX;
  encode.remaining_on_s = 100000;
  encode.upcoming_off_s = 3333333;

  uint8_t cbor_buffer[1024];
  size_t len = 0;

  EXPECT_EQ(
      power_info_reply_encode(&encode, cbor_buffer, sizeof(cbor_buffer), &len),
      CborNoError);

  PowerInfoReplyData decode;
  EXPECT_EQ(power_info_reply_decode(&decode, cbor_buffer, sizeof(cbor_buffer)),
            CborNoError);
  EXPECT_EQ(decode.total_on_s, encode.total_on_s);
  EXPECT_EQ(decode.remaining_on_s, encode.remaining_on_s);
  EXPECT_EQ(decode.upcoming_off_s, encode.upcoming_off_s);
}
