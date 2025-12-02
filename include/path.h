#pragma once
#include "lemlib/api.hpp"

extern lemlib::Chassis chassis;

enum intake_states {
  STOPPED,
  HOARD,
  LOW_GOAL,
  MIDDLE_GOAL,
  TOP_GOAL,
  SHORT_INTAKE,
};

struct point {
  double x;
  double y;
  double heading;
  intake_states state;
  bool tongue_state;
};