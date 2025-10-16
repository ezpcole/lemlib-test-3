#include "main.h"
#include "gui.h"
#include "liblvgl/llemu.hpp"
#include "pros/rtos.hpp"
#define PROXIMITY_THRESHOLD 100
#define BLUE_HUE 200
#define RED_HUE 10
#define HUE_THRESHOLD 20
#define THRESHOLD(a, b, c) (a < b + c && a > b - c)

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
pros::MotorGroup all_motors({-1, 2, -3, 4, -5, 6, -7, -10, 8, 9},
                            pros::MotorGearset::blue);

pros::Imu imu(11);
pros::Optical optical(12);
pros::Distance distance(13);
pros::adi::Pneumatics tongue('A', false);
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
    lateral_controller(10,  // proportional gain (kP)
                       0,   // integral gain (kI)
                       3,   // derivative gain (kD)
                       3,   // anti windup
                       1,   // small error range, in inches
                       100, // small error range timeout, in milliseconds
                       3,   // large error range, in inches
                       500, // large error range timeout, in milliseconds
                       20   // maximum acceleration (slew)
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
                       0    // maximum acceleration (slew)
    );

// input curve for throttle input during driver control
lemlib::ExpoDriveCurve
    throttle_curve(5,    // joystick deadband out of 127
                   10,   // minimum output where drivetrain will move out of 127
                   1.019 // expo curve gain
    );

// input curve for steer input during driver control
lemlib::ExpoDriveCurve
    steer_curve(5,    // joystick deadband out of 127
                10,   // minimum output where drivetrain will move out of 127
                1.019 // expo curve gain
    );

lemlib::Chassis chassis(old_drivetrain,     // drivetrain settings
                        lateral_controller, // lateral PID settings
                        angular_controller, // angular PID settings
                        sensors,            // odometry sensors
                        &throttle_curve, &steer_curve);

enum intake_states { STOPPED, HOARD, LOW_GOAL, MIDDLE_GOAL, TOP_GOAL };

intake_states intake_state = STOPPED;
optical_data_t optical_data;
bool color_sorting = true;

void ready_blocks() {
  intake_state = TOP_GOAL;
  speed = 0.5;
  while (distance.get() > 100) {
    pros::delay(20);
  }
  intake_state = STOPPED;
}

void initialize() {
  pros::lcd::initialize();
  chassis.calibrate(); // calibrate sensors
  lemlib::Pose default_pose(0, 0, 0);
  chassis.setPose(default_pose);

  // data_t gui_data = {&left_mg, &right_mg, &controller, &imu, nullptr,
  // &chassis}; gui gui(&gui_data);
  pros::Task intake_state_manager([&]() {
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
  });
  pros::Task color_sorting_manager([&]() {
    while (true) {
      optical_data_t old = optical_data;
      optical_data.hue = optical.get_hue();
      optical_data.brightness = optical.get_brightness();
      optical_data.saturation = optical.get_saturation();
      optical_data.proximity = optical.get_proximity();
      pros::lcd::print(0, "[Orange] Hue: %.2f Proximity: %d", optical.get_hue(),
                       optical.get_proximity());
      if (color_sorting) {
        if (optical_data.proximity > PROXIMITY_THRESHOLD &&
            old.proximity <= PROXIMITY_THRESHOLD) {
          // pros::lcd::print(4, "Obj Detected");
          if (THRESHOLD(optical_data.hue, BLUE_HUE, HUE_THRESHOLD)) {
            // pros::lcd::print(4, "Blue Detected");
            intake_state = TOP_GOAL;
            while (distance.get() > 60) {
              pros::delay(20);
            }
            while (distance.get() < 30) {
              pros::delay(20);
            }
            intake_state = HOARD;

          } else if (THRESHOLD(optical_data.hue, RED_HUE, HUE_THRESHOLD)) {
            // intake_state = HOARD;
          }
        }
      }
      pros::delay(20);
    }
  });
  // while (true) {
  //   gui.ui_update();
  //   pros::delay(20);
  // }
  // DESYNCRHONZIE THIS
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
  chassis.moveToPoint(0, 0, 5000);
  chassis.moveToPoint(82.535, 0, 5000);
  chassis.moveToPoint(82.535, 70.321, 5000);
  chassis.moveToPoint(-0.925, 69.026, 5000);
  chassis.moveToPoint(-0.185, 0.185, 5000);
}

void opcontrol() {

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
    }
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_A))
      ready_blocks();

    // for (int i = 0; i < all_motors.size(); i++) {
    //   pros::lcd::print(1 + i, "Motor %d Temp: %.2f", i + 1,
    //                    all_motors.get_temperature(i));
    // }

    // move the robot
    chassis.arcade(leftY, rightX);

    // delay to save resources
    pros::delay(25);
  }
}