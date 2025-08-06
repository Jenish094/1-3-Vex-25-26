#include "controls.h"
#include "devices.h"
#include "vex.h"
using namespace vex;

void usercontrol(void) {
  //Stop all motors
  LeftFront.stop();
  RightFront.stop();
  BackLeft.stop();
  BackRight.stop();
  IntakeTop.stop();
  IntakeBottom.stop();
  Chain.stop();

  while (1) {
    int leftSpeed = Controller1.Axis3.position(percent);  // Left stick Y-axis
    int rightSpeed = Controller1.Axis2.position(percent); // Right stick Y-axis

    LeftFront.spin(forward, leftSpeed, percent);
    BackLeft.spin(forward, leftSpeed, percent);

    RightFront.spin(forward, rightSpeed, percent);
    BackRight.spin(forward, rightSpeed, percent);

    //IntakeWheel
    if (Controller1.ButtonR1.pressing()) {
      IntakeTop.spin(forward, 100, percent);
      IntakeBottom.spin(reverse, 100, percent);
    }
    else if (Controller1.ButtonR2.pressing()) {
      IntakeTop.spin(reverse, 100, percent);
      IntakeBottom.spin(forward, 100, percent);
    }
    else {
      IntakeTop.stop();
      IntakeBottom.stop();
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
