#include <cstdio>
#include <ctime>

#include <stm32rcos/core.hpp>
#include <stm32rcos/hal.hpp>
#include <stm32rcos/peripheral.hpp>

#include "bno055.hpp"
#include "g4_i2c.hpp"

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

// I2C_HandleTypeDef *_bno055_i2c_port;

extern "C" void main_thread(void *) {
  using namespace stm32rcos::core;
  using namespace stm32rcos::peripheral;

  Uart<&huart2> uart2; // デバッグ出力用
  enable_stdout(uart2);

  I2C i2c_28(&hi2c1, 0x29); // BNO055のI2Cアドレスは0x28
  I2C i2c_29(&hi2c1, 0x28); // BNO055のI2Cアドレスは0x28
  Bno055 bno055_28(i2c_28);
  Bno055 bno055_29(i2c_29);

  while (!bno055_28.start(1000)) {
    printf("Failed to initialize BNO055. Retrying...\n");
    osDelay(100);
  }

  while (!bno055_29.start(1000)) {
    printf("Failed to initialize BNO055. Retrying...\n");
    osDelay(100);
  }

  while (true) {

    Quaternion quat_28, quat_29;
    AngularVelocity ang_vel_28, ang_vel_29;
    Acceleration acc_28, acc_29;

    // if (bno055_28.get_quaternion(quat_28)) {
    //   printf("Quaternion_28: w=%.3f, x=%.3f, y=%.3f, z=%.3f\r\n", quat_28.w,
    //          quat_28.x, quat_28.y, quat_28.z);
    // } else {
    //   printf("Failed to get quaternion data from BNO055.\n");
    // }
    // if (bno055_29.get_quaternion(quat_29)) {
    //   printf("Quaternion_29: w=%.3f, x=%.3f, y=%.3f, z=%.3f\r\n", quat_29.w,
    //          quat_29.x, quat_29.y, quat_29.z);
    // } else {
    //   printf("Failed to get quaternion data from BNO055.\n");
    // }

    // if (bno055_28.get_angular_velocity(ang_vel_28)) {
    //   printf("Quaternion_28: x=%.3f, y=%.3f, z=%.3f\r\n", ang_vel_28.x,
    //          ang_vel_28.y, ang_vel_28.z);
    // } else {
    //   printf("Failed to get quaternion data from BNO055.\n");
    // }

    // if (bno055_29.get_angular_velocity(ang_vel_29)) {
    //   printf("Quaternion_29: x=%.3f, y=%.3f, z=%.3f\r\n", ang_vel_29.x,
    //          ang_vel_29.y, ang_vel_29.z);
    // } else {
    //   printf("Failed to get quaternion data from BNO055.\n");
    // }

    if (bno055_28.get_acceleration(acc_28)) {
      printf("Quaternion_28: x=%.3f, y=%.3f, z=%.3f\r\n", acc_28.x, acc_28.y,
             acc_28.z);
    } else {
      printf("Failed to get quaternion data from BNO055.\n");
    }

    if (bno055_29.get_acceleration(acc_29)) {
      printf("Quaternion_29: x=%.3f, y=%.3f, z=%.3f\r\n", acc_29.x, acc_29.y,
             acc_29.z);
    } else {
      printf("Failed to get quaternion data from BNO055.\n");
    }
    osDelay(10);
  }
}
