#include "controls.h"
#include "devices.h"
#include "vex.h"

using namespace vex;

void handleIntakeControls() {
  if (Controller1.ButtonR1.pressing()) {
    IntakeWheels.spin(forward, 100, percent);
  } else if (Controller1.ButtonR2.pressing()) {
    IntakeWheels.spin(reverse, 100, percent);
  } else {
    IntakeWheels.stop();
  }
}

void handleChainControls() {
  if (Controller1.ButtonL1.pressing()) {
    Chain.spin(forward, 100, percent);
  } else if (Controller1.ButtonL2.pressing()) {
    Chain.spin(reverse, 100, percent);
  } else {
    Chain.stop();
  }
}
