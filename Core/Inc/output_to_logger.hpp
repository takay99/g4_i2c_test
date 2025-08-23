#pragma once

#include "csv_row.hpp"

#define LOGGER_CSV_ROW(FIELD)                                                  \
  FIELD(int, tick, "%d")                                                       \
  FIELD(float, get_gyr_x, "%.3f")                                              \
  FIELD(float, get_gyr_y, "%.3f")                                              \
  FIELD(float, get_gyr_z, "%.3f")                                              \
  FIELD(float, get_acc_x, "%.2f")                                              \
  FIELD(float, get_acc_y, "%.2f")                                              \
  FIELD(float, get_acc_z, "%.2f")                                              \
  FIELD(float, get_motor_acc_1, "%.2f")                                        \
  FIELD(float, get_motor_acc_2, "%.2f")                                        \
  FIELD(float, get_motor_control_signal_1, "%.0f")                             \
  FIELD(float, get_motor_control_signal_2, "%.0f")                             \
  FIELD(float, get_desire_yaw, "%.2f")                                         \
  FIELD(float, get_prevous_motor_rps_1, "%.2f")                                \
  FIELD(float, get_prevous_motor_rps_2, "%.2f")                                \
  FIELD(float, get_steer_angle, "%.3f")                                        \
  FIELD(float, get_motor_set_current_1, "%.3f")                                \
  FIELD(float, get_motor_set_current_2, "%.3f")

DEFINE_CSV_ROW(LoggerCsvRow, LOGGER_CSV_ROW)
