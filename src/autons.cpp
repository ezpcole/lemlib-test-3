#include "autons.h"
#include "skills.h"
ASSET(example_txt)

void old_long(lemlib::Chassis &chassis, intake_states &intake_state,
              pros::adi::Pneumatics &tongue, const int auton_wait) {
  chassis.moveToPoint(0, 26.566, 5000);
  lemlib::MoveToPointParams params;
  chassis.waitUntilDone();
  pros::delay(auton_wait);
  chassis.turnToPoint(17.3, 26.566, 1000);
  pros::delay(auton_wait);
  chassis.waitUntilDone();
  tongue.extend();
  intake_state = HOARD;
  pros::delay(auton_wait);
  chassis.moveToPoint(18, 36.566, 1000);
  chassis.waitUntilDone();
  chassis.turnToPoint(21, 26.566, 500);
  chassis.waitUntilDone();
  chassis.moveToPoint(19, 36.566, 1000);
  chassis.waitUntilDone();
  //   for (int i = 0; i < 7; i++) {
  //     chassis.arcade(-60, 0);
  //     pros::delay(100);
  //     chassis.arcade(127, 0);
  //     pros::delay(250);
  //   }
  pros::delay((150 + 100) * 7);
  // pros::delay(500);
  chassis.moveToPoint(0, chassis.getPose().y, 5000, {false});
  chassis.waitUntilDone();
  tongue.retract();
  pros::delay(auton_wait);
  chassis.turnToHeading(0, 1000);
  chassis.waitUntilDone();
  pros::delay(auton_wait);
  chassis.arcade(127, 0);
  pros::delay(3000);
  chassis.arcade(0, 0);
  chassis.setPose(lemlib::Pose(0.15, 46.285, 0));
  chassis.moveToPoint(0, 30.566, 5000, {false});
  chassis.waitUntilDone();
  pros::delay(auton_wait);
  chassis.turnToHeading(-90, 1000);
  chassis.waitUntilDone();
  pros::delay(auton_wait);
  chassis.moveToPoint(-21, 30.566, 5000);
  chassis.waitUntilDone();
  chassis.turnToPoint(-47, 30.566, 1000);
  //   pros::delay(auton_wait);

  //   chassis.arcade(-10, 0);
  //   chassis.waitUntil(1);
  //   chassis.arcade(0, 0);
  //   intake_state = TOP_GOAL;
  //   pros::delay(500);
  //   for (int i = 0; i < 3; i++) {
  //     chassis.arcade(-30, 0);
  //     pros::delay(100);
  //     chassis.arcade(50, 0);
  //     pros::delay(250);
  //   }
}

void nothing(lemlib::Chassis &chassis, intake_states &intake_state,
             pros::adi::Pneumatics &tongue, const int auton_wait) {
  intake_state = TOP_GOAL;
  chassis.arcade(100, 0);
  pros::delay(1000);
  chassis.arcade(0, 0);
}

void left(lemlib::Chassis &chassis, intake_states &intake_state,
          pros::adi::Pneumatics &tongue, const int auton_wait) {
  // Path
  intake_state = HOARD;
  chassis.moveToPose(0, 40.855, 276, 5000);
  chassis.waitUntilDone();
  chassis.moveToPose(27.405, 17.597, 270, 5000, {false});
  chassis.waitUntilDone();
  chassis.moveToPose(25.645, 35.475, 270, 5000, {false});
  chassis.waitUntilDone();
  pros::delay(auton_wait);
  intake_state = TOP_GOAL;
}

void new_center(lemlib::Chassis &chassis, intake_states &intake_state,
                pros::adi::Pneumatics &tongue, const int auton_wait) {}