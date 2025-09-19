#include "gui.h"
#include "liblvgl/lvgl.h"
#include <cstring>
#include <string>

// void gui::ui_init() {
//   lv_init();

//   //   lv_display_t *display = lv_display_create(480, 272);
//   lv_obj_t *label = lv_label_create(lv_screen_active());

//   lv_label_set_text(label, "Hello world ");
//   pros::Clock::time_point t = pros::Clock::now();
//   while (true) {
//     double duration = (pros::Clock::now() - t).count();
//     std::string str = std::to_string(duration);
//     const char *cstr = str.c_str();

//     lv_label_set_text(label, cstr);
//     lv_timer_handler();
//     pros::delay(10);
//   }
// }

// this works btw but  i need to asynchronize it