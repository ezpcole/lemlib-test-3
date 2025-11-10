#pragma once
#include "gui.h"
#include "main.h"

enum starting_side { LEFT, RIGHT };
struct auton_data {
  ANSI_COLOR color;
  starting_side side;
};

void old_long(lemlib::Chassis &chassis, intake_states &intake_state,
              pros::adi::Pneumatics &tongue, const int auton_wait);
void new_center(lemlib::Chassis &chassis, intake_states &intake_state,
                pros::adi::Pneumatics &tongue, const int auton_wait);
void left(lemlib::Chassis &chassis, intake_states &intake_state,
          pros::adi::Pneumatics &tongue, const int auton_wait);
void nothing(lemlib::Chassis &chassis, intake_states &intake_state,
             pros::adi::Pneumatics &tongue, const int auton_wait);