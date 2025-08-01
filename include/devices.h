#ifndef DEVICES_H
#define DEVICES_H

#include "vex.h"

extern vex::brain Brain;
extern vex::controller Controller1;

// Drivetrain motors
extern vex::motor FrontLmotor;
extern vex::motor BackLmotor;
extern vex::motor FrontRmotor;
extern vex::motor BackRmotor;

// Other devices
extern vex::motor IntakeMotor;
extern vex::motor IntakeMotor2;
extern vex::motor_group IntakeWheels;
extern vex::motor Chain;

#endif // DEVICES_H
