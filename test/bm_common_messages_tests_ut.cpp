#include "aanderaa_data_msg.h"
#include "barometric_pressure_data_msg.h"
#include "bm_common_pub_sub.h"
#include "bm_rbr_data_msg.h"
#include "bm_rbr_pressure_difference_signal_msg.h"
#include "bm_seapoint_turbidity_data_msg.h"
#include "bm_soft_data_msg.h"
#include "conductivity_msg.h"
#include "config_cbor_map_srv_reply_msg.h"
#include "config_cbor_map_srv_request_msg.h"
#include "device_test_svc_reply_msg.h"
#include "device_test_svc_request_msg.h"
#include "pme_dissolved_oxygen_msg.h"
#include "pme_wipe_msg.h"
#include "sensor_header_msg.h"
#include "sys_info_svc_reply_msg.h"
#include "gtest/gtest.h"
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
  d.data = static_cast<uint8_t *>(malloc(d.data_len));
  d.data[0] = 0x01;
  d.data[1] = 0x02;
  d.data[2] = 0x03;
  d.data[3] = 0x04;

  uint8_t cbor_buffer[1024];
  size_t len = 0;
  EXPECT_EQ(
      DeviceTestSvcReplyMsg::encode(d, cbor_buffer, sizeof(cbor_buffer), &len),
      CborNoError);
  EXPECT_EQ(len, 30);

  DeviceTestSvcReplyMsg::Data decode;
  EXPECT_EQ(DeviceTestSvcReplyMsg::decode(decode, cbor_buffer, len),
            CborNoError);
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
  EXPECT_EQ(
      DeviceTestSvcReplyMsg::encode(d, cbor_buffer, sizeof(cbor_buffer), &len),
      CborNoError);
  EXPECT_EQ(len, 26);

  DeviceTestSvcReplyMsg::Data decode;
  EXPECT_EQ(DeviceTestSvcReplyMsg::decode(decode, cbor_buffer, len),
            CborNoError);
  EXPECT_EQ(decode.success, true);
  EXPECT_EQ(decode.data_len, 0);
  EXPECT_TRUE(decode.data == NULL);
  free(d.data);
}

TEST_F(BmCommonTest, DeviceTestSvcRequestMsgTestData) {
  DeviceTestSvcRequestMsg::Data d;
  d.data_len = 4;
  d.data = static_cast<uint8_t *>(malloc(d.data_len));
  d.data[0] = 0x01;
  d.data[1] = 0x02;
  d.data[2] = 0x03;
  d.data[3] = 0x04;

  uint8_t cbor_buffer[1024];
  size_t len = 0;
  EXPECT_EQ(DeviceTestSvcRequestMsg::encode(d, cbor_buffer, sizeof(cbor_buffer),
                                            &len),
            CborNoError);
  EXPECT_EQ(len, 21);

  DeviceTestSvcRequestMsg::Data decode;
  EXPECT_EQ(DeviceTestSvcRequestMsg::decode(decode, cbor_buffer, len),
            CborNoError);
  EXPECT_EQ(decode.data_len, 4);
  EXPECT_EQ(decode.data[0], 0x01);
  EXPECT_EQ(decode.data[1], 0x02);
  EXPECT_EQ(decode.data[2], 0x03);
  EXPECT_EQ(decode.data[3], 0x04);
  free(d.data);
}

TEST_F(BmCommonTest, DeviceTestSvcRequestMsgTestZeroData) {
  DeviceTestSvcRequestMsg::Data d;
  d.data_len = 0;
  d.data = NULL;

  uint8_t cbor_buffer[1024];
  size_t len = 0;
  EXPECT_EQ(DeviceTestSvcRequestMsg::encode(d, cbor_buffer, sizeof(cbor_buffer),
                                            &len),
            CborNoError);
  EXPECT_EQ(len, 17);

  DeviceTestSvcRequestMsg::Data decode;
  EXPECT_EQ(DeviceTestSvcRequestMsg::decode(decode, cbor_buffer, len),
            CborNoError);
  EXPECT_EQ(decode.data_len, 0);
  EXPECT_TRUE(decode.data == NULL);
  free(d.data);
}

TEST_F(BmCommonTest, bmSeapointTurbidityTest) {
  BmSeapointTurbidityDataMsg::Data d;
  d.header.version = BmSeapointTurbidityDataMsg::VERSION;
  d.header.reading_time_utc_ms = 123456789;
  d.header.reading_uptime_millis = 987654321;
  d.header.sensor_reading_time_ms = 0xdeadc0de;
  d.s_signal = 0.1234;
  d.r_signal = 4000.4;

  uint8_t cbor_buffer[1024];
  size_t len = 0;
  EXPECT_EQ(BmSeapointTurbidityDataMsg::encode(d, cbor_buffer,
                                               sizeof(cbor_buffer), &len),
            CborNoError);
  EXPECT_EQ(len, 126);

  BmSeapointTurbidityDataMsg::Data decode;
  EXPECT_EQ(BmSeapointTurbidityDataMsg::decode(decode, cbor_buffer, len),
            CborNoError);
  EXPECT_EQ(decode.header.version, BmSeapointTurbidityDataMsg::VERSION);
  EXPECT_EQ(decode.header.reading_time_utc_ms, 123456789);
  EXPECT_EQ(decode.header.reading_uptime_millis, 987654321);
  EXPECT_EQ(decode.header.sensor_reading_time_ms, 0xdeadc0de);
  EXPECT_EQ(decode.s_signal, 0.1234);
  EXPECT_EQ(decode.r_signal, 4000.4);
}

TEST_F(BmCommonTest, BmRbrPressureDifferenceSignalMsgTest) {
  BmRbrPressureDifferenceSignalMsg::Data d;
  d.header.version = BmRbrPressureDifferenceSignalMsg::VERSION;
  d.header.reading_time_utc_ms = 123456789;
  d.header.reading_uptime_millis = 987654321;
  d.header.sensor_reading_time_ms = 0xdeadc0de;
  d.total_samples = 15;
  d.sequence_num = 22;
  d.num_samples = 10;
  d.residual_0 = 0.1234;
  d.residual_1 = 0.5678;
  d.difference_signal =
      static_cast<double *>(malloc(d.num_samples * sizeof(double)));
  for (size_t i = 0; i < d.num_samples; i++) {
    d.difference_signal[i] = i * 0.1;
  }
  uint8_t cbor_buffer[1024];
  size_t len = 0;
  EXPECT_EQ(BmRbrPressureDifferenceSignalMsg::encode(d, cbor_buffer,
                                                     sizeof(cbor_buffer), &len),
            CborNoError);
  EXPECT_EQ(len, 281);

  BmRbrPressureDifferenceSignalMsg::Data decode;
  decode.num_samples = 10;
  decode.difference_signal =
      static_cast<double *>(malloc(decode.num_samples * sizeof(double)));
  EXPECT_EQ(BmRbrPressureDifferenceSignalMsg::decode(decode, cbor_buffer, len),
            CborNoError);
  EXPECT_EQ(decode.header.version, BmRbrPressureDifferenceSignalMsg::VERSION);
  EXPECT_EQ(decode.header.reading_time_utc_ms, 123456789);
  EXPECT_EQ(decode.header.reading_uptime_millis, 987654321);
  EXPECT_EQ(decode.header.sensor_reading_time_ms, 0xdeadc0de);
  EXPECT_EQ(decode.total_samples, 15);
  EXPECT_EQ(decode.sequence_num, 22);
  EXPECT_EQ(decode.num_samples, 10);
  EXPECT_NEAR(decode.residual_0, 0.1234, 0.0001);
  EXPECT_NEAR(decode.residual_1, 0.5678, 0.0001);
  for (size_t i = 0; i < decode.num_samples; i++) {
    EXPECT_NEAR(decode.difference_signal[i], i * 0.1, 0.0001);
  }
}

TEST_F(BmCommonTest, BmRbrPressureDifferenceSignalMsgTestInvalidEncode) {
  BmRbrPressureDifferenceSignalMsg::Data d;
  memset(&d, 0, sizeof(d));
  d.header.version = BmRbrPressureDifferenceSignalMsg::VERSION;
  d.header.reading_time_utc_ms = 123456789;
  d.header.reading_uptime_millis = 987654321;
  d.header.sensor_reading_time_ms = 0xdeadc0de;
  d.difference_signal = static_cast<double *>(malloc(10 * sizeof(double)));
  for (size_t i = 0; i < d.num_samples; i++) {
    d.difference_signal[i] = i * 0.1;
  }
  uint8_t cbor_buffer[1024];
  size_t len = 0;
  EXPECT_EQ(BmRbrPressureDifferenceSignalMsg::encode(d, cbor_buffer,
                                                     sizeof(cbor_buffer), &len),
            CborErrorImproperValue);
  d.num_samples = 10;
  EXPECT_EQ(BmRbrPressureDifferenceSignalMsg::encode(d, NULL,
                                                     sizeof(cbor_buffer), &len),
            CborErrorOutOfMemory);
  free(d.difference_signal);
  d.difference_signal = NULL;
  EXPECT_EQ(BmRbrPressureDifferenceSignalMsg::encode(d, cbor_buffer,
                                                     sizeof(cbor_buffer), &len),
            CborErrorOutOfMemory);
}

TEST_F(BmCommonTest, BmRbrPressureDifferenceSignalMsgTestInvalidDecode) {
  BmRbrPressureDifferenceSignalMsg::Data d;
  d.header.version = BmRbrPressureDifferenceSignalMsg::VERSION;
  d.header.reading_time_utc_ms = 123456789;
  d.header.reading_uptime_millis = 987654321;
  d.header.sensor_reading_time_ms = 0xdeadc0de;
  d.total_samples = 15;
  d.sequence_num = 22;
  d.num_samples = 10;
  d.difference_signal =
      static_cast<double *>(malloc(d.num_samples * sizeof(double)));
  for (size_t i = 0; i < d.num_samples; i++) {
    d.difference_signal[i] = i * 0.1;
  }
  uint8_t cbor_buffer[1024];
  size_t len = 0;
  EXPECT_EQ(BmRbrPressureDifferenceSignalMsg::encode(d, cbor_buffer,
                                                     sizeof(cbor_buffer), &len),
            CborNoError);
  EXPECT_EQ(len, 281);

  BmRbrPressureDifferenceSignalMsg::Data decode;
  decode.num_samples = 10;
  decode.difference_signal = static_cast<double *>(malloc(10 * sizeof(double)));
  EXPECT_EQ(BmRbrPressureDifferenceSignalMsg::decode(decode, NULL, len),
            CborErrorOutOfMemory);
  decode.num_samples = 0;
  EXPECT_EQ(BmRbrPressureDifferenceSignalMsg::decode(decode, cbor_buffer, len),
            CborErrorOutOfMemory);
  free(decode.difference_signal);
  decode.difference_signal = NULL;
  decode.num_samples = 10;
  EXPECT_EQ(BmRbrPressureDifferenceSignalMsg::decode(decode, cbor_buffer, len),
            CborErrorOutOfMemory);
}

TEST_F(BmCommonTest, PmeDissolvedOxygenMsgTest) {
  PmeDissolvedOxygenMsg::Data d;
  d.header.version = PmeDissolvedOxygenMsg::VERSION;
  d.header.reading_time_utc_ms = 123456789;
  d.header.reading_uptime_millis = 987654321;
  d.header.sensor_reading_time_ms = 0xdeadc0de;
  d.temperature_deg_c = 18.93;
  d.do_mg_per_l = 7.891;
  d.quality = 0.987;
  d.do_saturation_pct = 100.0;
  d.salinity_ppt = 32.0;

  uint8_t cbor_buffer[1024];
  size_t len = 0;
  PmeDissolvedOxygenMsg::encode(d, cbor_buffer, sizeof(cbor_buffer), &len);
  EXPECT_EQ(len, 200);

  PmeDissolvedOxygenMsg::Data decode;
  PmeDissolvedOxygenMsg::decode(decode, cbor_buffer, len);
  EXPECT_EQ(decode.header.version, PmeDissolvedOxygenMsg::VERSION);
  EXPECT_EQ(decode.header.reading_time_utc_ms, 123456789);
  EXPECT_EQ(decode.header.reading_uptime_millis, 987654321);
  EXPECT_EQ(decode.header.sensor_reading_time_ms, 0xdeadc0de);
  EXPECT_EQ(decode.temperature_deg_c, 18.93);
  EXPECT_EQ(decode.do_mg_per_l, 7.891);
  EXPECT_EQ(decode.quality, 0.987);
  EXPECT_EQ(decode.do_saturation_pct, 100.0);
  EXPECT_EQ(decode.salinity_ppt, 32.0);
}

TEST_F(BmCommonTest, PmeWipeMsgTest) {
  PmeWipeMsg::Data d;
  d.header.version = PmeWipeMsg::VERSION;
  d.header.reading_time_utc_ms = 123456789;
  d.header.reading_uptime_millis = 987654321;
  d.header.sensor_reading_time_ms = 0xdeadc0de;
  d.wipe_time_sec = 5.8;
  d.start1_mA = 90.3;
  d.avg_mA = 90.1;
  d.start2_mA = 95.6;
  d.final_mA = 95.3;
  d.rsource = 90;

  uint8_t cbor_buffer[1024];
  size_t len = 0;
  PmeWipeMsg::encode(d, cbor_buffer, sizeof(cbor_buffer), &len);
  EXPECT_EQ(len, 202);

  PmeWipeMsg::Data decode;
  PmeWipeMsg::decode(decode, cbor_buffer, len);
  EXPECT_EQ(decode.header.version, PmeWipeMsg::VERSION);
  EXPECT_EQ(decode.header.reading_time_utc_ms, 123456789);
  EXPECT_EQ(decode.header.reading_uptime_millis, 987654321);
  EXPECT_EQ(decode.header.sensor_reading_time_ms, 0xdeadc0de);
  EXPECT_EQ(decode.wipe_time_sec, 5.8);
  EXPECT_EQ(decode.start1_mA, 90.3);
  EXPECT_EQ(decode.avg_mA, 90.1);
  EXPECT_EQ(decode.start2_mA, 95.6);
  EXPECT_EQ(decode.final_mA, 95.3);
  EXPECT_EQ(decode.rsource, 90);
}

TEST_F(BmCommonTest, barometricPressureMsgTest) {
  BarometricPressureDataMsg::Data d;
  d.header.version = BarometricPressureDataMsg::VERSION;
  d.header.reading_time_utc_ms = 123456789;
  d.header.reading_uptime_millis = 987654321;
  d.header.sensor_reading_time_ms = 0xdeadc0de;
  d.barometric_pressure_mbar = 1010.93;

  uint8_t cbor_buffer[1024];
  size_t len = 0;
  BarometricPressureDataMsg::encode(d, cbor_buffer, sizeof(cbor_buffer), &len);
  EXPECT_EQ(len, 125);

  BarometricPressureDataMsg::Data decode;
  BarometricPressureDataMsg::decode(decode, cbor_buffer, len);
  EXPECT_EQ(decode.header.version, BarometricPressureDataMsg::VERSION);
  EXPECT_EQ(decode.header.reading_time_utc_ms, 123456789);
  EXPECT_EQ(decode.header.reading_uptime_millis, 987654321);
  EXPECT_EQ(decode.header.sensor_reading_time_ms, 0xdeadc0de);
  EXPECT_EQ(decode.barometric_pressure_mbar, 1010.93);
}

TEST_F(BmCommonTest, AanderaaConductivityTest) {
    AanderaaConductivityMsg::Data d;
    d.header.version = AanderaaConductivityMsg::VERSION;
    d.header.reading_time_utc_ms = 123456789;
    d.header.reading_uptime_millis = 987654321;
    d.header.sensor_reading_time_ms = 0xdeadc0de;
    d.conductivity_ms_cm = 50.123;
    d.temperature_deg_c = 23.456;
    d.salinity_psu = 35.123;
    d.water_density_kg_m3 = 1025.83;
    d.sound_speed_m_s = 1498.15;

    uint8_t cbor_buffer[1024];
    size_t len = 0;
    AanderaaConductivityMsg::encode(d, cbor_buffer, sizeof(cbor_buffer), &len);
    EXPECT_EQ(len, 221);

    AanderaaConductivityMsg::Data decode;
    AanderaaConductivityMsg::decode(decode, cbor_buffer, len);
    EXPECT_EQ(decode.header.version, AanderaaConductivityMsg::VERSION);
    EXPECT_EQ(decode.header.reading_time_utc_ms, 123456789);
    EXPECT_EQ(decode.header.reading_uptime_millis, 987654321);
    EXPECT_EQ(decode.header.sensor_reading_time_ms, 0xdeadc0de);
    EXPECT_EQ(decode.conductivity_ms_cm, 50.123);
    EXPECT_EQ(decode.temperature_deg_c, 23.456);
    EXPECT_EQ(decode.salinity_psu, 35.123);
    EXPECT_EQ(decode.water_density_kg_m3, 1025.83);
    EXPECT_EQ(decode.sound_speed_m_s, 1498.15);
}