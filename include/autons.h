#pragma once
#include "gui.h"
#include "main.h"
#include "pros/adi.hpp"

enum starting_side { LEFT = -1, RIGHT = 1 };
struct auton_data {
  ANSI_COLOR color;
  starting_side side;
};

extern pros::adi::Pneumatics puncher;
extern int side;

void old_long(lemlib::Chassis &chassis, intake_states &intake_state,
              pros::adi::Pneumatics &tongue, const int auton_wait);
void new_center(lemlib::Chassis &chassis, intake_states &intake_state,
                pros::adi::Pneumatics &tongue, const int auton_wait);
void right(lemlib::Chassis &chassis, intake_states &intake_state,
           pros::adi::Pneumatics &tongue, const int auton_wait);
void nothing(lemlib::Chassis &chassis, intake_states &intake_state,
             pros::adi::Pneumatics &tongue, const int auton_wait);