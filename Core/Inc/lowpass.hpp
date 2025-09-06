#include <array>

class LowPassFilter {
private:
  static constexpr int val_qty = 20;
  std::array<float, val_qty> acc_vect = {0};
  std::array<float, val_qty> time_vect = {0};

public:
  float lowpass(float motor_acc, float time) {
    time_vect[0] = time;
    acc_vect[0] = motor_acc;

    float B = 0, C = 0, D = 0, E = 0;
    for (int i = 0; i < val_qty; i++) {
      B += time_vect[i] * time_vect[i];
      C += acc_vect[i];
      D += time_vect[i] * acc_vect[i];
      E += time_vect[i];
    }

    float a = (val_qty * D - C * E) / (val_qty * B - E * E);
    float b = (B * C - D * E) / (val_qty * B - E * E);
    float filtered_motor_acc =
        a * time + b; // 低域通過フィルタリングされたモータの加速度を計算

    for (int i = val_qty - 1; i > 0; i--) {
      time_vect[i] = time_vect[i - 1];
      acc_vect[i] = acc_vect[i - 1];
    }
    return filtered_motor_acc;
  }
};