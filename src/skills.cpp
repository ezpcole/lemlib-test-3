#include "skills.h"
#include "pros/adi.hpp"

void skills(lemlib::Chassis &chassis, std::vector<point> waypoints,
            intake_states &intake_state, pros::adi::Pneumatics &tongue) {
  // unfinished
  for (const auto &wp : waypoints) {
    chassis.moveToPoint(wp.x, wp.y, 5000);
    if (wp.tongue_state) {
      tongue.extend();
    } else {
      tongue.retract();
    }
    intake_state = wp.state;

    chassis.waitUntilDone();
    if (wp.heading != chassis.getPose().theta) {
      chassis.turnToHeading(wp.heading, 5000);
      chassis.waitUntilDone();
    }
  }
}