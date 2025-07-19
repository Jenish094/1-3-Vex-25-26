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

using namespace vex;
competition Competition;

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
  //Stop all motors
  FrontLmotor.stop();
  FrontRmotor.stop();
  BackLmotor.stop();
  BackRmotor.stop();
  IntakeWheels.stop();
  Chain.stop();

  while (1) {
    int leftSpeed = Controller1.Axis3.position(percent);  // Left stick Y-axis
    int rightSpeed = Controller1.Axis2.position(percent); // Right stick Y-axis

    FrontLmotor.spin(forward, leftSpeed, percent);
    BackLmotor.spin(forward, leftSpeed, percent);

    FrontRmotor.spin(forward, rightSpeed, percent);
    BackRmotor.spin(forward, rightSpeed, percent);

    //IntakeWheel
    if (Controller1.ButtonR1.pressing()) {
      IntakeWheels.spin(forward, 100, percent);
    }
      else if (Controller1.ButtonR2.pressing()) {
        IntakeWheels.spin(reverse, 100, percent);
      }
      else {
        IntakeWheels.stop();
      }
      
      //Chain
    if (Controller1.ButtonL1.pressing()) {
      Chain.spin(forward, 100, percent);
    }
      else if (Controller1.ButtonL2.pressing()) {
        Chain.spin(reverse, 100, percent);
      }
      else {
        Chain.stop();
      }

    wait(20, msec);
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
