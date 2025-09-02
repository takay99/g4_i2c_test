#pragma once
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <utility>

#include <Eigen/Geometry>

#include <stm32rcos/core.hpp>
#include <stm32rcos/hal.hpp>

class I2C {
public:
  // コンストラクタ
  I2C(I2C_HandleTypeDef *hi2c, uint16_t dev_addr)
      : hi2c_(hi2c), dev_addr_(dev_addr) {}

  bool transmit(const uint8_t *tx_data, uint16_t size, uint32_t timeout) {
    uint16_t dev_addr_8bit = dev_addr_ << 1;
    std::vector<uint8_t> data(tx_data, tx_data + size);
    return HAL_I2C_Master_Transmit(hi2c_, dev_addr_8bit, data.data(), size,
                                   timeout) == HAL_OK;
  }

  bool receive(uint8_t *rx_data, uint16_t size, uint32_t timeout) {
    uint16_t dev_addr_8bit = dev_addr_ << 1;
    return HAL_I2C_Master_Receive(hi2c_, dev_addr_8bit, rx_data, size,
                                  timeout) == HAL_OK;
  }

private:
  I2C_HandleTypeDef *hi2c_;
  uint16_t dev_addr_;
};