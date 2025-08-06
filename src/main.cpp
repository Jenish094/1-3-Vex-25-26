/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       1/3 - Jenish                                              */
/*    Created:      4/29/2025, 10:16:01 PM                                    */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"
#include "devices.h"
#include "controls.h"

using namespace vex;


void pre_auton(void) {
Brain.Screen.setFillColor(vex::color::white);
Brain.Screen.setPenColor(color::black);
Brain.Screen.setFont(prop60);
Brain.Screen.setCursor(3, 6);
Brain.Screen.print("1/3");
Brain.Screen.setFont(mono20);
Brain.Screen.setCursor(6, 4);
Brain.Screen.print("starting autonomous...");
}
void autonomous(void) {
Brain.Screen.print("Running Autonomous");
Drivetrain.driveFor(forward, 83.1, inches); //Move to goal
Drivetrain.turnFor(right, 180, degrees); // Turn 180 to position outtake to the goal
Drivetrain.driveFor(reverse, 0.5, inches);
Chain.spinFor(172, degrees); //Turn the chain
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
