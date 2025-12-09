#pragma once
#include <cstdio>
#include <ctime>
#include <stm32rcos/core.hpp>
#include <stm32rcos/hal.hpp>
#include <stm32rcos/peripheral.hpp>
#include <stm32rcos_drivers/c6x0.hpp>
using namespace stm32rcos_drivers;
class MotorFreeRev {
public:
  MotorFreeRev(C6x0 &motor, float gain) : motor_(motor), gain_(gain) {}
  float prevous_rpm_ = 0.0f;
  void free_rev(float &current) {

    current = gain_ * (motor_.get_rpm() - prevous_rpm_);
    prevous_rpm_ = motor_.get_rpm();
    if (current > 9000) {
      current = 0;
    }
    if (current < -9000) {
      current = 0;
    }
  }

private:
  C6x0 &motor_;
  float gain_;
};