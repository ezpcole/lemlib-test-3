#include "autons.h"
#include "lemlib/chassis/chassis.hpp"
#include "path.h"
#include "skills.h"
ASSET(example_txt)

void jiggle(int iter, int len1, int len2, int power, int delay) {
  for (int i = 0; i < iter; i++) {
    chassis.arcade(power, 0);
    pros::delay(len1);
    chassis.arcade(-power, 0);
    pros::delay(len2);
    pros::delay(delay);
  }
  chassis.arcade(0, 0);
}

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

  const int load_time = 5000;
  const int unload_time = 6000;
  chassis.setPose({-61.148, -17.828, 90});
  // chassis.setPose({-47.447, -14.514, 90}); // this is for the corner method
  // on the top right corner
  chassis.moveToPose(-25.798, -24.772, 225, 1500,
                     {.forwards = true, .minSpeed = 60, .earlyExitRange = 10});
  chassis.waitUntil(5);
  tongue.extend();
  chassis.waitUntilDone();
  intake_state = HOARD;
  chassis.moveToPose(-63.650, -51, 270, 3000, {.forwards = true});
  chassis.waitUntilDone();
  // match loader ^
  jiggle(3, 500, 50, 85, 0);
  pros::delay(load_time);
  chassis.moveToPose(-46.74, -42.103, 180, 5000, {.forwards = false});
  chassis.waitUntilDone();
  tongue.retract();
  intake_state = STOPPED;
  chassis.moveToPose(
      -23.686, -59.25, 90, 5000,
      {.forwards = true,
       .minSpeed = 40,
       .earlyExitRange =
           3.5}); // start of rush to other side of field, remove if buggy
  chassis.waitUntilDone();
  chassis.moveToPose(29.642, -59.25, 90, 5000,
                     {.forwards = true, .minSpeed = 40});
  chassis.waitUntilDone();
  chassis.moveToPose(54.402, -57.963, 90, 5000,
                     {.forwards = true, .minSpeed = 40});
  chassis.waitUntilDone();
  chassis.moveToPose(25.6, -48.605, 90, 2000,
                     {.forwards = false, .minSpeed = 15, .earlyExitRange = 0});
  // jiggle(1, 200, 400, 50, 0);
  // goal ^
  // chassis.arcade(-50, 0);
  chassis.waitUntilDone();
  intake_state = TOP_GOAL;
  pros::delay(250);
  intake_state = LOW_GOAL; // top goal
  tongue.extend();
  pros::delay(unload_time);
  // intake_state = LOW_GOAL;
  chassis.moveToPose(66, -47, 90, 3000, {.forwards = true});
  chassis.waitUntil(2.5);
  intake_state = HOARD;
  chassis.waitUntilDone();
  // match loader ^
  chassis.waitUntilDone();
  jiggle(3, 500, 50, 85, 0);
  pros::delay(load_time);
  chassis.moveToPose(25.907, -48.505, 90, 2000,
                     {.forwards = false, .minSpeed = 15, .earlyExitRange = 0});
  // goal ^
  chassis.waitUntilDone();
  intake_state = TOP_GOAL;
  pros::delay(250);
  intake_state = LOW_GOAL;
  // chassis.arcade(-30, 0);
  tongue.retract();
  pros::delay(unload_time);
  // intake_state = TOP_GOAL;
  chassis.moveToPose(45, -48.605, 90, 5000,
                     {.forwards = true, .minSpeed = 15, .earlyExitRange = 2});
  chassis.waitUntilDone();
  // chassis.turnToHeading(0, 5000, {.minSpeed = 30, .earlyExitRange = 20});
  chassis.waitUntilDone();
  chassis.moveToPose(39, -22, 270, 5000);
  chassis.waitUntilDone();
  chassis.moveToPose(-30, -25, 270, 5000);
  chassis.waitUntilDone();
  chassis.moveToPose(-46, 0, 270, 5000);
  chassis.waitUntilDone();
  chassis.arcade(127 * 6.5 / 8.0, 0);
  pros::delay(6000);
  chassis.cancelAllMotions();
  chassis.arcade(0, 0);

  // chassis.moveToPose( // aurafarm position
  //     64.752, -23.926, 180, 5000,
  //     {.forwards = false, .maxSpeed = 50, .minSpeed = 15, .earlyExitRange =
  //     2});
  // chassis.waitUntilDone();
  // chassis.moveToPose(64.752, 24.926, 180, 5000, // aurafarm boost
  //                    {.forwards = false, .minSpeed = 70, .earlyExitRange =
  //                    4});
  // chassis.waitUntilDone();
  // chassis.moveToPose(39.352, 23.926, 0, 5000, {.forwards = false});
  // chassis.waitUntilDone();
  // ts.extend();
  // chassis.moveToPose(59.659, 46.405, 90, 5000, {.forwards = true});
  // // match loader ^
  // chassis.waitUntilDone();
  // intake_state = HOARD;
  // pros::delay(2000);
  // intake_state = STOPPED;
  // chassis.moveToPose(46.74, 42.103, 0, 5000, {.forwards = false});
  // chassis.waitUntilDone();
  // ts.retract();

  // chassis.moveToPose(
  //     23.686, 59.7, 270, 5000,
  //     {.forwards = true,
  //      .minSpeed = 40,
  //      .earlyExitRange =
  //          3.5}); // start of rush to other side of field, remove if buggy
  // chassis.waitUntilDone();
  // chassis.moveToPose(-29.642, 59.7, 90, 5000,
  //                    {.forwards = true, .minSpeed = 40});
  // chassis.waitUntilDone();
  // chassis.moveToPose(-54.402, 57.963, 90, 5000,
  //                    {.forwards = true, .minSpeed = 40});
  // chassis.waitUntilDone();
  // chassis.moveToPose(-27.507, 47.605, 90, 5000,
  //                    {.forwards = false, .minSpeed = 15, .earlyExitRange =
  //                    1});
  // // goal ^
  // chassis.waitUntilDone();
  // intake_state = TOP_GOAL;
  // pros::delay(250);
  // intake_state = LOW_GOAL; // top goal
  // chassis.arcade(-30, 0);
  // ts.extend();
  // pros::delay(4000);
  // intake_state = TOP_GOAL;
  // chassis.moveToPose(-60.9, 50, 90, 5000, {.forwards = true});
  // chassis.waitUntilDone();
  // chassis.waitUntil(5);
  // intake_state = HOARD;
  // // match loader ^
  // chassis.waitUntilDone();
  // pros::delay(2000);
  // chassis.moveToPose(-27.507, 47.405, 90, 5000,
  //                    {.forwards = false, .minSpeed = 15, .earlyExitRange =
  //                    1});
  // // goal ^
  // chassis.waitUntilDone();
  // intake_state = TOP_GOAL;
  // pros::delay(250);
  // intake_state = LOW_GOAL;
  // chassis.arcade(-30, 0);
  // ts.retract();
  // pros::delay(4000);
  // intake_state = TOP_GOAL;
  // chassis.moveToPose(-75, 0, 270, 5000);
  // intake_state = LOW_GOAL;
}
void nothing(lemlib::Chassis &chassis, intake_states &intake_state,
             pros::adi::Pneumatics &tongue, const int auton_wait) {
  intake_state = TOP_GOAL;
  chassis.arcade(100, 0); // swap this to 100 if going for a little bit
  pros::delay(1000);
  chassis.arcade(0, 0);
}

void right(lemlib::Chassis &chassis, intake_states &intake_state,
           pros::adi::Pneumatics &tongue, const int auton_wait) {
  // Path
  intake_state = HOARD;
  chassis.moveToPose(7.988, 42.7, 30, 5000, {});
  chassis.waitUntil(27);
  tongue.extend();
  chassis.waitUntilDone();
  tongue.retract();
  chassis.moveToPose(32.25, 4.218, 180, 3000, {});
  // x is 33 for right side
  chassis.waitUntil(30);
  tongue.extend();
  chassis.waitUntilDone();
  for (int i = 0; i < 3; i++) {
    chassis.arcade(86, 0);
    pros::delay(350);
    chassis.arcade(-50, 0);
    pros::delay(250);
  }
  pros::delay(500);
  chassis.moveToPose(32, 32.378, 180, 5000, {.forwards = false});
  chassis.waitUntil(15);
  tongue.retract();
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
  chassis.moveToPose(-3, 39.5, 330, 6000, {});
  chassis.waitUntil(29);
  tongue.extend();
  chassis.waitUntilDone();
  tongue.retract();
  chassis.moveToPose(-29, 3, 180, 5000, {.lead = 0.3});
  // x is 33 for right side
  chassis.waitUntil(30);
  tongue.extend();
  chassis.waitUntilDone();
  for (int i = 0; i < 3; i++) {
    chassis.arcade(86, 0);
    pros::delay(300);
    chassis.arcade(-50, 0);
    pros::delay(300);
  }
  chassis.moveToPose(-31.5, 32, 180, 5000, {.forwards = false});
  chassis.waitUntil(15);
  tongue.retract();
  chassis.waitUntilDone();
  pros::delay(auton_wait);
  chassis.arcade(-30, 0);
  intake_state = LOW_GOAL; // this is actually top
                           // goal
}

void new_center(lemlib::Chassis &chassis, intake_states &intake_state,
                pros::adi::Pneumatics &tongue, const int auton_wait) {
  // chassis.calibrate(true);
  lemlib::Pose startingPose(-45.8, 6.421, 90);
  chassis.setPose(startingPose);
  chassis.moveToPoint(-23.8, 6.421, 1000, {.earlyExitRange = 0});
  chassis.waitUntilDone();
  chassis.turnToHeading(0, 750,
                        {.direction = AngularDirection::CCW_COUNTERCLOCKWISE,
                         .minSpeed = 20,
                         .earlyExitRange = 0});
  chassis.waitUntilDone();
  chassis.moveToPoint(-21, 26.636, 1000, {.maxSpeed = 50});
  intake_state = HOARD;
  chassis.waitUntil(8);
  tongue.extend(); // should grab the blocks by now
  chassis.waitUntilDone();
  pros::delay(500);
  chassis.moveToPoint(-23.4, 23.436, 1000, {.forwards = false});
  // tongue.retract(); commented out in hopes that match load work
  chassis.turnToHeading(-45, 1000,
                        {.direction = AngularDirection::CCW_COUNTERCLOCKWISE});
  chassis.waitUntilDone();
  chassis.moveToPoint(-10.5, 10.5, 1000, {.forwards = false});
  chassis.waitUntilDone();
  intake_state = MIDDLE_GOAL;
  pros::delay(1500);
  intake_state = STOPPED;
  chassis.moveToPoint(-47, 47, 3000, {});
  chassis.waitUntilDone();
  chassis.turnToHeading(270, 750,
                        {.direction = AngularDirection::CCW_COUNTERCLOCKWISE});
  chassis.waitUntilDone();
  tongue.extend();
  intake_state = HOARD;
  chassis.moveToPoint(-55.989, 47, 1500, {});
  chassis.waitUntilDone();
  pros::delay(2000);
  chassis.moveToPoint(-28.245, 47, 1500, {.forwards = false});
  chassis.waitUntilDone();
  intake_state = TOP_GOAL;
}