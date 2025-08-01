#include "controls.h"
#include "devices.h"

using namespace vex;

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
