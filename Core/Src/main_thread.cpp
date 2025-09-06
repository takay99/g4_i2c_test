#include <cstdio>
#include <ctime>

#include <stm32rcos/core.hpp>
#include <stm32rcos/hal.hpp>
#include <stm32rcos/peripheral.hpp>
#include <stm32rcos_drivers/c6x0.hpp>

#include "bno055.hpp"
#include "g4_i2c.hpp"
#include "lowpass.hpp"
#include "output_to_logger.hpp"

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart1;
extern FDCAN_HandleTypeDef hfdcan1;
extern I2C_HandleTypeDef hi2c1;

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim2;
///////////////
struct bno055_thread_args {
  Bno055 *bno055_28;
  Bno055 *bno055_29;

  void thread_bno055(void *args) {
    auto arg = static_cast<bno055_thread_args *>(args);
    while (true) {
      bno055_28.get_acceleration(acc_28);
      bno055_29.get_acceleration(acc_29);
      bno055_28.get_angular_velocity(ang_vel_28);
      bno055_29.get_angular_velocity(ang_vel_29);
      osDelay(10);
    }
  }
  ///////
  extern "C" void main_thread(void *) {
    using namespace stm32rcos::core;
    using namespace stm32rcos::peripheral;
    using namespace stm32rcos_drivers;

    Uart<&huart2> uart2; // デバッグ出力用
    enable_stdout(uart2);

    I2C i2c_28(&hi2c1, 0x29); // BNO055のI2Cアドレスは0x28
    I2C i2c_29(&hi2c1, 0x28); // BNO055のI2Cアドレスは0x28
    Bno055 bno055_28(i2c_28);
    Bno055 bno055_29(i2c_29);

    LowPassFilter lowpass_filter_1;
    LowPassFilter lowpass_filter_2;
    LowPassFilter lowpass_filter_vel_1;
    LowPassFilter lowpass_filter_vel_2;
    LowPassFilter lowpass_filter_imu;

    Can<&hfdcan1> can;
    C6x0Manager c610_manager(can);
    C6x0 c610_1(c610_manager, C6x0Type::C610, C6x0Id::ID_1); // front motor
    C6x0 c610_2(c610_manager, C6x0Type::C610, C6x0Id::ID_2); // rear motor
    can.start();

    float a1;
    float a2;
    float b1;
    float b2;

    float st;
    float th;

    float motor_acc_1;
    float motor_acc_2;
    static float prevous_motor_rps_1;
    static float prevous_motor_rps_2;

    static float prevous_time;

    int motor_control_singnnal_1;
    int motor_control_singnnal_2;

    float steer_angle;
    float desire_yaw;
    std::vector<float> csvrow;

    float motor_set_current_1;
    float motor_set_current_2;
    float control_boarder = 0.1;

    float yaw_control_Kp = 4000.0f;
    float yaw_control_Ki = 40.0f;
    float error_yaw_sum = 0.0f;

    Quaternion quat_28, quat_29;
    AngularVelocity ang_vel_28, ang_vel_29;
    Acceleration acc_28, acc_29;

    HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
    HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);
    HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
    HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2);

    while (!bno055_28.start(1000)) {
      printf("Failed to initialize BNO055. Retrying...\n");
      osDelay(100);
    }

    while (!bno055_29.start(1000)) {
      printf("Failed to initialize BNO055. Retrying...\n");
      osDelay(100);
    }
    ////////////////

    bno055_thread_args arg = {&bno055_28, &bno055_29};

    Thread bno055_thread(thread_bno055, &arg, 4096, osPriorityNormal);
    // ///////////
    while (true) {
      uint32_t start = osKernelGetTickCount();

      a1 = __HAL_TIM_GET_COMPARE(&htim3, TIM_CHANNEL_1);
      a2 = __HAL_TIM_GET_COMPARE(&htim3, TIM_CHANNEL_2);
      st = ((a1 / a2) - 0.8914) / 0.0257;
      steer_angle = st * 17.0f * 3.14f / 180.0f;

      b1 = __HAL_TIM_GET_COMPARE(&htim2, TIM_CHANNEL_1);
      b2 = __HAL_TIM_GET_COMPARE(&htim2, TIM_CHANNEL_2);
      th = ((b1 / (b2)) - 0.89175) / (0.03175);

      c610_manager.update();

      float motor_vel_1 = lowpass_filter_vel_1.lowpass(c610_1.get_rpm() / 60.0f,
                                                       osKernelGetTickCount());
      float motor_vel_2 = lowpass_filter_vel_2.lowpass(c610_2.get_rpm() / 60.0f,
                                                       osKernelGetTickCount());

      motor_acc_1 = (motor_vel_1 * 0.0410311 - prevous_motor_rps_1) * 1000 /
                    (osKernelGetTickCount() -
                     prevous_time); // 回転数×車輛速度のための係数×ミリ秒
      motor_acc_2 = (motor_vel_2 * 0.0410311 - prevous_motor_rps_2) * 1000 /
                    (osKernelGetTickCount() - prevous_time);

      float filtered_motor_acc_1 =
          lowpass_filter_1.lowpass(motor_acc_1, osKernelGetTickCount());
      float filtered_motor_acc_2 =
          lowpass_filter_2.lowpass(motor_acc_2, osKernelGetTickCount());

      prevous_time = osKernelGetTickCount();

      prevous_motor_rps_1 = motor_vel_1 * 0.0410311;
      prevous_motor_rps_2 = motor_vel_2 * 0.0410311;

      motor_set_current_1 = -1 * th * 9000;
      motor_set_current_2 = th * 9000;

      c610_1.set_current_ref(motor_set_current_1); // マイナス前進
      c610_2.set_current_ref(motor_set_current_2); // プラス前進
      c610_manager.transmit();

      // if (bno055_28.get_acceleration(acc_28)) {
      //   // printf("Quaternion_28: x=%.3f, y=%.3f, z=%.3f\r\n", acc_28.x,
      //   acc_28.y,
      //   //  acc_28.z);
      // } else {
      //   printf("Failed to get quaternion data from BNO055.\n");
      // }

      // if (bno055_29.get_acceleration(acc_29)) {
      //   // printf("Quaternion_29: x=%.3f, y=%.3f, z=%.3f\r\n", acc_29.x,
      //   acc_29.y,
      //   //  acc_29.z);
      // } else {
      //   printf("Failed to get quaternion data from BNO055.\n");
      // }

      LoggerCsvRow row;
      row.tick = osKernelGetTickCount();
      row.get_acc_x_28 = acc_28.x;
      row.get_acc_y_28 = acc_28.y;
      row.get_gyr_z_28 = ang_vel_28.z;
      row.get_acc_x_29 = acc_29.x;
      row.get_acc_y_29 = acc_29.y;
      row.get_gyr_z_29 = ang_vel_29.z;
      row.get_motor_acc_1 = filtered_motor_acc_1;
      row.get_motor_acc_2 = filtered_motor_acc_2;
      row.get_prevous_motor_rps_1 = prevous_motor_rps_1;
      row.get_prevous_motor_rps_2 = prevous_motor_rps_2;
      row.get_steer_angle = steer_angle;
      row.get_motor_set_current_1 = motor_set_current_1;
      row.get_motor_set_current_2 = motor_set_current_2;

      printf("%s\n", row.format());
      osDelayUntil(start + 10);
    }
  }
