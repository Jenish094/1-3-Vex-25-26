#include "devices.h"

using namespace vex;

// Global device instances
brain Brain;
controller Controller1;

// Drivetrain motors
motor FrontLmotor(PORT2, ratio6_1, false); //front left motor
motor BackLmotor(PORT1, ratio6_1, false); // back left motor
motor FrontRmotor(PORT3, ratio6_1, true); // reversed- front right motor
motor BackRmotor(PORT4, ratio6_1, true);  // reversed- back right motor

// Other devices
motor IntakeMotor(PORT5, ratio6_1, true); // intake motor
motor IntakeMotor2(PORT6, ratio6_1, false); // second intake motor
motor_group IntakeWheels(IntakeMotor, IntakeMotor2); // intake wheels motor group
motor Chain(PORT7, ratio6_1, false); //Chain
