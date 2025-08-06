#include "devices.h"

using namespace vex;

// Global device instances
brain Brain;
controller Controller1;

// Drivetrain motors
motor LeftFront(PORT1, ratio6_1, false);
motor BackLeft(PORT2, ratio6_1, false);
motor_group leftMotors = motor_group(LeftFront, BackLeft);
motor RightFront(PORT3, ratio6_1, true);
motor BackRight(PORT4, ratio6_1, true);
motor_group rightMotors = motor_group(RightFront, BackRight);

// Other devices
motor Chain(PORT5, ratio6_1, false);
motor IntakeTop(PORT6, ratio6_1, false);
motor IntakeBottom(PORT7, ratio6_1, true); // reversed
motor_group IntakeWheels = motor_group(IntakeTop, IntakeBottom);

drivetrain Drivetrain = drivetrain(leftMotors, rightMotors, 259.34, 320, 40, mm, 1);
competition Competition;
