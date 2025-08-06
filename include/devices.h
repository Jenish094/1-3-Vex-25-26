#ifndef DEVICES_H
#define DEVICES_H

#include "vex.h"

extern vex::brain Brain;
extern vex::controller Controller1;

// Drivetrain motors
extern vex::motor LeftFront;
extern vex::motor BackLeft;
extern vex::motor_group leftMotors;
extern vex::motor RightFront;
extern vex::motor BackRight;
extern vex::motor_group rightMotors;

// Other devices
extern vex::motor Chain;
extern vex::motor IntakeTop;
extern vex::motor IntakeBottom;

extern vex::drivetrain Drivetrain;
extern vex::competition Competition;

#endif // DEVICES_H
