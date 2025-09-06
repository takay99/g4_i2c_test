#pragma once

#include <cstddef>
#include <cstdint>

#include <stm32rcos/core.hpp>

#include "g4_i2c.hpp"

struct Quaternion {
  float w;
  float x;
  float y;
  float z;
};

struct AngularVelocity {
  float x;
  float y;
  float z;
};

struct Acceleration {
  float x;
  float y;
  float z;
};

class Bno055 {
public:
  // Page 0
  static constexpr uint8_t CHIP_ID = 0x00;
  static constexpr uint8_t ACC_ID = 0x01;
  static constexpr uint8_t MAG_ID = 0x02;
  static constexpr uint8_t GYR_ID = 0x03;
  static constexpr uint8_t SW_REV_ID_LSB = 0x04;
  static constexpr uint8_t SW_REV_ID_MSB = 0x05;
  static constexpr uint8_t BL_REV_ID = 0x06;
  static constexpr uint8_t PAGE_ID = 0x07;
  static constexpr uint8_t ACC_DATA_X_LSB = 0x08;
  static constexpr uint8_t ACC_DATA_X_MSB = 0x09;
  static constexpr uint8_t ACC_DATA_Y_LSB = 0x0A;
  static constexpr uint8_t ACC_DATA_Y_MSB = 0x0B;
  static constexpr uint8_t ACC_DATA_Z_LSB = 0x0C;
  static constexpr uint8_t ACC_DATA_Z_MSB = 0x0D;
  static constexpr uint8_t MAG_DATA_X_LSB = 0x0E;
  static constexpr uint8_t MAG_DATA_X_MSB = 0x0F;
  static constexpr uint8_t MAG_DATA_Y_LSB = 0x10;
  static constexpr uint8_t MAG_DATA_Y_MSB = 0x11;
  static constexpr uint8_t MAG_DATA_Z_LSB = 0x12;
  static constexpr uint8_t MAG_DATA_Z_MSB = 0x13;
  static constexpr uint8_t GYR_DATA_X_LSB = 0x14;
  static constexpr uint8_t GYR_DATA_X_MSB = 0x15;
  static constexpr uint8_t GYR_DATA_Y_LSB = 0x16;
  static constexpr uint8_t GYR_DATA_Y_MSB = 0x17;
  static constexpr uint8_t GYR_DATA_Z_LSB = 0x18;
  static constexpr uint8_t GYR_DATA_Z_MSB = 0x19;
  static constexpr uint8_t EUL_DATA_X_LSB = 0x1A;
  static constexpr uint8_t EUL_DATA_X_MSB = 0x1B;
  static constexpr uint8_t EUL_DATA_Y_LSB = 0x1C;
  static constexpr uint8_t EUL_DATA_Y_MSB = 0x1D;
  static constexpr uint8_t EUL_DATA_Z_LSB = 0x1E;
  static constexpr uint8_t EUL_DATA_Z_MSB = 0x1F;
  static constexpr uint8_t QUA_DATA_W_LSB = 0x20;
  static constexpr uint8_t QUA_DATA_W_MSB = 0x21;
  static constexpr uint8_t QUA_DATA_X_LSB = 0x22;
  static constexpr uint8_t QUA_DATA_X_MSB = 0x23;
  static constexpr uint8_t QUA_DATA_Y_LSB = 0x24;
  static constexpr uint8_t QUA_DATA_Y_MSB = 0x25;
  static constexpr uint8_t QUA_DATA_Z_LSB = 0x26;
  static constexpr uint8_t QUA_DATA_Z_MSB = 0x27;
  static constexpr uint8_t LIA_DATA_X_LSB = 0x28;
  static constexpr uint8_t LIA_DATA_X_MSB = 0x29;
  static constexpr uint8_t LIA_DATA_Y_LSB = 0x2A;
  static constexpr uint8_t LIA_DATA_Y_MSB = 0x2B;
  static constexpr uint8_t LIA_DATA_Z_LSB = 0x2C;
  static constexpr uint8_t LIA_DATA_Z_MSB = 0x2D;
  static constexpr uint8_t GRV_DATA_X_LSB = 0x2E;
  static constexpr uint8_t GRV_DATA_X_MSB = 0x2F;
  static constexpr uint8_t GRV_DATA_Y_LSB = 0x30;
  static constexpr uint8_t GRV_DATA_Y_MSB = 0x31;
  static constexpr uint8_t GRV_DATA_Z_LSB = 0x32;
  static constexpr uint8_t GRV_DATA_Z_MSB = 0x33;
  static constexpr uint8_t TEMP = 0x34;
  static constexpr uint8_t CALIB_STAT = 0x35;
  static constexpr uint8_t ST_RESULT = 0x36;
  static constexpr uint8_t INT_STA = 0x37;
  static constexpr uint8_t SYS_CLK_STATUS = 0x38;
  static constexpr uint8_t SYS_STATUS = 0x39;
  static constexpr uint8_t SYS_ERR = 0x3A;
  static constexpr uint8_t UNIT_SEL = 0x3B;
  static constexpr uint8_t OPR_MODE = 0x3D;
  static constexpr uint8_t PWR_MODE = 0x3E;
  static constexpr uint8_t SYS_TRIGGER = 0x3F;
  static constexpr uint8_t TEMP_SOURCE = 0x40;
  static constexpr uint8_t AXIS_MAP_CONFIG = 0x41;
  static constexpr uint8_t AXIS_MAP_SIGN = 0x42;
  static constexpr uint8_t ACC_OFFSET_X_LSB = 0x55;
  static constexpr uint8_t ACC_OFFSET_X_MSB = 0x56;
  static constexpr uint8_t ACC_OFFSET_Y_LSB = 0x57;
  static constexpr uint8_t ACC_OFFSET_Y_MSB = 0x58;
  static constexpr uint8_t ACC_OFFSET_Z_LSB = 0x59;
  static constexpr uint8_t ACC_OFFSET_Z_MSB = 0x5A;
  static constexpr uint8_t MAG_OFFSET_X_LSB = 0x5B;
  static constexpr uint8_t MAG_OFFSET_X_MSB = 0x5C;
  static constexpr uint8_t MAG_OFFSET_Y_LSB = 0x5D;
  static constexpr uint8_t MAG_OFFSET_Y_MSB = 0x5E;
  static constexpr uint8_t MAG_OFFSET_Z_LSB = 0x5F;
  static constexpr uint8_t MAG_OFFSET_Z_MSB = 0x60;
  static constexpr uint8_t GYR_OFFSET_X_LSB = 0x61;
  static constexpr uint8_t GYR_OFFSET_X_MSB = 0x62;
  static constexpr uint8_t GYR_OFFSET_Y_LSB = 0x63;
  static constexpr uint8_t GYR_OFFSET_Y_MSB = 0x64;
  static constexpr uint8_t GYR_OFFSET_Z_LSB = 0x65;
  static constexpr uint8_t GYR_OFFSET_Z_MSB = 0x66;
  static constexpr uint8_t ACC_RADIUS_LSB = 0x67;
  static constexpr uint8_t ACC_RADIUS_MSB = 0x68;
  static constexpr uint8_t MAG_RADIUS_LSB = 0x69;
  static constexpr uint8_t MAG_RADIUS_MSB = 0x6A;
  // Page 1
  static constexpr uint8_t ACC_CONFIG = 0x08;
  static constexpr uint8_t MAG_CONFIG = 0x09;
  static constexpr uint8_t GYR_CONFIG_0 = 0x0A;
  static constexpr uint8_t GYR_CONFIG_1 = 0x0B;
  static constexpr uint8_t ACC_SLEEP_CONFIG = 0x0C;
  static constexpr uint8_t GYR_SLEEP_CONFIG = 0x0D;
  static constexpr uint8_t INT_MSK = 0x0F;
  static constexpr uint8_t INT_EN = 0x10;
  static constexpr uint8_t ACC_AM_THRES = 0x11;
  static constexpr uint8_t ACC_INT_SETTINGS = 0x12;
  static constexpr uint8_t ACC_HG_DURATION = 0x13;
  static constexpr uint8_t ACC_HG_THRES = 0x14;
  static constexpr uint8_t ACC_NM_THRES = 0x15;
  static constexpr uint8_t ACC_NM_SET = 0x16;
  static constexpr uint8_t GYR_INT_SETTING = 0x17;
  static constexpr uint8_t GYR_HR_X_SET = 0x18;
  static constexpr uint8_t GYR_DUR_X = 0x19;
  static constexpr uint8_t GYR_HR_Y_SET = 0x1A;
  static constexpr uint8_t GYR_DUR_Y = 0x1B;
  static constexpr uint8_t GYR_HR_Z_SET = 0x1C;
  static constexpr uint8_t GYR_DUR_Z = 0x1D;
  static constexpr uint8_t GYR_AM_THRES = 0x1E;
  static constexpr uint8_t GYR_AM_SET = 0x1F;

  Bno055(I2C &i2c) : i2c_{i2c} {}

  bool start(uint32_t timeout) {
    stm32rcos::core::TimeoutHelper timeout_helper;
    while (!timeout_helper.is_timeout(timeout)) {
      if (!write_register(OPR_MODE, 0x00)) {
        continue;
      }
      if (!write_register(OPR_MODE, 0x08)) {
        continue;
      }
      return true;
    }
    return false;
  }

  bool write_register(uint8_t address, uint8_t data) {
    uint8_t buf[2] = {address, data};
    if (!i2c_.transmit(buf, sizeof(buf), 10)) {
      return false;
    }
    return true;
  }

  bool read_register(uint8_t address, uint8_t *data, size_t size) {
    if (!i2c_.transmit(&address, sizeof(address), 40)) {
      return false;
    }
    if (!i2c_.receive(data, size, 10)) {
      return false;
    }
    return true;
  }

  bool get_quaternion(Quaternion &quat) {
    uint8_t data[8];
    if (!read_register(QUA_DATA_W_LSB, data, sizeof(data))) {
      return false;
    }
    quat.w = (int16_t)((data[1] << 8) | data[0]);
    quat.x = (int16_t)((data[3] << 8) | data[2]);
    quat.y = (int16_t)((data[5] << 8) | data[4]);
    quat.z = (int16_t)((data[7] << 8) | data[6]);
    return true;
  }

  bool get_angular_velocity(AngularVelocity &ang_vel) {
    uint8_t data[6];
    if (!read_register(GYR_DATA_X_LSB, data, sizeof(data))) {
      return false;
    }
    ang_vel.x = (int16_t)((data[1] << 8) | data[0]) * 0.0010902f;
    ang_vel.y = (int16_t)((data[3] << 8) | data[2]) * 0.0010902f;
    ang_vel.z = (int16_t)((data[5] << 8) | data[4]) * 0.0010902f;
    // printf("Raw data: %02X %02X %02X %02X %02X %02X\n", data[0], data[1],
    //        data[2], data[3], data[4], data[5]);
    return true;
  }

  bool get_acceleration(Acceleration &acc) {
    uint8_t data[6];
    if (!read_register(LIA_DATA_X_LSB, data, sizeof(data))) {
      return false;
    }
    acc.x = (int16_t)((data[1] << 8) | data[0]) / 100.0f;
    acc.y = (int16_t)((data[1] << 8) | data[0]) / 100.0f;
    acc.z = (int16_t)((data[1] << 8) | data[0]) / 100.0f;
    return true;
  }

private:
  I2C &i2c_;
};
