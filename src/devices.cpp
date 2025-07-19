#include "devices.h"

// Drive motors
vex::motor FrontLmotor(vex::PORT2, vex::gearSetting::ratio6_1, false);
vex::motor BackLmotor(vex::PORT1, vex::gearSetting::ratio6_1, false);
vex::motor FrontRmotor(vex::PORT3, vex::gearSetting::ratio6_1, true);
vex::motor BackRmotor(vex::PORT4, vex::gearSetting::ratio6_1, true);

// Intake motors
vex::motor IntakeMotor(vex::PORT5, vex::gearSetting::ratio6_1, true);
vex::motor IntakeMotor2(vex::PORT6, vex::gearSetting::ratio6_1, false);
vex::motor_group IntakeWheels(IntakeMotor, IntakeMotor2);

// Chain
vex::motor Chain(vex::PORT7, vex::gearSetting::ratio6_1, false);
