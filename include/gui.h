// #pragma once
// #include "liblvgl/lvgl.h" // IWYU pragma: keep
// #include "main.h"         // IWYU pragma: keep

// typedef struct data {
//   pros::MotorGroup *left_mg;
//   pros::MotorGroup *right_mg;
//   pros::Controller *controller;
//   pros::Imu *imu;
//   std::vector<pros::DeviceType> *devices;
//   lemlib::Chassis *chassis;
// } data_t;
// typedef enum ANSI_COLOR {
//   BOLD = 1,
//   ITALIC = 3,
//   UNDERLINE = 4,
//   BLINKING = 5,
//   INVERSE = 7,
//   STRIKETHROUGH = 9,
//   BLACK = 30,
//   RED = 31,
//   GREEN = 32,
//   YELLOW = 33,
//   BLUE = 34,
//   MAGENTA = 35,
//   CYAN = 36,
//   WHITE = 37,
//   DEFAULT = 39
// } ANSI_COLOR;

// class gui {
// public:
//   int ui_update();
//   gui(data_t *data);

// private:
//   data_t *data;
//   struct widgets {
//     lv_obj_t *debug;

//   } widgets;
// }; // namespace gui