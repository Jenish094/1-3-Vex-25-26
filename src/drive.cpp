#include "drive.h"
#include "devices.h"
#include "vex.h"

using namespace vex;

void driveStraight(double distanceInInches, double speedPercent) {
  FrontLmotor.spinFor(forward, distanceInInches, inches, speedPercent, percent, false);
  BackLmotor.spinFor(forward, distanceInInches, inches, speedPercent, percent, false);
  FrontRmotor.spinFor(forward, distanceInInches, inches, speedPercent, percent, false);
  BackRmotor.spinFor(forward, distanceInInches, inches, speedPercent, percent);
}

void turnInPlace(double distanceInInches, double speedPercent) {
  // Left side reverse, right side forward
  FrontLmotor.spinFor(reverse, distanceInInches, inches, speedPercent, percent, false);
  BackLmotor.spinFor(reverse, distanceInInches, inches, speedPercent, percent, false);
  FrontRmotor.spinFor(forward, distanceInInches, inches, speedPercent, percent, false);
  BackRmotor.spinFor(forward, distanceInInches, inches, speedPercent, percent);
}
