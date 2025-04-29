/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       1/3 - Jenish                                              */
/*    Created:      4/29/2025, 10:16:01 PM                                    */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
// Were cooked
// This is a Pre-Season starter template, this controls the 4-motor drivetrain
#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

/*---------------------------------------------------------------------------*/
/*                          Defining Robot Devices                           */
/*---------------------------------------------------------------------------*/
// Here just change the port number to match the actual robot
motor FrontLmotor(PORT1, ratio6_1, false); //front left motor
motor BackLmotor(PORT2, ratio6_1, false); // back left motor
motor FrontRmotor(PORT3, ratio6_1, true); // reversed- front right motor
motor BackRmotor(PORT4, ratio6_1, true);  // reversed- back right motor
// 6:1 ratio was used to accellerate speed as our preseason theories lead to a fast-paced game
controller Controller1;

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                              Autonomous Task                              */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
}

/*---------------------------------------------------------------------------*/
/*                              User Control Task                            */
/*---------------------------------------------------------------------------*/
//Put all code inside the loop
// This code is for just mapping the controller buttons to the robot functionalities
void usercontrol(void) {
  while (1) {
    int leftSpeed = Controller1.Axis3.position(percent);  // Left stick Y-axis
    int rightSpeed = Controller1.Axis2.position(percent); // Right stick Y-axis

    FrontLmotor.spin(forward, leftSpeed, percent);
    BackLmotor.spin(forward, leftSpeed, percent);

    FrontRmotor.spin(forward, rightSpeed, percent);
    BackRmotor.spin(forward, rightSpeed, percent);

    wait(20, msec); // Sleep to prevent wasted resources
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
  pre_auton();

  while (true) {
    wait(100, msec);
  }
}
