#include "autons.h"
#include "lemlib/chassis/chassis.hpp"
#include "path.h"
#include "skills.h"
ASSET(example_txt)

void old_long(lemlib::Chassis &chassis, intake_states &intake_state,
              pros::adi::Pneumatics &tongue, const int auton_wait) {
  // chassis.moveToPoint(0, 26.566, 5000);
  // lemlib::MoveToPointParams params;
  // chassis.waitUntilDone();
  // pros::delay(auton_wait);
  // chassis.turnToPoint(17.3, 26.566, 1000);
  // pros::delay(auton_wait);
  // chassis.waitUntilDone();
  // tongue.extend();
  // intake_state = HOARD;
  // pros::delay(auton_wait);
  // chassis.moveToPoint(18, 36.566, 1000);
  // chassis.waitUntilDone();
  // chassis.turnToPoint(21, 26.566, 500);
  // chassis.waitUntilDone();
  // chassis.moveToPoint(19, 36.566, 1000);
  // chassis.waitUntilDone();
  // //   for (int i = 0; i < 7; i++) {
  // //     chassis.arcade(-60, 0);
  // //     pros::delay(100);
  // //     chassis.arcade(127, 0);
  // //     pros::delay(250);
  // //   }
  // pros::delay((150 + 100) * 7);
  // // pros::delay(500);
  // chassis.moveToPoint(0, chassis.getPose().y, 5000, {false});
  // chassis.waitUntilDone();
  // tongue.retract();
  // pros::delay(auton_wait);
  // chassis.turnToHeading(0, 1000);
  // chassis.waitUntilDone();
  // pros::delay(auton_wait);
  // chassis.arcade(127, 0);
  // pros::delay(3000);
  // chassis.arcade(0, 0);
  // chassis.setPose(lemlib::Pose(0.15, 46.285, 0));
  // chassis.moveToPoint(0, 30.566, 5000, {false});
  // chassis.waitUntilDone();
  // pros::delay(auton_wait);
  // chassis.turnToHeading(-90, 1000);
  // chassis.waitUntilDone();
  // pros::delay(auton_wait);
  // chassis.moveToPoint(-21, 30.566, 5000);
  // chassis.waitUntilDone();
  // chassis.turnToPoint(-47, 30.566, 1000);
  // //   pros::delay(auton_wait);

  // //   chassis.arcade(-10, 0);
  // //   chassis.waitUntil(1);
  // //   chassis.arcade(0, 0);
  // //   intake_state = TOP_GOAL;
  // //   pros::delay(500);
  // //   for (int i = 0; i < 3; i++) {
  // //     chassis.arcade(-30, 0);
  // //     pros::delay(100);
  // //     chassis.arcade(50, 0);
  // //     pros::delay(250);
  // //   }
}

void skills_test(lemlib::Chassis &chassis, intake_states &intake_state,
                 pros::adi::Pneumatics &tongue, const int auton_wait) {
  // Path
  intake_state = HOARD;
  chassis.setPose({-61.148, -17.828, 0});
  chassis.moveToPose(-33.798, -25.272, 225, 5000, {.forwards = true});
  chassis.moveToPose(-55.807, -46.796, 270, 5000, {.forwards = true});
  // match loader ^
  chassis.moveToPose(-35.74, -42.103, 180, 5000, {.forwards = true});
  chassis.moveToPose(-48.686, -60.066, 90, 5000, {.forwards = true});
  chassis.moveToPose(29.642, -60.066, 90, 5000, {.forwards = true});
  chassis.moveToPose(54.402, -57.963, 90, 5000, {.forwards = true});
  chassis.moveToPose(31.907, -47.605, 90, 5000, {.forwards = false});
  // goal ^
  chassis.moveToPose(57.639, -47.605, 90, 5000, {.forwards = true});
  // match loader ^
  chassis.moveToPose(32, -47.605, 90, 5000, {.forwards = true});
  // goal ^
  chassis.moveToPose(39.352, 23.926, 0, 5000, {.forwards = true});
  chassis.moveToPose(55.859, 46.582, 90, 5000, {.forwards = true});
  // match loader ^
  chassis.moveToPose(39.352, 40.433, 0, 5000, {.forwards = true});
  chassis.moveToPose(32.393, 60.5, 270, 5000, {.forwards = true});
  chassis.moveToPose(-35.416, 59.529, 270, 5000, {.forwards = true});
  chassis.moveToPose(-53.218, 58.558, 270, 5000, {.forwards = true});
  chassis.moveToPose(-33.798, 47.068, 270, 5000, {.forwards = true});
  // goal ^
  chassis.moveToPose(-55.322, 46.421, 270, 5000, {.forwards = true});
  // match loader ^
  chassis.moveToPose(-33.598, 47.068, 270, 5000, {.forwards = true});
  // goal ^
  chassis.moveToPose(-61.148, 28.942, 180, 5000, {.forwards = true});
  chassis.moveToPose(-62.28, 0, 180, 5000, {.forwards = true});
}
void nothing(lemlib::Chassis &chassis, intake_states &intake_state,
             pros::adi::Pneumatics &tongue, const int auton_wait) {
  intake_state = TOP_GOAL;
  chassis.arcade(100, 0);
  pros::delay(1000);
  chassis.arcade(0, 0);
}

void right(lemlib::Chassis &chassis, intake_states &intake_state,
           pros::adi::Pneumatics &tongue, const int auton_wait) {
  // Path
  intake_state = HOARD;
  chassis.moveToPose(7.988 * side, 41.7, 30, 5000, {});
  chassis.waitUntil(27);
  puncher.extend();
  chassis.waitUntilDone();
  puncher.retract();
  chassis.moveToPose(33 * side, 5.218, 180, 5000, {});
  // x is 33 for right side
  chassis.waitUntil(30);
  puncher.extend();
  chassis.waitUntilDone();
  for (int i = 0; i < 3; i++) {
    chassis.arcade(86, 0);
    pros::delay(300);
    chassis.arcade(-50, 0);
    pros::delay(300);
  }
  chassis.moveToPose(33 * side, 31.778, 180, 5000, {.forwards = false});
  chassis.waitUntil(15);
  puncher.retract();
  chassis.waitUntilDone();
  pros::delay(auton_wait);
  chassis.arcade(-30, 0);
  intake_state = LOW_GOAL; // this is actually top
                           // goal
}

void new_center(lemlib::Chassis &chassis, intake_states &intake_state,
                pros::adi::Pneumatics &tongue, const int auton_wait) {}