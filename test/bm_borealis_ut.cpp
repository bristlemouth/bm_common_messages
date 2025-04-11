#include "bm_borealis.h"
#include "gtest/gtest.h"
#include <cbor.h>
#include <string.h>

// The fixture for testing class
class BorealisMessages : public ::testing::Test {
protected:
  const char *spectrum_str = "2IicZBVtr9RhxENWIpHQSdhi9WDxey0/"
                             "YaQWg+mcK6SAp6Cg5YS695A7UApJL96XPsC6XYb+w4seJ2u+"
                             "YJbCTA==";
  const char *levels_str = "13f8IqBke95LXw04xEj2ETXehe4iYatNw/Pnp2FdMiY=";
  BorealisMessages() {}
  ~BorealisMessages() override {}
  void SetUp() override {}

  void TearDown() override {}
};

TEST_F(BorealisMessages, BorealisSpectrumMessage) {
  struct borealis_spectrum_data d;
  d.header.version = BOREALIS_SPECTRUM_MSG_VERSION;
  d.header.reading_time_utc_ms = 123456789;
  d.header.reading_uptime_millis = 987654321;
  d.header.sensor_reading_time_ms = 0xdeadc0de;
  d.dt = 1234.9875;
  d.df = 24012.99887766;
  d.bands_per_octave = 128;
  d.spectrum_as_base64 = (char *)spectrum_str;
  d.spectrum_length = strlen(spectrum_str);

  uint8_t cbor_buffer[1024];
  size_t len = 0;
  borealis_spectrum_data_encode(&d, cbor_buffer, sizeof(cbor_buffer), &len);

  struct borealis_spectrum_data decode = {};
  borealis_spectrum_data_decode(&decode, cbor_buffer, len);
  EXPECT_EQ(decode.header.version, BOREALIS_SPECTRUM_MSG_VERSION);
  EXPECT_EQ(decode.header.reading_time_utc_ms, 123456789);
  EXPECT_EQ(decode.header.reading_uptime_millis, 987654321);
  EXPECT_EQ(decode.header.sensor_reading_time_ms, 0xdeadc0de);
  EXPECT_FLOAT_EQ(decode.dt, 1234.9875);
  EXPECT_FLOAT_EQ(decode.df, 24012.99887766);
  EXPECT_EQ(decode.bands_per_octave, 128);
  free(decode.spectrum_as_base64);
}

TEST_F(BorealisMessages, BorealisLevelsMessage) {
  struct borealis_levels d;
  d.header.version = BOREALIS_LEVELS_MSG_VERSION;
  d.header.reading_time_utc_ms = 123456789;
  d.header.reading_uptime_millis = 987654321;
  d.header.sensor_reading_time_ms = 0xdeadc0de;
  d.dt = 1234.9875;
  d.first_band_index = 128;
  d.levels = (char *)levels_str;
  d.levels_length = strlen(levels_str);

  uint8_t cbor_buffer[1024];
  size_t len = 0;
  borealis_levels_encode(&d, cbor_buffer, sizeof(cbor_buffer), &len);

  struct borealis_levels decode = {};
  borealis_levels_decode(&decode, cbor_buffer, len);
  EXPECT_EQ(decode.header.version, BOREALIS_LEVELS_MSG_VERSION);
  EXPECT_EQ(decode.header.reading_time_utc_ms, 123456789);
  EXPECT_EQ(decode.header.reading_uptime_millis, 987654321);
  EXPECT_EQ(decode.header.sensor_reading_time_ms, 0xdeadc0de);
  EXPECT_FLOAT_EQ(decode.dt, 1234.9875);
  EXPECT_EQ(decode.first_band_index, 128);
  EXPECT_EQ(strncmp(d.levels, levels_str, strlen(levels_str)), 0);
  free(decode.levels);
}

TEST_F(BorealisMessages, BorealisLevelStatisticsMessage) {
  struct borealis_level_statistics d;
  d.header.version = BOREALIS_LEVELS_MSG_VERSION;
  d.header.reading_time_utc_ms = 123456789;
  d.header.reading_uptime_millis = 987654321;
  d.header.sensor_reading_time_ms = 0xdeadc0de;
  d.dt = 1234.9875;
  d.dt_report = 111.1111;
  d.first_band_index = 128;
  d.levels = (char *)levels_str;
  d.levels_length = strlen(levels_str);
  d.max_iqr = 0.12345;

  uint8_t cbor_buffer[1024];
  size_t len = 0;
  EXPECT_EQ(borealis_levels_statistics_encode(&d, cbor_buffer,
                                              sizeof(cbor_buffer), &len),
            CborNoError);

  struct borealis_level_statistics decode = {};
  EXPECT_EQ(borealis_levels_statistics_decode(&decode, cbor_buffer, len),
            CborNoError);
  EXPECT_EQ(decode.header.version, BOREALIS_LEVELS_MSG_VERSION);
  EXPECT_EQ(decode.header.reading_time_utc_ms, 123456789);
  EXPECT_EQ(decode.header.reading_uptime_millis, 987654321);
  EXPECT_EQ(decode.header.sensor_reading_time_ms, 0xdeadc0de);
  EXPECT_FLOAT_EQ(decode.dt, 1234.9875);
  EXPECT_FLOAT_EQ(decode.dt_report, 111.1111);
  EXPECT_EQ(decode.first_band_index, 128);
  EXPECT_EQ(strncmp(d.levels, levels_str, strlen(levels_str)), 0);
  EXPECT_FLOAT_EQ(decode.max_iqr, 0.12345);
  free(decode.levels);
}
