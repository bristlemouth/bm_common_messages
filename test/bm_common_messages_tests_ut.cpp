#include "bm_config.h"

#include "aanderaa_conductivity_msg.h"
#include "aanderaa_current_meter_msg.h"
#include "barometric_pressure_data_msg.h"
#include "bm_common_pub_sub.h"
#include "bm_rbr_data_msg.h"
#include "bm_rbr_pressure_difference_signal_msg.h"
#include "bm_seapoint_turbidity_data_msg.h"
#include "bm_soft_data_msg.h"
#include "config_cbor_map_srv_reply_msg.h"
#include "config_cbor_map_srv_request_msg.h"
#include "device_test_svc_reply_msg.h"
#include "device_test_svc_request_msg.h"
#include "pme_dissolved_oxygen_msg.h"
#include "pme_wipe_msg.h"
#include "power_battery_averages_msg.h"
#include "power_battery_msg.h"
#include "power_reading_averages_msg.h"
#include "power_reading_msg.h"
#include "power_solar_reading_msg.h"
#include "power_solar_reading_msg.h"
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
  EXPECT_EQ(DeviceTestSvcReplyMsg::encode(d, cbor_buffer, sizeof(cbor_buffer), &len),
            CborNoError);
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
  EXPECT_EQ(DeviceTestSvcReplyMsg::encode(d, cbor_buffer, sizeof(cbor_buffer), &len),
            CborNoError);
  EXPECT_EQ(len, 26);

  DeviceTestSvcReplyMsg::Data decode;
  EXPECT_EQ(DeviceTestSvcReplyMsg::decode(decode, cbor_buffer, len), CborNoError);
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
  EXPECT_EQ(DeviceTestSvcRequestMsg::encode(d, cbor_buffer, sizeof(cbor_buffer), &len),
            CborNoError);
  EXPECT_EQ(len, 21);

  DeviceTestSvcRequestMsg::Data decode;
  EXPECT_EQ(DeviceTestSvcRequestMsg::decode(decode, cbor_buffer, len), CborNoError);
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
  EXPECT_EQ(DeviceTestSvcRequestMsg::encode(d, cbor_buffer, sizeof(cbor_buffer), &len),
            CborNoError);
  EXPECT_EQ(len, 17);

  DeviceTestSvcRequestMsg::Data decode;
  EXPECT_EQ(DeviceTestSvcRequestMsg::decode(decode, cbor_buffer, len), CborNoError);
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
  EXPECT_EQ(BmSeapointTurbidityDataMsg::encode(d, cbor_buffer, sizeof(cbor_buffer), &len),
            CborNoError);
  EXPECT_EQ(len, 126);

  BmSeapointTurbidityDataMsg::Data decode;
  EXPECT_EQ(BmSeapointTurbidityDataMsg::decode(decode, cbor_buffer, len), CborNoError);
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
  d.difference_signal = static_cast<double *>(malloc(d.num_samples * sizeof(double)));
  for (size_t i = 0; i < d.num_samples; i++) {
    d.difference_signal[i] = i * 0.1;
  }
  uint8_t cbor_buffer[1024];
  size_t len = 0;
  EXPECT_EQ(BmRbrPressureDifferenceSignalMsg::encode(d, cbor_buffer, sizeof(cbor_buffer), &len),
            CborNoError);
  EXPECT_EQ(len, 281);

  BmRbrPressureDifferenceSignalMsg::Data decode;
  decode.num_samples = 10;
  decode.difference_signal = static_cast<double *>(malloc(decode.num_samples * sizeof(double)));
  EXPECT_EQ(BmRbrPressureDifferenceSignalMsg::decode(decode, cbor_buffer, len), CborNoError);
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
  EXPECT_EQ(BmRbrPressureDifferenceSignalMsg::encode(d, cbor_buffer, sizeof(cbor_buffer), &len),
            CborErrorImproperValue);
  d.num_samples = 10;
  EXPECT_EQ(BmRbrPressureDifferenceSignalMsg::encode(d, NULL, sizeof(cbor_buffer), &len),
            CborErrorOutOfMemory);
  free(d.difference_signal);
  d.difference_signal = NULL;
  EXPECT_EQ(BmRbrPressureDifferenceSignalMsg::encode(d, cbor_buffer, sizeof(cbor_buffer), &len),
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
  d.difference_signal = static_cast<double *>(malloc(d.num_samples * sizeof(double)));
  for (size_t i = 0; i < d.num_samples; i++) {
    d.difference_signal[i] = i * 0.1;
  }
  uint8_t cbor_buffer[1024];
  size_t len = 0;
  EXPECT_EQ(BmRbrPressureDifferenceSignalMsg::encode(d, cbor_buffer, sizeof(cbor_buffer), &len),
            CborNoError);
  EXPECT_EQ(len, 281);

  BmRbrPressureDifferenceSignalMsg::Data decode;
  decode.num_samples = 10;
  decode.difference_signal = static_cast<double *>(malloc(10 * sizeof(double)));
  EXPECT_EQ(BmRbrPressureDifferenceSignalMsg::decode(decode, NULL, len), CborErrorOutOfMemory);
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
  d.depth_m = 10.0;

  uint8_t cbor_buffer[1024];
  size_t len = 0;
  AanderaaConductivityMsg::encode(d, cbor_buffer, sizeof(cbor_buffer), &len);
  EXPECT_EQ(len, 234);

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
  EXPECT_EQ(decode.depth_m, 10.0);
}

TEST_F(BmCommonTest, AanderaaCurrentMeterTest) {
  AanderaaCurrentMeterMsg::Data d;
  d.header.version = AanderaaCurrentMeterMsg::VERSION;
  d.header.reading_time_utc_ms = 123456789;
  d.header.reading_uptime_millis = 987654321;
  d.header.sensor_reading_time_ms = 0xdeadc0de;
  d.abs_speed_cm_s = 10.54;
  d.direction_deg_m = 109.67;
  d.north_cm_s = 67.89;
  d.east_cm_s = 67.88;
  d.heading_deg_m = 12.34;
  d.tilt_x_deg = 23.45;
  d.tilt_y_deg = 34.56;
  d.single_ping_std_cm_s = 45.67;
  d.transducer_strength_db = 56.78;
  d.ping_count = 67.76;
  d.abs_tilt_deg = 78.87;
  d.max_tilt_deg = 89.98;
  d.std_tilt_deg = 90.09;
  d.temperature_deg_c = 23.456;

  uint8_t cbor_buffer[1024];
  size_t len = 0;
  AanderaaCurrentMeterMsg::encode(d, cbor_buffer, sizeof(cbor_buffer), &len);
  EXPECT_EQ(len, 416);

  AanderaaCurrentMeterMsg::Data decode;
  AanderaaCurrentMeterMsg::decode(decode, cbor_buffer, len);
  EXPECT_EQ(decode.header.version, AanderaaConductivityMsg::VERSION);
  EXPECT_EQ(decode.header.reading_time_utc_ms, 123456789);
  EXPECT_EQ(decode.header.reading_uptime_millis, 987654321);
  EXPECT_EQ(decode.header.sensor_reading_time_ms, 0xdeadc0de);
  EXPECT_EQ(decode.abs_speed_cm_s, 10.54);
  EXPECT_EQ(decode.direction_deg_m, 109.67);
  EXPECT_EQ(decode.north_cm_s, 67.89);
  EXPECT_EQ(decode.east_cm_s, 67.88);
  EXPECT_EQ(decode.heading_deg_m, 12.34);
  EXPECT_EQ(decode.tilt_x_deg, 23.45);
  EXPECT_EQ(decode.tilt_y_deg, 34.56);
  EXPECT_EQ(decode.single_ping_std_cm_s, 45.67);
  EXPECT_EQ(decode.transducer_strength_db, 56.78);
  EXPECT_EQ(decode.ping_count, 67.76);
  EXPECT_EQ(decode.abs_tilt_deg, 78.87);
  EXPECT_EQ(decode.max_tilt_deg, 89.98);
  EXPECT_EQ(decode.std_tilt_deg, 90.09);
  EXPECT_EQ(decode.temperature_deg_c, 23.456);
}

TEST_F(BmCommonTest, PowerReadingTest) {
  PowerReadingMsg::Data d;
  d.header.version = PowerReadingMsg::VERSION;
  d.header.reading_time_utc_ms = 123456789;
  d.header.reading_uptime_millis = 987654321;
  d.header.sensor_reading_time_ms = 0xdeadc0de;
  d.power_reading_type = PowerReadingMsg::SOURCE;
  d.voltage_v = 3.79;
  d.current_a = 0.258;
  d.status = PowerReadingMsg::OVERCURRENT | PowerReadingMsg::UNDERVOLTAGE;

  uint8_t cbor_buffer[1024];
  size_t len = 0;
  PowerReadingMsg::encode(d, cbor_buffer, sizeof(cbor_buffer), &len);
  EXPECT_EQ(len, 156);

  PowerReadingMsg::Data decode;
  PowerReadingMsg::decode(decode, cbor_buffer, len);
  EXPECT_EQ(decode.header.version, PowerReadingMsg::VERSION);
  EXPECT_EQ(decode.header.reading_time_utc_ms, 123456789);
  EXPECT_EQ(decode.header.reading_uptime_millis, 987654321);
  EXPECT_EQ(decode.header.sensor_reading_time_ms, 0xdeadc0de);
  EXPECT_EQ(decode.power_reading_type, PowerReadingMsg::SOURCE);
  EXPECT_EQ(decode.voltage_v, 3.79);
  EXPECT_EQ(decode.current_a, 0.258);
  EXPECT_EQ(decode.status, PowerReadingMsg::OVERCURRENT | PowerReadingMsg::UNDERVOLTAGE);

  // Test Parsing an incorrect message
  // We should parse the header correctly
  // and return CborErrorUnsupportedType
  AanderaaCurrentMeterMsg::Data bad_data;
  bad_data.header.version = AanderaaCurrentMeterMsg::VERSION;
  bad_data.header.reading_time_utc_ms = 123456789;
  bad_data.header.reading_uptime_millis = 987654321;
  bad_data.header.sensor_reading_time_ms = 0xdeadc0de;
  bad_data.abs_speed_cm_s = 10.54;
  bad_data.direction_deg_m = 109.67;
  bad_data.north_cm_s = 67.89;
  bad_data.east_cm_s = 67.88;
  bad_data.heading_deg_m = 12.34;
  bad_data.tilt_x_deg = 23.45;
  bad_data.tilt_y_deg = 34.56;
  bad_data.single_ping_std_cm_s = 45.67;
  bad_data.transducer_strength_db = 56.78;
  bad_data.ping_count = 67.76;
  bad_data.abs_tilt_deg = 78.87;
  bad_data.max_tilt_deg = 89.98;
  bad_data.std_tilt_deg = 90.09;
  bad_data.temperature_deg_c = 23.456;

  uint8_t bad_cbor_buffer[1024];
  size_t bad_len = 0;
  AanderaaCurrentMeterMsg::encode(bad_data, bad_cbor_buffer, sizeof(bad_cbor_buffer), &bad_len);
  EXPECT_EQ(bad_len, 416);

  PowerReadingMsg::Data bad_decode = {.header = {.version = 0,
                                                 .reading_time_utc_ms = 0,
                                                 .reading_uptime_millis = 0,
                                                 .sensor_reading_time_ms = 0},
                                      .power_reading_type = PowerReadingMsg::SOURCE,
                                      .voltage_v = 0,
                                      .current_a = 0,
                                      .status = 0};
  CborError err = PowerReadingMsg::decode(bad_decode, bad_cbor_buffer, bad_len);
  EXPECT_EQ(err, CborErrorUnsupportedType);
  EXPECT_EQ(bad_decode.header.version, PowerReadingMsg::VERSION);
  EXPECT_EQ(bad_decode.header.reading_time_utc_ms, 123456789);
  EXPECT_EQ(bad_decode.header.reading_uptime_millis, 987654321);
  EXPECT_EQ(bad_decode.header.sensor_reading_time_ms, 0xdeadc0de);
  EXPECT_EQ(bad_decode.power_reading_type, PowerReadingMsg::SOURCE);
  EXPECT_EQ(bad_decode.voltage_v, 0);
  EXPECT_EQ(bad_decode.current_a, 0);
  EXPECT_EQ(bad_decode.status, 0);
}

TEST_F(BmCommonTest, PowerReadingAveragesTest) {
  PowerReadingAveragesMsg::Data d;
  d.header.version = PowerReadingAveragesMsg::VERSION;
  d.header.reading_time_utc_ms = 123456789;
  d.header.reading_uptime_millis = 987654321;
  d.header.sensor_reading_time_ms = 0xdeadc0de;
  d.power_reading_type = PowerReadingMsg::SOURCE;
  d.status = PowerReadingMsg::OVERCURRENT | PowerReadingMsg::UNDERVOLTAGE;
  d.num_samples = 30;
  d.averaging_window_length_s = 29.9778;
  d.voltage_v_avg = 3.79;
  d.voltage_v_min = 3.65;
  d.voltage_v_max = 3.84;
  d.voltage_v_stdev = 0.012;
  d.current_a_avg = 0.258;
  d.current_a_min = 0.248;
  d.current_a_max = 0.270;
  d.current_a_stdev = 0.016;

  uint8_t cbor_buffer[1024];
  size_t len = 0;
  PowerReadingAveragesMsg::encode(d, cbor_buffer, sizeof(cbor_buffer), &len);
  EXPECT_EQ(len, 356);

  PowerReadingAveragesMsg::Data decode;
  PowerReadingAveragesMsg::decode(decode, cbor_buffer, len);
  EXPECT_EQ(decode.header.version, PowerReadingMsg::VERSION);
  EXPECT_EQ(decode.header.reading_time_utc_ms, 123456789);
  EXPECT_EQ(decode.header.reading_uptime_millis, 987654321);
  EXPECT_EQ(decode.header.sensor_reading_time_ms, 0xdeadc0de);
  EXPECT_EQ(decode.power_reading_type, PowerReadingMsg::SOURCE);
  EXPECT_EQ(decode.status, PowerReadingMsg::OVERCURRENT | PowerReadingMsg::UNDERVOLTAGE);
  EXPECT_EQ(decode.num_samples, 30);
  EXPECT_EQ(decode.averaging_window_length_s, 29.9778);
  EXPECT_EQ(decode.voltage_v_avg, 3.79);
  EXPECT_EQ(decode.voltage_v_min, 3.65);
  EXPECT_EQ(decode.voltage_v_max, 3.84);
  EXPECT_EQ(decode.voltage_v_stdev, 0.012);
  EXPECT_EQ(decode.current_a_avg, 0.258);
  EXPECT_EQ(decode.current_a_min, 0.248);
  EXPECT_EQ(decode.current_a_max, 0.270);
  EXPECT_EQ(decode.current_a_stdev, 0.016);
}

TEST_F(BmCommonTest, PowerBatteryTest) {
  CborError err = CborNoError;
  // Test with num_cells and num_temp_sensors == 1
  PowerBatteryMsg::Data d;
  d.header.version = PowerBatteryMsg::VERSION;
  d.header.reading_time_utc_ms = 123456789;
  d.header.reading_uptime_millis = 987654321;
  d.header.sensor_reading_time_ms = 0xdeadc0de;
  d.power_reading_type = PowerReadingMsg::SOURCE;
  d.status = PowerReadingMsg::OVERCURRENT | PowerReadingMsg::UNDERVOLTAGE;
  d.voltage_v = 6.79;
  d.current_a = 0.123;
  d.charge_ah = 5.6;
  d.capacity_ah = 7.0;
  d.percentage = 0.8;
  d.battery_status = PowerBatteryMsg::DISCHARGING;
  d.battery_health = PowerBatteryMsg::GOOD;
  d.num_cell_voltages = 1;
  d.num_temp_sensors = 1;
  d.cell_voltage_v = (double *)malloc(sizeof(double));
  d.cell_voltage_v[0] = 6.79;
  d.cell_temperature_c = (double *)malloc(sizeof(double));
  d.cell_temperature_c[0] = 25.6;

  uint8_t cbor_buffer[1024];
  size_t len = 0;
  PowerBatteryMsg::encode(d, cbor_buffer, sizeof(cbor_buffer), &len);
  EXPECT_EQ(len, 302);

  PowerBatteryMsg::Data decode = {};

  err = PowerBatteryMsg::decode(decode, cbor_buffer, len);
  EXPECT_EQ(err, CborNoError);
  EXPECT_EQ(decode.header.version, d.header.version);
  EXPECT_EQ(decode.header.reading_time_utc_ms, d.header.reading_time_utc_ms);
  EXPECT_EQ(decode.header.reading_uptime_millis, d.header.reading_uptime_millis);
  EXPECT_EQ(decode.header.sensor_reading_time_ms, d.header.sensor_reading_time_ms);
  EXPECT_EQ(decode.power_reading_type, d.power_reading_type);
  EXPECT_EQ(decode.status, d.status);
  EXPECT_EQ(decode.voltage_v, d.voltage_v);
  EXPECT_EQ(decode.current_a, d.current_a);
  EXPECT_EQ(decode.charge_ah, d.charge_ah);
  EXPECT_EQ(decode.capacity_ah, d.capacity_ah);
  EXPECT_EQ(decode.percentage, d.percentage);
  EXPECT_EQ(decode.battery_status, d.battery_status);
  EXPECT_EQ(decode.battery_health, d.battery_health);
  EXPECT_EQ(decode.num_cell_voltages, d.num_cell_voltages);
  EXPECT_EQ(decode.num_temp_sensors, d.num_temp_sensors);
  EXPECT_EQ(decode.cell_voltage_v[0], d.cell_voltage_v[0]);
  EXPECT_EQ(decode.cell_temperature_c[0], d.cell_temperature_c[0]);

  free(d.cell_voltage_v);
  free(d.cell_temperature_c);
  free(decode.cell_voltage_v);
  free(decode.cell_temperature_c);

  // Test with num_cells == 0
  PowerBatteryMsg::Data d2 = {};
  d2.header.version = PowerBatteryMsg::VERSION;
  d2.header.reading_time_utc_ms = 123456789;
  d2.header.reading_uptime_millis = 987654321;
  d2.header.sensor_reading_time_ms = 0xdeadc0de;
  d2.power_reading_type = PowerReadingMsg::SOURCE;
  d2.status = PowerReadingMsg::OVERCURRENT | PowerReadingMsg::UNDERVOLTAGE;
  d2.voltage_v = 6.79;
  d2.current_a = 0.123;
  d2.charge_ah = 5.6;
  d2.capacity_ah = 7.0;
  d2.percentage = 0.8;
  d2.battery_status = PowerBatteryMsg::DISCHARGING;
  d2.battery_health = PowerBatteryMsg::GOOD;
  d2.num_cell_voltages = 0;
  d2.num_temp_sensors = 0;

  uint8_t cbor_buffer2[1024];
  size_t len2 = 0;
  PowerBatteryMsg::encode(d2, cbor_buffer2, sizeof(cbor_buffer2), &len2);
  EXPECT_EQ(len2, 284);

  PowerBatteryMsg::Data decode2;
  err = PowerBatteryMsg::decode(decode2, cbor_buffer2, len2);
  EXPECT_EQ(err, CborNoError);
  EXPECT_EQ(decode2.header.version, d2.header.version);
  EXPECT_EQ(decode2.header.reading_time_utc_ms, d2.header.reading_time_utc_ms);
  EXPECT_EQ(decode2.header.reading_uptime_millis, d2.header.reading_uptime_millis);
  EXPECT_EQ(decode2.header.sensor_reading_time_ms, d2.header.sensor_reading_time_ms);
  EXPECT_EQ(decode2.power_reading_type, d2.power_reading_type);
  EXPECT_EQ(decode2.status, d2.status);
  EXPECT_EQ(decode2.voltage_v, d2.voltage_v);
  EXPECT_EQ(decode2.current_a, d2.current_a);
  EXPECT_EQ(decode2.charge_ah, d2.charge_ah);
  EXPECT_EQ(decode2.capacity_ah, d2.capacity_ah);
  EXPECT_EQ(decode2.percentage, d2.percentage);
  EXPECT_EQ(decode2.battery_status, d2.battery_status);
  EXPECT_EQ(decode2.battery_health, d2.battery_health);
  EXPECT_EQ(decode2.num_cell_voltages, d2.num_cell_voltages);
  EXPECT_EQ(decode2.num_temp_sensors, d2.num_temp_sensors);

  // Test with num_cell_voltages and num_temp_sensors == 5
  PowerBatteryMsg::Data d3;
  d3.header.version = PowerBatteryMsg::VERSION;
  d3.header.reading_time_utc_ms = 123456789;
  d3.header.reading_uptime_millis = 987654321;
  d3.header.sensor_reading_time_ms = 0xdeadc0de;
  d3.power_reading_type = PowerReadingMsg::SOURCE;
  d3.status = PowerReadingMsg::OVERCURRENT | PowerReadingMsg::UNDERVOLTAGE;
  d3.voltage_v = 6.79;
  d3.current_a = 0.123;
  d3.charge_ah = 5.6;
  d3.capacity_ah = 7.0;
  d3.percentage = 0.8;
  d3.battery_status = PowerBatteryMsg::DISCHARGING;
  d3.battery_health = PowerBatteryMsg::GOOD;
  d3.num_cell_voltages = 5;
  d3.num_temp_sensors = 5;
  d3.cell_voltage_v = (double *)malloc(sizeof(double) * d3.num_cell_voltages);
  d3.cell_voltage_v[0] = 6.79;
  d3.cell_voltage_v[1] = 5.79;
  d3.cell_voltage_v[2] = 7.79;
  d3.cell_voltage_v[3] = 6.54;
  d3.cell_voltage_v[4] = 6.90;
  d3.cell_temperature_c = (double *)malloc(sizeof(double) * d3.num_temp_sensors);
  d3.cell_temperature_c[0] = 25.60;
  d3.cell_temperature_c[1] = 24.63;
  d3.cell_temperature_c[2] = 23.26;
  d3.cell_temperature_c[3] = 27.90;
  d3.cell_temperature_c[4] = 25.49;

  uint8_t cbor_buffer3[1024];
  size_t len3 = 0;
  PowerBatteryMsg::encode(d3, cbor_buffer3, sizeof(cbor_buffer3), &len3);
  EXPECT_EQ(len3, 374);

  PowerBatteryMsg::Data decode3 = {};
  err = PowerBatteryMsg::decode(decode3, cbor_buffer3, len3);
  EXPECT_EQ(err, CborNoError);
  EXPECT_EQ(decode3.header.version, d3.header.version);
  EXPECT_EQ(decode3.header.reading_time_utc_ms, d3.header.reading_time_utc_ms);
  EXPECT_EQ(decode3.header.reading_uptime_millis, d3.header.reading_uptime_millis);
  EXPECT_EQ(decode3.header.sensor_reading_time_ms, d3.header.sensor_reading_time_ms);
  EXPECT_EQ(decode3.power_reading_type, d3.power_reading_type);
  EXPECT_EQ(decode3.status, d3.status);
  EXPECT_EQ(decode3.voltage_v, d3.voltage_v);
  EXPECT_EQ(decode3.current_a, d3.current_a);
  EXPECT_EQ(decode3.charge_ah, d3.charge_ah);
  EXPECT_EQ(decode3.capacity_ah, d3.capacity_ah);
  EXPECT_EQ(decode3.percentage, d3.percentage);
  EXPECT_EQ(decode3.battery_status, d3.battery_status);
  EXPECT_EQ(decode3.battery_health, d3.battery_health);
  EXPECT_EQ(decode3.num_cell_voltages, d3.num_cell_voltages);
  EXPECT_EQ(decode3.num_temp_sensors, d3.num_temp_sensors);
  EXPECT_EQ(decode3.cell_voltage_v[0], d3.cell_voltage_v[0]);
  EXPECT_EQ(decode3.cell_voltage_v[1], d3.cell_voltage_v[1]);
  EXPECT_EQ(decode3.cell_voltage_v[2], d3.cell_voltage_v[2]);
  EXPECT_EQ(decode3.cell_voltage_v[3], d3.cell_voltage_v[3]);
  EXPECT_EQ(decode3.cell_voltage_v[4], d3.cell_voltage_v[4]);
  EXPECT_EQ(decode3.cell_temperature_c[0], d3.cell_temperature_c[0]);
  EXPECT_EQ(decode3.cell_temperature_c[1], d3.cell_temperature_c[1]);
  EXPECT_EQ(decode3.cell_temperature_c[2], d3.cell_temperature_c[2]);
  EXPECT_EQ(decode3.cell_temperature_c[3], d3.cell_temperature_c[3]);
  EXPECT_EQ(decode3.cell_temperature_c[4], d3.cell_temperature_c[4]);

  free(d3.cell_voltage_v);
  free(d3.cell_temperature_c);
  free(decode3.cell_voltage_v);
  free(decode3.cell_temperature_c);
}

TEST_F(BmCommonTest, PowerBatteryAveragesTest) {
  // Test with num_cell_voltages and num_temp_sensors == 1
  PowerBatteryAveragesMsg::Data d;
  d.header.version = PowerBatteryAveragesMsg::VERSION;
  d.header.reading_time_utc_ms = 123456789;
  d.header.reading_uptime_millis = 987654321;
  d.header.sensor_reading_time_ms = 0xdeadc0de;
  d.power_reading_type = PowerReadingMsg::SOURCE;
  d.status = PowerReadingMsg::OVERCURRENT | PowerReadingMsg::UNDERVOLTAGE;
  d.num_samples = 15;
  d.averaging_window_length_s = 29.98;
  d.num_cell_voltages = 1;
  d.num_temp_sensors = 1;
  d.cell_voltage_v_avg = (double *)malloc(sizeof(double) * d.num_cell_voltages);
  d.cell_voltage_v_max = (double *)malloc(sizeof(double) * d.num_cell_voltages);
  d.cell_voltage_v_min = (double *)malloc(sizeof(double) * d.num_cell_voltages);
  d.cell_voltage_v_stdev = (double *)malloc(sizeof(double) * d.num_cell_voltages);
  d.cell_temperature_c_avg = (double *)malloc(sizeof(double) * d.num_temp_sensors);
  d.cell_temperature_c_max = (double *)malloc(sizeof(double) * d.num_temp_sensors);
  d.cell_temperature_c_min = (double *)malloc(sizeof(double) * d.num_temp_sensors);
  d.cell_temperature_c_stdev = (double *)malloc(sizeof(double) * d.num_temp_sensors);
  for (size_t i = 0; i < d.num_cell_voltages; i++) {
    d.cell_voltage_v_avg[i] = 6.85;
    d.cell_voltage_v_max[i] = 7.12;
    d.cell_voltage_v_min[i] = 6.58;
    d.cell_voltage_v_stdev[i] = 0.123;
    d.cell_temperature_c_avg[i] = 24.935;
    d.cell_temperature_c_max[i] = 26.49;
    d.cell_temperature_c_min[i] = 23.38;
    d.cell_temperature_c_stdev[i] = 0.126;
  }

  uint8_t cbor_buffer[1024];
  size_t len = 0;
  CborError err = PowerBatteryAveragesMsg::encode(d, cbor_buffer, sizeof(cbor_buffer), &len);
  EXPECT_EQ(err, CborNoError);
  EXPECT_EQ(len, 420);

  PowerBatteryAveragesMsg::Data decode = {};
  PowerBatteryAveragesMsg::decode(decode, cbor_buffer, len);
  EXPECT_EQ(decode.header.version, d.header.version);
  EXPECT_EQ(decode.header.reading_time_utc_ms, d.header.reading_time_utc_ms);
  EXPECT_EQ(decode.header.reading_uptime_millis, d.header.reading_uptime_millis);
  EXPECT_EQ(decode.header.sensor_reading_time_ms, d.header.sensor_reading_time_ms);
  EXPECT_EQ(decode.power_reading_type, d.power_reading_type);
  EXPECT_EQ(decode.status, d.status);
  EXPECT_EQ(decode.num_samples, d.num_samples);
  EXPECT_EQ(decode.averaging_window_length_s, d.averaging_window_length_s);
  EXPECT_EQ(decode.num_cell_voltages, d.num_cell_voltages);
  EXPECT_EQ(decode.num_temp_sensors, d.num_temp_sensors);
  for (size_t i = 0; i < d.num_cell_voltages; i++) {
    EXPECT_EQ(decode.cell_voltage_v_avg[i], d.cell_voltage_v_avg[i]);
    EXPECT_EQ(decode.cell_voltage_v_max[i], d.cell_voltage_v_max[i]);
    EXPECT_EQ(decode.cell_voltage_v_min[i], d.cell_voltage_v_min[i]);
    EXPECT_EQ(decode.cell_voltage_v_stdev[i], d.cell_voltage_v_stdev[i]);
    EXPECT_EQ(decode.cell_temperature_c_avg[i], d.cell_temperature_c_avg[i]);
    EXPECT_EQ(decode.cell_temperature_c_max[i], d.cell_temperature_c_max[i]);
    EXPECT_EQ(decode.cell_temperature_c_min[i], d.cell_temperature_c_min[i]);
    EXPECT_EQ(decode.cell_temperature_c_stdev[i], d.cell_temperature_c_stdev[i]);
  }

  PowerBatteryAveragesMsg::free(d);
  PowerBatteryAveragesMsg::free(decode);

  // Test to make sure the pointers get set to NULL
  EXPECT_FALSE(d.cell_voltage_v_avg);
  EXPECT_FALSE(d.cell_voltage_v_max);
  EXPECT_FALSE(d.cell_voltage_v_min);
  EXPECT_FALSE(d.cell_voltage_v_stdev);
  EXPECT_FALSE(d.cell_temperature_c_avg);
  EXPECT_FALSE(d.cell_temperature_c_max);
  EXPECT_FALSE(d.cell_temperature_c_min);
  EXPECT_FALSE(d.cell_temperature_c_stdev);

  // Should be safe to call again, i.e. won't crash test
  PowerBatteryAveragesMsg::free(d);
  PowerBatteryAveragesMsg::free(decode);

  // Check the pointers are still NULL
  EXPECT_FALSE(d.cell_voltage_v_avg);
  EXPECT_FALSE(d.cell_voltage_v_max);
  EXPECT_FALSE(d.cell_voltage_v_min);
  EXPECT_FALSE(d.cell_voltage_v_stdev);
  EXPECT_FALSE(d.cell_temperature_c_avg);
  EXPECT_FALSE(d.cell_temperature_c_max);
  EXPECT_FALSE(d.cell_temperature_c_min);
  EXPECT_FALSE(d.cell_temperature_c_stdev);

  // Test with num_cell_voltages and num_temp_sensors == 5
  PowerBatteryAveragesMsg::Data d2;
  d2.header.version = PowerBatteryAveragesMsg::VERSION;
  d2.header.reading_time_utc_ms = 123456789;
  d2.header.reading_uptime_millis = 987654321;
  d2.header.sensor_reading_time_ms = 0xdeadc0de;
  d2.power_reading_type = PowerReadingMsg::SOURCE;
  d2.status = PowerReadingMsg::OVERCURRENT | PowerReadingMsg::UNDERVOLTAGE;
  d2.num_samples = 15;
  d2.averaging_window_length_s = 29.98;
  d2.num_cell_voltages = 5;
  d2.num_temp_sensors = 5;
  d2.cell_voltage_v_avg = (double *)malloc(sizeof(double) * d2.num_cell_voltages);
  d2.cell_voltage_v_max = (double *)malloc(sizeof(double) * d2.num_cell_voltages);
  d2.cell_voltage_v_min = (double *)malloc(sizeof(double) * d2.num_cell_voltages);
  d2.cell_voltage_v_stdev = (double *)malloc(sizeof(double) * d2.num_cell_voltages);
  d2.cell_temperature_c_avg = (double *)malloc(sizeof(double) * d2.num_temp_sensors);
  d2.cell_temperature_c_max = (double *)malloc(sizeof(double) * d2.num_temp_sensors);
  d2.cell_temperature_c_min = (double *)malloc(sizeof(double) * d2.num_temp_sensors);
  d2.cell_temperature_c_stdev = (double *)malloc(sizeof(double) * d2.num_temp_sensors);
  for (size_t i = 0; i < d2.num_cell_voltages; i++) {
    d2.cell_voltage_v_avg[i] = 6.85;
    d2.cell_voltage_v_max[i] = 7.12;
    d2.cell_voltage_v_min[i] = 6.58;
    d2.cell_voltage_v_stdev[i] = 0.123;
    d2.cell_temperature_c_avg[i] = 24.935;
    d2.cell_temperature_c_max[i] = 26.49;
    d2.cell_temperature_c_min[i] = 23.38;
    d2.cell_temperature_c_stdev[i] = 0.126;
  }

  uint8_t cbor_buffer2[1024];
  size_t len2 = 0;
  err = PowerBatteryAveragesMsg::encode(d2, cbor_buffer2, sizeof(cbor_buffer2), &len2);
  EXPECT_EQ(err, CborNoError);
  EXPECT_EQ(len2, 708);

  PowerBatteryAveragesMsg::Data decode2 = {};
  PowerBatteryAveragesMsg::decode(decode2, cbor_buffer2, len2);
  EXPECT_EQ(decode2.header.version, d2.header.version);
  EXPECT_EQ(decode2.header.reading_time_utc_ms, d2.header.reading_time_utc_ms);
  EXPECT_EQ(decode2.header.reading_uptime_millis, d2.header.reading_uptime_millis);
  EXPECT_EQ(decode2.header.sensor_reading_time_ms, d2.header.sensor_reading_time_ms);
  EXPECT_EQ(decode2.power_reading_type, d2.power_reading_type);
  EXPECT_EQ(decode2.status, d2.status);
  EXPECT_EQ(decode2.num_samples, d2.num_samples);
  EXPECT_EQ(decode2.averaging_window_length_s, d2.averaging_window_length_s);
  EXPECT_EQ(decode2.num_cell_voltages, d2.num_cell_voltages);
  EXPECT_EQ(decode2.num_temp_sensors, d2.num_temp_sensors);
  for (size_t i = 0; i < d2.num_cell_voltages; i++) {
    EXPECT_EQ(decode2.cell_voltage_v_avg[i], d2.cell_voltage_v_avg[i]);
    EXPECT_EQ(decode2.cell_voltage_v_max[i], d2.cell_voltage_v_max[i]);
    EXPECT_EQ(decode2.cell_voltage_v_min[i], d2.cell_voltage_v_min[i]);
    EXPECT_EQ(decode2.cell_voltage_v_stdev[i], d2.cell_voltage_v_stdev[i]);
    EXPECT_EQ(decode2.cell_temperature_c_avg[i], d2.cell_temperature_c_avg[i]);
    EXPECT_EQ(decode2.cell_temperature_c_max[i], d2.cell_temperature_c_max[i]);
    EXPECT_EQ(decode2.cell_temperature_c_min[i], d2.cell_temperature_c_min[i]);
    EXPECT_EQ(decode2.cell_temperature_c_stdev[i], d2.cell_temperature_c_stdev[i]);
  }

  PowerBatteryAveragesMsg::free(d2);
  PowerBatteryAveragesMsg::free(decode2);
}

TEST_F(BmCommonTest, PowerSolarReadingTest) {
  CborError err = CborNoError;
  // Test with num_panels == 1
  PowerSolarReadingMsg::Data d;
  d.header.version = PowerSolarReadingMsg::VERSION;
  d.header.reading_time_utc_ms = 123456789;
  d.header.reading_uptime_millis = 987654321;
  d.header.sensor_reading_time_ms = 0xdeadc0de;
  d.power_reading_type = PowerReadingMsg::SOURCE;
  d.status = PowerReadingMsg::OKAY;
  d.voltage_v = 24.5;
  d.current_a = 8.3;
  d.mpp_position = 95.5;
  d.num_panels = 1;
  d.panel_temperatures = (double *)malloc(sizeof(double));
  d.panel_temperatures[0] = 45.2;
  d.panel_voltages = (double *)malloc(sizeof(double));
  d.panel_voltages[0] = 24.5;
  d.panel_currents = (double *)malloc(sizeof(double));
  d.panel_currents[0] = 8.3;

  uint8_t cbor_buffer[1024];
  size_t len = 0;
  PowerSolarReadingMsg::encode(d, cbor_buffer, sizeof(cbor_buffer), &len);
  EXPECT_EQ(len, 269);

  PowerSolarReadingMsg::Data decode = {};
  err = PowerSolarReadingMsg::decode(decode, cbor_buffer, len);
  EXPECT_EQ(err, CborNoError);
  EXPECT_EQ(decode.header.version, d.header.version);
  EXPECT_EQ(decode.header.reading_time_utc_ms, d.header.reading_time_utc_ms);
  EXPECT_EQ(decode.header.reading_uptime_millis, d.header.reading_uptime_millis);
  EXPECT_EQ(decode.header.sensor_reading_time_ms, d.header.sensor_reading_time_ms);
  EXPECT_EQ(decode.power_reading_type, d.power_reading_type);
  EXPECT_EQ(decode.status, d.status);
  EXPECT_EQ(decode.voltage_v, d.voltage_v);
  EXPECT_EQ(decode.current_a, d.current_a);
  EXPECT_EQ(decode.mpp_position, d.mpp_position);
  EXPECT_EQ(decode.num_panels, d.num_panels);
  EXPECT_EQ(decode.panel_temperatures[0], d.panel_temperatures[0]);
  EXPECT_EQ(decode.panel_voltages[0], d.panel_voltages[0]);
  EXPECT_EQ(decode.panel_currents[0], d.panel_currents[0]);

  free(d.panel_temperatures);
  free(d.panel_voltages);
  free(d.panel_currents);
  free(decode.panel_temperatures);
  free(decode.panel_voltages);
  free(decode.panel_currents);

  // Test with num_panels == 0
  PowerSolarReadingMsg::Data d2 = {};
  d2.header.version = PowerSolarReadingMsg::VERSION;
  d2.header.reading_time_utc_ms = 123456789;
  d2.header.reading_uptime_millis = 987654321;
  d2.header.sensor_reading_time_ms = 0xdeadc0de;
  d2.power_reading_type = PowerReadingMsg::SOURCE;
  d2.status = PowerReadingMsg::OKAY;
  d2.voltage_v = 24.5;
  d2.current_a = 8.3;
  d2.mpp_position = 95.5;
  d2.num_panels = 0;

  uint8_t cbor_buffer2[1024];
  size_t len2 = 0;
  PowerSolarReadingMsg::encode(d2, cbor_buffer2, sizeof(cbor_buffer2), &len2);
  EXPECT_EQ(len2, 242);

  PowerSolarReadingMsg::Data decode2 = {};
  err = PowerSolarReadingMsg::decode(decode2, cbor_buffer2, len2);
  EXPECT_EQ(err, CborNoError);
  EXPECT_EQ(decode2.header.version, d2.header.version);
  EXPECT_EQ(decode2.header.reading_time_utc_ms, d2.header.reading_time_utc_ms);
  EXPECT_EQ(decode2.header.reading_uptime_millis, d2.header.reading_uptime_millis);
  EXPECT_EQ(decode2.header.sensor_reading_time_ms, d2.header.sensor_reading_time_ms);
  EXPECT_EQ(decode2.power_reading_type, d2.power_reading_type);
  EXPECT_EQ(decode2.status, d2.status);
  EXPECT_EQ(decode2.voltage_v, d2.voltage_v);
  EXPECT_EQ(decode2.current_a, d2.current_a);
  EXPECT_EQ(decode2.mpp_position, d2.mpp_position);
  EXPECT_EQ(decode2.num_panels, d2.num_panels);

  // Test with num_panels == 5
  PowerSolarReadingMsg::Data d3;
  d3.header.version = PowerSolarReadingMsg::VERSION;
  d3.header.reading_time_utc_ms = 123456789;
  d3.header.reading_uptime_millis = 987654321;
  d3.header.sensor_reading_time_ms = 0xdeadc0de;
  d3.power_reading_type = PowerReadingMsg::SOURCE;
  d3.status = PowerReadingMsg::OKAY;
  d3.voltage_v = 120.0;
  d3.current_a = 40.5;
  d3.mpp_position = 98.3;
  d3.num_panels = 5;
  d3.panel_temperatures = (double *)malloc(sizeof(double) * d3.num_panels);
  d3.panel_temperatures[0] = 45.2;
  d3.panel_temperatures[1] = 46.1;
  d3.panel_temperatures[2] = 44.8;
  d3.panel_temperatures[3] = 47.3;
  d3.panel_temperatures[4] = 45.9;
  d3.panel_voltages = (double *)malloc(sizeof(double) * d3.num_panels);
  d3.panel_voltages[0] = 24.1;
  d3.panel_voltages[1] = 24.3;
  d3.panel_voltages[2] = 23.9;
  d3.panel_voltages[3] = 24.0;
  d3.panel_voltages[4] = 24.2;
  d3.panel_currents = (double *)malloc(sizeof(double) * d3.num_panels);
  d3.panel_currents[0] = 8.1;
  d3.panel_currents[1] = 8.2;
  d3.panel_currents[2] = 8.0;
  d3.panel_currents[3] = 8.15;
  d3.panel_currents[4] = 8.05;

  uint8_t cbor_buffer3[1024];
  size_t len3 = 0;
  PowerSolarReadingMsg::encode(d3, cbor_buffer3, sizeof(cbor_buffer3), &len3);
  EXPECT_EQ(len3, 377);

  PowerSolarReadingMsg::Data decode3 = {};
  err = PowerSolarReadingMsg::decode(decode3, cbor_buffer3, len3);
  EXPECT_EQ(err, CborNoError);
  EXPECT_EQ(decode3.header.version, d3.header.version);
  EXPECT_EQ(decode3.header.reading_time_utc_ms, d3.header.reading_time_utc_ms);
  EXPECT_EQ(decode3.header.reading_uptime_millis, d3.header.reading_uptime_millis);
  EXPECT_EQ(decode3.header.sensor_reading_time_ms, d3.header.sensor_reading_time_ms);
  EXPECT_EQ(decode3.power_reading_type, d3.power_reading_type);
  EXPECT_EQ(decode3.status, d3.status);
  EXPECT_EQ(decode3.voltage_v, d3.voltage_v);
  EXPECT_EQ(decode3.current_a, d3.current_a);
  EXPECT_EQ(decode3.mpp_position, d3.mpp_position);
  EXPECT_EQ(decode3.num_panels, d3.num_panels);
  EXPECT_EQ(decode3.panel_temperatures[0], d3.panel_temperatures[0]);
  EXPECT_EQ(decode3.panel_temperatures[1], d3.panel_temperatures[1]);
  EXPECT_EQ(decode3.panel_temperatures[2], d3.panel_temperatures[2]);
  EXPECT_EQ(decode3.panel_temperatures[3], d3.panel_temperatures[3]);
  EXPECT_EQ(decode3.panel_temperatures[4], d3.panel_temperatures[4]);
  EXPECT_EQ(decode3.panel_voltages[0], d3.panel_voltages[0]);
  EXPECT_EQ(decode3.panel_voltages[1], d3.panel_voltages[1]);
  EXPECT_EQ(decode3.panel_voltages[2], d3.panel_voltages[2]);
  EXPECT_EQ(decode3.panel_voltages[3], d3.panel_voltages[3]);
  EXPECT_EQ(decode3.panel_voltages[4], d3.panel_voltages[4]);
  EXPECT_EQ(decode3.panel_currents[0], d3.panel_currents[0]);
  EXPECT_EQ(decode3.panel_currents[1], d3.panel_currents[1]);
  EXPECT_EQ(decode3.panel_currents[2], d3.panel_currents[2]);
  EXPECT_EQ(decode3.panel_currents[3], d3.panel_currents[3]);
  EXPECT_EQ(decode3.panel_currents[4], d3.panel_currents[4]);

  free(d3.panel_temperatures);
  free(d3.panel_voltages);
  free(d3.panel_currents);
  free(decode3.panel_temperatures);
  free(decode3.panel_voltages);
  free(decode3.panel_currents);
}
