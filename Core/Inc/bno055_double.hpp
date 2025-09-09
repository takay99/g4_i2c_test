#pragma once

#include <cstddef>
#include <cstdint>

#include <stm32rcos/core.hpp>

#include "bno055.hpp"

AngularVelocity ang_vel_28, ang_vel_29;
Acceleration acc_28, acc_29;
struct Bno055Data {
  AngularVelocity ang_vel_28, ang_vel_29;
  Acceleration acc_28, acc_29;
};

class Bno055_double {
public:
  Bno055_double(I2C_HandleTypeDef *hi2c)

      : i2c_28(hi2c, 0x28), i2c_29(hi2c, 0x29), bno055_28(i2c_28),
        bno055_29(i2c_29) {}

  bool start(uint32_t timeout) {
    if (!bno055_28.start(timeout)) {
      return false;
    }
    if (!bno055_29.start(timeout)) {
      return false;
    }
    return true;
  }

  bool get_bno(Bno055Data &data) {
    if (!bno055_28.get_acceleration(data.acc_28)) {
      return false;
    }
    if (!bno055_29.get_acceleration(data.acc_29)) {
      return false;
    }
    if (!bno055_28.get_angular_velocity(data.ang_vel_28)) {
      return false;
    }
    if (!bno055_29.get_angular_velocity(data.ang_vel_29)) {
      return false;
    }
    return true;
  }

private:
  I2C i2c_28;
  I2C i2c_29;
  Bno055 bno055_28;
  Bno055 bno055_29;
};