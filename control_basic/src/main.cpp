#include "main.h"
#include <cmath>

void initialize() {}
void disabled() {}
void competition_initialize() {}
void autonomous() {}

void turnLeft90(pros::Motor& left_motor, pros::Motor& right_motor, double turn_degrees, int turn_speed) {
    double left_target = left_motor.get_position() - turn_degrees;
    double right_target = right_motor.get_position() + turn_degrees;
    left_motor.move_relative(-turn_degrees, turn_speed);
    right_motor.move_relative(turn_degrees, turn_speed);

    int timeout = 0;
    while (std::fabs(left_motor.get_position() - left_target) > 5 &&
           std::fabs(right_motor.get_position() - right_target) > 5 &&
           timeout < 200) {
        pros::delay(10);
        timeout++;
    }
}

void turnRight90(pros::Motor& left_motor, pros::Motor& right_motor, double turn_degrees, int turn_speed) {
    double left_target = left_motor.get_position() + turn_degrees;
    double right_target = right_motor.get_position() - turn_degrees;
    left_motor.move_relative(turn_degrees, turn_speed);
    right_motor.move_relative(-turn_degrees, turn_speed);

    int timeout = 0;
    while (std::fabs(left_motor.get_position() - left_target) > 5 &&
           std::fabs(right_motor.get_position() - right_target) > 5 &&
           timeout < 200) {
        pros::delay(10);
        timeout++;
    }
}

void opcontrol() {
    pros::Controller master(pros::E_CONTROLLER_MASTER);
    pros::Motor left_motor(2, pros::MotorGears::green, pros::MotorUnits::degrees);
    pros::Motor right_motor(-1, pros::MotorGears::green, pros::MotorUnits::degrees);

    const double TURN_DEGREES = 450;
    const double TURN_180 = 900;         // tune: wheel degrees for a 180 degree in-place turn
    const double DRIVE_DISTANCE = 400.0; // tune: wheel degrees for "a short distance"
    const int TURN_SPEED = 100;

    while (true) {
        // Tank drive: left stick -> left motor only, right stick -> right motor only
        int left = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int right = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
        left_motor.move(left);
        right_motor.move(right);

        // Debug: show button states live on the controller screen
        master.print(0, 0, "L:%d R:%d L1:%d R1:%d",
            master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT),
            master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT),
            master.get_digital(pros::E_CONTROLLER_DIGITAL_L1),
            master.get_digital(pros::E_CONTROLLER_DIGITAL_R1));

        // Turn counter-clockwise: D-pad LEFT
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) {
            turnLeft90(left_motor, right_motor, TURN_DEGREES, TURN_SPEED);
        }

        // Turn clockwise: D-pad RIGHT
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
            turnRight90(left_motor, right_motor, TURN_DEGREES, TURN_SPEED);
        }

        // Move forward, turn 180 clockwise, move back, turn 180 clockwise: D-pad UP
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) {

            // Step 1: drive forward a short distance
            double left_target = left_motor.get_position() + DRIVE_DISTANCE;
            double right_target = right_motor.get_position() + DRIVE_DISTANCE;
            left_motor.move_relative(DRIVE_DISTANCE, TURN_SPEED);
            right_motor.move_relative(DRIVE_DISTANCE, TURN_SPEED);
            int timeout = 0;
            while (std::fabs(left_motor.get_position() - left_target) > 5 &&
                   std::fabs(right_motor.get_position() - right_target) > 5 &&
                   timeout < 400) {
                pros::delay(10);
                timeout++;
            }

            // Step 2: turn 180 clockwise in place
            left_target = left_motor.get_position() + TURN_180;
            right_target = right_motor.get_position() - TURN_180;
            left_motor.move_relative(TURN_180, TURN_SPEED);
            right_motor.move_relative(-TURN_180, TURN_SPEED);
            timeout = 0;
            while (std::fabs(left_motor.get_position() - left_target) > 5 &&
                   std::fabs(right_motor.get_position() - right_target) > 5 &&
                   timeout < 400) {
                pros::delay(10);
                timeout++;
            }

            // Step 3: drive forward again (now heading back toward the start)
            left_target = left_motor.get_position() + DRIVE_DISTANCE;
            right_target = right_motor.get_position() + DRIVE_DISTANCE;
            left_motor.move_relative(DRIVE_DISTANCE, TURN_SPEED);
            right_motor.move_relative(DRIVE_DISTANCE, TURN_SPEED);
            timeout = 0;
            while (std::fabs(left_motor.get_position() - left_target) > 5 &&
                   std::fabs(right_motor.get_position() - right_target) > 5 &&
                   timeout < 400) {
                pros::delay(10);
                timeout++;
            }

            // Step 4: turn 180 clockwise again (ends facing original direction)
            left_target = left_motor.get_position() + TURN_180;
            right_target = right_motor.get_position() - TURN_180;
            left_motor.move_relative(TURN_180, TURN_SPEED);
            right_motor.move_relative(-TURN_180, TURN_SPEED);
            timeout = 0;
            while (std::fabs(left_motor.get_position() - left_target) > 5 &&
                   std::fabs(right_motor.get_position() - right_target) > 5 &&
                   timeout < 400) {
                pros::delay(10);
                timeout++;
            }
        }

        pros::delay(20);
    }
}