#include "controls.h"
#include "devices.h"
#include "vex.h"

using namespace vex;

controller Controller;

void handleIntakeControls() {
  if (Controller.ButtonR1.pressing()) {
    IntakeWheels.spin(forward, 100, percent);
  } else if (Controller.ButtonR2.pressing()) {
    IntakeWheels.spin(reverse, 100, percent);
  } else {
    IntakeWheels.stop();
  }
}

void handleChainControls() {
  if (Controller.ButtonL1.pressing()) {
    Chain.spin(forward, 100, percent);
  } else if (Controller.ButtonL2.pressing()) {
    Chain.spin(reverse, 100, percent);
  } else {
    Chain.stop();
  }
}
