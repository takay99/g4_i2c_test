#include <cstdio>
#include <ctime>

#include <stm32rcos/core.hpp>
#include <stm32rcos/hal.hpp>
#include <stm32rcos/peripheral.hpp>
#include <stm32rcos_drivers/bno055.hpp>
#include <stm32rcos_drivers/c6x0.hpp>

#include "bno055.h"
#include "lowpass.hpp"
#include "output_to_logger.hpp"

extern "C" void bno055_assignI2C(I2C_HandleTypeDef *hi2c_device);

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart1;
extern FDCAN_HandleTypeDef hfdcan1;
extern I2C_HandleTypeDef hi2c1;

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim2;

////////
#define LOWPASS_VAL_QTY 50

// typedef struct {
//   float acc_vect[LOWPASS_VAL_QTY];
//   float time_vect[LOWPASS_VAL_QTY];
// } lowpass_state_t;

// float lowpass_filter(lowpass_state_t *state, float motor_acc, float time) {
//   state->time_vect[0] = time;
//   state->acc_vect[0] = motor_acc;

//   float B = 0, C = 0, D = 0, E = 0;
//   for (int i = 0; i < LOWPASS_VAL_QTY; i++) {
//     B += state->time_vect[i] * state->time_vect[i];
//     C += state->acc_vect[i];
//     D += state->time_vect[i] * state->acc_vect[i];
//     E += state->time_vect[i];
//   }

//   float a = (LOWPASS_VAL_QTY * D - C * E) / (LOWPASS_VAL_QTY * B - E * E);
//   float b = (B * C - D * E) / (LOWPASS_VAL_QTY * B - E * E);
//   float filtered_motor_acc =
//       a * time + b; // 低域通過フィルタリングされたモータの加速度を計算

//   for (int i = LOWPASS_VAL_QTY - 1; i > 0; i--) {
//     state->time_vect[i] = state->time_vect[i - 1];
//     state->acc_vect[i] = state->acc_vect[i - 1];
//   }
//   return filtered_motor_acc;
// }
////////

#include "bno055.h"

#define I2C_HandleTypeDef int;
// I2C_HandleTypeDef *_bno055_i2c_port;

extern "C" void main_thread(void *) {
  using namespace stm32rcos::core;
  using namespace stm32rcos::peripheral;
  using namespace stm32rcos_drivers;

  Uart<&huart2> uart2; // デバッグ出力用
  enable_stdout(uart2);

  Can<&hfdcan1> can;
  C6x0Manager c610_manager(can);
  C6x0 c610_1(c610_manager, C6x0Type::C610, C6x0Id::ID_1); // front motor
  C6x0 c610_2(c610_manager, C6x0Type::C610, C6x0Id::ID_2); // rear motor
  can.start();

  LowPassFilter lowpass_filter_1;
  LowPassFilter lowpass_filter_2;
  LowPassFilter lowpass_filter_vel_1;
  LowPassFilter lowpass_filter_vel_2;

  LowPassFilter lowpass_filter_imu;

  HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);

  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2);

  // lowpass_state_t lowpass_state = {0};
  bno055_assignI2C(&hi2c1);
  bno055_setup();
  bno055_setOperationModeNDOF();

  while (true) {
    uint32_t start = osKernelGetTickCount();

    auto euler = bno055_getVectorEuler();

    printf("%.2f %.2f %.2f %.2f\r\n", euler.w, euler.x, euler.y, euler.z);

    // lowpass_filter(&lowpass_state, 0.0f, start / 1000.0f);

    osDelayUntil(start + 10);
  }
}
