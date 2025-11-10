#include "gui.h"
#include "liblvgl/lvgl.h" // IWYU pragma: keep

gui::gui(data_t *data) {
  this->data = data;

  lv_obj_t *debug = lv_label_create(lv_screen_active());
  lv_obj_align(debug, LV_ALIGN_TOP_LEFT, 0, 0);
  lv_label_set_text(debug, "GUI INIT");
  this->widgets.debug = debug;

  lv_obj_t *title = lv_label_create(lv_screen_active());
  lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 0);
  lv_label_set_text(title, "90385B Pumas: Orange");
}

int gui::ui_update() {
  char buf[256];
  std::vector<double> left_temp = data->left_mg->get_position_all();
  std::vector<double> right_temp = data->right_mg->get_position_all();
  sprintf(buf,
          "Temp 1: %.2f Temp 2: %.2f Temp 3: %.2f\nTemp 4: %.2f Temp "
          "5: %.2f Temp 6: %.2f",
          left_temp.at(0), left_temp.at(1), left_temp.at(2), right_temp.at(0),
          right_temp.at(1), right_temp.at(2));
  lv_label_set_text(this->widgets.debug, buf);

  return 0;
}