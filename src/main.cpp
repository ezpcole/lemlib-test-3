#include "main.h"
#include "autons.h"
#include "gui.h"
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/chassis/trackingWheel.hpp"
#include "lemlib/pid.hpp"
#include "lemlib/pose.hpp"
#include "liblvgl/llemu.hpp"
#include "path.h"
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/rtos.hpp"
#include "skills.h"
#include <chrono>

#define auton_wait 100
#define PROXIMITY_THRESHOLD 100
#define BLUE_HUE 220
#define RED_HUE 10
#define HUE_THRESHOLD 45
#define THRESHOLD(a, b, c) (a <= b + c && a >= b - c)
#define NM_MOTOR_THRESHOLD 0.5

ASSET(example1_txt)

pros::Controller controller(pros::E_CONTROLLER_MASTER);
pros::MotorGroup left_mg({-2, 3, -4}, pros::MotorGearset::blue);
pros::MotorGroup right_mg(
    {
        -12,
        13,
        14,
    },
    pros::MotorGearset::blue);

pros::Motor flexy_boi(1, pros::MotorGearset::green);
pros::Motor hood(1, pros::MotorGearset::green);
pros::Motor top_output(1, pros::MotorGearset::green);
pros::Motor intake(11, pros::MotorGearset::green);
// pros::MotorGroup all_motors({-7, -10, 8, 9}, pros::MotorGearset::blue);

pros::Imu imu(17);
pros::Optical optical(12);
pros::Distance distance(13);
pros::adi::Pneumatics wing('A', false);
pros::adi::Pneumatics middle_stage('C', false);
pros::adi::Pneumatics tongue('B', false);

double speed = 1.0;
int direction = 1;

lemlib::Drivetrain old_drivetrain(&left_mg, &right_mg,
                                  10.375, // CHANGE THIS
                                  lemlib::Omniwheel::NEW_4, 300, 2);
lemlib::OdomSensors sensors(
    nullptr, // vertical tracking wheel 1, set to null
    nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
    nullptr, // horizontal tracking wheel 1
    nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a
             // second one
    &imu     // inertial sensor
);
// lateral PID controller
lemlib::ControllerSettings
    lateral_controller(8,    // proportional gain (kP) 8
                       1,    // integral gain (kI) 0.5
                       7,    // derivative gain (kD) 7
                       2,    // anti windup 3
                       1,    // small error range, in inches 1
                       100,  // small error range timeout, in milliseconds 100
                       3,    // large error range, in inches 3
                       5000, // large error range timeout, in milliseconds 500
                       0     // maximum acceleration (slew) 60
    );

// angular PID controller
lemlib::ControllerSettings
    angular_controller(2.5, // proportional gain (kP)
                       0,   // integral gain (kI)
                       15,  // derivative gain (kD)
                       3,   // anti windup
                       1,   // small error range, in inches
                       100, // small error range timeout, in milliseconds
                       3,   // large error range, in inches TUNE ERROR RANGE
                       500, // large error range timeout, in milliseconds
                       0    // maximum acceleration (slew)
    );

// input curve for throttle input during driver control
lemlib::ExpoDriveCurve
    throttle_curve(5,    // joystick deadband out of 127
                   0,    // minimum output where drivetrain will move out of 127
                   1.006 // expo curve gain
    );

// input curve for steer input during driver control
lemlib::ExpoDriveCurve
    steer_curve(5,     // joystick deadband out of 127
                0,     // minimum output where drivetrain will move out of 127
                1.0075 // expo curve gain
    );

lemlib::Chassis chassis(old_drivetrain,     // drivetrain settings
                        lateral_controller, // lateral PID settings
                        angular_controller, // angular PID settings
                        sensors,            // odometry sensors
                        &throttle_curve, &steer_curve);

intake_states intake_state = STOPPED;
optical_data_t optical_data;
bool color_sorting = false;
bool threads_on = false;
int side = -1;

// void drivetrain_telemetry_fn(void *param) {
//   while (true) {
//     lemlib::Pose pose = chassis.getPose();
//     pros::lcd::print(4, "X: %.2f, Y: %.2f, Theta: %.2f", pose.x, pose.y,
//                      pose.theta);
//     pros::delay(20);
//   }
// }
void intake_state_manager_fn(void *param) {
  while (true) {
    switch (intake_state) {
    case STOPPED:
      middle_stage.extend();
      intake.brake();
      hood.brake();
      break;
    case HOARD:
      middle_stage.extend();
      intake.move(-127 * speed);
      hood.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
      hood.brake();
      break;
    case LOW_GOAL:
      middle_stage.extend();
      intake.move(127 * speed);
      hood.move(-127 * speed);
      break;
    case MIDDLE_GOAL:
      middle_stage.retract();
      intake.move(-127 * speed);
      hood.move(127 * speed);
      break;
    case TOP_GOAL:
      middle_stage.extend();
      intake.move(-127 * speed);
      hood.move(127 * speed);
      break;
    case SHORT_INTAKE:
      middle_stage.extend();
      intake.move(127 * speed);
      pros::delay(367);
      intake.move(-127 * speed);
      intake_state = HOARD;
      break;
    }
    pros::delay(20);
  }
}
void color_sorting_manager_fn(void *param) {
  while (true) {
    optical_data_t old = optical_data;
    optical_data.hue = optical.get_hue();
    optical_data.brightness = optical.get_brightness();
    optical_data.saturation = optical.get_saturation();
    optical_data.proximity = optical.get_proximity();
    // pros::lcd::print(0, "[Orange] Hue: %.2f Proximity: %d",
    // optical.get_hue(),
    //                  optical.get_proximity());
    if (color_sorting) {
      if (optical_data.proximity > PROXIMITY_THRESHOLD) {
        if (THRESHOLD(optical_data.hue, BLUE_HUE, HUE_THRESHOLD)) {
          pros::lcd::print(4, "Blue Detected");
          intake_state = TOP_GOAL;
          while (distance.get() > 45) {
            pros::delay(20);
          }
          while (distance.get() < 60) {
            pros::delay(20);
          }
          intake_state = HOARD;
          pros::lcd::print(4, "Blue no longer detected");

        } else if (THRESHOLD(optical_data.hue, RED_HUE, HUE_THRESHOLD)) {
          // intake_state = HOARD;
        }
      }
    }
    pros::delay(20);
  }
}
void dejam_fn(void *param) {
  // while (true) {
  //   double nm = low_hoard.get_torque();
  //   if (fabs(low_hoard.get_torque()) >= NM_MOTOR_THRESHOLD) {
  //     pros::delay(500);
  //     if (fabs(low_hoard.get_torque()) >= NM_MOTOR_THRESHOLD) {
  //       speed = -fabs(speed);
  //       pros::delay(100);
  //       speed = fabs(speed);
  //       pros::lcd::print(5, "jam nm: %.2f > %.2f", nm, NM_MOTOR_THRESHOLD);
  //     }
  //   }
  //   pros::delay(20);
  // }
}

void initialize() {
  imu.reset();
  pros::lcd::initialize();
  chassis.calibrate(); // calibrate sensors

  // data_t gui_data = {&left_mg, &right_mg, &controller, &imu, nullptr,
  // &chassis}; gui gui(&gui_data);

  // while (true) {
  //   gui.ui_update();
  //   pros::delay(20);
  // }
  // DESYNCRHONZIE THIS
}

void disabled() {
  tongue.retract();
  wing.extend();
}

void competition_initialize() {}

void autonomous() {
  if (!threads_on) {
    pros::Task intake_state_manager(intake_state_manager_fn);
    // pros::Task color_sorting_manager(color_sorting_manager_fn);
    pros::Task dejam(dejam_fn);
    // pros::Task drivetrain_telemetry(drivetrain_telemetry_fn);
    threads_on = true;
  }

  // skills(chassis,
  //        {
  //            {0, 0, 0, STOPPED, false},
  //            {0, 15.536, 0, HOARD, true},
  //        },
  //        intake_state, tongue);

  // old_long(chassis, intake_state, tongue, auton_wait);
  // nothing(chassis, intake_state, tongue, auton_wait);
  // skills_test(chassis, intake_state, tongue, auton_wait);
  // right(chassis, intake_state, wing, auton_wait);
  new_center(chassis, intake_state, tongue, auton_wait);

  lemlib::Pose default_pose(0, 0, 0);
}

void opcontrol() {
  if (!threads_on) {
    pros::Task intake_state_manager(intake_state_manager_fn);
    // pros::Task color_sorting_manager(color_sorting_manager_fn);
    pros::Task dejam(dejam_fn);

    threads_on = true;
  }
  // puncher.extend();

  // pros::Task auton(autonomous);
  int ts_num = 0;

  while (true) {
    // get left y and right x positions
    int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
    if (intake_state != SHORT_INTAKE) {
      if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
        wing.retract();
      else
        wing.extend();
      if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2))
        intake_state = TOP_GOAL;
      else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1))
        intake_state = MIDDLE_GOAL;
      else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2))
        intake_state = HOARD;

      else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B))
        intake_state = STOPPED;
    }

    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
      tongue.toggle();
    }

    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
      // auton.suspend();
      chassis.cancelAllMotions();
      pros::lcd::print(2, "color sorting: %s", color_sorting ? "on" : "off");
      if (color_sorting) {
        // color_sorting_manager.resume();
      } else {
        // color_sorting_manager.suspend();
      }
    }
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A))
      intake_state = LOW_GOAL;
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN))
      intake_state = SHORT_INTAKE;
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT))
      chassis.turnToHeading(chassis.getPose().theta + 90, 10000);
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) {
      chassis.moveToPoint(chassis.getPose().x, chassis.getPose().y + 12.5,
                          10000);
    }

    // for (int i = 0; i < all_motors.size(); i++) {
    //   pros::lcd::print(1 + i, "Motor %d Temp: %.2f", i + 1,
    //                    all_motors.get_temperature(i));
    // }

    // move the robot
    chassis.arcade(leftY * direction, rightX, false, 0.55);
    // delay to save resources
    pros::delay(25);
  }
}