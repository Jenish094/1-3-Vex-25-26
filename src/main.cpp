/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       1/3 - Jenish                                              */
/*    Created:      4/29/2025, 10:16:01 PM                                    */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
// We're cooked
#include "vex.h"
#include "devices.h"
#include "controls.h"
#include "drive.h"

using namespace vex;
competition Competition;
controller Controller;

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  FrontLmotor.stop();
  FrontRmotor.stop();
  BackLmotor.stop();
  BackRmotor.stop();
  IntakeWheels.stop();
  Chain.stop();
}

/*---------------------------------------------------------------------------*/
/*                              Autonomous Task                              */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  drive(11,100);
  turn(90,100);
  drive(43,100);

// insane autonomous
}

/*---------------------------------------------------------------------------*/
/*                              User Control Task                            */
/*---------------------------------------------------------------------------*/
//Put all code inside the loop
// This code is for just mapping the controller buttons to the robot functionalities
void usercontrol(void) {
  //Stop all motors
  FrontLmotor.stop();
  FrontRmotor.stop();
  BackLmotor.stop();
  BackRmotor.stop();
  IntakeWheels.stop();
  Chain.stop();

  while (1) {
    int drive = Controller.Axis3.position(percent);  // Left stick Y-axis
    int turn = Controller.Axis2.position(percent); // Right stick Y-axis

    int leftSpeed = drive + turn;
    int rightSpeed = drive - turn;

    FrontLmotor.spin(forward, leftSpeed, percent);
    BackLmotor.spin(forward, leftSpeed, percent);
    FrontRmotor.spin(forward, rightSpeed, percent);
    BackRmotor.spin(forward, rightSpeed, percent);

  handleIntakeControls();
  handleChainControls();

  wait(10, msec);
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
