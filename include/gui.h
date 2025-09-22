#pragma once
#include "liblvgl/lvgl.h" // IWYU pragma: keep
#include "main.h"         // IWYU pragma: keep

typedef struct data {
  pros::MotorGroup *left_mg;
  pros::MotorGroup *right_mg;
  pros::Controller *controller;
  pros::Imu *imu;
  std::vector<pros::DeviceType> *devices;
  lemlib::Chassis *chassis;
} data_t;

class gui {
public:
  int ui_update();
  gui(data_t *data);

private:
  data_t *data;
  struct widgets {
    lv_obj_t *debug;

  } widgets;
}; // namespace gui