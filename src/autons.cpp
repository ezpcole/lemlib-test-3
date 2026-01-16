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
  chassis.setPose({-61.148, -17.828, 90});
  // chassis.setPose({-47.447, -14.514, 90}); // this is for the corner method
  // on the top right corner
  chassis.moveToPose(-25.798, -23.272, 225, 5000, {.forwards = true});
  chassis.waitUntil(10);
  puncher.extend();
  chassis.waitUntilDone();

  chassis.moveToPose(-59.807, -49.796, 270, 5000, {.forwards = true});
  chassis.waitUntilDone();
  // match loader ^
  pros::delay(2000);
  chassis.moveToPose(-46.74, -42.103, 180, 5000, {.forwards = false});
  chassis.waitUntilDone();
  puncher.retract();
  intake_state = STOPPED;
  chassis.moveToPose(
      -23.686, -60.066, 90, 5000,
      {.forwards = true,
       .minSpeed =
           27}); // start of rush to other side of field, remove if buggy
  chassis.waitUntilDone();
  chassis.moveToPose(29.642, -59.566, 90, 5000, {.forwards = true});
  chassis.waitUntilDone();
  chassis.moveToPose(54.402, -57.963, 90, 5000, {.forwards = true});
  chassis.waitUntilDone();
  chassis.moveToPose(28.907, -48.605, 90, 5000,
                     {.forwards = false, .minSpeed = 15, .earlyExitRange = 1});
  // goal ^
  chassis.waitUntilDone();
  intake_state = TOP_GOAL;
  pros::delay(250);
  intake_state = LOW_GOAL; // top goal
  chassis.arcade(-30, 0);
  pros::delay(4000);
  puncher.extend();

  chassis.moveToPose(58.639, -48.605, 90, 5000, {.forwards = true});
  chassis.waitUntilDone();
  chassis.waitUntil(5);
  intake_state = HOARD;
  // match loader ^
  chassis.waitUntilDone();
  pros::delay(2000);
  chassis.moveToPose(32, -48.605, 90, 5000, {.forwards = false});
  // goal ^
  chassis.waitUntilDone();
  intake_state = LOW_GOAL;
  chassis.arcade(-30, 0);
  pros::delay(4000);
  puncher.retract();
  chassis.moveToPose(45, -48.605, 90, 5000, {.forwards = true});
  chassis.waitUntilDone();
  chassis.turnToHeading(180, 5000, {.minSpeed = 30, .earlyExitRange = 20});
  chassis.waitUntilDone();
  chassis.moveToPose( // aurafarm position
      62.352, -27.926, 180, 5000,
      {.forwards = false, .maxSpeed = 50, .minSpeed = 15, .earlyExitRange = 2});
  chassis.waitUntilDone();
  chassis.moveToPose(62.352, 24.926, 180, 5000, // aurafarm boost
                     {.forwards = false, .minSpeed = 30, .earlyExitRange = 4});
  chassis.waitUntilDone();
  chassis.moveToPose(39.352, 23.926, 180, 5000, {.forwards = false});
  chassis.waitUntilDone();
  puncher.extend();
  chassis.moveToPose(55.859, 46.582, 90, 5000, {.forwards = true});
  // match loader ^
  chassis.waitUntilDone();
  intake_state = HOARD;
  pros::delay(2000);
  chassis.moveToPose(39.352, 40.433, 0, 5000, {.forwards = true});
  chassis.waitUntilDone();
  puncher.retract();
  chassis.moveToPose(32.393, 60.5, 270, 5000, {.forwards = true});
  chassis.waitUntilDone();
  chassis.moveToPose(-35.416, 59.529, 270, 5000, {.forwards = true});
  chassis.waitUntilDone();
  chassis.moveToPose(-53.218, 58.558, 270, 5000, {.forwards = true});
  chassis.waitUntilDone();
  chassis.moveToPose(-33.798, 47.068, 270, 5000, {.forwards = false});
  // goal ^
  chassis.waitUntilDone();
  intake_state = LOW_GOAL;
  chassis.arcade(-30, 0);
  puncher.extend();
  pros::delay(4000);
  chassis.moveToPose(-55.322, 46.421, 270, 5000, {.forwards = true});
  intake_state = HOARD;
  // match loader ^
  chassis.waitUntilDone();
  pros::delay(2000);
  chassis.moveToPose(-33.598, 47.068, 270, 5000, {.forwards = false});
  puncher.retract();
  // goal ^
  chassis.waitUntilDone();
  intake_state = LOW_GOAL;
  chassis.arcade(-30, 0);
  pros::delay(4000);
  chassis.moveToPose(-61.148, 28.942, 180, 5000, {.forwards = true});
  chassis.waitUntilDone();
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
  chassis.moveToPose(7.988, 41.7, 30, 5000, {});
  chassis.waitUntil(27);
  puncher.extend();
  chassis.waitUntilDone();
  puncher.retract();
  chassis.moveToPose(33, 5.218, 180, 5000, {});
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
  chassis.moveToPose(33, 31.778, 180, 5000, {.forwards = false});
  chassis.waitUntil(15);
  puncher.retract();
  chassis.waitUntilDone();
  pros::delay(auton_wait);
  chassis.arcade(-30, 0);
  intake_state = LOW_GOAL; // this is actually top
                           // goal
}
void left(lemlib::Chassis &chassis, intake_states &intake_state,
          pros::adi::Pneumatics &tongue, const int auton_wait) {
  // Path
  intake_state = HOARD;
  chassis.moveToPose(-3, 39.5, 330, 6000, {.lead = 0.6, .maxSpeed = 50});
  chassis.waitUntil(29);
  puncher.extend();
  chassis.waitUntilDone();
  puncher.retract();
  chassis.moveToPose(-28.5, 3, 180, 5000, {.lead = 0.3});
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
  chassis.moveToPose(-28.5, 28, 180, 5000, {.forwards = false});
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