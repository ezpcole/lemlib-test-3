#include "main.h"
#include "autons.h"
#include "gui.h"
#include "lemlib/chassis/chassis.hpp"
#include "liblvgl/llemu.hpp"
#include "pros/adi.hpp"
#include "pros/misc.h"
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
pros::MotorGroup left_mg({-1, 2, -3}, pros::MotorGearset::blue);
pros::MotorGroup right_mg(
    {
        4,
        -5,
        6,
    },
    pros::MotorGearset::blue);
pros::Motor low_hoard(-7, pros::MotorGearset::green);
pros::Motor hood(-10, pros::MotorGearset::green);
pros::Motor top_output(8, pros::MotorGearset::green);
pros::Motor intake(9, pros::MotorGearset::green);
pros::MotorGroup all_motors({-7, -10, 8, 9}, pros::MotorGearset::blue);

pros::Imu imu(11);
pros::Optical optical(12);
pros::Distance distance(13);
pros::adi::Pneumatics tongue('A', false);
pros::adi::Pneumatics puncher('B', false);
double speed = 1.0;

lemlib::Drivetrain old_drivetrain(&left_mg, &right_mg,
                                  10, // CHANGE THIS
                                  lemlib::Omniwheel::NEW_4, 480, 2);
lemlib::Drivetrain new_drivetrain(&left_mg, &right_mg, 12.194,
                                  lemlib::Omniwheel::NEW_325, 360, 2);
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
    lateral_controller(8,   // proportional gain (kP)
                       0,   // integral gain (kI)
                       3,   // derivative gain (kD)
                       3,   // anti windup
                       1,   // small error range, in inches
                       100, // small error range timeout, in milliseconds
                       3,   // large error range, in inches
                       500, // large error range timeout, in milliseconds
                       40   // maximum acceleration (slew)
    );

// angular PID controller
lemlib::ControllerSettings
    angular_controller(2,   // proportional gain (kP)
                       0,   // integral gain (kI)
                       10,  // derivative gain (kD)
                       3,   // anti windup
                       1,   // small error range, in degrees
                       100, // small error range timeout, in milliseconds
                       3,   // large error range, in degrees
                       500, // large error range timeout, in milliseconds
                       40   // maximum acceleration (slew)
    );

// input curve for throttle input during driver control
lemlib::ExpoDriveCurve
    throttle_curve(5,    // joystick deadband out of 127
                   5,    // minimum output where drivetrain will move out of 127
                   1.019 // expo curve gain
    );

// input curve for steer input during driver control
lemlib::ExpoDriveCurve
    steer_curve(5,   // joystick deadband out of 127
                5,   // minimum output where drivetrain will move out of 127
                1.01 // expo curve gain
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

void ready_blocks() {
  intake_state = TOP_GOAL;
  speed = 0.5;
  while (distance.get() > 100) {
    pros::delay(20);
  }
  intake_state = STOPPED;
}
void yield_block(int timeout) {
  pros::Clock::time_point start = pros::Clock::now();

  while (optical_data.proximity < PROXIMITY_THRESHOLD) {
    pros::Clock::duration t = (pros::Clock::now() - start);
    if (std::chrono::milliseconds(t).count() > timeout / 2 && timeout != -1) {
      break;
    }
    pros::delay(20);
  }
  start = pros::Clock::now();
  while (optical_data.proximity > PROXIMITY_THRESHOLD) {
    pros::Clock::duration t = (pros::Clock::now() - start);
    if (std::chrono::milliseconds(t).count() > timeout / 2 && timeout != -1) {
      break;
    }
    pros::delay(20);
  }
}

void intake_state_manager_fn(void *param) {
  while (true) {
    switch (intake_state) {
    case STOPPED:
      low_hoard.brake();
      hood.brake();
      intake.brake();
      top_output.brake();
      break;
    case HOARD:
      intake.move(-127 * speed);
      low_hoard.brake();
      hood.move(-127 * speed);
      top_output.move(-127 * speed);
      break;
    case LOW_GOAL:
      intake.move(127 * speed);
      low_hoard.move(-127 * speed * 0.5);
      hood.move(-127 * speed);
      top_output.move(-127 * speed);
      break;
    case MIDDLE_GOAL:
      intake.move(-127 * speed);
      low_hoard.move(-127 * speed);
      hood.brake();
      top_output.move(127 * speed);
      break;
    case TOP_GOAL:
      intake.move(-127 * speed);
      low_hoard.move(-127 * speed);
      hood.move(127 * speed);
      top_output.move(-127 * speed);
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
  optical.set_led_pwm(100);
  optical.set_integration_time(50);
  pros::lcd::initialize();
  chassis.calibrate(); // calibrate sensors
  lemlib::Pose default_pose(0, 0, 0);
  chassis.setPose(default_pose);

  // data_t gui_data = {&left_mg, &right_mg, &controller, &imu, nullptr,
  // &chassis}; gui gui(&gui_data);

  // while (true) {
  //   gui.ui_update();
  //   pros::delay(20);
  // }
  // DESYNCRHONZIE THIS
}

void disabled() {
  puncher.retract();
  tongue.extend();
}

void competition_initialize() {}

void autonomous() {
  if (!threads_on) {
    pros::Task intake_state_manager(intake_state_manager_fn);
    pros::Task color_sorting_manager(color_sorting_manager_fn);
    pros::Task dejam(dejam_fn);
    threads_on = true;
  }

  // skills(chassis,
  //        {
  //            {0, 0, 0, STOPPED, false},
  //            {0, 15.536, 0, HOARD, true},
  //        },
  //        intake_state, tongue);

  old_long(chassis, intake_state, tongue, auton_wait);
  // nothing(chassis, intake_state, tongue, auton_wait);
  // left(chassis, intake_state, tongue, auton_wait);

  lemlib::Pose default_pose(0, 0, 0);
}

void opcontrol() {
  if (!threads_on) {
    pros::Task intake_state_manager(intake_state_manager_fn);
    pros::Task color_sorting_manager(color_sorting_manager_fn);
    pros::Task dejam(dejam_fn);
    threads_on = true;
  }
  // puncher.extend();

  while (true) {
    // get left y and right x positions
    int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1))
      intake_state = MIDDLE_GOAL;
    else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2))
      intake_state = TOP_GOAL;
    else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1))
      intake_state = LOW_GOAL;
    else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2))
      intake_state = HOARD;

    else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B))
      intake_state = STOPPED;

    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y))
      tongue.toggle();

    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
      color_sorting = !color_sorting;
      pros::lcd::print(2, "color sorting: %s", color_sorting ? "on" : "off");
      if (color_sorting) {
        // color_sorting_manager.resume();
      } else {
        // color_sorting_manager.suspend();
      }
    }
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_A))
      ready_blocks();
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN))
      puncher.toggle();
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT))
      chassis.turnToHeading(chassis.getPose().theta + 180, 100);
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) {
      chassis.moveToPose(chassis.getPose().x - 18, chassis.getPose().y - 18,
                         chassis.getPose().theta, 100);
    }

    // for (int i = 0; i < all_motors.size(); i++) {
    //   pros::lcd::print(1 + i, "Motor %d Temp: %.2f", i + 1,
    //                    all_motors.get_temperature(i));
    // }

    // move the robot
    chassis.curvature(leftY, rightX, false);

    // delay to save resources
    pros::delay(25);
  }
}