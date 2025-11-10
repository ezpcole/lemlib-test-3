#pragma once
#include "lemlib/api.hpp"
#include <vector>

enum intake_states { STOPPED, HOARD, LOW_GOAL, MIDDLE_GOAL, TOP_GOAL };
struct point {
  double x;
  double y;
  double heading;
  intake_states state;
  bool tongue_state;
};

void skills(lemlib::Chassis &chassis, std::vector<point> waypoints,
            intake_states &intake_state, pros::adi::Pneumatics &tongue);